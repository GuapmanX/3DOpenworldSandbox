#pragma once
#include "VertexStructs.h"


    struct CubeCCW
    {
        Quad Quads[6];
    };

    static CubeCCW CCWCreateCube(float x, float y, float z, float size)
    {
        CubeCCW C;

        //FRONT
        C.Quads[0].v0.Position = { x - size, y - size, z - size };
        C.Quads[0].v1.Position = { x + size, y - size, z - size };
        C.Quads[0].v2.Position = { x + size, y + size, z - size };
        C.Quads[0].v3.Position = { x - size, y + size, z - size };

        C.Quads[0].v0.TC = { 0.0f, 0.0f };
        C.Quads[0].v1.TC = { 1.0f, 0.0f };
        C.Quads[0].v2.TC = { 1.0f, 1.0f };
        C.Quads[0].v3.TC = { 0.0f, 1.0f };

        C.Quads[0].v0.NV = { 0.0f, 0.0f, -1.0f };
        C.Quads[0].v1.NV = { 0.0f, 0.0f, -1.0f };
        C.Quads[0].v2.NV = { 0.0f, 0.0f, -1.0f };
        C.Quads[0].v3.NV = { 0.0f, 0.0f, -1.0f };



        //TOP
        C.Quads[1].v0.Position = { x - size, y + size, z - size };
        C.Quads[1].v1.Position = { x + size, y + size, z - size };
        C.Quads[1].v2.Position = { x + size, y + size, z + size };
        C.Quads[1].v3.Position = { x - size, y + size, z + size };

        C.Quads[1].v0.TC = { 0.0f, 0.0f };
        C.Quads[1].v1.TC = { 1.0f, 0.0f };
        C.Quads[1].v2.TC = { 1.0f, 1.0f };
        C.Quads[1].v3.TC = { 0.0f, 1.0f };

        C.Quads[1].v0.NV = { 0.0f, 1.0f, 0.0f };
        C.Quads[1].v1.NV = { 0.0f, 1.0f, 0.0f };
        C.Quads[1].v2.NV = { 0.0f, 1.0f, 0.0f };
        C.Quads[1].v3.NV = { 0.0f, 1.0f, 0.0f };



        //LEFT
        C.Quads[2].v0.Position = { x - size, y - size, z - size };
        C.Quads[2].v1.Position = { x - size, y + size, z - size };
        C.Quads[2].v2.Position = { x - size, y + size, z + size };
        C.Quads[2].v3.Position = { x - size, y - size, z + size };

        C.Quads[2].v0.TC = { 0.0f, 0.0f };
        C.Quads[2].v1.TC = { 0.0f, 1.0f };
        C.Quads[2].v2.TC = { 1.0f, 1.0f };
        C.Quads[2].v3.TC = { 1.0f, 0.0f };

        //C.Quads[2].v0.TC = { 0.0f, 0.0f };
        //C.Quads[2].v1.TC = { 1.0f, 0.0f };
        //C.Quads[2].v2.TC = { 1.0f, 1.0f };
        //C.Quads[2].v3.TC = { 0.0f, 1.0f };

        C.Quads[2].v0.NV = { -1.0f, 0.0f, 0.0f };
        C.Quads[2].v1.NV = { -1.0f, 0.0f, 0.0f };
        C.Quads[2].v2.NV = { -1.0f, 0.0f, 0.0f };
        C.Quads[2].v3.NV = { -1.0f, 0.0f, 0.0f };




        //RIGHT
        C.Quads[3].v0.Position = { x + size, y - size, z - size };
        C.Quads[3].v1.Position = { x + size, y + size, z - size };
        C.Quads[3].v2.Position = { x + size, y + size, z + size };
        C.Quads[3].v3.Position = { x + size, y - size, z + size };


        C.Quads[3].v0.TC = { 0.0f, 0.0f };
        C.Quads[3].v1.TC = { 0.0f, 1.0f };
        C.Quads[3].v2.TC = { 1.0f, 1.0f };
        C.Quads[3].v3.TC = { 1.0f, 0.0f };

        //C.Quads[3].v0.TC = { 0.0f, 0.0f };
        //C.Quads[3].v1.TC = { 1.0f, 0.0f };
        //C.Quads[3].v2.TC = { 1.0f, 1.0f };
        //C.Quads[3].v3.TC = { 0.0f, 1.0f };

        C.Quads[3].v0.NV = { 1.0f, 0.0f, 0.0f };
        C.Quads[3].v1.NV = { 1.0f, 0.0f, 0.0f };
        C.Quads[3].v2.NV = { 1.0f, 0.0f, 0.0f };
        C.Quads[3].v3.NV = { 1.0f, 0.0f, 0.0f };



        //BACK
        C.Quads[4].v0.Position = { x - size, y - size, z + size };
        C.Quads[4].v1.Position = { x + size, y - size, z + size };
        C.Quads[4].v2.Position = { x + size, y + size, z + size };
        C.Quads[4].v3.Position = { x - size, y + size, z + size };

        C.Quads[4].v0.TC = { 0.0f, 0.0f };
        C.Quads[4].v1.TC = { 1.0f, 0.0f };
        C.Quads[4].v2.TC = { 1.0f, 1.0f };
        C.Quads[4].v3.TC = { 0.0f, 1.0f };

        C.Quads[4].v0.NV = { 0.0f, 0.0f, 1.0f };
        C.Quads[4].v1.NV = { 0.0f, 0.0f, 1.0f };
        C.Quads[4].v2.NV = { 0.0f, 0.0f, 1.0f };
        C.Quads[4].v3.NV = { 0.0f, 0.0f, 1.0f };



        //BOTTOM
        C.Quads[5].v0.Position = { x - size, y - size, z + size };
        C.Quads[5].v1.Position = { x + size, y - size, z + size };
        C.Quads[5].v2.Position = { x + size, y - size, z - size };
        C.Quads[5].v3.Position = { x - size, y - size, z - size };

        C.Quads[5].v0.TC = { 0.0f, 0.0f };
        C.Quads[5].v1.TC = { 1.0f, 0.0f };
        C.Quads[5].v2.TC = { 1.0f, 1.0f };
        C.Quads[5].v3.TC = { 0.0f, 1.0f };

        C.Quads[5].v0.NV = { 0.0f, -1.0f, 0.0f };
        C.Quads[5].v1.NV = { 0.0f, -1.0f, 0.0f };
        C.Quads[5].v2.NV = { 0.0f, -1.0f, 0.0f };
        C.Quads[5].v3.NV = { 0.0f, -1.0f, 0.0f };

        return C;
    }

    static bool CCWPopulateCubeIndexBuffer(unsigned int(&arr)[36])
    {
        //Creates an index buffer for a single cube
        for (int i = 0; i < 6; i++) {
            arr[i] = 0 + i * 4;
            arr[i + 1] = 1 + i * 4;
            arr[i + 2] = 2 + i * 4;
            arr[i + 3] = 2 + i * 4;
            arr[i + 4] = 3 + i * 4;
            arr[i + 5] = 0 + i * 4;
        }
        return true;
    };

    static void CCWSetQuadTextureData(CubeCCW& ref, int desiredQuad, AtlasData Data)
    {
        ref.Quads[desiredQuad].v0.AtlasData = Data;
        ref.Quads[desiredQuad].v1.AtlasData = Data;
        ref.Quads[desiredQuad].v2.AtlasData = Data;
        ref.Quads[desiredQuad].v3.AtlasData = Data;
    };