#include <SDL2/SDL.h>
#include<cstring>
#include<cmath>
#include "models/int2vec.cpp"
#include "helpers/math.h"

const int WIDTH = 1280;
const int HEIGHT = 720;

// auxilary funtion to set a pixel in the buffer
void set_pixel(Uint32* buffer, int x, int y, Uint32 color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        buffer[y * WIDTH + x] = color;
    }
    
}


// auxilary function to add 3 color values in one 32-bit int. 8 bit per color and alpha
Uint32 color_from_rgb(Uint8 r, Uint8 g, Uint8 b) {
    // Ignore alpha here and set it to full (255)
    // bit shift and bitwise or to add everything together
    return (255 << 24) | (r << 16) | (g << 8) | b;
}

// Draws a line from (x0,y0) to (x1,y1) with a given color
void draw_line(Uint32* buffer, int2vec pos1, int2vec pos2, Uint32 color) {
    int dx = std::abs(pos2.x - pos1.x);
    int dy = -std::abs(pos2.y - pos1.y);

    int step_x = (pos1.x < pos2.x) ? 1 : -1;
    int step_y = (pos1.y < pos2.y) ? 1 : -1;

    int error = dx + dy;

    while (true)
    {
        // 1. set the next pixel
        set_pixel(buffer, pos1.x, pos1.y, color);

        if (pos1.x == pos2.x && pos1.y == pos2.y)
        {
            break;
        }

        int error2 = 2 * error;

        if (error2 >= dy)
        {
            if (pos1.x == pos2.x)
            {
                break;
            }
            error += dy;
            pos1.x += step_x;
        }
        if (error2 <= dx)
        {
            if (pos1.y == pos2.y)
            {
                break;
            }
            error += dx;
            pos1.y += step_y;
        }
    }
}

void draw_triangle(Uint32* buffer, float2 a, float2 b, float2 c, Uint32 color) {
    // iterate over every pixel and check if it is inside the triangle
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            float2 currentPixel = float2{float(x),float(y)};
            bool inTriangle = math::PointInTriangle(a,b,c,currentPixel);
            if (inTriangle)
            {
                set_pixel(buffer, x, y, color);
            }
        }
    }
    
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("CPU Render Engine", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          WIDTH, HEIGHT, 
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* screen_texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                WIDTH, HEIGHT);

    // Array to store color value of every pixel on screen
    Uint32* pixel_buffer = new Uint32[WIDTH * HEIGHT];

    // Main loop flag
    bool running = true;
    SDL_Event event;

    // Event loop
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // 1. empty pixel_buffer, set every value to 0 to get black background
        memset(pixel_buffer, 0, WIDTH * HEIGHT * sizeof(Uint32));

        // Call draw functions here to draw a line or a triangle etc.
        // Uint32 white = color_from_rgb(255,255,255);
        // draw_line(pixel_buffer, {100,100}, {700,300}, white);
        // draw_line(pixel_buffer, {100, 200}, {700, 400}, color_from_rgb(255, 0, 0));
        // draw_line(pixel_buffer, {50, 50}, {50, 600}, color_from_rgb(0, 255, 0));

        // Draw a green Triangle
        float2 a = {WIDTH * 0.15f, HEIGHT * 0.85f}; // lower left point
        float2 b = {WIDTH * 0.5f, HEIGHT * 0.15f}; // upper point;
        float2 c = {WIDTH * 0.85f, HEIGHT * 0.15f}; // lower right point
        draw_triangle(pixel_buffer, a, b , c, color_from_rgb(0, 240, 0));

        // 2. Update texture with new pixel values
        SDL_UpdateTexture(screen_texture, NULL, pixel_buffer, WIDTH * sizeof(Uint32));

        // 3. render updated texture to the screen
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    delete[] pixel_buffer;
    SDL_DestroyTexture(screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}