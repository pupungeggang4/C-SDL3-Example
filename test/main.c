#include <SDL3/SDL.h>
#include <stdbool.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdio.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} AppContext;

AppContext ctx = { .running = true, .window = NULL, .renderer = NULL };

void main_loop(void *arg) {
    AppContext *ctx = (AppContext *)arg;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            ctx->running = false;
        }
    }

    if (ctx->renderer) {
        SDL_SetRenderDrawColor(ctx->renderer, 50, 100, 150, 255);
        SDL_RenderClear(ctx->renderer);
        SDL_RenderPresent(ctx->renderer);
    }

#ifdef __EMSCRIPTEN__
    if (!ctx->running) {
        emscripten_cancel_main_loop();
    }
#endif
}

int main(int argc, char *argv[]) {
    SDL_Log("1. SDL Init 시작");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init 실패: %s", SDL_GetError());
        return 1;
    }
    
    SDL_Log("2. 윈도우/렌더러 생성 시작");
    static AppContext ctx = { .running = true, .window = NULL, .renderer = NULL };
    if (!SDL_CreateWindowAndRenderer("SDL3", 640, 480, 0, &ctx.window, &ctx.renderer)) {
        SDL_Log("CreateWindowAndRenderer 실패: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_SetWindowSize(ctx.window, 640, 480);
    SDL_Log("3. 메인 루프 진입 직전");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, &ctx, 0, 1);
#else
    // ...
#endif
    return 0;
}

