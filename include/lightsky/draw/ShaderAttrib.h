/*
 * File:   ShaderAttrib.h
 * Author: Miles
 *
 * Created on February 18, 2016, 2:30 PM
 */

#ifndef __LS_DRAW_SHADERATTRIB_H__
#define __LS_DRAW_SHADERATTRIB_H__

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Vertex.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward declarations
-----------------------------------------------------------------------------*/
enum vertex_data_t : GLenum;
enum class vertex_attrib_t : int; // VertexAttrib.h
enum shader_stage_t : GLint;
class ShaderProgram;



/**----------------------------------------------------------------------------
 * This enumeration helps organize all of the default limits that the OpenGL
 * specification places on GLSL Shaders.
 *
 * It is recommended that these get validated during shader assembly in order
 * to ensure portability (a shader could work on one platform and fail on
 * another). It is likely that the GLSL compiler will throw an error because of
 * something not falling within a limit.
-----------------------------------------------------------------------------*/
enum shader_limits_t : unsigned {
    GLSL_MAX_TEXTURE_UNITS = 32, // GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
    GLSL_INVALID_LOCATION = (unsigned) - 1,

    /*
     * GLSL Uniforms
     */
    GLSL_MAX_COMBINED_UNIFORM_BLOCKS = 24, // GL_MAX_COMBINED_UNIFORM_BLOCKS
    GLSL_MAX_COMBINED_UNIFORM_COMPONENTS = 24, // GL_MAX_COMBINED_UNIFORM_COMPONENTS

    GLSL_MAX_UNIFORM_BLOCK_SIZE = 16384, // GL_MAX_UNIFORM_BLOCK_SIZE,
    GLSL_MAX_UNIFORM_BUFFERS = 24, // GL_MAX_UNIFORM_BUFFERS
    GLSL_MAX_UNIFORM_LOCATIONS = 1024, // GL_MAX_UNIFORM_LOCATIONS

    GLSL_MAX_VERTEX_UNIFORM_BLOCKS = 12, // GL_MAX_VERTEX_UNIFORM_BLOCKS
    GLSL_MAX_VERTEX_UNIFORM_COMPONENTS = 1024, // GL_MAX_VERTEX_UNIFORM_COMPONENTS
    GLSL_MAX_COMBINED_VECTOR_UNIFORM_COMPONENTS = 49408, // GL_MAX_VERTEX_UNIFORM_COMPONENTS + GL_MAX_UNIFORM_BLOCK_SIZE * GL_MAX_FRAGMENT_UNIFORM_BLOCKS / 4
    GLSL_MAX_VERTEX_UNIFORM_VECTORS = 256, // GL_MAX_VERTEX_UNIFORM_VECTORS

    GLSL_MAX_FRAGMENT_UNIFORM_BLOCKS = 12, // GL_MAX_FRAGMENT_UNIFORM_BLOCKS
    GLSL_MAX_FRAGMENT_UNIFORM_COMPONENTS = 896, // GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
    GLSL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = 49376, // GL_MAX_FRAGMENT_UNIFORM_COMPONENTS + GL_MAX_UNIFORM_BLOCK_SIZE * GL_MAX_FRAGMENT_UNIFORM_BLOCKS / 4
    GLSL_MAX_FRAGMENT_UNIFORM_VECTORS = 224, // GL_MAX_FRAGMENT_UNIFORM_VECTORS

    /*
     * Vertex Shader Data
     */
    GLSL_MAX_VERTEX_INPUTS = 16,
    GLSL_MAX_VERTEX_TEXTURES = 16,
    GLSL_MAX_VERTEX_OUTPUTS = 64,

