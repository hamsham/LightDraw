
/*
 * File:   GLContext.h
 * Author: miles
 *
 * Created on March 8, 2016, 3:12 PM
 */

#ifndef __LS_DRAW_GL_CONTEXT_H__
#define __LS_DRAW_GL_CONTEXT_H__

#include <utility> // std::move
#include <vector>

#include "lightsky/setup/Macros.h"

#include "lightsky/utils/Assertions.h"
#include "lightsky/utils/Pointer.h"

#include "lightsky/draw/BlendObject.h"
#include "lightsky/draw/DepthObject.h"

#include "lightsky/draw/BufferObject.h"
#include "lightsky/draw/FrameBuffer.h"
#include "lightsky/draw/IndexBuffer.h"
#include "lightsky/draw/PixelBuffer.h"
#include "lightsky/draw/RenderBuffer.h"
#include "lightsky/draw/ShaderObject.h"
#include "lightsky/draw/ShaderProgram.h"
#include "lightsky/draw/Texture.h"
#include "lightsky/draw/UniformBuffer.h"
#include "lightsky/draw/VertexArray.h"
#include "lightsky/draw/VertexBuffer.h"



namespace ls
{
namespace draw
{



/**----------------------------------------------------------------------------
 * GLDataList contains information about the lifetime of OpenGL objects.
-----------------------------------------------------------------------------*/
template <typename gl_type, class allocator_type = std::allocator<gl_type>>
class GLDataList
{
  public:
    typedef std::vector<gl_type, allocator_type> list_type;

    typedef gl_type value_type;

  private:
    list_type objects;

  public:
    ~GLDataList() noexcept;

    GLDataList() noexcept;

    GLDataList(const GLDataList& c) noexcept;

    GLDataList(GLDataList&& c) noexcept;

    GLDataList& operator=(const GLDataList& c) noexcept;

    GLDataList& operator=(GLDataList&& c) noexcept;

    void clear() noexcept;

    void reserve(const size_t numItems) noexcept;

    size_t add(const gl_type& o) noexcept;

    size_t add(gl_type&& o) noexcept;

    void remove(const size_t index) noexcept;

    gl_type release(const size_t index) noexcept;

    gl_type& operator[](const size_t index) noexcept;

    const gl_type& operator[](const size_t index) const noexcept;

    size_t size() const noexcept;

    size_t capacity() const noexcept;

    void shrink_to_fit() noexcept;

    gl_type* data() noexcept;

    const gl_type* data() const noexcept;

    gl_type& front() noexcept;

    const gl_type& front() const noexcept;

    gl_type& back() noexcept;

