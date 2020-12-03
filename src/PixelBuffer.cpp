
/*
 * File:   PixelBuffer.cpp
 * Author: miles
 *
 * Created on March 3, 2016, 11:02 PM
 */

#include <utility>

#include "lightsky/utils/Assertions.h"

#include "lightsky/draw/PixelBuffer.h"



namespace ls
{
namespace draw
{

/*-------------------------------------
 * Destructor
-------------------------------------*/
PixelBuffer::~PixelBuffer() noexcept
{
}

/*-------------------------------------
-------------------------------------*/
PixelBuffer::PixelBuffer(const pixel_transfer_t mode) noexcept :
    BufferObject{},
    transferMode{mode}
{
}

/*-------------------------------------
-------------------------------------*/
PixelBuffer::PixelBuffer(const PixelBuffer& p) noexcept :
    BufferObject{p},
    transferMode{p.transferMode}
{
}

/*-------------------------------------
-------------------------------------*/
PixelBuffer::PixelBuffer(PixelBuffer&& p) noexcept :
    BufferObject{std::move(p)},
    transferMode{p.transferMode}
{
    p.transferMode = pixel_transfer_t::PIXEL_TRANSFER_UPLOAD;
}

/*-------------------------------------
-------------------------------------*/
PixelBuffer& PixelBuffer::operator=(const PixelBuffer& p) noexcept
{
    BufferObject::operator=(p);
    transferMode = p.transferMode;

    return *this;
}

/*-------------------------------------
-------------------------------------*/
PixelBuffer& PixelBuffer::operator=(PixelBuffer&& p) noexcept
{
    BufferObject::operator=(std::move(p));
    transferMode = p.transferMode;
    p.transferMode = pixel_transfer_t::PIXEL_TRANSFER_UPLOAD;

    return *this;
}

/*-------------------------------------
 * Release all resources used by an inherited object's attributes.
-------------------------------------*/
void PixelBuffer::terminate_attribs() noexcept
{
    transferMode = pixel_transfer_t::PIXEL_TRANSFER_UPLOAD;
}

/*-------------------------------------
 * Copy attributes from the input object into *this.
-------------------------------------*/
bool PixelBuffer::copy_attribs(const BufferObject& from) noexcept
{
    LS_DEBUG_ASSERT(from.get_type() == this->get_type());

    transferMode = static_cast<const PixelBuffer&>(from).transferMode;
    return true;
}

/*-------------------------------------
-------------------------------------*/
void PixelBuffer::set_transfer_mode(const pixel_transfer_t mode) noexcept
{
    LS_DEBUG_ASSERT(
        mode == pixel_transfer_t::PIXEL_TRANSFER_UPLOAD ||
        mode == pixel_transfer_t::PIXEL_TRANSFER_DOWNLOAD
    );

    transferMode = mode;
}
} // end draw namespace
} // end ls namespace
