
# #####################################
# External build for Boost-Regex
# #####################################
find_package(Boost COMPONENTS regex)

if(NOT Boost_REGEX_LIBRARY)
    message("-- Building Boost Regex from source")

    set(BOOST_VERSION "1.69.0" CACHE STRING "Version of Boost to build.")
    mark_as_advanced(BOOST_VERSION)

    set(BOOST_BRANCH "boost-${BOOST_VERSION}" CACHE STRING "Git branch or tag for checking out Boost.")
    #xset(BOOST_BRANCH "master" CACHE STRING "Git branch or tag for checking out Boost.")
    mark_as_advanced(BOOST_BRANCH)

    if (WIN32)
        set(BOOST_BOOTSTRAP ${EXTERNAL_PROJECT_PREFIX}/src/Boost_Regex/bootstrap.bat)
    else()
        set(BOOST_BOOTSTRAP ${EXTERNAL_PROJECT_PREFIX}/src/Boost_Regex/bootstrap.sh)
    endif()

    set(BOOST_B2 ${EXTERNAL_PROJECT_PREFIX}/src/Boost_Regex-build/b2 --prefix=${EXTERNAL_PROJECT_PREFIX} --build-type=minimal --build-dir=${EXTERNAL_PROJECT_PREFIX}/src/Boost_Regex-build --with-regex variant=release link=shared threading=single runtime-link=shared)

    # Include Boost
    ExternalProject_Add(
        Boost_Regex
        PREFIX
            ${EXTERNAL_PROJECT_PREFIX}
        GIT_REPOSITORY
            "https://github.com/boostorg/boost.git"
        GIT_SHALLOW
            TRUE
        GIT_PROGRESS
            TRUE
        GIT_TAG
            "${BOOST_BRANCH}"
        GIT_SUBMODULES
            ""
        UPDATE_COMMAND
            ${GIT_EXECUTABLE} fetch
        CONFIGURE_COMMAND
            ${BOOST_BOOTSTRAP} --prefix=${EXTERNAL_PROJECT_PREFIX}
        BUILD_COMMAND
            cd ${EXTERNAL_PROJECT_PREFIX}/src/Boost_Regex && ${BOOST_B2} stage
        INSTALL_DIR
            ${EXTERNAL_PROJECT_PREFIX}
        INSTALL_COMMAND
            cd ${EXTERNAL_PROJECT_PREFIX}/src/Boost_Regex && ${BOOST_B2} install
    )

    # Add the imported library target
    add_library(Boost_REGEX_LIBRARY SHARED IMPORTED)
    set_target_properties(Boost_REGEX_LIBRARY PROPERTIES IMPORTED_LOCATION ${EXTERNAL_PROJECT_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}boost_regex${CMAKE_SHARED_LIBRARY_SUFFIX}.${BOOST_VERSION})
    add_dependencies(Boost_REGEX_LIBRARY Boost_Regex)
endif()
