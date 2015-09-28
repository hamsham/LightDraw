/* 
 * File:   draw/shaderObject.h
 * Author: Miles Lacey
 *
 * Created on January 21, 2014, 6:54 PM
 */

#ifndef __LS_DRAW_SHADER_OBJECT_H__
#define	__LS_DRAW_SHADER_OBJECT_H__

#include <utility>

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**----------------------------------------------------------------------------
 * Enumeration to determine the type of shader that should be created on the GPU
-----------------------------------------------------------------------------*/
enum shader_stage_t : unsigned {
    SHADER_STAGE_VERTEX     = GL_VERTEX_SHADER,
    SHADER_STAGE_FRAGMENT   = GL_FRAGMENT_SHADER
};

/**----------------------------------------------------------------------------
 * Shader Object Base Class
 * 
 * Shader objects are attached to programs in order to manipulate rendering on
 * the GPU
-----------------------------------------------------------------------------*/
template <shader_stage_t shaderType>
class shaderObject {
    friend class shaderProgram;
    
    private:
        /**
         * Handle to the GPU-side shader object.
         */
        GLuint objectId = 0;
        
    public:
        /**
         * @brief Constructor
         * 
         * Initializes all members within *this.
         */
        shaderObject();
        
        /**
         * @brief Copy Constructor -- Deleted
         */
        shaderObject(const shaderObject&) = delete;
        
        /**
         * @brief Move Constructor
         * 
         * Copy the handle to the shader object owned by the input argument,
         * reset the moved object's shader handle to 0.
         */
        shaderObject(shaderObject&& so);
        
        /**
         * @brief Copy Operator -- Deleted
         */
        shaderObject& operator=(const shaderObject&) = delete;
        
        /**
         * @brief Move Operator
         * 
         * Move the values of the input argument and reset the moved object's
         * values to 0.
         * 
         * @param so
         * A shaderProgram to move into *this.
         * 
         * @return A reference to *this
         */
        shaderObject& operator=(shaderObject&& so);
        
        /**
         * Destroy this object and free any GPU memory it uses.
         */
        ~shaderObject();
        
        /**
         * Free all memory used by this shader object.
         */
        void terminate();
        
        /**
         * Compile a shader and put it into GPU memory.
         * 
         * @param data A pointer to a string which contains the shader's textual
         * content.
         * 
         * @param size
         * the size, in bytes, of the shader's text data. Let this remain zero
         * if you are sure that the data string is null-terminated.
         * 
         * @return TRUE if the shader was successfully compiled, FALSE if
         * otherwise.
         */
        bool init(const char* data, int size = 0);

        /**
         * Compile a set of shader strings and put them into GPU memory.
         *
         * @param numStrings
         * The number of shader strings which are to be passed to the GPU for
         * compilation.
         *
         * @param data
         * A pointer to an array of strings which contain the shader's textual
         * content.
         *
         * @param sizes
         * An array of integers, specifying the sizes, in bytes, of each
         * shader's textual data. Set this to NULL if you are sure that the
         * data in each string is null-terminated.
         *
         * @return TRUE if the shader was successfully compiled, FALSE if
         * otherwise.
         */
        bool init(const unsigned numStrings, const char* const* data, const int* sizes = nullptr);
        
        /**
         * Get a shader's GPU-assigned ID
         */
        GLuint getId() const;
};

/*-----------------------------------------------------------------------------
    Typedefs and external templates
-----------------------------------------------------------------------------*/
LS_DECLARE_CLASS_TYPE(vertexShader, shaderObject, SHADER_STAGE_VERTEX);
LS_DECLARE_CLASS_TYPE(fragmentShader, shaderObject, SHADER_STAGE_FRAGMENT);

} // end draw namespace
} // end ls namespace

#include "lightsky/draw/generic/shaderObject_impl.h"

#endif	/* __LS_DRAW_SHADER_OBJECT_H__ */

