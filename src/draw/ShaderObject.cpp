/*
 * File:   draw/shaderObject.cpp
 * Author: Miles Lacey
 *
 * Created on January 21, 2014, 6:54 PM
 */

#include <cstring> // strlen
#include <new> // std::nothrow
#include <utility> // std::move()

#include "lightsky/setup/Compiler.h"

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Copy.h"

#include "lightsky/draw/ShaderObject.h"
#include "lightsky/draw/VAOAttrib.h"



/*-----------------------------------------------------------------------------
 Regex Setup
-----------------------------------------------------------------------------*/
#if defined(LS_COMPILER_GNU)
    // std::regex support wasn't addded to Clang until version 3.5.
    // A bug in Clang's libc++, prevents it from distinguishing escaped parenthesis in raw-string literals.
    #if defined(LS_COMPILER_CLANG)
        #define LS_REQUIRE_BOOST_REGEX

    // std::regex support wasn't addded to GCC until version 4.9.2
    #elif defined(LS_COMPILER_GCC) && (LS_COMPILER_GCC == 4) && (LS_COMPILER_GCC_MAJ < 9)
        #define LS_REQUIRE_BOOST_REGEX

    #endif
#endif

#ifdef LS_REQUIRE_BOOST_REGEX
    #include <boost/regex.hpp>

namespace ls {
namespace rgx {
    using boost::cregex_iterator;
    using boost::regex;
    using boost::cmatch;
    namespace regex_constants = boost::regex_constants;
} // end rgx namespace
} // end std::namespace

#else
    #include <regex> // should be boost::regex if using G++ 4.9 or earlier.

namespace ls {
namespace rgx {
    using std::cregex_iterator;
    using std::regex;
    using std::cmatch;
    namespace regex_constants = std::regex_constants;
} // end rgx namespace
} // end std::namespace

#endif



