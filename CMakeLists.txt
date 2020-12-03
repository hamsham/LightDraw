
# -------------------------------------
# Project Setup
# -------------------------------------
project(LightDraw CXX)
set(OUTPUT_NAME lsdraw)



# -------------------------------------
# CMake Setup
# -------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})



# -------------------------------------
# Compiler Setup
# -------------------------------------
# MinGW doesn't always play nice with the Windows SDK
if (MINGW)
    set(CMAKE_SYSTEM_NAME Windows)

    # These defines were made in Windows 2000
    add_definitions(-D WINVER=0x0602)
    add_definitions(-D_WIN32_WINNT=0x0602)
elseif(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()



# -------------------------------------
# Finders
# -------------------------------------
# Only using OpenGL for its libraries, not headers
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(Threads REQUIRED)



# -------------------------------------
# 3rd-party libraries
# -------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_MODULE_PATH}")
include(assimpConfig.cmake)
include(FreeImageConfig.cmake)
include(freetypeConfig.cmake)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "3.5")
        set(LS_REQUIRE_BOOST_REGEX TRUE CACHE BOOL "Build LightDraw with boost::regex instead of std::regex.")
        include(Boost_RegexConfig.cmake)
    endif()
endif()



# -------------------------------------
# Header Paths
# -------------------------------------
set(LS_DRAW_HEADERS
    include/lightsky/draw/Animation.h
    include/lightsky/draw/AnimationChannel.h
    include/lightsky/draw/AnimationKeyList.h
    include/lightsky/draw/AnimationPlayer.h
    include/lightsky/draw/AnimationProperty.h
    include/lightsky/draw/Atlas.h
    include/lightsky/draw/BlendObject.h
    include/lightsky/draw/BoundingBox.h
    include/lightsky/draw/BufferObject.h
    include/lightsky/draw/Camera.h
    include/lightsky/draw/Color.h
    include/lightsky/draw/DepthObject.h
    include/lightsky/draw/Draw.h
    include/lightsky/draw/DrawBatch.h
    include/lightsky/draw/DrawParams.h
    include/lightsky/draw/FBOAssembly.h
    include/lightsky/draw/FBOAttrib.h
    include/lightsky/draw/FontResource.h
    include/lightsky/draw/FrameBuffer.h
    include/lightsky/draw/GeometryUtils.h
    include/lightsky/draw/GLContext.h
    include/lightsky/draw/GLQuery.h
    include/lightsky/draw/GLSLCommon.h
    include/lightsky/draw/ImageBuffer.h
    include/lightsky/draw/IndexBuffer.h
    include/lightsky/draw/MatrixStack.h
    include/lightsky/draw/OcclusionMeshLoader.h
    include/lightsky/draw/PackedVertex.h
    include/lightsky/draw/PixelBuffer.h
    include/lightsky/draw/RBOAssembly.h
    include/lightsky/draw/RenderBuffer.h
    include/lightsky/draw/RenderPass.h
    include/lightsky/draw/RenderValidation.h
    include/lightsky/draw/SceneFileLoader.h
    include/lightsky/draw/SceneFileUtility.h
    include/lightsky/draw/SceneGraph.h
    include/lightsky/draw/SceneMaterial.h
    include/lightsky/draw/SceneMesh.h
    include/lightsky/draw/SceneNode.h
    include/lightsky/draw/SceneRenderData.h
    include/lightsky/draw/Setup.h
    include/lightsky/draw/ShaderAssembly.h
    include/lightsky/draw/ShaderAttrib.h
    include/lightsky/draw/ShaderAttribArray.h
    include/lightsky/draw/ShaderBlockAttrib.h
    include/lightsky/draw/ShaderObject.h
    include/lightsky/draw/ShaderProgram.h
    include/lightsky/draw/ShaderUniform.h
    include/lightsky/draw/TextMeshLoader.h
    include/lightsky/draw/Texture.h
    include/lightsky/draw/TextureAssembly.h
    include/lightsky/draw/TextureAttrib.h
    include/lightsky/draw/Transform.h
    include/lightsky/draw/UniformBuffer.h
    include/lightsky/draw/VAOAssembly.h
    include/lightsky/draw/VAOAttrib.h
    include/lightsky/draw/VBOAttrib.h
    include/lightsky/draw/Vertex.h
    include/lightsky/draw/VertexArray.h
    include/lightsky/draw/VertexBuffer.h
    include/lightsky/draw/VertexUtils.h
)



