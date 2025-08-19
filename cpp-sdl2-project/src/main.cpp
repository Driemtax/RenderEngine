#include <SDL2/SDL.h>
#include<cstring>
#include<cmath>
#include "models/int2vec.cpp"
#include "helpers/math.h"
#include "models/mesh.cpp"

const int WIDTH = 1280;
const int HEIGHT = 720;

struct m4x4 
{
    float m[4][4] = { 0 };
};

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

// Draws a triangle with given points a,b,c and color
void fill_triangle(Uint32* buffer, float2 a, float2 b, float2 c, Uint32 color) {
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

void draw_triangle (Uint32* buffer, triangle tri, Uint32 color) {
    // draw a line from a -> b, b -> c, c -> a
    int2vec a,b,c;
    a = int2vec{int(tri.p[0].x), int(tri.p[0].y)};
    b = int2vec{int(tri.p[1].x), int(tri.p[1].y)};
    c = int2vec{int(tri.p[2].x), int(tri.p[2].y)};

    draw_line(buffer, a, b, color);
    draw_line(buffer, b, c, color);
    draw_line(buffer, c, a, color);
}

mesh createCube() {
    mesh meshCube = mesh();
    meshCube.tris = {
        // SOUTH
        {{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}},
        {{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}},

        // EAST
        {{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}},
        {{{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}}},
        
        // NORTH
        {{{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}}},
        {{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}},
        
        // WEST
        {{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}},
        {{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},

        // TOP
        {{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}},
        {{{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}},

        // BOTTOM
        {{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}},
        {{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}},
    };

    return meshCube;
}

// Test function to draw multiple triangles with different properties
void test_triangle_rendering(Uint32* buffer) {
    // 1. Grid of small triangles (overlapping)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            float2 a = {i * 80.0f + 50, j * 80.0f + 50};
            float2 b = {i * 80.0f + 100, j * 80.0f + 50};
            float2 c = {i * 80.0f + 75, j * 80.0f + 20};
            
            // Rainbow colors based on position
            Uint8 r = (i * 32) % 256;
            Uint8 g = (j * 51) % 256;
            Uint8 bl = ((i + j) * 40) % 256;
            
            fill_triangle(buffer, a, b, c, color_from_rgb(r, g, bl));
        }
    }
    
    // 2. Two triangles forming a rectangle (quad)
    float2 rect_a = {400, 200};
    float2 rect_b = {600, 200};
    float2 rect_c = {400, 350};
    float2 rect_d = {600, 350};
    
    // First triangle (top-left, top-right, bottom-left)
    fill_triangle(buffer, rect_a, rect_b, rect_c, color_from_rgb(255, 100, 100));
    // Second triangle (top-right, bottom-right, bottom-left)
    fill_triangle(buffer, rect_b, rect_d, rect_c, color_from_rgb(100, 100, 255));
    
    // 3. Large overlapping triangles
    float2 big1_a = {200, 400};
    float2 big1_b = {800, 500};
    float2 big1_c = {300, 650};
    fill_triangle(buffer, big1_a, big1_b, big1_c, color_from_rgb(255, 255, 0));
    
    float2 big2_a = {400, 350};
    float2 big2_b = {900, 400};
    float2 big2_c = {500, 700};
    fill_triangle(buffer, big2_a, big2_b, big2_c, color_from_rgb(0, 255, 255));
    
    // 4. Triangles going off-screen (clipping test)
    // Top edge
    float2 clip1_a = {WIDTH * 0.3f, -50};
    float2 clip1_b = {WIDTH * 0.7f, -50};
    float2 clip1_c = {WIDTH * 0.5f, 100};
    fill_triangle(buffer, clip1_a, clip1_b, clip1_c, color_from_rgb(255, 0, 255));
    
    // Right edge
    float2 clip2_a = {WIDTH - 50, HEIGHT * 0.3f};
    float2 clip2_b = {WIDTH + 100, HEIGHT * 0.5f};
    float2 clip2_c = {WIDTH - 50, HEIGHT * 0.7f};
    fill_triangle(buffer, clip2_a, clip2_b, clip2_c, color_from_rgb(0, 255, 0));
    
    // Bottom edge
    float2 clip3_a = {WIDTH * 0.6f, HEIGHT - 50};
    float2 clip3_b = {WIDTH * 0.9f, HEIGHT + 100};
    float2 clip3_c = {WIDTH * 0.8f, HEIGHT - 100};
    fill_triangle(buffer, clip3_a, clip3_b, clip3_c, color_from_rgb(255, 128, 0));
    
    // Left edge
    float2 clip4_a = {-100, HEIGHT * 0.6f};
    float2 clip4_b = {150, HEIGHT * 0.4f};
    float2 clip4_c = {50, HEIGHT * 0.8f};
    fill_triangle(buffer, clip4_a, clip4_b, clip4_c, color_from_rgb(128, 0, 255));
    
    // 5. Very thin triangles (edge case testing)
    float2 thin1_a = {1000, 100};
    float2 thin1_b = {1200, 102};
    float2 thin1_c = {1100, 200};
    fill_triangle(buffer, thin1_a, thin1_b, thin1_c, color_from_rgb(255, 255, 255));
    
    // 6. Degenerate case - very small triangle
    float2 tiny_a = {50, 50};
    float2 tiny_b = {52, 50};
    float2 tiny_c = {51, 52};
    fill_triangle(buffer, tiny_a, tiny_b, tiny_c, color_from_rgb(255, 0, 0));
}

// Auxilery function to multiply a 4x4 matrix m with a 3d vector in and save the result in a 3d vector out
void MultiplyMatrixVektor(float3 &in, float3 &out, m4x4 &m) {
        out.x = in.x * m.m[0][0] + in.y * m.m[1][0] + in.z * m.m[2][0] + m.m[3][0];
        out.y = in.x * m.m[0][1] + in.y * m.m[1][1] + in.z * m.m[2][1] + m.m[3][1];
        out.z = in.x * m.m[0][2] + in.y * m.m[1][2] + in.z * m.m[2][2] + m.m[3][2];

        float w = in.x * m.m[0][3] + in.y * m.m[1][3] + in.z * m.m[2][3] + m.m[3][3];
        if (w != 0.0f)
        {
            out.x /= w; out.y /= w; out.z /= w;
        }
        
    }

void test_rotating_cube3D(Uint32* buffer, m4x4 projMat, mesh object, float fTheta) {
    // Set up rotation matrices
    m4x4 matRotZ, matRotX;

    // Rotation on Z-axis
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    // Rotation on X-axis
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);
    matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f);
    matRotX.m[2][2] = cosf(fTheta * 0.5);
    matRotX.m[3][3] = 1;

    // Draw Triangles
    for (auto tri : object.tris) {
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        // Rotate in Z-Axis
        MultiplyMatrixVektor(tri.p[0], triRotatedZ.p[0], matRotZ);
        MultiplyMatrixVektor(tri.p[1], triRotatedZ.p[1], matRotZ);
        MultiplyMatrixVektor(tri.p[2], triRotatedZ.p[2], matRotZ);

        // Rotate in X-Axis
        MultiplyMatrixVektor(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        MultiplyMatrixVektor(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        MultiplyMatrixVektor(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

        // Offset into the screen
        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        // Project triangles from 3D --> 2D
        MultiplyMatrixVektor(triTranslated.p[0], triProjected.p[0], projMat);
        MultiplyMatrixVektor(triTranslated.p[1], triProjected.p[1], projMat);
        MultiplyMatrixVektor(triTranslated.p[2], triProjected.p[2], projMat);

        // Scale into view
        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f; 
        triProjected.p[0].x *= 0.5f * float(WIDTH);
        triProjected.p[0].y *= 0.5f * float(HEIGHT);
        triProjected.p[1].x *= 0.5f * float(WIDTH);
        triProjected.p[1].y *= 0.5f * float(HEIGHT);
        triProjected.p[2].x *= 0.5f * float(WIDTH);
        triProjected.p[2].y *= 0.5f * float(HEIGHT);

        // rasterize triangle (for now just draw a wireframe triangle)
        draw_triangle(buffer, triProjected, color_from_rgb(255, 255, 255));
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

    // Projection Matrix
    m4x4 matProj;

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)HEIGHT / (float)WIDTH;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    // Main loop flag
    bool running = true;
    SDL_Event event;

    // Timer 
    Uint32 lastTime = SDL_GetTicks();
    float fTheta = 0.0f;

    mesh cube = createCube();

    // Event loop
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float fElapsedTime = (currentTime - lastTime) / 1000.f;
        lastTime = currentTime;

        fTheta += 1.0f * fElapsedTime;
        
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
        // float2 a = {WIDTH * 0.15f, HEIGHT * 0.85f}; // lower left point
        // float2 b = {WIDTH * 0.25f, HEIGHT * 0.15f}; // upper point;
        // float2 c = {WIDTH * 0.35f, HEIGHT * 0.85f}; // lower right point
        // draw_triangle(pixel_buffer, a, b , c, color_from_rgb(0, 240, 0));

        // Draw a lot of test triangles
        // test_triangle_rendering(pixel_buffer);

        // Render 3D Cube
        test_rotating_cube3D(pixel_buffer, matProj, cube, fTheta);

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