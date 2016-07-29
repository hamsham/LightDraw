
#include <memory> // std::nothrow
#include <utility> // std::move

#include "lightsky/utils/Copy.h" // utils::fast_memset

#include "lightsky/draw/ShaderBlockAttrib.h"



namespace ls {
namespace draw {



/*-------------------------------------
 * Destructor
-------------------------------------*/
ShaderBlockAttrib::~ShaderBlockAttrib() noexcept {
}

/*-------------------------------------
 * Constructor
-------------------------------------*/
ShaderBlockAttrib::ShaderBlockAttrib() noexcept :
    numBytes{},
    binding{},
    index{},
    name{},
    numMembers{},
    memberNames{},
    memberElements{},
    memberTypes{},
    memberIndices{},
    memberOffsets{},
    memberStrides{}
{}

/*-------------------------------------
 * Copy Constructor
-------------------------------------*/
ShaderBlockAttrib::ShaderBlockAttrib(const ShaderBlockAttrib& b) noexcept {
    // delegate
    *this = b;
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
ShaderBlockAttrib::ShaderBlockAttrib(ShaderBlockAttrib&& b) noexcept {
    // delegate
    *this = std::move(b);
}

/*-------------------------------------
 * Copy Operator
-------------------------------------*/
ShaderBlockAttrib& ShaderBlockAttrib::operator=(const ShaderBlockAttrib& b) noexcept {
    numBytes = b.numBytes;
    binding = b.binding;
    index = b.index;
    name = b.name;
    numMembers = b.numMembers;
    
    if (!numMembers) {
        memberNames.reset();
        memberElements.reset();
        memberTypes.reset();
        memberIndices.reset();
        memberOffsets.reset();
        memberStrides.reset();
    }

    memberNames.reset(new(std::nothrow) std::string[numMembers]);
    memberElements.reset(new(std::nothrow) unsigned[numMembers]);
    memberTypes.reset(new(std::nothrow) vertex_data_t[numMembers]);
    memberIndices.reset(new(std::nothrow) unsigned[numMembers]);
    memberOffsets.reset(new(std::nothrow) unsigned[numMembers]);
    memberStrides.reset(new(std::nothrow) unsigned[numMembers]);

    for (unsigned i = 0; i < numMembers; ++i) {
        memberNames[i]      = b.memberNames[i];
        memberElements[i]   = b.memberElements[i];
        memberTypes[i]      = b.memberTypes[i];
        memberIndices[i]    = b.memberIndices[i];
        memberOffsets[i]    = b.memberOffsets[i];
        memberStrides[i]    = b.memberStrides[i];
    }

    return *this;
}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
ShaderBlockAttrib& ShaderBlockAttrib::operator=(ShaderBlockAttrib&& b) noexcept {
    numBytes = b.numBytes;
    b.numBytes = 0;
    
    binding = b.binding;
    b.binding = 0;
    
    index = b.index;
    b.index = 0;
    
    name = std::move(b.name);
    
    numMembers = b.numMembers;
    b.numMembers = 0;
    
    memberNames = std::move(b.memberNames);
    memberElements = std::move(b.memberElements);
    memberTypes = std::move(b.memberTypes);
    memberIndices = std::move(b.memberIndices);
    memberOffsets = std::move(b.memberOffsets);
    memberStrides = std::move(b.memberStrides);
    
    return *this;
}

/*-------------------------------------
 * Reset all internal parameters to their default values
-------------------------------------*/
void ShaderBlockAttrib::reset() noexcept {
    numBytes = 0;
    binding = 0;
    index = 0;
    name.clear();
    numMembers = 0;
    
    memberNames.reset();
    memberElements.reset();
    memberTypes.reset();
    memberIndices.reset();
    memberOffsets.reset();
    memberStrides.reset();
}

/*-------------------------------------
 * Allow a Shader Assembly to run OpenGL introspection
-------------------------------------*/
bool ShaderBlockAttrib::run_block_introspection(const GLuint shaderId, const unsigned shaderBlockIndex) noexcept {
    GLint maxUniformBlockNameLen = 0;
    utils::Pointer<GLchar[]> currentBlockName{nullptr};
    
    glGetProgramiv(shaderId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxUniformBlockNameLen);
    LS_LOG_GL_ERR();

    LS_LOG_MSG("Running introspection on uniform ", shaderBlockIndex, " of shader ", shaderId, '.');

    // Retrieve the length of the longest uniform block name
    GLint nameLen = 0;
    glGetActiveUniformBlockiv(shaderId, shaderBlockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);
    LS_LOG_GL_ERR();

    // Allocate once, and only once
    currentBlockName.reset(new(std::nothrow) GLchar[maxUniformBlockNameLen]);
    LS_DEBUG_ASSERT(currentBlockName.get() != nullptr);
    
    // clear garbage
    ls::utils::fast_memset(currentBlockName.get(), '\0', maxUniformBlockNameLen);

    // Using the length, retrieve the name of the current uniform block
    glGetActiveUniformBlockName(shaderId, shaderBlockIndex, nameLen, nullptr, currentBlockName.get());
    name = currentBlockName.get();
    LS_LOG_GL_ERR();

    // Get the current block's binding point with regard to the current shader.
    const GLuint blockIndex = glGetUniformBlockIndex(shaderId, currentBlockName.get());
    LS_LOG_GL_ERR();
    
    // This should only occur if the input shaderBlockIndex is invalid
    LS_DEBUG_ASSERT(blockIndex != GL_INVALID_INDEX);

    glUniformBlockBinding(shaderId, blockIndex, shaderBlockIndex);
    LS_LOG_GL_ERR();

    glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &numBytes);
    LS_LOG_GL_ERR();

    glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_BINDING, &binding);
    LS_LOG_GL_ERR();
    
