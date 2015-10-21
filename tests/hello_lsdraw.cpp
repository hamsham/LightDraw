
#include <new> // std::nothrow

#include "context.h"
#include "display.h"

#include "main.h"

namespace global {
    display* pDisplay = nullptr;
    context renderContext;
}

/*-----------------------------------------------------------------------------
 * Example System Object
-----------------------------------------------------------------------------*/
class mainSystem final : virtual public ls::game::gameSystem {
    public:
        mainSystem();
        
        mainSystem(const mainSystem&) = delete;
        
        mainSystem(mainSystem&&);
        
        virtual ~mainSystem();
        
        mainSystem& operator=(const mainSystem&) = delete;
        
        mainSystem& operator=(mainSystem&&);
        
        bool start() override;
};

/*-------------------------------------
 * Destructor
-------------------------------------*/
mainSystem::~mainSystem() {
}

/*-------------------------------------
 * Contstructor
-------------------------------------*/
mainSystem::mainSystem() {
}

/*-------------------------------------
 * Move Constructor
-------------------------------------*/
mainSystem::mainSystem(mainSystem&& ms) :
    gameSystem{std::move(ms)}
{}

/*-------------------------------------
 * Move Operator
-------------------------------------*/
mainSystem& mainSystem::operator=(mainSystem&& ms) {
    gameSystem::operator=(std::move(ms));
    
    return *this;
}

/*-------------------------------------
 * System Startup
-------------------------------------*/
bool mainSystem::start() {
    if (!gameSystem::start()) {
        LS_LOG_ERR("Error: Unable to start the main program.");
        return false;
    }
    
    // push some states and run the game
    if (!this->pushGameState(new fbState{})) {
        LS_LOG_ERR("Error: Unable to start a framebuffer state.");
    }
    
    if (!this->pushGameState(new uiState{})) {
        LS_LOG_ERR("Error: Unable to start the UI state.");
        clearGameStates();
        return false;
    }
    
    return true;
}

/*-----------------------------------------------------------------------------
 * Main() Methids
-----------------------------------------------------------------------------*/
/*-------------------------------------
 * Forward declarations
-------------------------------------*/
bool initSubsystems();
void terminateSubsystems();

/*-------------------------------------
 * main()
-------------------------------------*/
int main() {
    mainSystem sys{};
    
    if (!initSubsystems()) {
        std::cerr << "Unable to initialize SDL." << std::endl;
        goto quitTest;
    }
    std::cout << "LightSky Successfully initialized.\n" << std::endl;
    
    global::pDisplay = new(std::nothrow) display();
    if (!global::pDisplay || !global::pDisplay->init(math::vec2i{800, 600})) {
        std::cerr << "Unable to create a display." << std::endl;
        goto quitTest;
    }
    global::pDisplay->setFullScreenMode(FULLSCREEN_WINDOW);
    
    if (!global::renderContext.init(*global::pDisplay)) {
        std::cerr << "Unable to create a render context." << std::endl;
        goto quitTest;
    }
    
    if (!sys.start()) {
        std::cerr << "Unable to create the main program." << std::endl;
        goto quitTest;
    }
    
    std::cout << "Successfully created the main program." << std::endl;
    while (sys.isRunnable()) {
        global::renderContext.makeCurrent(*global::pDisplay);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sys.run();
        global::renderContext.flip(*global::pDisplay);
    }
    
    quitTest:
    sys.stop();
    global::renderContext.terminate();
    delete global::pDisplay;
    terminateSubsystems();
    std::cout << "LightSky successfully terminated.\n" << std::endl;
    
    return 0;
}

/*-------------------------------------
    initialization.
-------------------------------------*/
bool initSubsystems() {
    if (SDL_WasInit(0) == SDL_INIT_EVERYTHING) {
        return true;
    }
    
    SDL_SetMainReady();
    
    constexpr uint32_t sdlInitFlags = 0
        | SDL_INIT_TIMER
        | SDL_INIT_AUDIO
        | SDL_INIT_VIDEO
        | SDL_INIT_EVENTS
        | SDL_INIT_JOYSTICK
        | SDL_INIT_GAMECONTROLLER
        | 0;
    
    if (SDL_Init(sdlInitFlags) < 0) {
        LS_LOG_ERR(
            "Unable to initialize SDL due to error ", SDL_GetError(), '\n',
            "Complain to your local programmer.\n"
        );
        return false;
    }
    LS_LOG_MSG(
        "Successfully initialized SDL.\n",
        SDL_GetError(), '\n'
    );
    
    /*
     * Setup the necessary parameters for OpenGL 3.3
     */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
#ifdef LS_DEBUG
        | SDL_GL_CONTEXT_DEBUG_FLAG
#endif
    );
    SDL_ClearError();
    
    return true;
}

/*-------------------------------------
    termination
-------------------------------------*/
void terminateSubsystems() {
    if (SDL_WasInit(0)) {
        SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
        SDL_Quit();
    }
}
