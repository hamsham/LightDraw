
/*
 * File:   GLContext.cpp
 * Author: miles
 *
 * Created on March 8, 2016, 3:12 PM
 */

#include "ls/draw/GLContext.h"



namespace ls {
namespace draw {



/*-----------------------------------------------------------------------------
 * GLDataList
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Prevent Copies of abstract BufferObject types
-------------------------------------*/
template <>
GLDataList<utils::Pointer<BufferObject>>&GLDataList<utils::Pointer<BufferObject>>::operator =(const GLDataList& c) noexcept {
    (void)c;
    LS_ASSERT(0 != "Cannot copy Buffer Objects!");
    return *this;
}

/*-------------------------------------
 * Ensure Abstract Buffer object types can be reserved
-------------------------------------*/
template <>
void GLDataList<utils::Pointer<BufferObject>>::reserve(const unsigned numItems) noexcept {
    if (numItems < objects.size()) {
        for (unsigned i = 0; i < objects.size(); ++i) {
            objects[i]->terminate();
        }
    }

    objects.reserve(numItems);
}

/*-------------------------------------
 * Prevent Copies of abstract BufferObject types
-------------------------------------*/
template <>
unsigned GLDataList<utils::Pointer<BufferObject>>::add(const utils::Pointer<BufferObject>& o) noexcept {
    (void)o;
    LS_ASSERT(0 != "Cannot copy Buffer Objects!");
    return (unsigned) - 1;
}

/*-------------------------------------
 * Prevent Copies of abstract BufferObject types
-------------------------------------*/
template <>
unsigned GLDataList<utils::Pointer<BufferObject>>::add(utils::Pointer<BufferObject>&& o) noexcept {
    for (unsigned i = 0; i < size(); ++i) {
        utils::Pointer<BufferObject>& current = objects[i];

        if (current->gpu_id() == o->gpu_id()) {
            current = std::move(o);
            return i;
        }
    }

    objects.emplace_back(std::move(o));
    return objects.size() - 1;
}

/*-------------------------------------
 * Terminate abstract BufferObject types
-------------------------------------*/
template <>
void GLDataList<utils::Pointer<BufferObject>>::remove(const unsigned index) noexcept {
    objects[index]->terminate();
    objects.erase(objects.begin() + index);
}

/*-------------------------------------
-------------------------------------*/
LS_DEFINE_CLASS_TYPE(GLDataList, utils::Pointer<BufferObject>);
LS_DEFINE_CLASS_TYPE(GLDataList, FrameBuffer);
LS_DEFINE_CLASS_TYPE(GLDataList, IndexBuffer);
LS_DEFINE_CLASS_TYPE(GLDataList, RenderBuffer);
LS_DEFINE_CLASS_TYPE(GLDataList, ShaderObject);
LS_DEFINE_CLASS_TYPE(GLDataList, ShaderProgram);
LS_DEFINE_CLASS_TYPE(GLDataList, Texture);
LS_DEFINE_CLASS_TYPE(GLDataList, PixelBuffer);
LS_DEFINE_CLASS_TYPE(GLDataList, UniformBuffer);
LS_DEFINE_CLASS_TYPE(GLDataList, VertexArray);
LS_DEFINE_CLASS_TYPE(GLDataList, VertexBuffer);



/*-----------------------------------------------------------------------------
 * GLContextData
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Terminate
-------------------------------------*/
void GLContextData::terminate() noexcept {
    vbos.clear();
    ibos.clear();
    textures.clear();
    fbos.clear();
    rbos.clear();
    shaders.clear();
    progs.clear();
    ubos.clear();
    vaos.clear();
    blendModes.clear();
    depthModes.clear();
}



} // end draw namespace
} // end ls namespace
