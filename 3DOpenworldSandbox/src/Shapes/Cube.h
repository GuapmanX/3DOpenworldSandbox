#pragma once
#include <array>

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
    };

    struct Quad
    {
        Vertex v0, v1, v2, v3;
    };

    struct Cube
    {
        Quad q0, q1, q2, q3, q4, q5;
    };

    static Cube CreateCube(float x, float y, float z, float size)
    {
        Cube C;

        //FRONT
        C.q0.v0.Position = { x - size, y - size, z - size };
        C.q0.v1.Position = { x + size, y - size, z - size };
        C.q0.v2.Position = { x + size, y + size, z - size };
        C.q0.v3.Position = { x - size, y + size, z - size };

        C.q0.v0.TC = { 0.0f, 0.0f };
        C.q0.v1.TC = { 1.0f, 0.0f };
        C.q0.v2.TC = { 1.0f, 1.0f };
        C.q0.v3.TC = { 0.0f, 1.0f };

        C.q0.v0.NV = { 0.0f, 0.0f, -1.0f };
        C.q0.v1.NV = { 0.0f, 0.0f, -1.0f };
        C.q0.v2.NV = { 0.0f, 0.0f, -1.0f };
        C.q0.v3.NV = { 0.0f, 0.0f, -1.0f };

        //TOP
        C.q1.v0.Position = { x - size, y + size, z - size };
        C.q1.v1.Position = { x + size, y + size, z - size };
        C.q1.v2.Position = { x + size, y + size, z + size };
        C.q1.v3.Position = { x - size, y + size, z + size };

        C.q1.v0.TC = { 0.0f, 0.0f };
        C.q1.v1.TC = { 1.0f, 0.0f };
        C.q1.v2.TC = { 1.0f, 1.0f };
        C.q1.v3.TC = { 0.0f, 1.0f };

        C.q1.v0.NV = { 0.0f, 1.0f, 0.0f };
        C.q1.v1.NV = { 0.0f, 1.0f, 0.0f };
        C.q1.v2.NV = { 0.0f, 1.0f, 0.0f };
        C.q1.v3.NV = { 0.0f, 1.0f, 0.0f };

        //LEFT
        C.q2.v0.Position = { x - size, y - size, z - size };
        C.q2.v1.Position = { x - size, y + size, z - size };
        C.q2.v2.Position = { x - size, y + size, z + size };
        C.q2.v3.Position = { x - size, y - size, z + size };

        C.q2.v0.TC = { 0.0f, 0.0f };
        C.q2.v1.TC = { 1.0f, 0.0f };
        C.q2.v2.TC = { 1.0f, 1.0f };
        C.q2.v3.TC = { 0.0f, 1.0f };

        C.q2.v0.NV = { -1.0f, 0.0f, 0.0f };
        C.q2.v1.NV = { -1.0f, 0.0f, 0.0f };
        C.q2.v2.NV = { -1.0f, 0.0f, 0.0f };
        C.q2.v3.NV = { -1.0f, 0.0f, 0.0f };


        //RIGHT
        C.q3.v0.Position = { x + size, y - size, z - size };
        C.q3.v1.Position = { x + size, y + size, z - size };
        C.q3.v2.Position = { x + size, y + size, z + size };
        C.q3.v3.Position = { x + size, y - size, z + size };

        C.q3.v0.TC = { 0.0f, 0.0f };
        C.q3.v1.TC = { 1.0f, 0.0f };
        C.q3.v2.TC = { 1.0f, 1.0f };
        C.q3.v3.TC = { 0.0f, 1.0f };

        C.q3.v0.NV = { 1.0f, 0.0f, 0.0f };
        C.q3.v1.NV = { 1.0f, 0.0f, 0.0f };
        C.q3.v2.NV = { 1.0f, 0.0f, 0.0f };
        C.q3.v3.NV = { 1.0f, 0.0f, 0.0f };

        //BACK
        C.q4.v0.Position = { x - size, y - size, z + size };
        C.q4.v1.Position = { x + size, y - size, z + size };
        C.q4.v2.Position = { x + size, y + size, z + size };
        C.q4.v3.Position = { x - size, y + size, z + size };

        C.q4.v0.TC = { 0.0f, 0.0f };
        C.q4.v1.TC = { 1.0f, 0.0f };
        C.q4.v2.TC = { 1.0f, 1.0f };
        C.q4.v3.TC = { 0.0f, 1.0f };

        C.q4.v0.NV = { 0.0f, 0.0f, 1.0f };
        C.q4.v1.NV = { 0.0f, 0.0f, 1.0f };
        C.q4.v2.NV = { 0.0f, 0.0f, 1.0f };
        C.q4.v3.NV = { 0.0f, 0.0f, 1.0f };

        //BOTTOM
        C.q5.v0.Position = { x - size, y - size, z + size };
        C.q5.v1.Position = { x + size, y - size, z + size };
        C.q5.v2.Position = { x + size, y - size, z - size };
        C.q5.v3.Position = { x - size, y - size, z - size };

        C.q5.v0.TC = { 0.0f, 0.0f };
        C.q5.v1.TC = { 1.0f, 0.0f };
        C.q5.v2.TC = { 1.0f, 1.0f };
        C.q5.v3.TC = { 0.0f, 1.0f };

        C.q5.v0.NV = { 0.0f, -1.0f, 0.0f };
        C.q5.v1.NV = { 0.0f, -1.0f, 0.0f };
        C.q5.v2.NV = { 0.0f, -1.0f, 0.0f };
        C.q5.v3.NV = { 0.0f, -1.0f, 0.0f };

        return C;
    };

    static bool PopulateCubeIndexBuffer(unsigned int(&arr)[36])
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