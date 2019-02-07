
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
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)



# -------------------------------------
# 3rd-party libraries
# -------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_MODULE_PATH}")
include(assimpConfig.cmake)
include(FreeImageConfig.cmake)
include(Boost_RegexConfig.cmake)
include(SDL2Config.cmake)
include(freetypeConfig.cmake)



# -------------------------------------
# Include Paths
# -------------------------------------
include_directories(include)



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
# Dependency Setup
# -------------------------------------
set(
    LS_DRAW_INTERNAL_DEPS
    lssetup
    lsutils
    lsmath
)

if (NOT MSVC)
    set(LS_DRAW_EXTERNAL_DEPS
        ${OPENGL_gl_LIBRARY}
        ${SDL2_LIBS}
        ${FREETYPE_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${FREEIMAGE_LIBRARIES}
    )
else()
    set(LS_DRAW_EXTERNAL_DEPS
        ${OPENGL_gl_LIBRARY}
        ${SDL2_LIBS}
        ${FREETYPE_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${FREEIMAGE_LIBRARIES}
        pthread
    )
endif()



# -------------------------------------
# Library Setup
# -------------------------------------
# Keeping LightDraw as a static library until Vulkan support is added.
add_library(${OUTPUT_NAME} STATIC ${LS_DRAW_SOURCES})

# Internal dependencies
add_dependencies(${OUTPUT_NAME} Boost_REGEX_LIBRARY)

# Linking Setup
target_link_libraries(
    ${OUTPUT_NAME}
    ${LS_DRAW_INTERNAL_DEPS}
    ${LS_DRAW_EXTERNAL_DEPS})



# -------------------------------------
# Library Installation Setup
# -------------------------------------
install(TARGETS "${OUTPUT_NAME}"
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/lightsky/draw DESTINATION include/lightsky)



# -------------------------------------
# LS Draw Testing Area
# -------------------------------------
set(LS_BUILD_DRAW_TESTS TRUE CACHE BOOL "Build tests for the LightDraw library.")

#if(LS_BUILD_DRAW_TESTS)
#    add_subdirectory("tests")
#endif(LS_BUILD_DRAW_TESTS)
