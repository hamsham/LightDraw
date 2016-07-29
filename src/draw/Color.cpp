
#include <cassert>
#include <cstdint>

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/Color.h"

namespace ls {

/*-------------------------------------
 * Convert an InternalFormat to a standard format
-------------------------------------*/
draw::pixel_layout_t draw::get_color_layout(const pixel_format_t internalFormat) noexcept {
    switch (internalFormat) {
        case COLOR_FMT_DEPTH_16U:
        case COLOR_FMT_DEPTH_24U:
        case COLOR_FMT_DEPTH_32F:
            return COLOR_LAYOUT_DEPTH;

        case COLOR_FMT_DEPTH_STENCIL_24_8:
        case COLOR_FMT_DEPTH_STENCIL_32_8:
            return COLOR_LAYOUT_DEPTH_STENCIL;

        case COLOR_FMT_R_8:
        case COLOR_FMT_R_16F:
        case COLOR_FMT_R_32F:
            return COLOR_LAYOUT_R;

        case COLOR_FMT_R_8I:
        case COLOR_FMT_R_8U:
        case COLOR_FMT_R_16I:
        case COLOR_FMT_R_16U:
        case COLOR_FMT_R_32I:
        case COLOR_FMT_R_32U:
            return COLOR_LAYOUT_R_I;

        case COLOR_FMT_RG_8:
        case COLOR_FMT_RG_16F:
        case COLOR_FMT_RG_32F:
            return COLOR_LAYOUT_RG;

        case COLOR_FMT_RG_8I:
        case COLOR_FMT_RG_8U:
        case COLOR_FMT_RG_16I:
        case COLOR_FMT_RG_16U:
        case COLOR_FMT_RG_32I:
        case COLOR_FMT_RG_32U:
            return COLOR_LAYOUT_RG_I;

        case COLOR_FMT_RGB_8:
        case COLOR_FMT_RGB_16F:
        case COLOR_FMT_RGB_32F:
            return COLOR_LAYOUT_RGB;

        case COLOR_FMT_RGB_8I:
        case COLOR_FMT_RGB_8U:
        case COLOR_FMT_RGB_16I:
        case COLOR_FMT_RGB_16U:
        case COLOR_FMT_RGB_32I:
        case COLOR_FMT_RGB_32U:
            return COLOR_LAYOUT_RGB_I;

        case COLOR_FMT_RGBA_8:
        case COLOR_FMT_RGBA_16F:
        case COLOR_FMT_RGBA_32F:
            return COLOR_LAYOUT_RGBA;

        case COLOR_FMT_RGBA_8I:
        case COLOR_FMT_RGBA_8U:
        case COLOR_FMT_RGBA_16I:
        case COLOR_FMT_RGBA_16U:
        case COLOR_FMT_RGBA_32I:
        case COLOR_FMT_RGBA_32U:
            return COLOR_LAYOUT_RGBA_I;

            // SRGB & SRGBA
        case COLOR_FMT_SRGB_8:
            return COLOR_LAYOUT_RGB;
        case COLOR_FMT_SRGBA_8:
            return COLOR_LAYOUT_RGBA;

            // Special color formats
        case COLOR_FMT_RGB_565:
        case COLOR_FMT_RGB_111110:
        case COLOR_FMT_RGB_9_E5:
            return COLOR_LAYOUT_RGB;

        case COLOR_FMT_RGBA_5_1:
        case COLOR_FMT_RGBA_10_2:
        case COLOR_FMT_RGBA_4:
            return COLOR_LAYOUT_RGBA;

        case COLOR_FMT_RGBA_10_2U:
            return COLOR_LAYOUT_RGBA_I;

#ifdef LS_DRAW_BACKEND_GL
        case COLOR_FMT_COMPRESSED_RGB:
            return COLOR_LAYOUT_DEFAULT_RGB;

        case COLOR_FMT_COMPRESSED_RGBA:
            return COLOR_LAYOUT_DEFAULT_RGBA;
#endif /* LS_PLATFORM_DESKTOP */
            
        case COLOR_FMT_DEFAULT_RGB:
            return COLOR_LAYOUT_DEFAULT_RGB;

        case COLOR_FMT_DEFAULT_RGBA:
            return COLOR_LAYOUT_DEFAULT_RGBA;

        default:
            LS_DEBUG_ASSERT(false);
            break;
    }

    return COLOR_LAYOUT_INVALID;
}

/*-------------------------------------
 * Convert an InternalFormat to a byte layout
-------------------------------------*/
draw::color_type_t draw::get_color_type(const pixel_format_t internalFormat) noexcept {
    switch (internalFormat) {
            /*
             * Depth, Stencil, and Shadow formats
             */
        case COLOR_FMT_DEPTH_16U:
            return COLOR_TYPE_USHORT;
        case COLOR_FMT_DEPTH_24U:
            return COLOR_TYPE_UINT;
        case COLOR_FMT_DEPTH_32F:
            return COLOR_TYPE_FLOAT;
        case COLOR_FMT_DEPTH_STENCIL_24_8:
            return COLOR_TYPE_UINT_24_8;
        case COLOR_FMT_DEPTH_STENCIL_32_8:
            return COLOR_TYPE_UINT_32F_32I;

            /*
             * Unsigned Formats
             */
        case COLOR_FMT_R_8:
        case COLOR_FMT_R_8U:
        case COLOR_FMT_RG_8:
        case COLOR_FMT_RG_8U:
        case COLOR_FMT_RGB_8:
        case COLOR_FMT_RGB_8U:
        case COLOR_FMT_RGBA_8:
        case COLOR_FMT_RGBA_8U:
            return COLOR_TYPE_UBYTE;

        case COLOR_FMT_R_16U:
        case COLOR_FMT_RG_16U:
        case COLOR_FMT_RGB_16U:
        case COLOR_FMT_RGBA_16U:
            return COLOR_TYPE_USHORT;

        case COLOR_FMT_R_32U:
        case COLOR_FMT_RG_32U:
        case COLOR_FMT_RGB_32U:
        case COLOR_FMT_RGBA_32U:
            return COLOR_TYPE_UINT;

            /*
             * Signed Formats
             */
        case COLOR_FMT_R_8I:
        case COLOR_FMT_RG_8I:
        case COLOR_FMT_RGB_8I:
        case COLOR_FMT_RGBA_8I:
            return COLOR_TYPE_BYTE;

        case COLOR_FMT_R_16I:
        case COLOR_FMT_RG_16I:
        case COLOR_FMT_RGB_16I:
        case COLOR_FMT_RGBA_16I:
            return COLOR_TYPE_SHORT;

        case COLOR_FMT_R_32I:
        case COLOR_FMT_RG_32I:
        case COLOR_FMT_RGB_32I:
        case COLOR_FMT_RGBA_32I:
            return COLOR_TYPE_INT;

            /*
             * Float Formats
             */
        case COLOR_FMT_R_16F:
        case COLOR_FMT_RG_16F:
        case COLOR_FMT_RGB_16F:
        case COLOR_FMT_RGBA_16F:
            return COLOR_TYPE_HALF_FLOAT;

        case COLOR_FMT_R_32F:
        case COLOR_FMT_RG_32F:
        case COLOR_FMT_RGB_32F:
        case COLOR_FMT_RGBA_32F:
            return COLOR_TYPE_FLOAT;

            /*
             * SRGB Color Space
             */
        case COLOR_FMT_SRGB_8:
        case COLOR_FMT_SRGBA_8:
            return COLOR_TYPE_UBYTE;

            /*
             * Special color formats
             */
        case COLOR_FMT_RGBA_5_1:
            return COLOR_TYPE_USHORT_5551;
        case COLOR_FMT_RGBA_4:
            return COLOR_TYPE_USHORT_4444;
        case COLOR_FMT_RGB_565:
            return COLOR_TYPE_USHORT_565;

        case COLOR_FMT_RGB_111110:
            return COLOR_TYPE_UINT_111110F;
        case COLOR_FMT_RGB_9_E5:
            return COLOR_TYPE_UINT_999_E5;
        case COLOR_FMT_RGBA_10_2:
            return COLOR_TYPE_UINT_2101010;
        case COLOR_FMT_RGBA_10_2U:
            return COLOR_TYPE_UINT_2101010;

            /*
             * Edge Cases
             */
#ifdef LS_DRAW_BACKEND_GL
        case COLOR_FMT_COMPRESSED_RGB:
        case COLOR_FMT_COMPRESSED_RGBA:
            return COLOR_TYPE_DEFAULT;
#endif /* LS_PLATFORM_DESKTOP */

            /*
             * Edge Cases
             */
        case COLOR_FMT_DEFAULT_RGB:
        case COLOR_FMT_DEFAULT_RGBA:
            return COLOR_TYPE_DEFAULT;

        default:
            LS_DEBUG_ASSERT(false);
            break;
    }

    return COLOR_TYPE_INVALID;
}

/*-------------------------------------
 * Get the number of components used by a pixel
-------------------------------------*/
unsigned draw::get_num_pixel_components(const pixel_format_t internalFormat) noexcept {
    switch (internalFormat) {
        case COLOR_FMT_DEPTH_STENCIL_24_8:
        case COLOR_FMT_DEPTH_STENCIL_32_8:
            return 2;

        case COLOR_FMT_DEPTH_16U:
        case COLOR_FMT_DEPTH_24U:
        case COLOR_FMT_DEPTH_32F:
        case COLOR_FMT_R_8:
        case COLOR_FMT_R_8I:
        case COLOR_FMT_R_8U:
        case COLOR_FMT_R_16I:
        case COLOR_FMT_R_16U:
        case COLOR_FMT_R_16F:
        case COLOR_FMT_R_32I:
        case COLOR_FMT_R_32U:
        case COLOR_FMT_R_32F:
            return 1;

        case COLOR_FMT_RG_8:
        case COLOR_FMT_RG_8I:
            return 1;
        case COLOR_FMT_RG_8U:
        case COLOR_FMT_RG_16I:
        case COLOR_FMT_RG_16U:
        case COLOR_FMT_RG_16F:
        case COLOR_FMT_RG_32I:
        case COLOR_FMT_RG_32U:
        case COLOR_FMT_RG_32F:
            return 2;

        case COLOR_FMT_RGB_8:
        case COLOR_FMT_RGB_8I:
        case COLOR_FMT_RGB_8U:
        case COLOR_FMT_RGB_16I:
        case COLOR_FMT_RGB_16U:
        case COLOR_FMT_RGB_16F:
        case COLOR_FMT_RGB_32I:
        case COLOR_FMT_RGB_32U:
        case COLOR_FMT_RGB_32F:
            return 3;

        case COLOR_FMT_RGBA_8:
        case COLOR_FMT_RGBA_8I:
        case COLOR_FMT_RGBA_8U:
        case COLOR_FMT_RGBA_16I:
        case COLOR_FMT_RGBA_16U:
        case COLOR_FMT_RGBA_16F:
        case COLOR_FMT_RGBA_32I:
        case COLOR_FMT_RGBA_32U:
        case COLOR_FMT_RGBA_32F:
            return 4;

        case COLOR_FMT_SRGB_8:
            return 3;

        case COLOR_FMT_SRGBA_8:
            return 4;

        case COLOR_FMT_RGB_565:
        case COLOR_FMT_RGB_111110:
        case COLOR_FMT_RGB_9_E5:
            return 3;

        case COLOR_FMT_RGBA_5_1:
        case COLOR_FMT_RGBA_10_2:
        case COLOR_FMT_RGBA_10_2U:
        case COLOR_FMT_RGBA_4:
            return 4;
            
#ifdef LS_DRAW_BACKEND_GL
        case COLOR_FMT_COMPRESSED_RGB:
            return 3;
        case COLOR_FMT_COMPRESSED_RGBA:
            return 4;
#endif /* LS_PLATFORM_DESKTOP */

        case COLOR_FMT_DEFAULT_RGB:
            return 3;

        case COLOR_FMT_DEFAULT_RGBA:
            return 4;

        case COLOR_FMT_INVALID:
        default:
            break;
    }

    return 0;
}

/*-------------------------------------
 * Get the number of bytes occupied by a pixel
-------------------------------------*/
unsigned draw::get_num_color_bytes(const color_type_t colorType) noexcept {
    switch (colorType) {
        case COLOR_TYPE_BYTE:
            return sizeof (int8_t);

        case COLOR_TYPE_UBYTE:
            return sizeof (uint8_t);

        case COLOR_TYPE_SHORT:
            return sizeof (int16_t);

        case COLOR_TYPE_USHORT:
        case COLOR_TYPE_USHORT_565:
        case COLOR_TYPE_USHORT_5551:
        case COLOR_TYPE_USHORT_4444:
            return sizeof (uint16_t);

        case COLOR_TYPE_INT:
            return sizeof (int32_t);

        case COLOR_TYPE_UINT:
        case COLOR_TYPE_UINT_24_8:
        case COLOR_TYPE_UINT_32F_32I:
        case COLOR_TYPE_UINT_111110F:
        case COLOR_TYPE_UINT_999_E5:
        case COLOR_TYPE_UINT_2101010:
            return sizeof (uint32_t);

        case COLOR_TYPE_HALF_FLOAT:
            return sizeof (float) / 2;

        case COLOR_TYPE_FLOAT:
            return sizeof (float);

        case COLOR_TYPE_INVALID:
        default:
            break;
    }

    return 0;
}



} // end ls namespace