# -------------------------------------
# Source Paths
# -------------------------------------
set(LS_DRAW_SOURCES
    src/AnimationChannel.cpp
    src/Animation.cpp
    src/AnimationKeyList.cpp
    src/AnimationPlayer.cpp
    src/Atlas.cpp
    src/BlendObject.cpp
    src/BoundingBox.cpp
    src/BufferObject.cpp
    src/Camera.cpp
    src/Color.cpp
    src/DepthObject.cpp
    src/DrawBatch.cpp
    src/DrawParams.cpp
    src/FBOAssembly.cpp
    src/FBOAttrib.cpp
    src/FontResource.cpp
    src/FrameBuffer.cpp
    src/GeometryUtils.cpp
    src/GLContext.cpp
    src/GLQuery.cpp
    src/ImageBuffer.cpp
    src/IndexBuffer.cpp
    src/MatrixStack.cpp
    src/OcclusionMeshLoader.cpp
    src/PixelBuffer.cpp
    src/RBOAssembly.cpp
    src/RenderBuffer.cpp
    src/RenderPass.cpp
    src/RenderValidation.cpp
    src/SceneFileLoader.cpp
    src/SceneFileUtility.cpp
    src/SceneGraph.cpp
    src/SceneMaterial.cpp
    src/SceneMesh.cpp
    src/SceneNode.cpp
    src/SceneRenderData.cpp
    src/Setup.cpp
    src/ShaderAssembly.cpp
    src/ShaderAttribArray.cpp
    src/ShaderAttrib.cpp
    src/ShaderBlockAttrib.cpp
    src/ShaderObject.cpp
    src/ShaderProgram.cpp
    src/ShaderUniform.cpp
    src/TextMeshLoader.cpp
    src/TextureAssembly.cpp
    src/TextureAttrib.cpp
    src/Texture.cpp
    src/Transform.cpp
    src/UniformBuffer.cpp
    src/VAOAssembly.cpp
    src/VAOAttrib.cpp
    src/VBOAttrib.cpp
    src/VertexArray.cpp
    src/VertexBuffer.cpp
    src/Vertex.cpp
    src/VertexUtils.cpp
)



# -------------------------------------
# Library Setup
# -------------------------------------
# Keeping LightDraw as a static library until Vulkan support is added.
add_library(${OUTPUT_NAME} STATIC ${LS_DRAW_HEADERS} ${LS_DRAW_SOURCES})

target_include_directories(${OUTPUT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(${OUTPUT_NAME} SYSTEM BEFORE
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/include>
    PRIVATE ${FREETYPE_INCLUDE_DIRS}
)

# Linking Setup
target_link_libraries(
    ${OUTPUT_NAME}
        PUBLIC LightSky::Setup
        PUBLIC LightSky::Utils
        PUBLIC LightSky::Math
        PUBLIC ${FREETYPE_LIBRARIES}
        PUBLIC ${ASSIMP_LIBRARIES}
        PUBLIC ${FREEIMAGE_LIBRARIES}
        PUBLIC Threads::Threads)

if (LS_REQUIRE_BOOST_REGEX)
    message("-- Building LS Draw with Boost::Regex support")
    target_link_libraries(${OUTPUT_NAME} PUBLIC ${Boost_REGEX_LIBRARY})
    target_compile_definitions(${OUTPUT_NAME} PUBLIC -DLS_REQUIRE_BOOST_REGEX)
endif()

if (OpenGL::EGL AND OPENGL_opengl_LIBRARY)
    message("-- Found EGL library: ${OPENGL_egl_LIBRARY}")
    target_link_libraries(
        ${OUTPUT_NAME}
            PUBLIC OpenGL::EGL
    )
elseif(OpenGL::GLX)
    message("-- Found GLX library: ${OPENGL_glx_LIBRARY}")
    target_link_libraries(
        ${OUTPUT_NAME}
            PUBLIC OpenGL::GLX
    )
endif()

if (OpenGL::EGL AND OpenGL::OpenGL)
    message("-- Found GLVND GL driver library: ${OPENGL_opengl_LIBRARY}")
    target_link_libraries(
        ${OUTPUT_NAME}
            PUBLIC OpenGL::OpenGL
    )
else()
    message("-- Found Legacy GL driver library: ${OPENGL_gl_LIBRARY}")
    target_link_libraries(
        ${OUTPUT_NAME}
            PUBLIC OpenGL::GL
    )
endif()



# -------------------------------------
# Library Installation Setup
# -------------------------------------
install(TARGETS ${OUTPUT_NAME}
    EXPORT LightDraw
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/lightsky/draw DESTINATION include/lightsky)

install(EXPORT LightDraw
    FILE LightDraw.cmake
    NAMESPACE LightSky::
    DESTINATION lib/cmake/LightSky
)

export(EXPORT LightDraw NAMESPACE LightSky::)
add_library(LightSky::Draw ALIAS lsdraw)



# -------------------------------------
# LS Draw Testing Area
# -------------------------------------
set(LS_BUILD_DRAW_TESTS TRUE CACHE BOOL "Build tests for the LightDraw library.")

#if(LS_BUILD_DRAW_TESTS)
#    add_subdirectory("tests")
#endif(LS_BUILD_DRAW_TESTS)