    /*
     * Fragment Shader Data
     */
    GLSL_MAX_FRAGMENT_INPUTS = 60,
    GLSL_MAX_FRAGMENT_TEXTURES = 16,
    GLSL_MAX_FRAGMENT_OUTPUTS = 4,
    
#ifdef LS_DRAW_BACKEND_GL
    GLSL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS = 1, // GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
    GLSL_MAX_GEOMETRY_TEXTURES = 16, // GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
    GLSL_MAX_GEOMETRY_UNIFORM_COMPONENTS = 1024, // GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
    GLSL_MAX_GEOMETRY_UNIFORM_BLOCKS = 12, // GL_MAX_GEOMETRY_UNIFORM_BLOCKS
    GLSL_MAX_GEOMETRY_INPUT_COMPONENTS = 64, // GL_MAX_GEOMETRY_INPUT_COMPONENTS
    GLSL_MAX_GEOMETRY_OUTPUT_COMPONENTS = 128, // GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
#endif
};



/**----------------------------------------------------------------------------
 * A ShaderAttrib contains all of the basic meta-information about shader
 * inputs and outputs. It can be used to match vertex buffer attribs to
 * vertex input variables, or determine what the outputs of a Fragment shader
 * are.
-----------------------------------------------------------------------------*/
class ShaderAttrib {
    private:
        /**
         * Attribute layout location.
         */
        uint32_t location;

        /**
         * Standard vertex component count (1 for scalars, 2-3 per vector)
         */
        uint32_t components;

        /**
         * Vertex Type (not base type).
         */
        vertex_data_t type;
        
        /**
         * For arrays of uniform data, this member contains the number of
         * elements per uniform. The default value is 1 for uniform objects
         * which are not arrays.
         */
        uint32_t numElements;
        
        /**
         * Member to contain a hash of the attribute name for quick reference.
         */
        uint32_t nameHash;

        /**
         * Name of the Shader Attrib as it's labeled in GLSL.
         * 
         * Setting the name of a ShaderAttrib will cause the 'nameHash' member
         * to be reinitialized for faster comparison with other ShaderAttribute
         * objects.
         */
        utils::Pointer<GLchar[]> name;

    public:
        /**
         * Destructor
         */
        ~ShaderAttrib() noexcept;

        /**
         * Constructor
         */
        ShaderAttrib() noexcept;

        /**
         * Copy Constructor
         * 
         * Copies all data from the input parameter into *this. Since
         * ShaderAttributes contain dynamic C-style strings for names, there
         * will be a runtime execution cost to allocate space for a copy of
         * the 'name' member.
         */
        ShaderAttrib(const ShaderAttrib& s) noexcept;

        /**
         * Move Constructor
         * 
         * Moves all data from the input parameter into *this and resets the
         * input parameter's members to their defaults.
         */
        ShaderAttrib(ShaderAttrib&& s) noexcept;

        /**
         * Copy Operator
         * 
         * Copies all data from the input parameter into *this. Since
         * ShaderAttributes contain dynamic C-style strings for names, there
         * will be a runtime execution cost to allocate space for a copy of
         * the 'name' member.
         * 
         * @return A reference to *this.
         */
        ShaderAttrib& operator=(const ShaderAttrib& s) noexcept;

        /**
         * Move Operator
         * 
         * Moves all data from the input parameter into *this and resets the
         * input parameter's members to their defaults.
         * 
         * @return A reference to *this.
         */
        ShaderAttrib& operator=(ShaderAttrib&& s) noexcept;
        
        /**
         * Determine if two shader attributes are of the same type.
         * 
         * @param s
         * A constant reference to a shader attribute to compare with *this.
         * 
         * @return TRUE if the input shader's internal members contain values
         * which match the data in *this, or FALSE if not.
         */
        bool operator==(const ShaderAttrib& s) const noexcept;
        
        /**
         * Determine if two shader attributes are not of the same type.
         * 
         * @param s
         * A constant reference to a shader attribute to compare with *this.
         * 
         * @return TRUE if the input shader's internal members contain values
         * which differ from the data in *this, or FALSE if not.
         */
        bool operator!=(const ShaderAttrib& s) const noexcept;

        /**
         * Manually set the uniform/VAO location of a ShaderAttribue.
         * 
         * @param layoutLocation
         * An unsigned integral value which can be used to determine the GLSL
         * uniform location of *this ShaderAttribute.
         */
        void set_location(const unsigned layoutLocation) noexcept;

