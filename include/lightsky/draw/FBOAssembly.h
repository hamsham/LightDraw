
/*
 * File:   FBOAssembly.h
 * Author: miles
 *
 * Created on March 4, 2016, 4:30 PM
 */

#ifndef __LS_DRAW_FBO_ASSEMBLY_H__
#define __LS_DRAW_FBO_ASSEMBLY_H__

#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/FBOAttrib.h"

namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * Forward Declarations
-----------------------------------------------------------------------------*/
class FrameBuffer;



/**----------------------------------------------------------------------------
 * Framebuffer Object assembly
-----------------------------------------------------------------------------*/
class FBOAssembly {
  private:
    FBOAttrib attribs[fbo_attach_t::FBO_ATTACHMENT_MAX];

    /**
     * Reorder the internal FBO attributes so all non-NULL attrib pointers in
     * "texAttribs" are at the front of the array.
     *
     * Only values within the "texAttribs" array that are not NULL are
     * bound to the output FBO. To make management of internal resources
     * easier, all non-NULL FBO pointers within "texAttribs" are packed
     * into the front of the array.
     */
    void pack_vbo_attribs() noexcept;

    /**
     * Retrieve the index of an FBOAttrib "sentry". This index can be used to
     * determine how many attributes in *this are capable of being validated.
     *
     * All current attributes will be validated before this function returns,
     * moving invalid attributes to the back of the internal attrib array if
     * necessary.
     *
     * @return An unsigned integer containing the number of attributes which
     * can currently be validated and are held at the front of the internal
     * FBO attribute array.
     */
    unsigned get_num_verifiable_attribs() const noexcept;

    /**
     * Determine if an FBOAttrib has a valid render target.
     *
     * @param a
     * A constant reference to an FBOAttrib object to be validated.
     *
     * @return TRUE if the data within an attribute references a non-null,
     * initialized texture or renderbuffer, FALSE if not.
     */
    static bool attrib_has_targets(const FBOAttrib& a) noexcept;

    /**
     * Determine if an FBOAttrib has a valid color attachment.
     *
     * @param a
     * A constant reference to an FBOAttrib object to be validated.
     *
     * @return TRUE if the FBO color attachment within the input parameter is
     * valid, FALSE if not.
     */
    static bool attrib_has_attachments(const FBOAttrib& a) noexcept;

    /**
     * Attach a texture to the currently bound framebuffer
     *
     * @param attrib
     * A constant reference to an FBOAttrib object which contains texture data
     * to attach to the framebuffer which is currently being assembled.
     *
     * @return TRUE if the texture could successfully be attached, FALSE if
     * not.
     */
    bool attach_target_texture(const FBOAttrib& attrib) const noexcept;

    /**
     * Attach a render buffer to the currently bound framebuffer
     *
     * @param attrib
     * A constant reference to an FBOAttrib object which contains render buffer
     * data to attach to the framebuffer which is currently being assembled.
     *
     * @return TRUE if the render buffer could successfully be attached, FALSE
     * if not.
     */
    bool attach_target_renderbuffer(const FBOAttrib& attrib) const noexcept;

    /**
     * Determine if a framebuffer has successfully been created by querying
     * OpenGL for its completion status.
     *
     * @param fboId
     * A handle to the current framebuffer object being assembled.
     *
     * @return TRUE if the framebuffer could be completed, FALSE if not.
     */
    bool check_fbo_completion(const unsigned fboId) const noexcept;

  public:
    /**
     * Destructor
     */
    ~FBOAssembly() noexcept;

    /**
     * Constructor
     */
    FBOAssembly() noexcept;

    /**
     * Copy Constructor
     *
     * Copies data from the input parameter into *this.
     *
     * @param a
     * A constant l-value reference to another FBOAssembly who's data will be
     * copied into *this.
     */
    FBOAssembly(const FBOAssembly& a) noexcept;

