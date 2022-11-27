#pragma once
#include <vector>
using std::vector;


struct Color
{
    float r;
    float g;
    float b;
    Color(float r,float g, float b)
    {
        this->r=r;
        this->g=g;
        this->b=b;
    }
    Color(){}
};

struct Pos
{
    float x;
    float y;
    float z;
    Pos(float x,float y,float z) {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    Pos(){}
};

class DataForDraw
{
public:
    static float PI;
public:
    Pos pos;
    float height;
    float radius;
    float rate;
    int sp;
    Color color;
    bool live;
public:
    DataForDraw(Color c, Pos p, float height=1.0f, float radius=0.7, int sp=540, float rate = 0.0f);
    ~DataForDraw();
    unsigned int VBO;
    vector<float> vdata;
    int vcount;
    void initVertexData();
    void updateVetexData();
    void bindForDraw();
};