        /**
         * Retrieve the vertex attribute location of *this ShaderAttribute.
         * 
         * This value can be specified for uniforms in GLSL code by using the
         * 'layout' specifier. For Vertex Array object, this index value is
         * specified manually in CPU-side code through glVertexAttribPointer.
         * 
         * @return An unsigned integral value which can be used to determine
         * the vertex attribute location of *this ShaderAttribute.
         */
        unsigned get_location() const noexcept;

        /**
         * Retrieve the number of components in a single shader data type.
         * 
         * For scalar types, this function will return 1. Vector types will
         * return 2-4, depending on their number of dimensions. Matrix types
         * will return 2-4, depending on the number of columns they contain.
         * 
         * @return An integral value which can be used to determine the number
         * of dimensions a single shader data type contains.
         */
        unsigned get_num_components() const noexcept;

        /**
         * Retrieve the number of subcomponents in *this ShaderAttribute.
         * 
         * For most data types, this value will only return 1. Matrix data
         * types will return a number from 2-4 depending on the number of rows
         * they have.
         * 
         * @return The number of subcomponents in a single shader data type.
         */
        unsigned get_num_subcomponents() const noexcept;

        /**
         * Assign a data type to *this attribute.
         * 
         * This function will also determine the number of components that a
         * vertex data type has.
         * 
         * @param vertexType
         * A value from the vertex_data_t enumeration which will be associated
         * with *this ShaderAttribute.
         */
        void set_type(const vertex_data_t vertexType) noexcept;

        /**
         * Retrieve the data type of *this shader attribute.
         *
         * @return A value from the vertex_data_t enumeration which can
         * determine if *this object represents a vertex integer, float,
         * vector, matrix, or other types.
         */
        vertex_data_t get_type() const noexcept;

        /**
         * Retrieve the base data type of *this vertex attribute.
         * 
         * Vector and matrix types can be composed of integers, floats,
         * 4doubles, and (in some cases) other data types. Use this function to
         * identify the basic type of an attribute.
         * 
         * @return The data type of *this attribute's individual components.
         */
        vertex_data_t get_base_type() const noexcept;
        
        /**
         * Set the number of elements per array of attribute data.
         * 
         * For shader attributes which are not arrays, this member must be 1.
         * 
         * @param arrayElementCount
         * The number of array elements utilized by a ShaderAttribute.
         */
        void set_num_elements(const unsigned arrayElementCount = 1) noexcept;
        
        /**
         * Retrieve the number of elements per array of attribute data.
         * 
         * For shader attributes which are not arrays, this member will only
         * return a value of 1.
         * 
         * @return The number of array elements utilized by a ShaderAttribute.
         */
        unsigned get_num_elements() const noexcept;

        /**
         * Assign a name to *this shader attribute.
         * 
         * @param attirbName
         * A constant C-style string containing the name of *this shader
         * attribute.
         * 
         * @param numChars
         * Contains the number of characters in the input string which should
         * be used as the name for *this. Set the parameter to 0 if the input
         * string is null-terminated. and the entire input string should be
         * used as *this object's name.
         * 
         * @return TRUE if the name was successfully changed, FALSE if not.
         */
        bool set_name(const char* const attribName, const unsigned numChars = 0) noexcept;

        /**
         * Retrieve the name of *this shader attribute.
         * 
         * GLSL uniforms require a name to be identified by CPU-side code. Use
         * this member to identify and modify shader uniforms.
         * 
         * @return A constant reference to an managed C-style strign,
         * containing the name of *this shader attribute.
         */
        const utils::Pointer<GLchar[]>& get_name() const noexcept;
        
        /**
         * Retrieve the integer ID hashed from *this ShaderAttrib's name.
         * 
         * @return An unsigned integral type, containing a hash of *this
         * attribute's name. A value of 0 will be returned if the name was not
         * set.
         */
        uint32_t get_name_hash() const noexcept;
        
