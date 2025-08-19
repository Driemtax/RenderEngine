#ifndef ENGINE_H
#define ENGINE_H

#include<SDL2/SDL.h>
#include "../models/mesh.cpp"
#include "../models/int2vec.cpp"
#include "../helpers/math.h"

struct m4x4 {
    float m[4][4] = { 0 };
};

class RenderEngine
{
private:
    // SDL Components
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* screen_texture;
    Uint32* pixel_buffer;

    // Engine state
    int screen_width;
    int screen_height;
    bool is_running;

    // 3D projection
    m4x4 projectionMatrix;

    // Timer
    Uint32 last_time;
    float elapsed_time;

    // internal helper functions
    void setup_projection_matrix(float fov, float near_plane, float far_plane) {
        float fAspectRatio = (float)screen_height / (float)screen_width;
        float fFovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

        // empty matrix first
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                projectionMatrix.m[i][j] = 0.0f;
            }
            
        }

        projectionMatrix.m[0][0] = fAspectRatio * fFovRad;
        projectionMatrix.m[1][1] = fFovRad;
        projectionMatrix.m[2][2] = far_plane / (far_plane - near_plane);
        projectionMatrix.m[3][2] = (-far_plane * near_plane) / (far_plane - near_plane);
        projectionMatrix.m[2][3] = 1.0f;
        projectionMatrix.m[3][3] = 0.0f;
    }
    void clear_screen() {
        memset(pixel_buffer, 0, screen_width * screen_height * sizeof(Uint32));
    };
    void present_frame() {
        SDL_UpdateTexture(screen_texture, NULL, pixel_buffer, screen_width * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    void update_timer() {
        Uint32 current_time = SDL_GetTicks();
        elapsed_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
    };

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

public:
    RenderEngine(int width, int height, const char* title) : 
        window(nullptr), renderer(nullptr), screen_texture(nullptr), 
        pixel_buffer(nullptr), screen_width(width), screen_height(height), 
        is_running(false), last_time(0), elapsed_time(0.0f) {
    }
    ~RenderEngine() {
        shutdown();
    };

    // Core engine functions
    bool initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            return false;
        }

        window = SDL_CreateWindow("3D CPU Engine",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                screen_width, screen_height,
                                SDL_WINDOW_SHOWN);
        
        if (!window)
        {
            SDL_Quit();
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        
        screen_texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                screen_width, screen_height);

        if (!screen_texture)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        pixel_buffer = new Uint32[screen_width * screen_height];
        if (!pixel_buffer)
        {
            SDL_DestroyTexture(screen_texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        setup_projection_matrix(90.0f, 0.1f, 1000.f);
        last_time = SDL_GetTicks();
        is_running = true;

        return true;
    };
    void shutdown() {
        if (pixel_buffer)
        {
            delete[] pixel_buffer;
            pixel_buffer = nullptr;
        }

        if (screen_texture)
        {
            SDL_DestroyTexture(screen_texture);
            screen_texture = nullptr;
        }

        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if (window) 
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
        is_running = false;
    };
    bool should_close() {
        return !is_running;
    };
    void poll_events() {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }

            // Handle Keyboard / Mouse Input here later
        }
        update_timer();
    };

    // Utility

    // auxilary function to add 3 color values in one 32-bit int. 8 bit per color and alpha
    Uint32 color_from_rbg(Uint8 r, Uint8 g, Uint8 b) {
        // Ignore alpha here and set it to full (255)
        // bit shift and bitwise or to add everything together
        return (255 << 24) | (r << 16) | (g << 8) | b;
    }

    // Rendering functions
    void render_mesh(const mesh& object, float rotation_angle) {
        clear_screen();

        // Set up rotation matrices
        m4x4 matRotZ, matRotX;

        // Rotation on Z-axis
        matRotZ.m[0][0] = cosf(rotation_angle);
        matRotZ.m[0][1] = sinf(rotation_angle);
        matRotZ.m[1][0] = -sinf(rotation_angle);
        matRotZ.m[1][1] = cosf(rotation_angle);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;

        // Rotation on X-axis
        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(rotation_angle * 0.5f);
        matRotX.m[1][2] = sinf(rotation_angle * 0.5f);
        matRotX.m[2][1] = -sinf(rotation_angle * 0.5f);
        matRotX.m[2][2] = cosf(rotation_angle * 0.5);
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
            MultiplyMatrixVektor(triTranslated.p[0], triProjected.p[0], projectionMatrix);
            MultiplyMatrixVektor(triTranslated.p[1], triProjected.p[1], projectionMatrix);
            MultiplyMatrixVektor(triTranslated.p[2], triProjected.p[2], projectionMatrix);

            // Scale into view
            triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
            triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
            triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f; 
            triProjected.p[0].x *= 0.5f * float(screen_width);
            triProjected.p[0].y *= 0.5f * float(screen_height);
            triProjected.p[1].x *= 0.5f * float(screen_width);
            triProjected.p[1].y *= 0.5f * float(screen_height);
            triProjected.p[2].x *= 0.5f * float(screen_width);
            triProjected.p[2].y *= 0.5f * float(screen_height);

            // rasterize triangle (for now just draw a wireframe triangle)
            draw_triangle(triProjected, color_from_rbg(255, 255, 255));
            //fill_triangle(triProjected, color_from_rbg(255, 255, 255));
        }

        present_frame();
    };
    
    // auxilary funtion to set a pixel in the buffer
    void set_pixel(int x, int y, Uint32 color) {
        if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
            pixel_buffer[y * screen_width + x] = color;
        }
    };

    // Draws a line from (x0,y0) to (x1,y1) with a given color
    void draw_line(int2vec from, int2vec to, Uint32 color) {
        int dx = std::abs(to.x - from.x);
        int dy = -std::abs(to.y - from.y);

        int step_x = (from.x < to.x) ? 1 : -1;
        int step_y = (from.y < to.y) ? 1 : -1;

        int error = dx + dy;

        while (true)
        {
            // 1. set the next pixel
            set_pixel(from.x, from.y, color);

            if (from.x == to.x && from.y == to.y)
            {
                break;
            }

            int error2 = 2 * error;

            if (error2 >= dy)
            {
                if (from.x == to.x)
                {
                    break;
                }
                error += dy;
                from.x += step_x;
            }
            if (error2 <= dx)
            {
                if (from.y == to.y)
                {
                    break;
                }
                error += dx;
                from.y += step_y;
            }
        }
    }

    // Draws a wireframed triangle by drawing lines between each point of the triangle with a given color
    void draw_triangle(const triangle& tri, Uint32 color) {
        // draw a line from a -> b, b -> c, c -> a
        int2vec a,b,c;
        a = int2vec{int(tri.p[0].x), int(tri.p[0].y)};
        b = int2vec{int(tri.p[1].x), int(tri.p[1].y)};
        c = int2vec{int(tri.p[2].x), int(tri.p[2].y)};

        draw_line(a, b, color);
        draw_line(b, c, color);
        draw_line(c, a, color);
    };

    // Draws a triangle with given points a,b,c and color
    void fill_triangle(const triangle& tri, Uint32 color) {
        // iterate over every pixel and check if it is inside the triangle
        for (int x = 0; x < screen_width; x++)
        {
            for (int y = 0; y < screen_height; y++)
            {
                float2 currentPixel = float2{float(x), float(y)};
                float2 a,b,c;
                a = float2{tri.p[0].x, tri.p[0].y};
                b = float2{tri.p[1].x, tri.p[1].y};
                c = float2{tri.p[2].x, tri.p[2].y};
                bool inTriangle = math::PointInTriangle(a,b,c,currentPixel);
                if (inTriangle)
                {
                    set_pixel(x, y, color);
                }
            }
        }
        
    }

    float get_elapsed_time() const { return elapsed_time; }

};

#endif
