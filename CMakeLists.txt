
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
# Finders
# -------------------------------------
# Only using OpenGL for its libraries, not headers
include(FindOpenGL)

# FreeType requires special care
include(FindFreetype)

find_package(ASSIMP REQUIRED)



# -------------------------------------
# Include Paths
# -------------------------------------
include_directories("include")

include_directories(${FREETYPE_INCLUDE_DIRS})



# -------------------------------------
# Source Paths
# -------------------------------------
set(LS_DRAW_SOURCES
    src/draw/AnimationChannel.cpp
    src/draw/Animation.cpp
    src/draw/AnimationKeyList.cpp
    src/draw/AnimationPlayer.cpp
    src/draw/Atlas.cpp
    src/draw/BlendObject.cpp
    src/draw/BoundingBox.cpp
    src/draw/BufferObject.cpp
    src/draw/Camera.cpp
    src/draw/Color.cpp
    src/draw/DepthObject.cpp
    src/draw/DrawBatch.cpp
    src/draw/DrawParams.cpp
    src/draw/FBOAssembly.cpp
    src/draw/FBOAttrib.cpp
    src/draw/FontResource.cpp
    src/draw/FrameBuffer.cpp
    src/draw/GeometryUtils.cpp
    src/draw/GLContext.cpp
    src/draw/GLQuery.cpp
    src/draw/ImageResource.cpp
    src/draw/IndexBuffer.cpp
    src/draw/MatrixStack.cpp
    src/draw/OcclusionMeshLoader.cpp
    src/draw/PixelBuffer.cpp
    src/draw/RBOAssembly.cpp
    src/draw/RenderBuffer.cpp
    src/draw/RenderPass.cpp
    src/draw/RenderValidation.cpp
    src/draw/SceneFileLoader.cpp
    src/draw/SceneFileUtility.cpp
    src/draw/SceneGraph.cpp
    src/draw/SceneMaterial.cpp
    src/draw/SceneMesh.cpp
    src/draw/SceneNode.cpp
    src/draw/SceneRenderData.cpp
    src/draw/Setup.cpp
    src/draw/ShaderAssembly.cpp
    src/draw/ShaderAttribArray.cpp
    src/draw/ShaderAttrib.cpp
    src/draw/ShaderBlockAttrib.cpp
    src/draw/ShaderObject.cpp
    src/draw/ShaderProgram.cpp
    src/draw/ShaderUniform.cpp
    src/draw/TextMeshLoader.cpp
    src/draw/TextureAssembly.cpp
    src/draw/TextureAttrib.cpp
    src/draw/Texture.cpp
    src/draw/Transform.cpp
    src/draw/UniformBuffer.cpp
    src/draw/VAOAssembly.cpp
    src/draw/VAOAttrib.cpp
    src/draw/VBOAttrib.cpp
    src/draw/VertexArray.cpp
    src/draw/VertexBuffer.cpp
    src/draw/Vertex.cpp
    src/draw/VertexUtils.cpp
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

set(
    LS_DRAW_EXTERNAL_DEPS
    ${OPENGL_gl_LIBRARY}
    ${FREETYPE_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    freeimage
)



# -------------------------------------
# Library Setup
# -------------------------------------
# Keeping LightDraw as a static library until Vulkan support is added.
add_library("${OUTPUT_NAME}" STATIC "${LS_DRAW_SOURCES}")

# Internal dependencies
add_dependencies(${LS_DRAW_INTERNAL_DEPS})

# Linking Setup
target_link_libraries("${OUTPUT_NAME}" ${LS_DRAW_INTERNAL_DEPS} ${LS_DRAW_EXTERNAL_DEPS})



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