/*-----------------------------------------------------------------------------
 Anonymous helper functions
-----------------------------------------------------------------------------*/
namespace {

constexpr char const VERT_SHADER_QUERY[] = u8R"***((?:location\s*\=\s*(\d+).*\))?\s*in\s+(bool|int|uint|float|[biud]?vec[2-4]?|mat[2-4](x[2-4])?)\s+([a-zA-Z_]\w*)\s*(?:\[(\d+)\]\s*)?;)***";
constexpr char const FRAG_SHADER_QUERY[] = u8R"***((?:location\s*\=\s*(\d+).*\))?\s*out\s+(bool|int|uint|float|[biud]?vec[2-4]?|mat[2-4](x[2-4])?)\s+([a-zA-Z_]\w*)\s*(?:\[(\d+)\]\s*)?;)***";

// Indices of shader regex groupings.



enum shader_regex_group_t : unsigned {
    SHADER_REGEX_LAYOUT = 1,
    SHADER_REGEX_TYPE = 2,
    SHADER_REGEX_NAME = 4,
    SHADER_REGEX_COMPONENTS = 3,
};

/*-------------------------------------
 * Retrieve the layout of a shader attrib
-------------------------------------*/
inline unsigned _get_shader_attrib_layout(const ls::rgx::cmatch& shaderMatches) {
    const std::string& layout = shaderMatches[shader_regex_group_t::SHADER_REGEX_LAYOUT];
    return layout.empty()
        ? ls::draw::shader_limits_t::GLSL_INVALID_LOCATION
        : (unsigned)std::stoi(layout);
}

/*-------------------------------------
 * Retrieve the components of a shader attrib
-------------------------------------*/
inline unsigned _get_shader_attrib_components(const ls::rgx::cmatch& shaderMatches) {
    const std::string& components = shaderMatches[shader_regex_group_t::SHADER_REGEX_COMPONENTS];
    return components.empty()
        ? 1
        : (unsigned)std::stoi(components);
}

/*-------------------------------------
 * Retrieve the name of a shader attrib
-------------------------------------*/
inline const std::string _get_shader_attrib_name(const ls::rgx::cmatch& shaderMatches) {
    return shaderMatches[shader_regex_group_t::SHADER_REGEX_NAME].str();
}

/*-------------------------------------
 * Retrieve the type of a shader attrib
-------------------------------------*/
ls::draw::vertex_data_t _get_shader_attrib_type(const ls::rgx::cmatch& shaderMatches) {
    using ls::draw::vertex_data_t;

    const std::string& type = shaderMatches[shader_regex_group_t::SHADER_REGEX_TYPE];

    static const char* const names[] = {
        "bool", "int", "uint", "float",
        "bvec2", "ivec2", "uvec2", "vec2",
        "bvec3", "ivec3", "uvec3", "vec3",
        "bvec4", "ivec4", "uvec4", "vec4",
        "mat2", "mat2x2", "mat2x3", "mat2x4",
        "mat3", "mat3x2", "mat3x3", "mat3x4",
        "mat4", "mat4x2", "mat4x3", "mat4x4"
    };

    constexpr vertex_data_t types[] = {
        vertex_data_t::VERTEX_DATA_BYTE, vertex_data_t::VERTEX_DATA_INT, vertex_data_t::VERTEX_DATA_UINT, vertex_data_t::VERTEX_DATA_FLOAT,
        vertex_data_t::VERTEX_DATA_VEC_2B, vertex_data_t::VERTEX_DATA_VEC_2I, vertex_data_t::VERTEX_DATA_VEC_2UI, vertex_data_t::VERTEX_DATA_VEC_2F,
        vertex_data_t::VERTEX_DATA_VEC_3B, vertex_data_t::VERTEX_DATA_VEC_3I, vertex_data_t::VERTEX_DATA_VEC_3UI, vertex_data_t::VERTEX_DATA_VEC_3F,
        vertex_data_t::VERTEX_DATA_VEC_4B, vertex_data_t::VERTEX_DATA_VEC_4I, vertex_data_t::VERTEX_DATA_VEC_4UI, vertex_data_t::VERTEX_DATA_VEC_4F,
        vertex_data_t::VERTEX_DATA_MAT_2F, vertex_data_t::VERTEX_DATA_MAT_2F, vertex_data_t::VERTEX_DATA_MAT_2x3F, vertex_data_t::VERTEX_DATA_MAT_2x4F,
        vertex_data_t::VERTEX_DATA_MAT_3F, vertex_data_t::VERTEX_DATA_MAT_3x2F, vertex_data_t::VERTEX_DATA_MAT_3F, vertex_data_t::VERTEX_DATA_MAT_3x4F,
        vertex_data_t::VERTEX_DATA_MAT_4F, vertex_data_t::VERTEX_DATA_MAT_4x2F, vertex_data_t::VERTEX_DATA_MAT_4x3F, vertex_data_t::VERTEX_DATA_MAT_4F
    };

    static_assert(LS_ARRAY_SIZE(names) == LS_ARRAY_SIZE(types), "Please ensure the input and output vertex data type arrays are the same size.");

    constexpr unsigned numTypes = LS_ARRAY_SIZE(types);

    for (unsigned i = 0; i < numTypes; ++i) {
        if (type == names[i]) {
            return types[i];
        }
    }

    return vertex_data_t::VERTEX_DATA_UNKNOWN;
}



} // end anonymous namespace

/*-----------------------------------------------------------------------------
 * ls Draw Namespace
-----------------------------------------------------------------------------*/
namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 Shader Object Methods
-----------------------------------------------------------------------------*/

/*-------------------------------------
    Destructor
-------------------------------------*/
ShaderObject::~ShaderObject() noexcept {
}

/*-------------------------------------
    Constructor
-------------------------------------*/
ShaderObject::ShaderObject() noexcept :
    gpuId {0},
    shaderStage {SHADER_STAGE_INVALID},
    attribs {}
{}

/*-------------------------------------
    Copy Constructor
-------------------------------------*/
ShaderObject::ShaderObject(const ShaderObject& s) noexcept :
    gpuId {s.gpuId},
    shaderStage {s.shaderStage},
    attribs {s.attribs}
{}

/*-------------------------------------
    Move Constructor
-------------------------------------*/
ShaderObject::ShaderObject(ShaderObject&& s) noexcept :
    gpuId {s.gpuId},
    shaderStage {s.shaderStage},
    attribs {std::move(s.attribs)}
{
    s.gpuId = 0;
    s.shaderStage = SHADER_STAGE_INVALID;
}

/*-------------------------------------
    Copy Operator
-------------------------------------*/
ShaderObject& ShaderObject::operator =(const ShaderObject& s) noexcept {
    gpuId = s.gpuId;
    shaderStage = s.shaderStage;
    attribs = s.attribs;

    return *this;
}

