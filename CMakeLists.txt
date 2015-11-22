
# -------------------------------------
# Project Setup
# -------------------------------------
project(LightMath CXX)
set(OUTPUT_NAME lsdraw)



# -------------------------------------
# Freetype requires special care
# -------------------------------------
include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})



# -------------------------------------
# Source Paths
# -------------------------------------
include_directories("include")

set(LS_DRAW_SOURCES
    "src/draw/Atlas.cpp"
    "src/draw/BlendObject.cpp"
    "src/draw/BoundingBox.cpp"
    "src/draw/BufferObject.cpp"
    "src/draw/Camera.cpp"
    "src/draw/Color.cpp"
    "src/draw/DepthObject.cpp"
    "src/draw/FontResource.cpp"
    "src/draw/FrameBuffer.cpp"
    "src/draw/GeometryUtils.cpp"
    "src/draw/ImageResource.cpp"
    "src/draw/MatrixStack.cpp"
    "src/draw/RenderBuffer.cpp"
    "src/draw/Setup.cpp"
    "src/draw/ShaderObject.cpp"
    "src/draw/ShaderProgram.cpp"
    "src/draw/ShaderUniform.cpp"
    "src/draw/Texture.cpp"
    "src/draw/Transform.cpp"
    "src/draw/VertexArray.cpp"
    "src/draw/VertexAttrib.cpp"
    "src/draw/VertexUtils.cpp"
)



# -------------------------------------
# Library Setup
# -------------------------------------
add_library("${OUTPUT_NAME}" STATIC "${LS_DRAW_SOURCES}")

# Dependencies
target_link_libraries("${OUTPUT_NAME}" lssetup lsutils lsmath)



# -------------------------------------
# Library Installation Setup
# -------------------------------------
install(TARGETS "${OUTPUT_NAME}" ARCHIVE DESTINATION lib)
install(DIRECTORY include/lightsky/draw DESTINATION include/lightsky)



# -------------------------------------
# LS Draw Testing Area
# -------------------------------------
set(LS_BUILD_DRAW_TESTS TRUE CACHE BOOL "Build tests for the LightDraw library.")

#if(LS_BUILD_DRAW_TESTS)
#    add_subdirectory("tests")
#endif(LS_BUILD_DRAW_TESTS)
