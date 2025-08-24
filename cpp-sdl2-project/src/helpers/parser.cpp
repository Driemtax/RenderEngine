#include "parser.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>



int parser::parse_vertex_index(const std::string& vertex_string) {
    size_t slash_pos = vertex_string.find('/');
    if (slash_pos != std::string::npos)
    {
        return std::stoi(vertex_string.substr(0, slash_pos));
    }
    return std::stoi(vertex_string);
    
}

mesh parser::parse_wavefont_file(const std::string& path) {
    mesh object;
    std::vector<float3> vertices;

    // open file
    std::ifstream file(path);
    if (!file.is_open())
    {
        return object;
    }

    std::string line;
    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        // parse vertices
        if (prefix == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(float3{x, y, z});
        }

        // parse faces
        else if (prefix == "f")
        {
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            // extract vertices indecies
            int v1 = parse_vertex_index(vertex1);
            int v2 = parse_vertex_index(vertex2);
            int v3 = parse_vertex_index(vertex3);

            // obj files have 1-based indecies, c++ is 0-based
            v1--; v2--; v3--;

            // create triangle
            triangle tri;
            tri.p[0] = vertices[v1];
            tri.p[1] = vertices[v2];
            tri.p[2] = vertices[v3];

            object.tris.push_back(tri);
        }
    }

    file.close();
    return object;
}