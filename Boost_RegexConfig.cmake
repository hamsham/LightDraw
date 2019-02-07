
# #####################################
# External build for Boost-Regex
# #####################################
#find_package(Boost COMPONENTS regex)

#if(NOT Boost_REGEX_LIBRARY)
    message("-- Downloading Boost Regex from source")

    set(BOOST_BRANCH "master" CACHE STRING "Git branch or tag for checking out Boost-Regex.")
    mark_as_advanced(BOOST_BRANCH)

    # Include Boost
    ExternalProject_Add(
        Boost_REGEX_LIBRARY
        PREFIX
            ${EXTERNAL_PROJECT_PREFIX}
        GIT_REPOSITORY
            "https://github.com/boostorg/regex.git"
        GIT_SHALLOW
            TRUE
        GIT_PROGRESS
            TRUE
        GIT_TAG
            "${BOOST_BRANCH}"
        UPDATE_COMMAND
            ${GIT_EXECUTABLE} fetch
        CONFIGURE_COMMAND
            ""
        BUILD_COMMAND
            ""
        INSTALL_DIR
            ${EXTERNAL_PROJECT_PREFIX}
        INSTALL_COMMAND
            ${CMAKE_COMMAND} -E copy_directory "${EXTERNAL_PROJECT_PREFIX}/src/Boost_REGEX_LIBRARY/include/boost" "${EXTERNAL_PROJECT_PREFIX}/include"
    )
#endif()