    const gl_type& back() const noexcept;
};

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
GLDataList<gl_type, alloc_type>::~GLDataList() noexcept
{
    clear();
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
GLDataList<gl_type, alloc_type>::GLDataList() noexcept :
    objects{}
{
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
GLDataList<gl_type, alloc_type>::GLDataList(const GLDataList& c) noexcept
{
    *this = c;
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
GLDataList<gl_type, alloc_type>::GLDataList(GLDataList&& c) noexcept
{
    *this = std::move(c);
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline GLDataList<gl_type, alloc_type>& GLDataList<gl_type, alloc_type>::operator=(const GLDataList& c) noexcept
{
    objects = c.objects;
    return *this;
}

template <>
GLDataList<utils::Pointer<BufferObject>>& GLDataList<utils::Pointer<BufferObject>>::operator=(const GLDataList& c) noexcept;

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline GLDataList<gl_type, alloc_type>& GLDataList<gl_type, alloc_type>::operator=(GLDataList&& c) noexcept
{
    objects = std::move(c.objects);
    return *this;
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline void GLDataList<gl_type, alloc_type>::clear() noexcept
{
    reserve(0);
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
void GLDataList<gl_type, alloc_type>::reserve(const size_t numItems) noexcept
{
    if (numItems < objects.size())
    {
        for (size_t i = 0; i < objects.size(); ++i)
        {
            objects[i].terminate();
        }
    }

    objects.reserve(numItems);
}

template <>
void GLDataList<utils::Pointer<BufferObject>>::reserve(const size_t numItems) noexcept;

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
size_t GLDataList<gl_type, alloc_type>::add(const gl_type& o) noexcept
{
    for (size_t i = 0; i < objects.size(); ++i)
    {
        gl_type& current = objects[i];

        if (current.gpu_id() && current.gpu_id() == o.gpu_id())
        {
            current = o;
            return i;
        }
    }

    objects.push_back(o);
    return objects.size() - 1;
}

template <>
size_t GLDataList<utils::Pointer<BufferObject>>::add(const utils::Pointer<BufferObject>& o) noexcept;

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
size_t GLDataList<gl_type, alloc_type>::add(gl_type&& o) noexcept
{
    for (size_t i = 0; i < size(); ++i)
    {
        gl_type& current = objects[i];

        if (current.gpu_id() && current.gpu_id() == o.gpu_id())
        {
            current = std::move(o);
            return i;
        }
    }

    objects.emplace_back(std::move(o));
    return objects.size() - 1;
}

template <>
size_t GLDataList<utils::Pointer<BufferObject>>::add(utils::Pointer<BufferObject>&& o) noexcept;

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline void GLDataList<gl_type, alloc_type>::remove(const size_t index) noexcept
{
    objects[index].terminate();
    objects.erase(objects.begin() + index);
}

template <>
void GLDataList<utils::Pointer<BufferObject>>::remove(const size_t index) noexcept;

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline gl_type GLDataList<gl_type, alloc_type>::release(const size_t index) noexcept
{
    gl_type ret = std::move(objects[index]);
    objects.erase(objects.begin() + index);
    return ret;
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline gl_type& GLDataList<gl_type, alloc_type>::operator[](const size_t index) noexcept
{
    return objects[index];
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline const gl_type& GLDataList<gl_type, alloc_type>::operator[](const size_t index) const noexcept
{
    return objects[index];
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline size_t GLDataList<gl_type, alloc_type>::size() const noexcept
{
    return objects.size();
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline size_t GLDataList<gl_type, alloc_type>::capacity() const noexcept
{
    return objects.capacity();
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline void GLDataList<gl_type, alloc_type>::shrink_to_fit() noexcept
{
    objects.shrink_to_fit();
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline gl_type* GLDataList<gl_type, alloc_type>::data() noexcept
{
    return objects.data();
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline const gl_type* GLDataList<gl_type, alloc_type>::data() const noexcept
{
    return objects.data();
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline gl_type& GLDataList<gl_type, alloc_type>::front() noexcept
{
    LS_ASSERT(size() > 0);
    return objects[0];
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline const gl_type& GLDataList<gl_type, alloc_type>::front() const noexcept
{
    LS_ASSERT(size() > 0);
    return objects[0];
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline gl_type& GLDataList<gl_type, alloc_type>::back() noexcept
{
    LS_ASSERT(size() > 0);
    return objects[size() - 1];
}

/*-------------------------------------
-------------------------------------*/
template <typename gl_type, class alloc_type>
inline const gl_type& GLDataList<gl_type, alloc_type>::back() const noexcept
{
    LS_ASSERT(size() > 0);
    return objects[size() - 1];
}

/*-------------------------------------
 * External Template Instantiations
-------------------------------------*/
LS_DECLARE_CLASS_TYPE(BufferDataList, GLDataList, utils::Pointer<BufferObject>);

LS_DECLARE_CLASS_TYPE(IBODataList, GLDataList, IndexBuffer);

LS_DECLARE_CLASS_TYPE(FBODataList, GLDataList, FrameBuffer);

LS_DECLARE_CLASS_TYPE(RBODataList, GLDataList, RenderBuffer);

LS_DECLARE_CLASS_TYPE(ShaderObjectDataList, GLDataList, ShaderObject);

LS_DECLARE_CLASS_TYPE(ShaderProgramDataList, GLDataList, ShaderProgram);

LS_DECLARE_CLASS_TYPE(TextureDataList, GLDataList, Texture);

LS_DECLARE_CLASS_TYPE(PBODataList, GLDataList, PixelBuffer);

LS_DECLARE_CLASS_TYPE(UBODataList, GLDataList, UniformBuffer);

LS_DECLARE_CLASS_TYPE(VAODataList, GLDataList, VertexArray);

LS_DECLARE_CLASS_TYPE(VBODataList, GLDataList, VertexBuffer);



/**----------------------------------------------------------------------------
 * GLContextData provides data for all OpenGL object types.
-----------------------------------------------------------------------------*/
struct GLContextData
{
    VBODataList vbos;

    IBODataList ibos;

    TextureDataList textures;

    FBODataList fbos;

    RBODataList rbos;

    ShaderObjectDataList shaders;

    ShaderProgramDataList progs;

    UBODataList ubos;

    VAODataList vaos;

    std::vector<BlendObject> blendModes;

    std::vector<DepthObject> depthModes;

    void terminate() noexcept;
};
} // end draw namespace
} // end ls namespace

#endif /* __LS_DRAW_GL_CONTEXT_H__ */