    /**
     * Move Constructor
     *
     * Moves data from the input parameter into *this and leaves the input
     * parameter in its default state.
     *
     * @param a
     * An r-value reference to another FBOAssembly who's data will be moved
     * into *this.
     */
    FBOAssembly(FBOAssembly&& a) noexcept;

    /**
     * Copy Operator
     *
     * Copies data from the input parameter into *this.
     *
     * @param a
     * A constant l-value reference to another FBOAssembly who's data will be
     * copied into *this.
     *
     * @return A reference to *this.
     */
    FBOAssembly& operator=(const FBOAssembly& a) noexcept;

    /**
     * Move Operator
     *
     * Moves data from the input parameter into *this and leaves the input
     * parameter in its default state.
     *
     * @param a
     * An r-value reference to another FBOAssembly who's data will be moved
     * into *this.
     *
     * @return A reference to *this.
     */
    FBOAssembly& operator=(FBOAssembly&& a) noexcept;

    /**
     * Retrieve the internal list of FBO attributes form *this.
     *
     * @return A pointer to a constant array of FBO attributes.
     */
    const FBOAttrib* get_attribs() const noexcept;

    /**
     * Retrieve the number of attributes currently reserved in the internal
     * attribute array.
     *
     * @return An unsigned integral type which contains the number of FBO
     * attributes currently reserved in *this.
     */
    unsigned get_num_attribs() const noexcept;

    /**
     * Assign an internal attribute to a preexisting texture object.
     *
     * For easier management of internal data, it's possible the requested
     * attribute will be moved to a position in the internal FBOAttrib array
     * with a lower index. Valid attributes are kept at the front of the attrib
     * array while invalid attributes remain at the back.
     *
     * @param attribIndex
     * An array index which will be used to modify a single FBO attribute in
     * *this.
     *
     * @param attachment
     * A reference to a texture object which will replace the internal
     * FBOAttrib object located at the requested index.
     *
     * @param type
     * A value from the fbo_attach_t enumeration which will determine the type
     * of color attachment to pair with the input texture object.
     *
     * @return TRUE if the assigned internal attrib was set to the one passed
     * into the function, FALSE if the requested attribute index is outside of
     * the internal attrib array's range or the input texture is not valid.
     */
    bool set_attrib(const unsigned attribIndex, Texture& attachment, const fbo_attach_t type) noexcept;

    /**
     * Assign an internal attribute to a preexisting render buffer.
     *
     * For easier management of internal data, it's possible the requested
     * attribute will be moved to a position in the internal FBOAttrib array
     * with a lower index. Valid attributes are kept at the front of the attrib
     * array while invalid attributes remain at the back.
     *
     * @param attribIndex
     * An array index which will be used to modify a single FBO attribute in
     * *this.
     *
     * @param attachment
     * A reference to a render buffer object which will replace the internal
     * FBOAttrib object located at the requested index.
     *
     * @param type
     * A value from the fbo_attach_t enumeration which will determine the type
     * of color attachment to pair with the input render buffer.
     *
     * @return TRUE if the assigned internal attrib was set to the one passed
     * into the function, FALSE if the requested attribute index is outside of
     * the internal attrib array's range or the input RBO is not valid.
     */
    bool set_attrib(const unsigned attribIndex, RenderBuffer& attachment, const fbo_attach_t type) noexcept;

    /**
     * Assign an internal attribute to one managed by client code.
     *
     * For easier management of internal data, it's possible the requested
     * attribute will be moved to a position in the internal FBOAttrib array
     * with a lower index. Valid attributes are kept at the front of the attrib
     * array while invalid attributes remain at the back.
     *
     * @param attribIndex
     * An array index which will be used to modify a single FBO attribute in
     * *this.
     *
     * @param attachment
     * A constant reference to an FBO attribute which will replace the internal
     * one located at the requested index.
     *
     * @return TRUE if the assigned internal attrib was set to the one passed
     * into the function, FALSE if the requested attribute index is outside of
     * the internal attrib array's range.
     */
    bool set_attrib(const unsigned attribIndex, const FBOAttrib& attachment) noexcept;

