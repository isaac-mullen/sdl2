#include <SDL.h>
#include <math.h>

/* window dimensions */
#define WIN_WIDTH 900
#define WIN_HEIGHT 600

/* colors */
#define WHITE 0xffffffff
#define BLACK 0x00000000

#define RAY_STEP

/* TODO -->  Get initial direction of the ray. 'atan2()' Compute end node using "analytical intersection testing" -- SEE CLAUDE CONVO
 */
struct RayNode {
    double x;
    double y;
};

/* DO I ACTUALLY NEED THIS??? */
struct Ray {
    struct RayNode ray[];
};

/* objects */
struct Circle {
    double x;
    double y;
    double r;
};

struct RGBA {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

/* prototypes */
void FillCircle(SDL_Renderer *renderer, struct Circle circle, Uint32 color);
struct RGBA uint32_to_rgba(Uint32 uint32);

/*---------------------------------------------------------------------------------------------------------*/
int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL_Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        900, 600,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    struct Circle circle = {200, 200, 80}; /* circle to be drawn */

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { /* listening for a quit event (pressing the X button) */
                running = 0;
            }
            if (event.type == SDL_MOUSEMOTION) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
            }
        }
        /* clearing the background */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        /* drawing circle */
        FillCircle(renderer, circle, WHITE);

        /* updating window with and renderer changes */
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
/*---------------------------------------------------------------------------------------------------------*/

/* Used to render a circle */
void FillCircle(SDL_Renderer *renderer, struct Circle circle, Uint32 color) {
    /* pre-compute square of radius to avoid taking square root for every
    pixel to get the distance from the circles center. */
    double radiusSquared = pow(circle.r, 2);
    struct RGBA rgba = uint32_to_rgba(color);
    /* only looping over the square that encompasses the circle to avoid checking every pixel in the window */
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
        for (double y = circle.y- circle.r; y <= circle.y + circle.r; y++) {
            double distanceSquared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distanceSquared < radiusSquared) {
                SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            }
        }
    }
}

/* util function to convert between uint32 color and rgba values */
struct RGBA uint32_to_rgba(Uint32 uint32) {
    struct RGBA rgba = {
        (uint32 >> 24) & 0xff,
        (uint32 >> 16) & 0xff,
        (uint32 >> 8) & 0xff,
        (uint32 >> 0) & 0xff
    };
    return rgba;
}