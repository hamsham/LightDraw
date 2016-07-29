/*
 * File:   imageResource.h
 * Author: Miles Lacey
 *
 * Created on February 2, 2014, 1:42 PM
 */

#ifndef __LS_DRAW_IMAGE_RESOURCE_H__
#define __LS_DRAW_IMAGE_RESOURCE_H__

#include <string>

#include "ls/draw/Setup.h"
#include "ls/utils/Resource.h"
#include "ls/draw/Color.h"

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
 * contains raw texture data.
 *
 * The inherited member "pData" will be reinterpreted as a pointer to a
 * FIBITMAP structure so as to make loading and saving easier to maintain.
-----------------------------------------------------------------------------*/
class ImageResource final : public ls::utils::Resource {
  private:
    /**
     * imgSize represents the pixel width & height of a loaded image
     */
    math::vec2i imgSize = {0};

    /**
     * Pixel format of the loaded image
     */
    color_type_t pixelType = COLOR_TYPE_DEFAULT;

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
    pixel_format_t intFormat = COLOR_FMT_DEFAULT_RGB;


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
    pixel_layout_t extFormat = COLOR_LAYOUT_DEFAULT_RGB;

  public:
    /**
     * @brief Constructor
     */
    ImageResource();

    /**
     * @brief Copy Constructor -- DELETED
     */
    ImageResource(const ImageResource&) = delete;

    /**
     * @brief Move Operator
     *
     * Moves data from the source operand into *this. No copies are
     * performed.
     *
     * @param ir
     * An r-value reference to a temporary image resource object.
     */
    ImageResource(ImageResource&& ir);

    /**
     * @brief Destructor
     *
     * Calls "unload()" and releases all memory from *this.
     */
    virtual ~ImageResource();

    /**
     * @brief Copy Operator -- DELETED
     */
    ImageResource& operator=(const ImageResource&) = delete;

    /**
     * @brief Move Operator
     *
     * Moves all data from the source operand into *this. No copies are
     * performed.
     *
     * @param ir
     * An r-value reference to a temporary image resource object.
     *
     * @return a reference to *this.
     */
    ImageResource& operator=(ImageResource&& ir);

    /**
     * @brief Load an image file
     *
     * @param filename
     * A string object containing the relative path name to a file that
     * should be loadable into memory.
     *
     * @return true if the file was successfully loaded. False if not.
     */
    virtual bool load_file(const std::string& filename) override;

    /**
     * @brief Save an image file
     *
     * @param filename
     * A string object containing the relative path name to a file that
     * should be saved to the computer.
     *
     * @return true if the file was successfully saved. False if not.
     */
    virtual bool save_file(const std::string& filename) const override;

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
    bool save_file(const std::string& filename, img_file_t filetype) const;

    /**
     * @brief Save an image file in a specific format
     *
     * @param filename
     * A wide string object containing the relative path name to a file that
     * should be saved to the computer.
     *
     * @param filetype
     * An img_file_t, representing the file format that should be used when
     * saving image data.
     *
     * @return true if the file was successfully saved. False if not.
     */
    virtual bool save_file(const std::wstring& filename) const override;

    /**
     * @brief Unload
     *
     * Free all memory used by *this.
     */
    virtual void unload() override;

    /**
     * Get the raw, loaded, image data contained within *this.
     * The underlying type has been cast from a FreeImage FIBITMAP*.
     *
     * @return a void pointer to the raw image bits.
     */
    virtual void* get_data() const override;

    /**
     * Get the pixel size of the currently loaded image
     *
     * @return a 2D integer vector containing the width and height of the
     * loaded image, in pixels.
     */
    math::vec2i get_pixel_size() const;

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
 * Saving Data
-------------------------------------*/
inline bool ImageResource::save_file(const std::string& filename) const {
    return save_file(filename, img_file_t::IMG_FILE_PNG);
}

/*-------------------------------------
 * Saving Data (wide-string)
-------------------------------------*/
inline bool ImageResource::save_file(const std::wstring& filename) const {
    const std::string&& file = ls::utils::wide_to_mb_string(filename);
    return save_file(file, img_file_t::IMG_FILE_PNG);
}

/*-------------------------------------
 * Get the pixel size of the currently loaded image
-------------------------------------*/
inline math::vec2i ImageResource::get_pixel_size() const {
    return imgSize;
}

/*-------------------------------------
 * Get the GPU-compatible format of the currently loaded image
-------------------------------------*/
inline color_type_t ImageResource::get_pixel_type() const {
    return pixelType;
}

/*-------------------------------------
 * Get the number of bits per pixel in the image.
-------------------------------------*/
inline unsigned ImageResource::get_bpp() const {
    return bitsPerPixel;
}

/*-------------------------------------
 * Get the OpenGL-compatible CPU-Side image format.
-------------------------------------*/
inline pixel_format_t ImageResource::get_internal_format() const {
    return intFormat;
}

/*-------------------------------------
 * Get the OpenGL-compatible GPU-Side image format.
-------------------------------------*/
inline pixel_layout_t ImageResource::get_external_format() const {
    return extFormat;
}

} // end draw namespace
} // end ls namespace

#endif  /* __LS_DRAW_IMAGE_RESOURCE_H__ */