    /**
     * Retrieve information about a single attribute in *this.
     *
     * @param attribIndex
     * An array index which will be used to query a single FBO attribute in
     * *this.
     *
     * @return A constant referent to an FBO attachment contained in *this.
     * An assertion will be raised if the requested attribute is outside of
     * the current assembly's array of FBO attributes.
     */
    const FBOAttrib& get_attrib(const unsigned attribIndex) const noexcept;

    /**
     * Retrieve the attachment type of a single attrib in *this.
     *
     * @param attribIndex
     * An array index which will be used to query a single FBO attribute in
     * *this.
     *
     * @return A value from the fbo_attach_t enumeration which can be used to
     * determine the color attachment for a single FBO attribute.
     */
    fbo_attach_t get_attrib_attach_type(const unsigned attribIndex) const noexcept;

    /**
     * Set the color attachment type for a single FBO attribute in *this.
     *
     * This function does not check for setting duplicate attributes.
     *
     * @param attribIndex
     * An array index which will be used to modify a single FBO attribute in
     * *this.
     *
     * @param type
     * A value from the fbo_attach_t enumeration which will set the color
     * attachment type of a single attribute in *this.
     *
     * @return TRUE if the selected attribute was successfully able to set its
     * color attachment to the valid type requested, FALSE if not.
     */
    bool set_attrib_attach_type(const unsigned attribIndex, const fbo_attach_t type) noexcept;

    /**
     * Determine the target type of a particular attribute in *this.
     *
     * @param attribIndex
     * An array index which will be used to query a single FBO attribute in
     * *this.
     *
     * @return A vallue from the fbo_target_t enumeration which can be used to
     * determine if the requested attrib is a texture or render buffer object.
     */
    fbo_target_t get_attrib_target_type(const unsigned attribIndex) const noexcept;

    /**
     * Determine if a particular FBO attribute is valid or invalid.
     *
     * This test ensures the requested attribute has a valid texture or RBO
     * object, a valid color/depth/stencil attachment, and is not a duplicate
     * of another attribute.
     *
     * @param attribIndex
     * An array index which will be used to select a single FBO attribute in
     * *this to validate.
     *
     * @return TRUE if the requested FBO attrib is valid, FALSE if not.
     */
    bool is_attrib_valid(const unsigned attribIndex) const noexcept;

    /**
     * Remove all texture, render buffer, and FBOAttrib references from *this.
     */
    void clear() noexcept;

    /**
     * Determine if this assembly has enough information in order to assemble a
     * valid framebuffer object.
     *
     * @return TRUE if *this assembly can be used to create a valid framebuffer
     * object, given its current state, FALSE if not.
     */
    bool is_assembly_valid() const noexcept;

    /**
     * Assemble a framebuffer object using all of the internal attachments.
     *
     * @param fbo
     * A reference to a framebuffer object who's internal attributes are to be
     * assembled into a valid FBO within OpenGL.
     *
     * @return TRUE if the output FBO was able to be assembled into a valid
     * framebuffer object within OpenGL, FALSE if not.
     */
    bool assemble(FrameBuffer& fbo) const noexcept;
};

/*-------------------------------------
 * Retrieve the current list of FBO attributes
-------------------------------------*/
inline const FBOAttrib* FBOAssembly::get_attribs() const noexcept {
    return attribs;
}

/*-------------------------------------
 * Retrieve the total number of attributes held by the internal attrib array.
-------------------------------------*/
inline unsigned FBOAssembly::get_num_attribs() const noexcept {
    return fbo_attach_t::FBO_ATTACHMENT_MAX;
}



} // end draw namespace
} // end ls namespace


#endif /* __LS_DRAW_FBO_ASSEMBLY_H__ */
