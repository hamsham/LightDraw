/* 
 * File:   draw/vertexArray.h
 * Author: Miles Lacey
 *
 * Created on February 7, 2014, 8:01 PM
 */

#ifndef __LS_DRAW_VERTEX_ARRAY_H__
#define	__LS_DRAW_VERTEX_ARRAY_H__

#include "lightsky/draw/setup.h"

namespace ls {
namespace draw {

/**
 * @brief Vertex Array Object
 * 
 * Represents a single VAO within OpenGL. VAO object store the state of
 * multiple vertex buffer object attributes into one container.
 */
class vertexArray {
    private:
        /**
         * A handle to the vertex array object residing on the GPU.
         */
        unsigned vaoId = 0;

    public:
        /**
         * @brief Constructor
         */
        vertexArray();
        
        /**
         * @brief Copy Constructor - DELETED
         */
        vertexArray(const vertexArray&) = delete;
        
        /**
         * @brief Move Constructor
         * 
         * This moves the data contained within the constructor parameter into
         * *this. No copies are performed.
         * 
         * @param va
         * A vertexArray object whose data will be moved into *this.
         */
        vertexArray(vertexArray&&);
        
        /**
         * @brief Destructor
         * 
         * Releases all resources used by *this.
         */
        ~vertexArray();
        
        /**
         * @brief Copy Operator - DELETED
         */
        vertexArray& operator=(const vertexArray&) = delete;
        
        /**
         * @brief Move Operator
         * 
         * This moves the data contained within the method parameter into *this.
         * No copies are performed.
         * 
         * @param va
         * A vertexArray object whose data will be moved into *this.
         * 
         * @return A reference to *this.
         */
        vertexArray& operator=(vertexArray&&);
        
        /**
         * @brief Array initialization.
         * 
         * Generates a vertex array object on the GPY that will be used to
         * describe the memory layout of a data buffer.
         * 
         * @return true if the buffer was successfully created, false if not.
         */
        bool init();
        
        /**
         * Terminate the vertex array and release all of its resources back to
         * the GPU.
         */
        void terminate();
        
        /**
         * Determine if there is data used by this object
         * 
         * @returns true if this object has data residing on GPU memory, false
         * if not.
         */
        bool isValid() const;
        
        /**
         * Get the GPU-assigned ID for this VAO
         * 
         * @return an unsigned integral type representing this VAO.
         */
        unsigned getId() const;
        
        /**
         * Enable an attribute contained within the vertex array.
         * 
         * @param index
         * The array-index to an attribute that is to be enabled.
         */
        void enableAttrib(int index);
        
        /**
         * Disable an attribute contained in the array.
         * 
         * @param index
         * The array-index to an attribute that is to be disabled.
         */
        void disableAttrib(int index);
        
        /**
         * Set the memory layout/offset of an attribute in the vertex array.
         * @param index
         * The array attribute to be modified
         * 
         * @param elementsPerVert
         * The number of elements contained within the vertex.
         * 
         * @param type
         * The openGL data type.
         * 
         * @param normalize
         * Should the data be normalized to a float?
         * 
         * @param vertexOffset
         * An offset, in bytes, between each vertex in the array.
         * 
         * @param elementOffset
         * A byte-offset from the start of each vertex.
         * 
         */
        void setAttribOffset(
            unsigned index,
            unsigned elementsPerVert,
            int type,
            bool normalize,
            int vertexOffset,
            const void* elementOffset
        );
        
        /**
         * Get the byte-offset to an element in the array.
         * 
         * @param index
         * An element's index in the array.
         * 
         * @return a byte-offset to the attribute requested.
         */
        void* getAttribOffset(int index) const;
        
        /**
         * Set the rate at which an attribute should repeat during instanced
         * draw calls on the GPU.
         * 
         * @param index
         * The attribute's index within the array.
         * 
         * @param instancesPerAttrib
         * The number of instances that should be drawn before repeating
         * another render of the array attribute.
         */
        void setAttribInstanceRate(int index, int instancesPerAttrib);
        
        /**
         * Bind this vertex array to the current global rendering context.
         */
        void bind() const;
        
        /**
         * Unbind this vertex array object from the current render context.
         */
        void unbind() const;
};

} // end draw namespace
} // end ls namespace

namespace ls {
namespace draw {

/*-------------------------------------
    Terminate the vertex array and release all of its resources back to
    the GPU.
-------------------------------------*/
inline void vertexArray::terminate() {
    glDeleteVertexArrays(1, &vaoId);
    vaoId = 0;
}

/*-------------------------------------
    Determine if there is data used by this object
-------------------------------------*/
inline bool vertexArray::isValid() const {
    return vaoId != 0;
}

/*-------------------------------------
    Get the GPU-assigned ID for this VAO
-------------------------------------*/
inline unsigned vertexArray::getId() const {
    return vaoId;
}

/*-------------------------------------
    Enable an attribute contained within the vertex array.
-------------------------------------*/
inline void vertexArray::enableAttrib(int index) {
    glEnableVertexAttribArray(index);
}

/*-------------------------------------
    Disable an attribute contained in the array.
-------------------------------------*/
inline void vertexArray::disableAttrib(int index) {
    glDisableVertexAttribArray(index);
}

/*-------------------------------------
    Set the memory layout/offset of an attribute in the vertex array.
-------------------------------------*/
inline void vertexArray::setAttribOffset(
    unsigned index,
    unsigned elementsPerVert,
    int type,
    bool normalize,
    int vertexOffset,
    const void* elementOffset
) {
    glVertexAttribPointer(index, elementsPerVert, type, normalize, vertexOffset, elementOffset);
}

/*-------------------------------------
    Get the byte-offset to an element in the array.
-------------------------------------*/
inline void* vertexArray::getAttribOffset(int index) const {
    void* offset;
    glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &offset);
    return offset;
}

/*-------------------------------------
    Set the rate at which an attribute should repeat during instanced
    draw calls on the GPU.
-------------------------------------*/
inline void vertexArray::setAttribInstanceRate(int index, int instancesPerAttrib) {
    glVertexAttribDivisor(index, instancesPerAttrib);
}

/*-------------------------------------
    Bind this vertex array to the current global rendering context.
-------------------------------------*/
inline void vertexArray::bind() const {
    glBindVertexArray(vaoId);
}

/*-------------------------------------
    Unbind this vertex array object from the current render context.
-------------------------------------*/
inline void vertexArray::unbind() const {
    glBindVertexArray(0);
}

} // end draw namespace
} // end ls namespace

#endif	/* __LS_DRAW_VERTEX_ARRAY_H__ */
