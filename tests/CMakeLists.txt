
# -------------------------------------
# Project Setup
# -------------------------------------
project(lsdraw_tests CXX)



# -------------------------------------
# Include Paths
# -------------------------------------
include_directories(".")



# -------------------------------------
# Dependency Setup
# -------------------------------------
set(
    LS_DRAW_TEST_DEPS
    lssetup
    lsutils
    lsmath
    lsdraw
)



# -------------------------------------
# Include Paths
# -------------------------------------
include_directories(".")



# -------------------------------------
# Source Paths
# -------------------------------------
set(LS_DRAW_PROTO_COMMON_SOURCES)

set(
    LS_DRAW_PROTO_HELLO_SOURCES
    "hello_lsdraw.cpp"
    ${LS_DRAW_PROTO_COMMON_SOURCES}
)



# -------------------------------------
# Building and Linking Targets
# -------------------------------------
add_executable(hello_lsdraw ${LS_DRAW_PROTO_HELLO_SOURCES})
add_dependencies(hello_lsdraw ${LS_DRAW_TEST_DEPS})
