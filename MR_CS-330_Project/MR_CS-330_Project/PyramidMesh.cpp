#include "PyramidMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


PyramidMesh::PyramidMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float width, float height, float length)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), width(width), height(height), length(length)
{
}

PyramidMesh::PyramidMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float width, float height, float length)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), width(width), height(height), length(length)
{
}


// #################
// # Other methods #
// #################


void PyramidMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }
    float r = color.r;
    float g = color.g;
    float b = color.b;
    float a = color.a;

    // Define relative offsets from width, height, and length
    float right = width / 2.0f;
    float left = -right;
    float top = height / 2.0f;
    float bottom = -top;
    float front = length / 2.0f;
    float back = -front;
    float center = 0.0f;

    if (vertexMode == POSITION_COLOR) {
        vertexBuffer = {
            // Vertex Positions (x, y, z),   Colors (r, g, b, a)
            // Base Corners
             left, bottom, front,   r, g, b, a, // Left, Bottom, Front
             left, bottom,  back,   r, g, b, a, // Left, Bottom, Back
            right, bottom,  back,   r, g, b, a, // Right, Bottom, Back
            right, bottom, front,   r, g, b, a, // Right, Bottom, Front

            // Peak
             center,  top,  center,   r, g, b, a, // Center, Top, Center
        };

        elementBuffer = {
            0, 1, 2,  // Triangle 1 - Base: Left, Back
            2, 3, 0,  // Triangle 2 - Base: Right, Front
            0, 4, 3,  // Triangle 3 - Side: Front
            1, 4, 0,  // Triangle 4 - Side: Left
            2, 4, 1,  // Triangle 5 - Side: Back
            3, 4, 2,  // Triangle 6 - Side: Right
        };
    }
    else if (vertexMode == POSITION_UV) {
        // Reserve space for each vertex coordinate index
        // each side will get one triangle made of 3 vertices and the base will be two triangels
        int elementBufferResize = (4 + 2) * 3;
        elementBuffer.resize(elementBufferResize);
        vertexBuffer = {
            // Vertex Positions (x, y, z), -  Texture UV (u, v)
            // Base Back Left Triangle
             left, bottom, front,       clampU(0.0f), clampV(0.0f),  // Left, Bottom, Front - Left, Bottom
             left, bottom,  back,       clampU(0.0f), clampV(1.0f),  // Left, Bottom, Back - Left, Top
            right, bottom,  back,       clampU(1.0f), clampV(1.0f),  // Right, Bottom, Back - Right, Top

            // Base Front Right Triangle
             left, bottom, front,       clampU(0.0f), clampV(0.0f),  // Left, Bottom, Front - Left, Bottom
            right, bottom, front,       clampU(1.0f), clampV(0.0f),  // Right, Bottom, Front - Right, Bottom
            right, bottom,  back,       clampU(1.0f), clampV(1.0f),  // Right, Bottom, Back - Right, Top

            // Front Side
              left, bottom,  front,     clampU(0.0f), clampV(0.0f),  // Left, Bottom, Front - Left, Bottom
            center,    top, center,     clampU(0.5f), clampV(1.0f),  // Center, Top, Center - Center, Top
             right, bottom,  front,     clampU(1.0f), clampV(0.0f),  // Right, Bottom, Front - Right, Bottom

            // Left Side - Flipped texture horazontally for nice wrap between sides
              left, bottom,   back,     clampU(1.0f), clampV(0.0f),  // Left, Bottom, Back - Right, Bottom
            center,    top, center,     clampU(0.5f), clampV(1.0f),  // Center, Top, Center - Center, Top
              left, bottom,  front,     clampU(0.0f), clampV(0.0f),  // Left, Bottom, Front - Left, Bottom

            // Back Side - Flipped texture horazontally for nice wrap between sides
              left, bottom,   back,     clampU(1.0f), clampV(0.0f),  // Left, Bottom, Back - Right, Bottom
            center,    top, center,     clampU(0.5f), clampV(1.0f),  // Center, Top, Center - Center, Top
             right, bottom,   back,     clampU(0.0f), clampV(0.0f),  // Right, Bottom, Back - Left, Bottom

            // Right Side
             right, bottom,   back,     clampU(0.0f), clampV(0.0f),  // Right, Bottom, Back - Left, Bottom
            center,    top, center,     clampU(0.5f), clampV(1.0f),  // Center, Top, Center - Center, Top
             right, bottom,  front,     clampU(1.0f), clampV(0.0f),  // Right, Bottom, Front - Right, Bottom
        };

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }
    else if (vertexMode == POSITION_NORMAL_UV) {
        // Reserve space for each vertex coordinate
        // Each side will get one triangle and the base will get 2 triangles
        // Each triangle will use 3 vertices each with 8 attributes 
        int floatsPerVertexNormalUV = floatsPerVertex + floatsPerNormal + floatsPerUV;
        int vertexBufferResize = (4 + 2) * 3 * floatsPerVertexNormalUV;
        vertexBuffer.resize(vertexBufferResize);

        // Reserve space for each vertex coordinate index
        // each face will get 2 triangle made of 3 vertexes
        int elementBufferResize = (4 + 2) * 3;
        elementBuffer.resize(elementBufferResize);

        glm::vec3 vertex1;
        glm::vec3 vertex2;
        glm::vec3 vertex3;
        glm::vec3 normal;
        // Vertex Positions (x, y, z), - Normal (nx, ny, nz) -  Texture UV (u, v)
        // Bottom - Back Left Triangle
        vertex1 = glm::vec3(left, bottom, front);
        vertex2 = glm::vec3(left, bottom, back);
        vertex3 = glm::vec3(right, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 7) = clampV(1);

        // Bottom - Front Right Triangle
        vertex1 = glm::vec3(left, bottom, front);
        vertex2 = glm::vec3(right, bottom, front);
        vertex3 = glm::vec3(right, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 7) = clampV(1);


        // Front
        vertex1 = glm::vec3(left, bottom, front);
        vertex2 = glm::vec3(center, top, center);
        vertex3 = glm::vec3(right, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 6) = clampU(0.5f);
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 7) = clampV(0);


        // Left
        vertex1 = glm::vec3(left, bottom, back);
        vertex2 = glm::vec3(center, top, center);
        vertex3 = glm::vec3(left, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 6) = clampU(0.5f);
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 7) = clampV(0);


        // Back
        vertex1 = glm::vec3(left, bottom, back);
        vertex2 = glm::vec3(center, top, center);
        vertex3 = glm::vec3(right, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 6) = clampU(0.5f);
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 7) = clampV(0);


        // Right Side
        vertex1 = glm::vec3(right, bottom, back);
        vertex2 = glm::vec3(center, top, center);
        vertex3 = glm::vec3(right, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 6) = clampU(0.5f);
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 7) = clampV(0);

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }
    else {
        throw "Vertex mode not implemented yet.";
    }
}
