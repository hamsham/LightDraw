/* 
 * File:   display.cpp
 * Author: Miles Lacey
 * 
 * Created on November 15, 2013, 8:50 PM
 */

#include <cstdio>
#include <utility>

#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

#include "display.h"

/**
 * Print SDL window flags to the console.
 * This is primarily used for debugging.
 * 
 * @param flags
 * A bit field of difference SDL flags., see https://wiki.libsdl.org/ for more
 * information.
 */
void printWindowFlags(uint32_t flags) {
    LS_LOG_MSG(
        "\tWindow Flags:\n",
        "\t\tSDL_WINDOW_FULLSCREEN:         ",  0 < (flags & SDL_WINDOW_FULLSCREEN), '\n',
        "\t\tSDL_WINDOW_FULLSCREEN_DESKTOP: ",  0 < (flags & SDL_WINDOW_FULLSCREEN_DESKTOP), '\n',
        "\t\tSDL_WINDOW_OPENGL:             ",  0 < (flags & SDL_WINDOW_OPENGL), '\n',
        "\t\tSDL_WINDOW_SHOWN:              ",  0 < (flags & SDL_WINDOW_SHOWN), '\n',
        "\t\tSDL_WINDOW_HIDDEN:             ",  0 < (flags & SDL_WINDOW_HIDDEN), '\n',
        "\t\tSDL_WINDOW_BORDERLESS:         ",  0 < (flags & SDL_WINDOW_BORDERLESS), '\n',
        "\t\tSDL_WINDOW_RESIZABLE:          ",  0 < (flags & SDL_WINDOW_RESIZABLE), '\n',
        "\t\tSDL_WINDOW_MINIMIZED:          ",  0 < (flags & SDL_WINDOW_MINIMIZED), '\n',
        "\t\tSDL_WINDOW_MAXIMIZED:          ",  0 < (flags & SDL_WINDOW_MAXIMIZED), '\n',
        "\t\tSDL_WINDOW_INPUT_GRABBED:      ",  0 < (flags & SDL_WINDOW_INPUT_GRABBED), '\n',
        "\t\tSDL_WINDOW_INPUT_FOCUS:        ",  0 < (flags & SDL_WINDOW_INPUT_FOCUS), '\n',
        "\t\tSDL_WINDOW_MOUSE_FOCUS:        ",  0 < (flags & SDL_WINDOW_MOUSE_FOCUS), '\n',
        "\t\tSDL_WINDOW_FOREIGN:            ",  0 < (flags & SDL_WINDOW_FOREIGN), '\n',
        "\t\tSDL_WINDOW_ALLOW_HIGHDPI:      ",  0 < (flags & SDL_WINDOW_ALLOW_HIGHDPI), '\n'
    );
}

/*-------------------------------------
    Display constructor
-------------------------------------*/
display::display() {
    fullScreenMode = SDL_WINDOW_FULLSCREEN;
}

/*-------------------------------------
    Display move constructor
-------------------------------------*/
display::display(display&& d) :
    pWindow{d.pWindow},
    windowIsNative{d.windowIsNative}
{
    d.pWindow = nullptr;
    d.windowIsNative = false;
}

/*-------------------------------------
    Display move operator
-------------------------------------*/
display& display::operator=(display&& d) {
    pWindow = d.pWindow;
    d.pWindow = nullptr;
    
    windowIsNative = d.windowIsNative;
    d.windowIsNative = false;
    
    return *this;
}

/*-------------------------------------
    Display destructor
-------------------------------------*/
display::~display() {
    terminate();
}

/*-------------------------------------
    Display Initialization using a native window handle
-------------------------------------*/
bool display::init(void* const hwnd) {
    LS_LOG_MSG("Attempting to link with a preexisting display.");
    
    if (hwnd == nullptr) {
        LS_LOG_ERR("\tNo display information provided.\n");
        return false;
    }
    
    LS_LOG_MSG("\tLoading internal flags for a native window.");
    
    
    display tempDisp{};
    
    if (!tempDisp.init(math::vec2i{0})) {
        LS_LOG_ERR("\tUnable to create a shared context for a native window.\n");
        return false;
    }
    else {
        printWindowFlags(SDL_GetWindowFlags(tempDisp.getWindow()));
        
        // SDL requires a string, containing the SDL_Window* pointer, to be
        // formatted with "%p". using std::to_string() does not work.
        char nativeHandleStr[sizeof(void*)*2]; // <--- only big enough to stringify a pointer
        sprintf(nativeHandleStr, "%p", (void*)tempDisp.getWindow());

        LS_LOG_MSG("\tCopying window flags from ", nativeHandleStr, " to ", hwnd, '.');
        
        const int wasFlagCopied = SDL_SetHintWithPriority(
            SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT,
            nativeHandleStr, SDL_HINT_OVERRIDE
        );
        
        if (wasFlagCopied != SDL_TRUE) {
            LS_LOG_ERR("\tUnable to toggle internal OpenGL 3.3 compatibility.\n");
            return false;
        }
    }
    
    // Create the window
    pWindow = SDL_CreateWindowFrom(hwnd);
    
    if (!pWindow) {
        LS_LOG_ERR("\tUnable to link with a preexisting display.");
        LS_LOG_ERR('\t', SDL_GetError(), '\n');
        terminate();
        return false;
    }
    else {
        LS_LOG_MSG("\tSuccessfully linked with a preexisting display.\n");
        windowIsNative = true;
        printWindowFlags(SDL_GetWindowFlags(pWindow));
        SDL_GL_LoadLibrary(nullptr); // must be called in order to avoid crashing
    }
    
    return true;
}

