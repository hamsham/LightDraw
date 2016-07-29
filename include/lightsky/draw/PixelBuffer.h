
/*
 * File:   PixelBuffer.h
 * Author: miles
 *
 * Created on March 3, 2016, 11:02 PM
 */

#ifndef __LS_DRAW_PIXEL_BUFFER_H__
#define __LS_DRAW_PIXEL_BUFFER_H__

#include "ls/draw/BufferObject.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Enumerations
-----------------------------------------------------------------------------*/
enum pixel_transfer_t : GLenum {
    PIXEL_TRANSFER_UPLOAD = buffer_use_t::VBO_BUFFER_PIXEL_UPLOAD,
    PIXEL_TRANSFER_DOWNLOAD = buffer_use_t::VBO_BUFFER_PIXEL_DOWNLOAD,
};



/**----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
class PixelBuffer : public BufferObject {
  private:
    /**
     * Member to keep track of whether *this PBO is uploading or downloading data
     * from the GPU.
     */
    pixel_transfer_t transferMode;

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
     * Destructor
     *
     * Cleans up all CPU-side data from *this.
     *
     * No GPU-side data will be deleted unless an explicit call to *terminate is
     * made in client code.
     */
    virtual ~PixelBuffer() noexcept;

    /**
     * Constructor
     *
     * @param mode
     * A value from the pixel_transfer_t enumeration which determines if *this
     * PBO should be used for uploading or downloading data from the GPU,
     */
    PixelBuffer(pixel_transfer_t mode = pixel_transfer_t::PIXEL_TRANSFER_UPLOAD) noexcept;

    /**
     * Copy Constructor
     *
     * Copies data from the input parameter into *this.
     *
     * @param p
     * A pixel buffer object whose data will be copied into *this.
     */
    PixelBuffer(const PixelBuffer& p) noexcept;

    /**
     * Move Constructor
     *
     * Move data from the input parameter into *this.
     *
     * @param p
     * A pixel buffer object whose data will be moved into *this.
     */
    PixelBuffer(PixelBuffer&& p) noexcept;

    /**
     * Copy Operator
     *
     * Copies data from the input parameter into *this.
     *
     * @param p
     * A pixel buffer object whose data will be copied into *this.
     *
     * @return A reference to *this.
     */
    PixelBuffer& operator=(const PixelBuffer& p) noexcept;

    /**
     * Move Operator
     *
     * Move data from the input parameter into *this.
     *
     * @param p
     * A pixel buffer object whose data will be moved into *this.
     *
     * @return A reference to *this.
     */
    PixelBuffer& operator=(PixelBuffer&& p) noexcept;

    /**
     * Set a flag to determine if *this should upload data to or download data
     * from OpenGL.
     *
     * @param mode
     * A value from the pixel_transfer enumeration which can be used to determine
     * if pixel data should upload to or download from OpenGL.
     */
    void set_transfer_mode(const pixel_transfer_t mode) noexcept;

    /**
     * Retrieve the transfer mode of a pixel buffer object.
     *
     * This can be used to determine if a PBO is uploading data to OpenGL or
     * downloading data to the CPU.
     *
     * @return A value from the pixel_transfer enumeration which can be used to
     * determine if pixel data is uploading to or downloading from OpenGL.
     */
    pixel_transfer_t get_transfer_mode() const noexcept;

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
};

/*-------------------------------------
-------------------------------------*/
inline buffer_use_t PixelBuffer::get_type() const noexcept {
    return (buffer_use_t) transferMode;
}

/*-------------------------------------
-------------------------------------*/
inline pixel_transfer_t PixelBuffer::get_transfer_mode() const noexcept {
    return transferMode;
}

/*-------------------------------------
-------------------------------------*/
inline unsigned PixelBuffer::get_num_attribs() const noexcept {
    return 0;
}


} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_PIXEL_BUFFER_H__ */
