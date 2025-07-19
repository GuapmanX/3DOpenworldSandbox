#pragma once

struct AtlasData
{
    float AtlasSizeX, AtlasSizeY;
    float TextureSizeX, TextureSizeY;
    float TexturePosX, TexturePosY;
};

struct TextureID
{
    int id;
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
    RGB Color = { 1.0f, 0.0f, 0.0f };
    TextureCoordinates2D TC{ 0.0f, 0.0f };
    NormalVector NV{ 0.0f,0.0f,0.0f };
    AtlasData AtlasData{ 256.0f, 256.0f, 16.0f, 16.0f, 0.0f, 15.0f };
};

struct Quad
{
    Vertex v0, v1, v2, v3;
};