/*-------------------------------------
    Display Initialization With no default window handle
-------------------------------------*/
bool display::init(const math::vec2i inResolution, bool isFullScreen) {
    LS_LOG_MSG("Attempting to create an OpenGL 3.3-compatible display through SDL.");
    
    Uint32 windowFlags =
        SDL_WINDOW_OPENGL       |
        SDL_WINDOW_SHOWN        |
        SDL_WINDOW_INPUT_FOCUS  |
        SDL_WINDOW_MOUSE_FOCUS  |
        SDL_WINDOW_RESIZABLE    |
        0;
    
    if (isFullScreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
        LS_LOG_MSG("\tFullscreen: TRUE.");
    }
    else {
        LS_LOG_MSG("\tFullscreen: FALSE.");
    }
    
    // Create the window
    pWindow = SDL_CreateWindow(
        "LightSky", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        inResolution[0], inResolution[1], windowFlags
    );
    
    if (!pWindow) {
        LS_LOG_ERR("\tUnable to create a display object.");
        LS_LOG_ERR('\t', SDL_GetError(), '\n');
        terminate();
        return false;
    }
    else {
        windowIsNative = false;
        LS_LOG_MSG("\tSuccessfully created a window through SDL.");
    }
    
    return true;
}

/*-------------------------------------
    Display Termination
-------------------------------------*/
void display::terminate() {
    if (pWindow != nullptr && windowIsNative == false) {
        SDL_DestroyWindow(pWindow);
    }
    
    pWindow = nullptr;
    windowIsNative = false;
}

/*-------------------------------------
    Display Resolution Handling
-------------------------------------*/
const math::vec2i display::getResolution() const {
    int x, y;
    SDL_GetWindowSize(pWindow, &x, &y);
    return math::vec2i{x, y};
}

/*-------------------------------------
    Set the current display's resolution, in pixels.
-------------------------------------*/
void display::setResolution(const math::vec2i inResolution) {
    SDL_SetWindowSize(pWindow, inResolution[0], inResolution[1]);
}

/*-------------------------------------
    Fullscreen management
-------------------------------------*/
void display::setFullScreenState(bool fs) {
    if (fs == true) {
        SDL_SetWindowFullscreen(pWindow, fullScreenMode);
        SDL_DisableScreenSaver();
    }
    else {
        SDL_SetWindowFullscreen(pWindow, 0);
        SDL_EnableScreenSaver();
    }
}

/*-------------------------------------
    Query Fullscreen
-------------------------------------*/
bool display::getFullScreenState() const {
    return pWindow != nullptr
    && (SDL_GetWindowFlags(pWindow) & SDL_WINDOW_FULLSCREEN) != 0;
}

/*-------------------------------------
    Set how the window should handle the full resolution of the current
    display.
-------------------------------------*/
void display::setFullScreenMode(fullscreen_t fsType) {
    LS_DEBUG_ASSERT(fsType == FULLSCREEN_DISPLAY || fsType == FULLSCREEN_WINDOW);
    
    fullScreenMode = fsType;
    
    if (fsType == FULLSCREEN_DISPLAY) {
        fullScreenMode = SDL_WINDOW_FULLSCREEN;
    }
    else {
        fullScreenMode = SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    
    // apply the current fullscreen mode if requested
    if (getFullScreenState() == true) {
        setFullScreenState(true);
    }
}

/*-------------------------------------
    Get the current fullscreen-handling method.
-------------------------------------*/
fullscreen_t display::getFullScreenMode() const {
    return fullScreenMode == SDL_WINDOW_FULLSCREEN
        ? FULLSCREEN_DISPLAY
        : FULLSCREEN_WINDOW;
}


/*-------------------------------------
    Get a handle to the SDL_Window responsible for the window that this
    object references.
-------------------------------------*/
SDL_Window* display::getWindow() const {
    return pWindow;
}