/*-------------------------------------
    Move Operator
-------------------------------------*/
ShaderObject& ShaderObject::operator =(ShaderObject&& s) noexcept {
    gpuId = s.gpuId;
    s.gpuId = 0;

    shaderStage = s.shaderStage;
    s.shaderStage = SHADER_STAGE_INVALID;

    attribs = std::move(s.attribs);

    return *this;
}

/*-------------------------------------
    Shader Loading (multiple shaders).
-------------------------------------*/
utils::Pointer<GLchar[]> ShaderObject::get_shader_string(
    const GLuint shaderId,
    const shader_string_t stringType
) noexcept {
    LS_DEBUG_ASSERT(
        stringType == shader_string_t::SHADER_STRING_LOG ||
        stringType == shader_string_t::SHADER_STRING_SOURCE
    );

    GLint shaderStringLen = 0;
    glGetShaderiv(shaderId, stringType, &shaderStringLen);

    // Allocate some memory to temporarily store the log data. Add +1 for
    // null-termination
    utils::Pointer<GLchar[]> shaderStringData(new(std::nothrow) GLchar[shaderStringLen+1]);
    
    if (!shaderStringData) {
        return shaderStringData;
    }
    
    utils::fast_memset(shaderStringData.get(), '\0', shaderStringLen+1);

    if (stringType == shader_string_t::SHADER_STRING_LOG) {
        glGetShaderInfoLog(shaderId, shaderStringLen, nullptr, shaderStringData.get());
    }
    else {
        glGetShaderSource(shaderId, shaderStringLen, nullptr, shaderStringData.get());
    }

    return shaderStringData;
}

/*-------------------------------------
    Parse a shader for its information & attribs.
-------------------------------------*/
bool ShaderObject::introspect_attributes() noexcept {
    LS_LOG_MSG(
        "\tAttempting to parse all attributes in a shader of type ",
        LS_ENUM_VAL(shaderStage), '.'
    );

    // only supported shader types at the moment.
    LS_DEBUG_ASSERT(
        shaderStage == shader_stage_t::SHADER_STAGE_VERTEX ||
        shaderStage == shader_stage_t::SHADER_STAGE_FRAGMENT
    );

    const utils::Pointer<GLchar[]>&& shaderSource = get_shader_source();
    const char* const attribPattern = (shaderStage == shader_stage_t::SHADER_STAGE_VERTEX)
        ? VERT_SHADER_QUERY
        : FRAG_SHADER_QUERY;

    rgx::regex fragRegex {attribPattern, rgx::regex::ECMAScript};
    const unsigned numSourceChars = strlen(shaderSource.get());
    const rgx::cregex_iterator start{shaderSource, shaderSource+numSourceChars, fragRegex, rgx::regex_constants::match_any};
    const rgx::cregex_iterator end;
    unsigned numAttribs = 0;

    // count the number of attribs to preallocate
    for (rgx::cregex_iterator iter = start; iter != end; ++iter) {
        ++numAttribs;
    }

    // All shaders need at least one attribute to function correctly. Since
    // this function is called after the compilation stage, not finding any
    // attributes is a parse error.
    LS_DEBUG_ASSERT(numAttribs > 0);

    attribs.reset_num_attribs(numAttribs);

    unsigned currentAttrib = 0;
    for (rgx::cregex_iterator iter = start; iter != end; ++iter) {
        ShaderAttrib& attrib = attribs.get_attrib(currentAttrib++);

        attrib.set_location(_get_shader_attrib_layout(*iter));
        attrib.set_name(_get_shader_attrib_name(*iter).c_str());
        attrib.set_type(_get_shader_attrib_type(*iter));
        attrib.set_num_elements(_get_shader_attrib_components(*iter));

        LS_LOG_MSG(
            "\t\tFound shader attribute:",
            "\n\t\t\tName:            ", attrib.get_name(),
            "\n\t\t\tLocation:        ", attrib.get_location(),
            "\n\t\t\tType:            ", LS_ENUM_VAL(attrib.get_type()),
            "\n\t\t\tElements:        ", attrib.get_num_elements(),
            "\n\t\t\tComponents:      ", attrib.get_num_components(),
            "\n\t\t\tSubcomponents:   ", attrib.get_num_subcomponents()
        );
    }

    if (attribs.get_num_attribs() < 1) {
        LS_LOG_ERR(
            "\t\tUnable to parse any attributes from a shader of type ",
            LS_ENUM_VAL(shaderStage), ".\n"
        );

        return false;
    }

    LS_LOG_MSG(
        "\t\tSuccessfully parsed ", numAttribs, " attributes from a shader of type ",
        LS_ENUM_VAL(shaderStage), ".\n"
    );

    return true;
}