        /**
         * Retrieve the number of bytes which a shader attribute requires on
         * the GPU.
         * 
         * This can be useful for introspection of data types contained within
         * a uniform buffer object or uniform block in GLSL.
         * 
         * @return The size, in bytes, which a uniform data type occupies in
         * GLSL.
         */
        unsigned get_num_bytes() const noexcept;
};

/*-------------------------------------
 * Set the location of an attribute
-------------------------------------*/
inline void ShaderAttrib::set_location(const unsigned layoutLocation) noexcept {
    location = layoutLocation;
}

/*-------------------------------------
 * Retrieve an attribute location
-------------------------------------*/
inline unsigned ShaderAttrib::get_location() const noexcept {
    return location;
}

/*-------------------------------------
 * Get the number of components of an attribute
-------------------------------------*/
inline unsigned ShaderAttrib::get_num_components() const noexcept {
    return components;
}

/*-------------------------------------
 * Retrieve the number of components within an attrib.
-------------------------------------*/
inline unsigned ShaderAttrib::get_num_subcomponents() const noexcept {
    return ls::draw::get_vertex_subcomponents(type);
}

/*-------------------------------------
 * Set the attribute type
-------------------------------------*/
inline void ShaderAttrib::set_type(const vertex_data_t vertexType) noexcept {
    type = vertexType;
    components = ls::draw::get_vertex_components(vertexType);
}

/*-------------------------------------
 * Retrieve the attribute type.
-------------------------------------*/
inline vertex_data_t ShaderAttrib::get_type() const noexcept {
    return type;
}

/*-------------------------------------
 * Retrieve the attribute base type.
-------------------------------------*/
inline vertex_data_t ShaderAttrib::get_base_type() const noexcept {
    return draw::get_vertex_base_type(type);
}

/*-------------------------------------
 * Set the number of elements per array of attribute data.
-------------------------------------*/
inline void ShaderAttrib::set_num_elements(const unsigned arrayElementCount) noexcept {
    LS_DEBUG_ASSERT(arrayElementCount > 0);
    numElements = arrayElementCount;
}

/*-------------------------------------
 * Retrieve the number of elements per array of attribute data.
-------------------------------------*/
inline unsigned ShaderAttrib::get_num_elements() const noexcept {
    return numElements;
}

/*-------------------------------------
 * Retrieve the name of an attribute
-------------------------------------*/
inline const utils::Pointer<GLchar[]>& ShaderAttrib::get_name() const noexcept {
    return name;
}

/*-------------------------------------
 * Retrieve the hash of an attribute's name
-------------------------------------*/
inline uint32_t ShaderAttrib::get_name_hash() const noexcept {
    return nameHash;
}

/*-------------------------------------
 * Retrieve the total number of bytes occupied by a shader attribute.
-------------------------------------*/
inline unsigned ShaderAttrib::get_num_bytes() const noexcept {
    return draw::get_num_vertex_bytes(type) * /* get_num_components() * get_num_subcomponents() * */ get_num_elements();
}



/*-----------------------------------------------------------------------------
 * Utility functions for Shader Attributes
-----------------------------------------------------------------------------*/
/**------------------------------------
 * @brief Get information about an active uniform located in a shader.
 *
 * @param index
 * Indicates the index of the uniform to be queried.
 *
 * @param varSize
 * Indicates the number of elements contained within the uniform. This
 * will be a value of 1 for all variables that are not arrays.
 *
 * @param varType
 * Used to determine the variable's data type; such as an int, float,
 * sampler, matrix, or sampler array.
 *
 * @param oGLAttribMaxLenFlag
 * Must be either GL_ACTIVE_UNIFORM_MAX_LENGTH or
 * GL_ACTIVE_ATTRIBUTE_MAX_LENGTH.
 *
 * @returns the name of a vertex or uniform attribute as it is known in a GLSL
 * shader.
-------------------------------------*/
utils::Pointer<GLchar[]> get_attrib_name(
    const ShaderProgram& prog,
    const GLint index,
    GLint& outVarSize,
    GLenum& outVarType,
    const GLint attribLenFlag
) noexcept;



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_SHADERATTRIB_H__ */
