/*
 * File:   draw/ShaderUniform.h
 * Author: miles
 *
 * Created on July 7, 2015, 10:04 PM
 */

#ifndef __LS_DRAW_SHADER_UNIFORM_H__
#define __LS_DRAW_SHADER_UNIFORM_H__

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/VAOAttrib.h"
#include "lightsky/draw/ShaderAttrib.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Shader Uniform Object
-----------------------------------------------------------------------------*/
class ShaderUniform {
    private:
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
         * 
         * This member is not meant to be accessed externally. Ever.
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
        
        /**
         * Pointer to a buffer of data which will be sent to a shader when
         * * this uniform gets bound to a shader during a render pass.
         */
        utils::Pointer<char[]> buffer;
    
    public:
        /**
         * Destructor
         * 
         * Delegates to the 'terminate()' function.
         */
        ~ShaderUniform() noexcept;
        
        /**
         * Constructor
         * 
         * Initializes all internal members to their default values.
         */
        ShaderUniform() noexcept;
        
        /**
         * Copy Constructor
         * 
         * @param u
         * A constant reference to another ShaderUniform who's data will be
         * copied into *this.
         */
        ShaderUniform(const ShaderUniform& u) noexcept;
        
        /**
         * Move Constructor
         * 
         * @param u
         * An r-value reference to ShaderUniform object who's data will be
         * moved into *this.
         */
        ShaderUniform(ShaderUniform&& u) noexcept;
        
        /**
         * Copy Operator
         * 
         * @param u
         * A constant reference to another ShaderUniform who's data will be
         * copied into *this.
         * 
         * @return A reference to *this.
         */
        ShaderUniform& operator=(const ShaderUniform& u) noexcept;
        
        /**
         * Move Constructor
         * 
         * @param u
         * An r-value reference to ShaderUniform object who's data will be
         * moved into *this.
         * 
         * @return A reference to *this.
         */
        ShaderUniform& operator=(ShaderUniform&& u) noexcept;

        /**
         * Initialize *this with a buffer of data.
         * 
         * @param dataType
         * Determines the type of data being used by *this uniform.
         * 
         * @param pData
         * A pointer to a location in memory which contains some data that will
         * be used to initialize *this. This location in memory must match the
         * data type represented by the 'dataType' parameter and the number of
         * array elements specified by the 'elementCount' parameter.
         * 
         * @param elementCount
         * Determines the number of array elements which are stored in 'pData'.
         * This parameter must contain a value of 1 if the object is not an
         * array.
         * 
         * @return TRUE if *this object was successfully initialized, FALSE if
         * not.
         */
        bool init(const vertex_data_t dataType, const void* const pData, const unsigned elementCount = 1) noexcept;
        
        /**
         * Free all dynamic memory used by *this and restore all internal
         * members to their default values.
         */
        void terminate() noexcept;
        
        /**
         * Determine if *this represents a valid uniform object.
         * 
         * @return TRUE if *this ShaderUniform object contains a name and an
         * initialized buffer of data, FALSE if not.
         */
        bool is_valid() const noexcept;

        /**
         * Set the name of *this ShaderUniform object and hash it.
         * 
         * @param attirbName
         * A constant C-style string containing the name of *this shader
         * uniform.
         * 
         * @param numChars
         * Contains the number of characters in the input string which should
         * be used as the name for *this. Set the parameter to 0 if the input
         * string is null-terminated. and the entire input string should be
         * used as *this object's name.
         * 
         * @return TRUE if the name was successfully changed, FALSE if not.
         */
        bool set_name(const char* const uniformName, const unsigned numChars = 0) noexcept;

        /**
         * Retrieve the name of *this ShaderUniform.
         * 
         * @return A pointer to a null-terminated string, containing the name
         * of *this.
         */
        const utils::Pointer<char[]>& get_name() const noexcept;
        
        /**
         * Retrieve the 32-bit hash value, calculated from the internal name.
         * 
         * @return A 32-bit integer which contains a hash of the 'name'
         * member.
         */
        uint32_t get_name_hash() const noexcept;
};

