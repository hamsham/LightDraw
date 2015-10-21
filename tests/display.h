/* 
 * File:   display.h
 * Author: Miles Lacey
 *
 * Created on November 15, 2013, 8:50 PM
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#include "main.h"

/*-----------------------------------------------------------------------------
    Non-Namespaced Forward Declarations
-----------------------------------------------------------------------------*/
struct SDL_Window;

/**----------------------------------------------------------------------------
    @brief Full-Screen Flags
    
    These flags can be used to determine the full-screen mode of a display
    object.
-----------------------------------------------------------------------------*/
enum fullscreen_t : int {
    /**
     * Display in standard fullscreen
     */
    FULLSCREEN_DISPLAY,
    
    /**
     * Display a borderless window at the current display's full resolution.
     */
    FULLSCREEN_WINDOW,
    
    /**
     * @brief Default Full-Screen mode
     * 
     * Currently uses a standard full-screen mode.
     */
    FULLSCREEN_DEFAULT = FULLSCREEN_DISPLAY
};

/**----------------------------------------------------------------------------
 * @brief Display Class
 * 
 * This class is responsible for Opening an OpenGL 3.3-compatible device
 * context (a window within the OS). It contains the object responsible for
 * rendering or drawing in OpenGL.
-----------------------------------------------------------------------------*/
class display {
    private:
        /**
         * Internal handle to the device context created by SDL.
         */
        SDL_Window* pWindow = nullptr;
        
        /**
         * Keep track of whether a native hardware handle is being used.
         */
        bool windowIsNative = false;
        
        /**
         * Member to keep track of whether the display should use the default
         * fullscreen mode, or present the render context in a borderless
         * window that matches the user's display resolution.
         */
        int fullScreenMode;

    public:
        /**
         * @brief Constructor
         */
        display();
        
        /**
         * Copy Constructor - DELETED
         */
        display(const display&) = delete;
        
        /**
         * @brief Move operator
         * 
         * Moves all members from the input parameter into *this. No copies are
         * performed. Any existing window handles will be invalidated.
         * 
         * @param d
         * An r-value reference to another display object that *this will take
         * ownership of.
         */
        display(display&& d);
        
        /**
         * @brief Destructor
         * 
         * Closes the window and frees all resources used by *this.
         */
        ~display();
        
        /**
         * Copy Operator - DELETED
         */
        display& operator=(const display& d) = delete;
        
        /**
         * @brief Move operator
         * 
         * Moves all members from the input parameter into *this. No copies are
         * performed. Any existing window handles will be invalidated.
         * 
         * @param d
         * An r-value reference to another display object that *this will take
         * ownership of.
         */
        display& operator=(display&& d);
        
        /**
         * Create an display object from a native OS hardware handle.
         * 
         * @param hwnd
         * A pointer to the operating systen's native window type.
         * 
         * @param isFullScreen
         * Determine if the window should be made full-screen.
         * 
         * @return bool
         * TRUE if a window could be successfully created, or
         * FALSE if otherwise.
         */
        bool init(void* const hwnd);
        
        /**
         * Initialize/Open a window within the OS.
         * 
         * @param inResolution
         * The desired window resolution, in pixels.
         * 
         * @param isFullScreen
         * Determine if the window should be made full-screen.
         * 
         * @return TRUE if the display initialized properly, FALSE is not.
         */
        bool init(const math::vec2i inResolution, bool isFullScreen = false);
        
        /**
         * Close the window and free all memory/resources used by *this.
         */
        void terminate();
        
        /**
         * Get the resolution, in pixels, of the display referenced by *this.
         * 
         * @return math::vec2i
         */
        const math::vec2i getResolution() const;
        
        /**
         * Set the resolution, in pixels, that this display should be.
         * 
         * @param inResolution
         * A new resolution, contained within a 2d integral vector, represented
         * in pixels.
         */
        void setResolution(const math::vec2i inResolution);
        
        /**
         * Set whether or not this display should be made fullscreen.
         * 
         * @param fs
         * TRUE to enable a fullscreen window, FALSE to reduce the display down
         * to a simple window.
         */
        void setFullScreenState(bool fs);
        
        /**
         * Determine if the current display is in fullscreen mode.
         * 
         * @return TRUE if the display is in fullscreen mode, FALSE if not.
         */
        bool getFullScreenState() const;
        
        /**
         * Set how the window should handle the full resolution of the current
         * display.
         * 
         * @param fs
         * Set to FULLSCREEN_DISPLAY in order to use the default fullscreen
         * mode, or use FULLSCREEN_WINDOW in order to make the window become
         * borderless and use the entire available resolution.
         */
        void setFullScreenMode(fullscreen_t fs = FULLSCREEN_DEFAULT);
        
        /**
         * Get the current fullscreen-handling method.
         * 
         * @return fullscreen_t
         */
        fullscreen_t getFullScreenMode() const;
        
        /**
         * Determine if this object holds a handle to an open window.
         * 
         * @return TRUE if a window is open, FALSE if not.
         */
        bool isRunning() const;
        
        /**
         * Get a handle to the SDL_Window responsible for the window that this
         * object references.
         * 
         * @return SDL_Window.
         */
        SDL_Window* getWindow() const;
        
        /**
         * Determine if the current display is using a native window handle.
         * 
         * @return bool
         * TRUE if this display was created using a previously existing OS
         * window handle, or FALSE if the display was created using an internal
         * method.
         */
        bool usingNativeWindow() const;
};

/*-------------------------------------
    Determine if this object holds a handle to an open window.
-------------------------------------*/
inline bool display::isRunning() const {
    return pWindow != nullptr;
}

/*-------------------------------------
    Determine if the current display is using a native window handle.
-------------------------------------*/
inline bool display::usingNativeWindow() const {
    return windowIsNative;
}

#endif	/* DISPLAY_H */
