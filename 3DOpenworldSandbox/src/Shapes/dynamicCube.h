#pragma once

#include "VertexStructs.h"
#include "vector"

enum Faces :int
{
    Front, Top, Left, Right, Back, Bottom
};

template <int FaceAmount>
struct Cube
{
    Quad Quads[FaceAmount];
    const int Size = FaceAmount;
    int Face[FaceAmount];

    inline Quad& GetVertexData() {
        return Quads;
    }

    void SetQuadTextureData(Faces desiredQuad, AtlasData Data)
    {
        int face = 0;
        for (int i = 0; i < FaceAmount; i++)
        {
            if (Face[i] == desiredQuad)
            {
                face = i;
                break;
            }
        }

        Quads[face].v0.AtlasData = Data;
        Quads[face].v1.AtlasData = Data;
        Quads[face].v2.AtlasData = Data;
        Quads[face].v3.AtlasData = Data;
    }

    inline unsigned int GetBufferSize()
    {
        return sizeof(Quads);
    }
};


static int HowManySides(const bool arr[6])
{
    int faces = 0;
    for (int i = 0; i < 6; i++)
    {
        if (arr[i]) {
            faces++;
        }
    }
    return faces;
}

#include <iostream>

template<int FaceAmount>
static Cube<FaceAmount> CreateCube(float x, float y, float z, float size,const bool render[6])
{
    Cube<FaceAmount> C;
    //const int FacesAmount = HowManySides(render);


    int iterator = 0;

    //FRONT
    if (render[Faces::Front] && (iterator < FaceAmount)) {
        C.Quads[iterator].v0.Position = { x - size, y - size, z + size };
        C.Quads[iterator].v1.Position = { x + size, y - size, z + size };
        C.Quads[iterator].v2.Position = { x + size, y + size, z + size };
        C.Quads[iterator].v3.Position = { x - size, y + size, z + size };

        C.Quads[iterator].v0.TC = { 0.0f, 0.0f };
        C.Quads[iterator].v1.TC = { 1.0f, 0.0f };
        C.Quads[iterator].v2.TC = { 1.0f, 1.0f };
        C.Quads[iterator].v3.TC = { 0.0f, 1.0f };

        C.Quads[iterator].v0.NV = { 0.0f, 0.0f, -1.0f };
        C.Quads[iterator].v1.NV = { 0.0f, 0.0f, -1.0f };
        C.Quads[iterator].v2.NV = { 0.0f, 0.0f, -1.0f };
        C.Quads[iterator].v3.NV = { 0.0f, 0.0f, -1.0f };

        C.Face[iterator] = Faces::Front;
        iterator++;
    }


    //TOP
    if (render[Faces::Top] && (iterator < FaceAmount)) {
        C.Quads[iterator].v0.Position = { x - size, y + size, z + size };
        C.Quads[iterator].v1.Position = { x + size, y + size, z + size };
        C.Quads[iterator].v2.Position = { x + size, y + size, z - size };
        C.Quads[iterator].v3.Position = { x - size, y + size, z - size };

        C.Quads[iterator].v0.TC = { 0.0f, 0.0f };
        C.Quads[iterator].v1.TC = { 1.0f, 0.0f };
        C.Quads[iterator].v2.TC = { 1.0f, 1.0f };
        C.Quads[iterator].v3.TC = { 0.0f, 1.0f };

        C.Quads[iterator].v0.NV = { 0.0f, 1.0f, 0.0f };
        C.Quads[iterator].v1.NV = { 0.0f, 1.0f, 0.0f };
        C.Quads[iterator].v2.NV = { 0.0f, 1.0f, 0.0f };
        C.Quads[iterator].v3.NV = { 0.0f, 1.0f, 0.0f };

        C.Face[iterator] = Faces::Top;
        iterator++;
    }


    //LEFT
    if (render[Faces::Left] && (iterator < FaceAmount)) {
        C.Quads[iterator].v0.Position = { x - size, y - size, z - size };
        C.Quads[iterator].v1.Position = { x - size, y - size, z + size };
        C.Quads[iterator].v2.Position = { x - size, y + size, z + size };
        C.Quads[iterator].v3.Position = { x - size, y + size, z - size };

        C.Quads[iterator].v0.TC = { 0.0f, 0.0f };
        C.Quads[iterator].v1.TC = { 1.0f, 0.0f };
        C.Quads[iterator].v2.TC = { 1.0f, 1.0f };
        C.Quads[iterator].v3.TC = { 0.0f, 1.0f };

        C.Quads[iterator].v0.NV = { -1.0f, 0.0f, 0.0f };
        C.Quads[iterator].v1.NV = { -1.0f, 0.0f, 0.0f };
        C.Quads[iterator].v2.NV = { -1.0f, 0.0f, 0.0f };
        C.Quads[iterator].v3.NV = { -1.0f, 0.0f, 0.0f };

        C.Face[iterator] = Faces::Left;
        iterator++;
    }



    //RIGHT
    if (render[Faces::Right] && (iterator < FaceAmount)) {
        C.Quads[iterator].v0.Position = { x + size, y - size, z + size };
        C.Quads[iterator].v1.Position = { x + size, y - size, z - size };
        C.Quads[iterator].v2.Position = { x + size, y + size, z - size };
        C.Quads[iterator].v3.Position = { x + size, y + size, z + size };

        C.Quads[iterator].v0.TC = { 0.0f, 0.0f };
        C.Quads[iterator].v1.TC = { 1.0f, 0.0f };
        C.Quads[iterator].v2.TC = { 1.0f, 1.0f };
        C.Quads[iterator].v3.TC = { 0.0f, 1.0f };

        C.Quads[iterator].v0.NV = { 1.0f, 0.0f, 0.0f };
        C.Quads[iterator].v1.NV = { 1.0f, 0.0f, 0.0f };
        C.Quads[iterator].v2.NV = { 1.0f, 0.0f, 0.0f };
        C.Quads[iterator].v3.NV = { 1.0f, 0.0f, 0.0f };

        C.Face[iterator] = Faces::Right;
        iterator++;
    }


    //BACK
    if (render[Faces::Back] && (iterator < FaceAmount)) {
        C.Quads[iterator].v0.Position = { x + size, y - size, z - size };
        C.Quads[iterator].v1.Position = { x - size, y - size, z - size };
        C.Quads[iterator].v2.Position = { x - size, y + size, z - size };
        C.Quads[iterator].v3.Position = { x + size, y + size, z - size };

        C.Quads[iterator].v0.TC = { 0.0f, 0.0f };
        C.Quads[iterator].v1.TC = { 1.0f, 0.0f };
        C.Quads[iterator].v2.TC = { 1.0f, 1.0f };
        C.Quads[iterator].v3.TC = { 0.0f, 1.0f };

        C.Quads[iterator].v0.NV = { 0.0f, 0.0f, 1.0f };
        C.Quads[iterator].v1.NV = { 0.0f, 0.0f, 1.0f };
        C.Quads[iterator].v2.NV = { 0.0f, 0.0f, 1.0f };
        C.Quads[iterator].v3.NV = { 0.0f, 0.0f, 1.0f };

        C.Face[iterator] = Faces::Back;
        iterator++;
    }


    //BOTTOM
    std::cout << "Bottom: " << render[Faces::Bottom] << "Enough storage: " << (iterator < FaceAmount) << std::endl;
    if (render[Faces::Bottom] && (iterator < FaceAmount)) {
        C.Quads[iterator].v0.Position = { x - size, y - size, z - size };
        C.Quads[iterator].v1.Position = { x + size, y - size, z - size };
        C.Quads[iterator].v2.Position = { x + size, y - size, z + size };
        C.Quads[iterator].v3.Position = { x - size, y - size, z + size };

        C.Quads[iterator].v0.TC = { 0.0f, 0.0f };
        C.Quads[iterator].v1.TC = { 1.0f, 0.0f };
        C.Quads[iterator].v2.TC = { 1.0f, 1.0f };
        C.Quads[iterator].v3.TC = { 0.0f, 1.0f };

        C.Quads[iterator].v0.NV = { 0.0f, -1.0f, 0.0f };
        C.Quads[iterator].v1.NV = { 0.0f, -1.0f, 0.0f };
        C.Quads[iterator].v2.NV = { 0.0f, -1.0f, 0.0f };
        C.Quads[iterator].v3.NV = { 0.0f, -1.0f, 0.0f };

        C.Face[iterator] = Faces::Bottom;
    }
    /*else {
        std::cout << render[Faces::Bottom] << std::endl;
        std::cout << (iterator < FaceAmount) << std::endl;
    }*/


    return C;
};


template<int FaceAmount>
static void SetQuadTextureData(Cube<FaceAmount>& ref, Faces desiredQuad, AtlasData Data)
{
    int face = 0;
    for (int i = 0; i < FaceAmount; i++)
    {
        if (ref.Face[i] == desiredQuad)
        {
            face = i;
            break;
        }
    }

    ref.Quads[face].v0.AtlasData = Data;
    ref.Quads[face].v1.AtlasData = Data;
    ref.Quads[face].v2.AtlasData = Data;
    ref.Quads[face].v3.AtlasData = Data;
};

static unsigned int GetFullCubeSize()
{
    return sizeof(Quad) * 6;
}