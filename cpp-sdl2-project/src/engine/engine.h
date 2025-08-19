#include "models/mesh.cpp"

struct m4x4 
{
    float m[4][4] = { 0 };
};

class engine
{
private:
    mesh meshCube;
    m4x4 projectionMatrix;
    int screenHeight;
    int screenWidth;
    float fTheta;

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
    engine(int height, int width);
    ~engine();
};

engine::engine(int height, int width)
{
    this->screenHeight = height;
    this->screenWidth = width;
}

engine::~engine()
{
}
