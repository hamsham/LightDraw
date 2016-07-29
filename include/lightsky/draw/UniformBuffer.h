
#ifndef __LS_DRAW_UNIFORM_BUFFER_H__
#define __LS_DRAW_UNIFORM_BUFFER_H__

#include "lightsky/draw/BufferObject.h"

#include "lightsky/draw/ShaderBlockAttrib.h"



namespace ls {
namespace draw {

/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class ShaderProgram;



/**----------------------------------------------------------------------------
 * Uniform Buffer Class
-----------------------------------------------------------------------------*/
class UniformBuffer : public BufferObject {
    private:
        /**
         * Uniform Buffer Attributes should be initialized from at least one
         * compiled and linked ShaderProgram object. This way, the UBO can have
         * its data shared across shaders with known data layouts.
         */
        ShaderBlockAttrib attribs;
    
    protected:
        /**
         * @brief Release all resources used by an inherited object's
         * attributes.
         */
        virtual void terminate_attribs() noexcept override;

        /**
         * @brief Copy attributes from the input object into *this.
         *
         * Buffer Attributes are a way of describing data within a GPU buffer
         * to client code.
         *
         * @param from
         * A constant reference to another BufferObject which contains one or
         * more attributes to copy.
         *
         * @return TRUE if *this object was able to copy all attributes from
         * the input parameter, FALSE if not.
         */
        virtual bool copy_attribs(const BufferObject& from) noexcept override;
      
    public:
        /**
         * @brief Destructor
         *
         * Releases all CPU-side resources used by *this.
         */
        virtual ~UniformBuffer() noexcept;

        /**
         * @brief Standard Constructor.
         */
        UniformBuffer() noexcept;
        
        /**
         * @brief Copy Constructor
         *
         * Copies all CPU-side data from the input parameter into *this. No
         * data on the GPU is copied in order to avoid hidden costs when *this
         * is used in an STL container.
         *
         * @param ubo
         * A buffer object whose data will be copied into *this object's GPU
         * memory.
         */
        UniformBuffer(const UniformBuffer& ubo)  noexcept;
        
        /**
         * @brief Move Constructor
         *
         * This moves the data contained within the constructor parameter into
         * *this. No copies are performed.
         *
         * @param ubo
         * A buffer object whose data will be moved into *this.
         */
        UniformBuffer(UniformBuffer&& ubo)  noexcept;
        
        /**
         * @brief Copy Operator
         *
         * Copies all CPU-side data from the input parameter into *this. No
         * data on the GPU is copied in order to avoid hidden costs when *this
         * is used in an STL container.
         *
         * @param ubo
         * A buffer object whose data will be copied into *this object's CPU
         * memory.
         *
         * @return A reference to *this.
         */
        UniformBuffer& operator=(const UniformBuffer& ubo) noexcept;
        
        /**
         * @brief Move Operator
         *
         * This moves the data contained within the method parameter into *this.
         * No copies are performed.
         *
         * @param ubo
         * A vertex buffer object whose data will be moved into *this.
         *
         * @return A reference to *this.
         */
        UniformBuffer& operator=(UniformBuffer&& ubo) noexcept;

        /**
         * @brief Retrieve the Usage of *this buffer on the GPU.
         *
         * This should help to determine if an OpenGL Buffer object is a
         * vertex, index, texture, uniform, or buffer object of another type.
         *
         * @return A value from the buffer_use_t enumeration which can help to
         * determine what type of buffer *this is at runtime.
         */
        virtual buffer_use_t get_type() const noexcept override;

        /**
         * @brief Retrieve the number of buffer attributes that are contained
         * within a buffer object.
         *
         * @return An unsigned integral type which contains the number of
         * attributes contained within a buffer object.
         */
        virtual unsigned get_num_attribs() const noexcept override;
        
        /**
         * Setup all internal attributes and memory allocations based on the
         * information retrieved from a ShaderProgram.
         * 
         * Calling this function will discard any previous data which was
         * previously contained in the UBO.
         * 
         * @param p
         * A constant reference to a ShaderProgram object which has been
         * compiled and assembled without error.
         * 
         * @param blockIndex
         * An unsigned integral type, containing the index of the input program's
        *  ShaderBlockAttrib which will be copied into *this.
         * 
         * @return TRUE *this UniformBuffer could successfully be initialized
         * using the data found within the input ShaderProgram object, FALSE
         * if not.
         */
        bool setup_attribs(const ShaderProgram& p, const unsigned blockIndex) noexcept;
        
        /**
         * Retrieve a reference to the uniform attributes contained within
         * *this.
         * 
         * @return A constant reference to the ShaderBlockAttribs in *this.
         */
        const ShaderBlockAttrib& get_attribs() const noexcept;
        
        /**
         * Manually assign a shader block index to *this UBO's internal block
         * attribute member.
         * 
         * @param blockIndex
         * An unsigned integral type which can help *this reference a specific
         * shader uniform block index in GLSL.
         */
        void set_block_index(const unsigned blockIndex) noexcept;
        
        /**
         * Retrieve the current uniform block index which *this UBO references.
         * 
         * By default, this should return 0;
         * 
         * @return An unsigned integral type which can be used to determine
         * what uniform block index *this UBO references in GLSL.
         */
        unsigned get_block_index() const noexcept;
        
        /**
         * Manually assign a shader block binding slot to *this UBO's internal
         * block attribute member.
         * 
         * @param bindSlot
         * An unsigned integral type which can help *this reference a specific
         * shader uniform block bins-slot in GLSL.
         */
        void set_block_binding(const unsigned bindSlot) noexcept;
        
        /**
         * Retrieve the current uniform block binding which *this UBO
         * references.
         * 
         * By default, this should return 0;
         * 
         * @return An unsigned integral type which can be used to determine
         * what uniform block binding slot *this UBO references in GLSL.
         */
        unsigned get_block_binding() const noexcept;
};

/*-------------------------------------
 * Terminate all internal attributes
-------------------------------------*/
inline void UniformBuffer::terminate_attribs() noexcept {
    attribs.reset();
}

/*-------------------------------------
 * Get the internal buffer type
-------------------------------------*/
inline buffer_use_t UniformBuffer::get_type() const noexcept {
    return buffer_use_t::VBO_BUFFER_UNIFORM_BUFFER;
}

/*-------------------------------------
 * Retrieve the number of attributes in *this.
-------------------------------------*/
inline unsigned UniformBuffer::get_num_attribs() const noexcept {
    return attribs.get_num_members();
}

/*-------------------------------------
 * Retrieve the attributes in *this.
-------------------------------------*/
inline const ShaderBlockAttrib& UniformBuffer::get_attribs() const noexcept {
    return attribs;
}

/*-------------------------------------
 * Manually assign a block index
-------------------------------------*/
inline void UniformBuffer::set_block_index(const unsigned blockIndex) noexcept {
    attribs.index = blockIndex;
}

/*-------------------------------------
 * retrieve the current block index
-------------------------------------*/
inline unsigned UniformBuffer::get_block_index() const noexcept {
    return attribs.index;
}

/*-------------------------------------
 * override the current block binding
-------------------------------------*/
inline void UniformBuffer::set_block_binding(const unsigned bindSlot) noexcept {
    attribs.binding = bindSlot;
}

/*-------------------------------------
 * retrieve the current bind slot
-------------------------------------*/
inline unsigned UniformBuffer::get_block_binding() const noexcept {
    return attribs.binding;
}



} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_UNIFORM_BUFFER_H__ */