/*-------------------------------------
 * Retrieve a uniform object's name
-------------------------------------*/
inline const utils::Pointer<char[]>& ShaderUniform::get_name() const noexcept {
    return name;
}

/*-------------------------------------
 * Retrieve a uniform object's hash
-------------------------------------*/
inline uint32_t ShaderUniform::get_name_hash() const noexcept {
    return nameHash;
}



/*-----------------------------------------------------------------------------
 * Shader Uniform Functions
-----------------------------------------------------------------------------*/
/**------------------------------------
 * @brief Set a single uniform integer variable
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, int val) {
    glUniform1i(uniformId, val);
}

/**------------------------------------
 * @brief Set two uniform integer variables
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, int val0, int val1) {
    glUniform2i(uniformId, val0, val1);
}

/**------------------------------------
 * @brief Set three uniform integer variables
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, int val0, int val1, int val2) {
    glUniform3i(uniformId, val0, val1, val2);
}

/**------------------------------------
 * @brief Set four uniform integer variables
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, int val0, int val1, int val2, int val3) {
    glUniform4i(uniformId, val0, val1, val2, val3);
}

/**------------------------------------
 * @brief Set a uniform 2d vector of integers
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, const math::vec2i& val) {
    glUniform2iv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 3d vector of integers
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, const math::vec3i& val) {
    glUniform3iv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 4d vector of integers
-------------------------------------*/
inline void set_shader_uniform_int(GLint uniformId, const math::vec4i& val) {
    glUniform4iv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a single uniform unsigned int variable
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val) {
    glUniform1ui(uniformId, val);
}

/**------------------------------------
 * @brief Set two uniform unsigned int variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val0, unsigned val1) {
    glUniform2ui(uniformId, val0, val1);
}

/**------------------------------------
 * @brief Set three uniform unsigned int variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val0, unsigned val1, unsigned val2) {
    glUniform3ui(uniformId, val0, val1, val2);
}

/**------------------------------------
 * @brief Set four uniform unsigned int variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, unsigned val0, unsigned val1, unsigned val2, unsigned val3) {
    glUniform4ui(uniformId, val0, val1, val2, val3);
}

/**------------------------------------
 * @brief Set a uniform 2d vector of unsigned ints
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec2ui& val) {
    glUniform2uiv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 3d vector of unsigned ints
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec3ui& val) {
    glUniform3uiv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 4d vector of unsigned ints
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec4ui& val) {
    glUniform4uiv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a single uniform float variable
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val) {
    glUniform1f(uniformId, val);
}

/**------------------------------------
 * @brief Set two uniform float variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val0, float val1) {
    glUniform2f(uniformId, val0, val1);
}

/**------------------------------------
 * @brief Set three uniform float variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val0, float val1, float val2) {
    glUniform3f(uniformId, val0, val1, val2);
}

/**------------------------------------
 * @brief Set four uniform float variables
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, float val0, float val1, float val2, float val3) {
    glUniform4f(uniformId, val0, val1, val2, val3);
}

/**------------------------------------
 * @brief Set a uniform 2d vector of floats
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec2& val) {
    glUniform2fv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 3d vector of floats
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec3& val) {
    glUniform3fv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 4d vector of floats
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::vec4& val) {
    glUniform4fv(uniformId, 1, val.v);
}

/**------------------------------------
 * @brief Set a uniform 2d matrix
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::mat2& val, bool transpose = false) {
    glUniformMatrix2fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/**------------------------------------
 * @brief Set a uniform 3d matrix
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::mat3& val, bool transpose = false) {
    glUniformMatrix3fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}

/**------------------------------------
 * @brief Set a uniform 4d matrix
-------------------------------------*/
inline void set_shader_uniform(GLint uniformId, const math::mat4& val, bool transpose = false) {
    glUniformMatrix4fv(uniformId, 1, transpose ? GL_TRUE : GL_FALSE, &val[0]);
}



} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_SHADER_UNIFORM_H__ */
