/*
 * File:   ImageBuffer.h
 * Author: Miles Lacey
 *
 * Created on February 2, 2014, 1:42 PM
 */

#ifndef __LS_DRAW_IMAGE_RESOURCE_H__
#define __LS_DRAW_IMAGE_RESOURCE_H__

#include <string>

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/Setup.h"
#include "lightsky/draw/Color.h"



/*-----------------------------------------------------------------------------
 * Forward declarations
-----------------------------------------------------------------------------*/
struct FIBITMAP;



/*-----------------------------------------------------------------------------
 * LS Draw namespace
-----------------------------------------------------------------------------*/
namespace ls {
namespace draw {



/**----------------------------------------------------------------------------
 * Enumerations for saving image files.
-----------------------------------------------------------------------------*/
enum class img_file_t {
    IMG_FILE_BMP,
    IMG_FILE_EXR,
    IMG_FILE_GIF,
    IMG_FILE_HDR,
    IMG_FILE_ICO,
    IMG_FILE_JPG,
    IMG_FILE_J2K,
    IMG_FILE_PNG, // default
    IMG_FILE_PPM,
    IMG_FILE_TGA,
    IMG_FILE_TIF,
    IMG_FILE_WBP,
    IMG_FILE_XPM
};



/**----------------------------------------------------------------------------
 * @brief A resource object to load image files.
 *
 * This class can load images using the FreeImage library, The pData member
 * contains raw texture data. It has been marked final until another otherwise
 * required.
 *
 * The inherited member "pData" will be reinterpreted as a pointer to a
 * FIBITMAP structure so as to make loading and saving easier to maintain.
-----------------------------------------------------------------------------*/
class ImageBuffer final {

    // Public static interfaces
    public:
        enum class img_status_t {
            FILE_LOAD_SUCCESS       = 0,
            FILE_NOT_FOUND          = -1,
            INVALID_FILE_NAME       = -2,
            INVALID_FILE_TYPE       = -3,
            UNSUPPORTED_FILE_TYPE   = -4,
            UNSUPPORTED_FORMAT      = -5,
            INTERNAL_ERROR          = -6
        };


    private:
        FIBITMAP* pImage;
        
        /**
         * imgSize represents the pixel width & height of a loaded image
         */
        math::vec3i imgSize;

        /**
         * Pixel format of the loaded image
         */
        color_type_t pixelType;

        /**
         * Number of bits per pixel in the image
         */
        unsigned bitsPerPixel = 0;

        /**
         * @brief CPU-Side format of an image
         *
         * This can be any of the following:
         * LS_GRAY_N
         * LS_RG_N
         * LS_RGB_N
         * LS_RGBA_N
         * LS_RGB_32F
         * LS_RGBA_32F
         * where N is 8, 16, or 32
         */
        pixel_format_t intFormat;

        /**
         * @brief CPU-Side format of an image
         *
         * This can be any of the following:
         * LS_GRAY
         * LS_RG
         * LS_RGB
         * LS_RGBA
         * LS_RGB16F
         * LS_RGBA32F
         * LS_RGB16F
         * LS_RGBA32F
         */
        pixel_layout_t extFormat;

    public:
        /**
         * @brief Constructor
         */
        ImageBuffer();

        /**
         * @brief Copy Constructor
         * 
         * Copies all image data from the input parameter into *this.
         * 
         * @param ib
         * A constant reference to another ImageBuffer object which will have all
         * of its data copied into *this.
         */
        ImageBuffer(const ImageBuffer& ib);

        /**
         * @brief Move Operator
         *
         * Moves data from the source operand into *this. No copies are
         * performed.
         *
         * @param ib
         * An r-value reference to a temporary image resource object.
         */
        ImageBuffer(ImageBuffer&& ib);

        /**
         * @brief Destructor
         *
         * Calls "unload()" and releases all memory from *this.
         */
        ~ImageBuffer();

