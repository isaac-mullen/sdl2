#include <SDL.h>
#include <math.h>

/* window dimensions */
#define WIN_WIDTH 900
#define WIN_HEIGHT 600

/* colors */
#define WHITE 0xffffffff
#define BLACK 0x00000000
#define YELLOW 0xffff0000
#define NUM_RAYS 100
#define TAU (M_PI * 2)

/* TODO -->  Get initial direction of the ray. 'atan2()' Compute end node using "analytical intersection testing" -- SEE CLAUDE CONVO
 */
struct Ray {
    double x1;
    double y1;
    double angle;
    double x2;
    double y2;
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
void generateRays(struct Ray rays[], int numRays, struct Circle circle);
void drawRays(struct Ray rays[], int numRays, SDL_Renderer *renderer, Uint32 uint32_color);

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

        /* DRAW RAYS */
        struct Ray rays[NUM_RAYS];
        generateRays(rays, NUM_RAYS, circle);
        drawRays(rays, NUM_RAYS, renderer, YELLOW);

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

/* util function to generate (x) number of rays */
void generateRays(struct Ray rays[], int numRays, struct Circle circle) {
    double rayLength = WIN_WIDTH + WIN_HEIGHT; /* arbitrarily long distance to ensure offscreenedness*/
    for (int i = 0; i < numRays; i++) {
        double angle = (TAU / numRays) * i;

        rays[i].angle = angle;
        /* starting the ray at the boundary of circle to avoid weird artifacting from overlapping rays*/
        rays[i].x1 = circle.x + cos(angle) * circle.r;
        rays[i].y1 = circle.y + -(sin(angle)) * circle.r;
        /* calculating end point using raylength */
        rays[i].x2 = circle.x + cos(angle) * rayLength;
        rays[i].y2 = circle.y + -(sin(angle)) * rayLength;
    }
}

void drawRays(struct Ray rays[], int numRays, SDL_Renderer *renderer, Uint32 uint32_color) {
    struct RGBA color = uint32_to_rgba(uint32_color);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i < numRays; i++) {
        SDL_RenderDrawLine(renderer, rays[i].x1, rays[i].y1, rays[i].x2, rays[i].y2);
    }
}
