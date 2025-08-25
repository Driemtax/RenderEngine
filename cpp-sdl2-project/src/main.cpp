#include "engine/engine.h"
#include "helpers/parser.h"
#include "string"

const int WIDTH = 1280;
const int HEIGHT = 720;

// Draws a triangle with given points a,b,c and color
// void fill_triangle(Uint32* buffer, float2 a, float2 b, float2 c, Uint32 color) {
//     // iterate over every pixel and check if it is inside the triangle
//     for (int x = 0; x < WIDTH; x++)
//     {
//         for (int y = 0; y < HEIGHT; y++)
//         {
//             float2 currentPixel = float2{float(x),float(y)};
//             bool inTriangle = math::PointInTriangle(a,b,c,currentPixel);
//             if (inTriangle)
//             {
//                 set_pixel(buffer, x, y, color);
//             }
//         }
//     }
    
// }

mesh create_cube() {
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
// void test_triangle_rendering(Uint32* buffer) {
//     // 1. Grid of small triangles (overlapping)
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 5; j++) {
//             float2 a = {i * 80.0f + 50, j * 80.0f + 50};
//             float2 b = {i * 80.0f + 100, j * 80.0f + 50};
//             float2 c = {i * 80.0f + 75, j * 80.0f + 20};
            
//             // Rainbow colors based on position
//             Uint8 r = (i * 32) % 256;
//             Uint8 g = (j * 51) % 256;
//             Uint8 bl = ((i + j) * 40) % 256;
            
//             fill_triangle(buffer, a, b, c, color_from_rgb(r, g, bl));
//         }
//     }
    
//     // 2. Two triangles forming a rectangle (quad)
//     float2 rect_a = {400, 200};
//     float2 rect_b = {600, 200};
//     float2 rect_c = {400, 350};
//     float2 rect_d = {600, 350};
    
//     // First triangle (top-left, top-right, bottom-left)
//     fill_triangle(buffer, rect_a, rect_b, rect_c, color_from_rgb(255, 100, 100));
//     // Second triangle (top-right, bottom-right, bottom-left)
//     fill_triangle(buffer, rect_b, rect_d, rect_c, color_from_rgb(100, 100, 255));
    
//     // 3. Large overlapping triangles
//     float2 big1_a = {200, 400};
//     float2 big1_b = {800, 500};
//     float2 big1_c = {300, 650};
//     fill_triangle(buffer, big1_a, big1_b, big1_c, color_from_rgb(255, 255, 0));
    
//     float2 big2_a = {400, 350};
//     float2 big2_b = {900, 400};
//     float2 big2_c = {500, 700};
//     fill_triangle(buffer, big2_a, big2_b, big2_c, color_from_rgb(0, 255, 255));
    
//     // 4. Triangles going off-screen (clipping test)
//     // Top edge
//     float2 clip1_a = {WIDTH * 0.3f, -50};
//     float2 clip1_b = {WIDTH * 0.7f, -50};
//     float2 clip1_c = {WIDTH * 0.5f, 100};
//     fill_triangle(buffer, clip1_a, clip1_b, clip1_c, color_from_rgb(255, 0, 255));
    
//     // Right edge
//     float2 clip2_a = {WIDTH - 50, HEIGHT * 0.3f};
//     float2 clip2_b = {WIDTH + 100, HEIGHT * 0.5f};
//     float2 clip2_c = {WIDTH - 50, HEIGHT * 0.7f};
//     fill_triangle(buffer, clip2_a, clip2_b, clip2_c, color_from_rgb(0, 255, 0));
    
//     // Bottom edge
//     float2 clip3_a = {WIDTH * 0.6f, HEIGHT - 50};
//     float2 clip3_b = {WIDTH * 0.9f, HEIGHT + 100};
//     float2 clip3_c = {WIDTH * 0.8f, HEIGHT - 100};
//     fill_triangle(buffer, clip3_a, clip3_b, clip3_c, color_from_rgb(255, 128, 0));
    
//     // Left edge
//     float2 clip4_a = {-100, HEIGHT * 0.6f};
//     float2 clip4_b = {150, HEIGHT * 0.4f};
//     float2 clip4_c = {50, HEIGHT * 0.8f};
//     fill_triangle(buffer, clip4_a, clip4_b, clip4_c, color_from_rgb(128, 0, 255));
    
//     // 5. Very thin triangles (edge case testing)
//     float2 thin1_a = {1000, 100};
//     float2 thin1_b = {1200, 102};
//     float2 thin1_c = {1100, 200};
//     fill_triangle(buffer, thin1_a, thin1_b, thin1_c, color_from_rgb(255, 255, 255));
    
//     // 6. Degenerate case - very small triangle
//     float2 tiny_a = {50, 50};
//     float2 tiny_b = {52, 50};
//     float2 tiny_c = {51, 52};
//     fill_triangle(buffer, tiny_a, tiny_b, tiny_c, color_from_rgb(255, 0, 0));
// }

int main(int argc, char* argv[]) {
    // Init engine
    RenderEngine engine(WIDTH, HEIGHT, "CPU Render Engine");

    if (!engine.initialize())
    {
        return -1;
    }

    // Load/create objects
    std::string path = "/home/driemtax/dev/RenderEngine/cpp-sdl2-project/assets/cube.obj";
    mesh object = parser::parse_wavefont_file(path);

    float rotation = 0.0f;

    // Main loop
    while (!engine.should_close())
    {
        engine.poll_events();

        // Update rotation
        rotation += 1.0f * engine.get_elapsed_time();

        // Render scene
        engine.render_mesh(object, rotation);
    }

    engine.shutdown();
    return 0;
}