        /**
         * @brief Copy Operator
         * 
         * Copies all image data from the input parameter into *this.
         * 
         * @param ib
         * A constant reference to another ImageBuffer object which will have all
         * of its data copied into *this.
         * 
         * @return
         * A reference to *this.
         */
        ImageBuffer& operator=(const ImageBuffer& ib);

        /**
         * @brief Move Operator
         *
         * Moves all data from the source operand into *this. No copies are
         * performed.
         *
         * @param ib
         * An r-value reference to a temporary image resource object.
         *
         * @return a reference to *this.
         */
        ImageBuffer& operator=(ImageBuffer&& ib);
        
        /**
         * Retrieve the total number of bytes contained within *this object's
         * internal buffer.
         * 
         * @return An unsigned integral type, containing the number of bytes
         * used by *this to hold an image in memory. A value of 0u is returned
         * if no data is being managed by *this.
         */
        unsigned get_num_bytes() const;

        /**
         * @brief Load an image file
         *
         * @param filename
         * A string object containing the relative path name to a file that
         * should be loadable into memory.
         *
         * @return true if the file was successfully loaded. False if not.
         */
        img_status_t load_file(const std::string& filename);

        /**
         * @brief Save an image file in a specific format
         *
         * @param filename
         * A string object containing the relative path name to a file that
         * should be saved to the computer.
         *
         * @param filetype
         * An img_file_t, representing the file format that should be used when
         * saving image data.
         *
         * @return true if the file was successfully saved. False if not.
         */
        bool save_file(
            const std::string& filename,
            img_file_t filetype = img_file_t::IMG_FILE_PNG
        ) const;

        /**
         * @brief Unload
         *
         * Free all memory used by *this.
         */
        void unload();

        /**
         * Get the raw, loaded, image data contained within *this.
         * The underlying type has been cast from a FreeImage FIBITMAP*.
         *
         * @return a void pointer to the raw image bits.
         */
        const void* get_data() const;

        /**
         * Get the pixel size of the currently loaded image
         *
         * @return a 2D integer vector containing the width and height of the
         * loaded image, in pixels.
         */
        const math::vec3i& get_pixel_size() const;

        /**
         * Get the format of the currently loaded image.
         * For example, LS_UNSIGNED_BYTE, LS_INT, LS_FLOAT, etc.
         */
        color_type_t get_pixel_type() const;

        /**
         * Get the number of bits per pixel in the image.
         *
         * @return 0, 1, 2, 4, 8, 16, 24, 32, 48, 64, 96, or 128
         */
        unsigned get_bpp() const;

        /**
         * Get the OpenGL-compatible CPU-Side image format.
         *
         * @return An enumeration containing image format information that can
         * be used when setting up textures in OpenGL.
         */
        pixel_format_t get_internal_format() const;

        /**
         * Get the OpenGL-compatible CPU-Side/GPU-Side image format pair.
         *
         * @return An enumeration containing image format information that can
         * be used when setting up textures in OpenGL.
         */
        pixel_layout_t get_external_format() const;
};



/*-------------------------------------
 * Get the pixel size of the currently loaded image
-------------------------------------*/
inline const math::vec3i& ImageBuffer::get_pixel_size() const {
    return imgSize;
}



/*-------------------------------------
 * Get the GPU-compatible format of the currently loaded image
-------------------------------------*/
inline color_type_t ImageBuffer::get_pixel_type() const {
    return pixelType;
}



/*-------------------------------------
 * Get the number of bits per pixel in the image.
-------------------------------------*/
inline unsigned ImageBuffer::get_bpp() const {
    return bitsPerPixel;
}



/*-------------------------------------
 * Get the OpenGL-compatible CPU-Side image format.
-------------------------------------*/
inline pixel_format_t ImageBuffer::get_internal_format() const {
    return intFormat;
}



/*-------------------------------------
 * Get the OpenGL-compatible GPU-Side image format.
-------------------------------------*/
inline pixel_layout_t ImageBuffer::get_external_format() const {
    return extFormat;
}



} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_IMAGE_RESOURCE_H__ */
