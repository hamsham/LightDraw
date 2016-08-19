
#ifndef __LS_DRAW_PACKED_VERTEX_H__
#define __LS_DRAW_PACKED_VERTEX_H__

#include "lightsky/utils/Endian.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
template <utils::endian_t>
struct PackedVertex_2_10_10_10;

/*-------------------------------------
 * Vertex Packing Structure
-------------------------------------*/
typedef PackedVertex_2_10_10_10<utils::get_endian_order()> PackedVertex;



/*-----------------------------------------------------------------------------
 * Vertex Packing Structure Template
-----------------------------------------------------------------------------*/
template <utils::endian_t>
struct PackedVertex_2_10_10_10 {
    // "Unknwown endianness being used. Please update the vertex normal packing structure.
};



/*-------------------------------------
 * Little-Endian Version
-------------------------------------*/
template <>
struct alignas(sizeof(int32_t)) PackedVertex_2_10_10_10<utils::endian_t::LS_LITTLE_ENDIAN> {
    int32_t x: 10;
    int32_t y: 10;
    int32_t z: 10;
    int32_t w: 2;
    
    constexpr PackedVertex_2_10_10_10(const math::vec3& v) noexcept :
        x{(int32_t)(v[0] * 511.f)},
        y{(int32_t)(v[1] * 511.f)},
        z{(int32_t)(v[2] * 511.f)},
        w{0}
    {}
    
    explicit inline operator int32_t() const noexcept {
        return *reinterpret_cast<const int32_t*>(this);
    }
};



/*-------------------------------------
 * Big-Endian Version
-------------------------------------*/
template <>
struct alignas(sizeof(int32_t)) PackedVertex_2_10_10_10<utils::endian_t::LS_BIG_ENDIAN> {
    int32_t w: 2;
    int32_t z: 10;
    int32_t y: 10;
    int32_t x: 10;
    
    constexpr PackedVertex_2_10_10_10(const math::vec3& v) noexcept :
        w{0},
        z{(int32_t)(v[2] * 511.f)},
        y{(int32_t)(v[1] * 511.f)},
        x{(int32_t)(v[0] * 511.f)}
    {}
    
    explicit inline operator int32_t() const noexcept {
        return *reinterpret_cast<const int32_t*>(this);
    }
};



/*-------------------------------------
 * Big-Endian Version
-------------------------------------*/
template <>
struct alignas(sizeof(int32_t)) PackedVertex_2_10_10_10<utils::endian_t::LS_PDP_ENDIAN> {
    int32_t y: 10;
    int32_t x: 10;
    int32_t w: 2;
    int32_t z: 10;
    
    // Double-check on ARM, this may be backwards
    constexpr PackedVertex_2_10_10_10(const math::vec3& v) noexcept :
        y{(int32_t)(v[1] * 511.f)},
        x{(int32_t)(v[0] * 511.f)},
        w{0},
        z{(int32_t)(v[2] * 511.f)}
    {}
    
    explicit inline operator int32_t() const noexcept {
        return *reinterpret_cast<const int32_t*>(this);
    }
};



/**------------------------------------
 * @brief Convert a 3-dimensional vertex normal to a packed vertex normal,
 * following the VERTEX_DATA_2_10I format or similar.
 *
 * @param norm
 * A constant reference to a normalized vector within the range of [-1, 1],
 * inclusive,
 *
 * @return A signed 32-bit integer containing a vertex normal with data in the
 * range of [-2^10, 2^10].
-------------------------------------*/
inline int32_t pack_vertex_normal(const math::vec3& norm) noexcept {
    // Keeping this commented-out for now. It seems that using this method will
    // cause normals pointing towards the negative X-direction to ONLY point in
    // the negative X-direction, losing all information related to Y and Z.
    /*
    return 0
    | (math::scale_num_to_range<float, int32_t>(norm[0], -1.f, 1.f, -512, 511) << 0)
    | (math::scale_num_to_range<float, int32_t>(norm[1], -1.f, 1.f, -512, 511) << 10)
    | (math::scale_num_to_range<float, int32_t>(norm[2], -1.f, 1.f, -512, 511) << 20);
    */
    
    // Even though we're specifying endianness, it should still be worth
    // investigating portability issues on different ARM processors since they
    // can be either little or big endian modes.
    
    PackedVertex pn{norm};
    
    // undefined behavior FTW.
    return (int32_t)pn;
}




} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_PACKED_VERTEX_H__ */
