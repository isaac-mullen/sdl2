#include <SDL.h>
#include <math.h>

/* window dimensions */
#define WIN_WIDTH 900
#define WIN_HEIGHT 600

/* colors */
#define WHITE 0xffffffff

/* shapes (objects) */
struct Circle {
    double x;
    double y;
    double r;
};

/* prototypes */
void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color);

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

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    struct Circle circle = {200, 200, 80};
    FillCircle(surface, circle, WHITE);
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { /* listening for a quit event (pressing the X button) */
                running = 0;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
/*---------------------------------------------------------------------------------------------------------*/

/* Used to render a circle */
void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color) {
    /* pre-compute radius squared to avoid doing square root for every
    pixel to see the distance from the center.*/
    double radiusSquared = pow(circle.r, 2);
    /* only looping over the square that encompasses the circle to avoid checking every pixel in the window */
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
        for (double y = circle.y- circle.r; y <= circle.y + circle.r; y++) {
            double distanceSquared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distanceSquared < radiusSquared) {
                SDL_Rect pixel = {x, y, 1, 1}; /* SDL_Rest uses ints to store their data. demoting our double (narrowing conversion) */
                SDL_FillRect(surface, &pixel, WHITE);
            }
        }
    }
}