    static_assert(sizeof(GLint) == sizeof(decltype(numMembers)), "Bad type conversion for OpenGL introspection");
    GLint* const pNumMembers = reinterpret_cast<GLint*>(&numMembers);
    
    glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, pNumMembers);
    LS_LOG_GL_ERR();

    index = blockIndex;
    memberIndices.reset(new(std::nothrow) unsigned[numMembers]);
    memberOffsets.reset(new(std::nothrow) unsigned[numMembers]);
    memberStrides.reset(new(std::nothrow) unsigned[numMembers]);
    
    if (!memberIndices || !memberOffsets || !memberStrides) {
        LS_LOG_ERR("\tFailed to allocate space for uniform block members.\n");
        return false;
    }
    
    static_assert(sizeof(GLint) == sizeof(decltype(memberOffsets)::value_type), "Bad type conversion for OpenGL introspection");
    static_assert(sizeof(GLint) == sizeof(decltype(memberStrides)::value_type), "Bad type conversion for OpenGL introspection");
    GLint* const pOffsets = reinterpret_cast<GLint*>(memberOffsets.get());
    GLint* const pStrides = reinterpret_cast<GLint*>(memberStrides.get());

    glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)memberIndices.get());
    LS_LOG_GL_ERR();

    glGetActiveUniformsiv(shaderId, numMembers, memberIndices.get(), GL_UNIFORM_OFFSET, pOffsets);
    LS_LOG_GL_ERR();

    glGetActiveUniformsiv(shaderId, numMembers, memberIndices.get(), GL_UNIFORM_ARRAY_STRIDE, pStrides);
    LS_LOG_GL_ERR();

    memberNames.reset(new(std::nothrow) std::string[numMembers]);
    memberElements.reset(new(std::nothrow) unsigned[numMembers]);
    memberTypes.reset(new(std::nothrow) vertex_data_t[numMembers]);
    
    LS_LOG_MSG(
        "\tUniform Block Name:      ", name,
        "\n\tUniform Block Index:     ", index,
        "\n\tUniform Block Binding:   ", binding,
        "\n\tUniform Block Size:      ", numBytes,
        "\n\tUniform Block Members:   ", numMembers
    );

    extract_uniform_block_members(shaderId, currentBlockName, maxUniformBlockNameLen);
    
    LS_LOG_MSG("\tDone.\n");
    
    return true;
}

/*-------------------------------------
 * Get all info about block members
-------------------------------------*/
bool ShaderBlockAttrib::extract_uniform_block_members(
    const GLuint shaderId,
    utils::Pointer<GLchar[]>& nameBuffer,
    const unsigned nameBufferLen
) noexcept {
    for (unsigned j = 0; j < numMembers; ++j) {
        static_assert(sizeof(GLuint) == sizeof(decltype(memberIndices)::value_type), "Bad type conversion for OpenGL introspection");
        static_assert(sizeof(GLint) == sizeof(decltype(memberElements)::value_type), "Bad type conversion for OpenGL introspection");
        static_assert(sizeof(GLenum) == sizeof(decltype(memberTypes)::value_type), "Bad type conversion for OpenGL introspection");
        
        GLuint* const pMemberIndex = reinterpret_cast<GLuint*>(&memberIndices[j]);
        GLint* const pMemberElements = reinterpret_cast<GLint*>(&memberElements[j]);
        GLenum* const pMemberType = reinterpret_cast<GLenum*>(&memberTypes[j]);

        // clear old data
        ls::utils::fast_memset(nameBuffer.get(), '\0', nameBufferLen);

        glGetActiveUniform(shaderId, *pMemberIndex, nameBufferLen, nullptr, pMemberElements, pMemberType, nameBuffer.get());
        LS_LOG_GL_ERR();

        memberNames[j].reserve(nameBufferLen);
        memberNames[j] = nameBuffer.get();
        
        LS_LOG_MSG(
            "\t\tBlock Member Name:       ", memberNames[j],
            "\n\t\tBlock Member Index:      ", memberIndices[j],
            "\n\t\tBlock Member Element:    ", memberElements[j],
            "\n\t\tBlock Member Offset:     ", memberOffsets[j],
            "\n\t\tBlock Member Stride:     ", memberStrides[j],
            "\n\t\tBlock Member Type:       ", memberTypes[j],
            "\n\t\tBlock Member Size:       ", draw::get_num_vertex_bytes((draw::vertex_data_t)memberTypes[j])
        );
    }
    
    return true;
}


} // end draw namespace
} // end ls namespace
