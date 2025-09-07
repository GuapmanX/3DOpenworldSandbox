#pragma once

struct AtlasData
{

    float TexturePosX, TexturePosY;
};

struct Vector3
{
    float x, y, z;
};

struct RGB
{
    float R, G, B;
};

struct TextureCoordinates2D
{
    float x, y;
};


struct NormalVector
{
    float x, y, z;
};

struct Vertex
{
    Vector3 Position{ 0.0f ,0.0f, 0.0f };
    TextureCoordinates2D TC{ 0.0f, 0.0f };
    NormalVector NV{ 0.0f,0.0f,0.0f };
    AtlasData AtlasData{ 0.0f, 0.0f };
};

struct Quad
{
    Vertex v0, v1, v2, v3;
};