/*-------------------------------------
    Free all memory used by this shader object.
-------------------------------------*/
void ShaderObject::terminate() noexcept {
    glDeleteShader(gpuId);
    gpuId = 0;
    shaderStage = shader_stage_t::SHADER_STAGE_INVALID;
    attribs.reset_num_attribs(0);
}

/*-------------------------------------
    Shader Loading (multiple shaders).
-------------------------------------*/
bool ShaderObject::init(
    const shader_stage_t shaderSourceType,
    const unsigned numStrings,
    const char* const* data,
    const int* const sizes
) noexcept {
    LS_LOG_MSG("Attempting to compile a shader object.");

    LS_LOG_MSG("\tCreating a shader ID.");
    GLuint shaderId;
    shaderId = glCreateShader(shaderSourceType);

    if (!shaderId) {
        LS_LOG_ERR("\t\tUnable to create a handle to a shader object on the GPU.\n");
        return false;
    }
    LS_LOG_MSG("\t\tDone");

    LS_LOG_MSG("\tUploading source data of type ", shaderSourceType, " to shader object ", shaderId, '.');
    
    // If the size is zero, opengl will just look for null-termination in the data
    glShaderSource(shaderId, numStrings, const_cast<const char**>(data), sizes);
    LS_LOG_GL_ERR();
    
    LS_LOG_MSG("\t\tDone.");

    LS_LOG_MSG("\tCompiling shader object ", shaderId, '.');
    glCompileShader(shaderId);
    LS_LOG_GL_ERR();

    GLint shaderStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
    LS_LOG_GL_ERR();

    const utils::Pointer<GLchar[]>& infoLog = get_shader_string(shaderId, shader_string_t::SHADER_STRING_LOG);

    // get the log information for the loaded shader
    LS_LOG_MSG("\t\tShader compilation log: ", infoLog.get(), '\n');
    
    if (shaderStatus != GL_TRUE) {
        glDeleteShader(shaderId);
        return false;
    }

    gpuId = shaderId;
    shaderStage = shaderSourceType;

#ifdef LS_DRAW_BACKEND_GL
    if (shaderSourceType != shader_stage_t::SHADER_STAGE_GEOMETRY) {
        LS_ASSERT(introspect_attributes());
    }
#else
    LS_ASSERT(introspect_attributes());
#endif

    LS_LOG_MSG(
        "\t\tDone. Successfully loaded a shader object.",
        "\n\t\tInfo Log: ", infoLog.get(), '\n'
    );

    LS_LOG_GL_ERR();

    return true;
}

/*-------------------------------------
 Recreate a ShaderObject from OpenGL queries
-------------------------------------*/
bool ShaderObject::recreate_from_id(const GLuint shaderId) noexcept {
#ifdef LS_DRAW_BACKEND_GL
    if (shaderStage == shader_stage_t::SHADER_STAGE_GEOMETRY) {
        LS_LOG_ERR("Unable to recreate shader attributes for a geometry shader.");
        return false;
    }
#endif
    
    LS_LOG_MSG("Attempting to recreate a ShaderObject by querying OpenGL.");

    if (glIsShader(shaderId) != GL_TRUE) {
        LS_LOG_ERR(
            "\tFailed to recreate a ShaderObject. The input ID ", shaderId,
            " does not represent a valid shader on the GPU.\n"
        );
        return false;
    }

    GLint shaderType = 0;
    glGetShaderiv(shaderId, GL_SHADER_TYPE, &shaderType);

    LS_DEBUG_ASSERT(
        shaderType == shader_stage_t::SHADER_STAGE_VERTEX ||
        shaderType == shader_stage_t::SHADER_STAGE_FRAGMENT
    );

    ShaderObject temp {};
    temp.gpuId = shaderId;
    temp.shaderStage = (shader_stage_t)shaderType;

    if (!temp.introspect_attributes()) {
        LS_LOG_ERR("\tUnable to query the recreated shader ", shaderId, " for input/output attributes.\n");
        return false;
    }

    // I love a happy ending
    LS_LOG_MSG("\tSuccessfully recreated a ShaderObject from the shader ID ", shaderId, "\n");

    *this = std::move(temp);
    return true;
}



} // end draw namespace
} // end ls namespace
