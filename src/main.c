
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>

static const struct
{
    const char* key;
    const char* value;
} extended_metadata[] =
{
    { SDL_PROP_APP_METADATA_URL_STRING, "https://youtube.com/@ErikYuzwa" },
    { SDL_PROP_APP_METADATA_CREATOR_STRING, "@erikyuzwa" },
    { SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "MIT" },
    { SDL_PROP_APP_METADATA_TYPE_STRING, "tutorial" }
};

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

    SDL_Log("App Init");

    size_t i;

    if (!SDL_SetAppMetadata("SDL3 App Callback Window", "1.0", "com.erikyuzwa.sdl3")) {
        return SDL_APP_FAILURE;
    }

    for (i = 0; i < SDL_arraysize(extended_metadata); i++) {
        if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) {
            return SDL_APP_FAILURE;
        }
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState* state = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!state) return SDL_APP_FAILURE;

    SDL_CreateWindowAndRenderer("SDL3 App Callback Window", 800, 600, SDL_WINDOW_RESIZABLE, &state->window, &state->renderer);
    if (!state->window || !state->renderer) {
        SDL_Log("Window or Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(state->window);
        return SDL_APP_FAILURE;
    }


    state->running = true;

    *appstate = state;
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {

    AppState* state = (AppState*)appstate;

    switch (event->type) {
    case SDL_EVENT_QUIT:
        state->running = false;
        break;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_ESCAPE) {
            SDL_Log("ESC pressed, exiting");
            state->running = false;
        }
        else {
            SDL_Log("Key: %s", SDL_GetKeyName(event->key.key));
        }
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        SDL_Log("Window Resized to %d x %d", event->window.data1, event->window.data2);
        break;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        SDL_Log("Window close requested");
        state->running = false;
        break;

    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

    AppState* state = (AppState*)appstate;

    // Clear screen
    SDL_SetRenderDrawColor(state->renderer, 40, 40, 60, 255);
    SDL_RenderClear(state->renderer);

    // Render your game content here...

    SDL_RenderPresent(state->renderer);

    return state->running ? SDL_APP_CONTINUE : SDL_APP_SUCCESS; // return SDL_APP_SUCCESS to quit
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {

    SDL_Log("App Quit");

    if (appstate != NULL) {
        AppState* state = (AppState*)appstate;
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
}
