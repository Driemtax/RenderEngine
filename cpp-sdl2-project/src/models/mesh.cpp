#ifndef MESH_CPP
#define MESH_CPP

#include "triangle.cpp"
#include <vector>
using namespace std;

struct mesh
{
    vector<triangle> tris;
};

#endif