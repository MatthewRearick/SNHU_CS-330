#include "CubeMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


CubeMesh::CubeMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float width, float height, float length)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), width(width), height(height), length(length)
{
}

CubeMesh::CubeMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float width, float height, float length)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), width(width), height(height), length(length)
{
}


// #################
// # Other methods #
// #################


void CubeMesh::generateVertices()
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

    GLfloat oneThird = 1.0f / 3.0f;
    GLfloat twoThird = 2.0f / 3.0f;

    if (vertexMode == POSITION_COLOR) {
        vertexBuffer = {
            // Vertex Positions (x, y, z),   Colors (r, g, b, a)
            // Bottom Corners
             left, bottom, front,   r, g, b, a, // Left, Bottom, Front
             left, bottom,  back,   r, g, b, a, // Left, Bottom, Back
            right, bottom,  back,   r, g, b, a, // Right, Bottom, Back
            right, bottom, front,   r, g, b, a, // Right, Bottom, Front

            // Top Corners
             left, top, front,   r, g, b, a, // Left, Top, Front
             left, top,  back,   r, g, b, a, // Left, Top, Back
            right, top,  back,   r, g, b, a, // Right, Top, Back
            right, top, front,   r, g, b, a, // Right, Top, Front
        };

        elementBuffer = {
            0, 1, 2,    // Triangle 1 - Bottom: Left, Back
            0, 3, 2,    // Triangle 2 - Bottom: Right, Front
            4, 0, 3,    // Triangle 3 - Front: Bottom, Left
            4, 7, 3,    // Triangle 4 - Front: Top, Right
            5, 1, 0,    // Triangle 5 - Left: Back, Bottom
            5, 4, 0,    // Triangle 6 - Left: Front, Top
            6, 2, 1,    // Triangle 7 - Back: Bottom, Right
            6, 5, 1,    // Triangle 8 - Back: Top, Left
            7, 3, 2,    // Triangle 9 - Right: Front, Bottom
            7, 6, 2,    // Triangle 10 - Right: Back, Top
            5, 4, 7,    // Triangle 11 - Top: Front, Left
            5, 6, 7     // Triangle 12 - Top: Back, Right
        };
    }
    else if (vertexMode == POSITION_UV) {
        // Reserve space for each vertex coordinate index
        // each face will get 2 triangle made of 3 vertexes
        int elementBufferResize = 6 * 2 * 3;
        elementBuffer.resize(elementBufferResize);

        vertexBuffer = {
            // Vertex Positions (x, y, z), -  Texture UV (u, v)
            // Bottom - Back Left Triangle
             left, bottom, front,          clampU(0), clampV(0.5f),
             left, bottom,  back,   clampU(oneThird), clampV(0.5f),
            right, bottom,  back,   clampU(oneThird), clampV(1.0f),

            // Bottom - Front Right Triangle
             left, bottom, front,          clampU(0), clampV(0.5f),
            right, bottom, front,          clampU(0), clampV(1.0f),
            right, bottom,  back,   clampU(oneThird), clampV(1.0f),

            // Front - Bottom Left Triangle
             left,    top, front,   clampU(oneThird), clampV(0.5f),
             left, bottom, front,   clampU(oneThird), clampV(0.0f),
            right, bottom, front,   clampU(twoThird), clampV(0.0f),

            // Front - Top Right Triangle
             left,    top, front,   clampU(oneThird), clampV(0.5f),
            right,    top, front,   clampU(twoThird), clampV(0.5f),
            right, bottom, front,   clampU(twoThird), clampV(0.0f),

            // Left - Bottom Back Triangle
            left,    top,  back,          clampU(0), clampV(0.5f),
            left, bottom,  back,          clampU(0), clampV(0.0f),
            left, bottom, front,   clampU(oneThird), clampV(0.0f),

            // Left - Top Front Triangle
            left,    top,  back,          clampU(0), clampV(0.5f),
            left,    top, front,   clampU(oneThird), clampV(0.5f),
            left, bottom, front,   clampU(oneThird), clampV(0.0f),

            // Back - Bottom Right Triangle
            right,    top, back,   clampU(twoThird), clampV(1.0f),
            right, bottom, back,   clampU(oneThird), clampV(1.0f),
             left, bottom, back,   clampU(oneThird), clampV(0.5f),

            // Back - Top Left Triangle
            right,    top, back,   clampU(twoThird), clampV(1.0f),
             left,    top, back,   clampU(twoThird), clampV(0.5f),
             left, bottom, back,   clampU(oneThird), clampV(0.5f),

            // Right - Bottom Front Triangle
            right,    top, front,   clampU(twoThird), clampV(0.5f),
            right, bottom, front,   clampU(twoThird), clampV(0.0f),
            right, bottom,  back,          clampU(1), clampV(0.0f),

            // Right - Top Back Triangle
            right,    top, front,   clampU(twoThird), clampV(0.5f),
            right,    top,  back,          clampU(1), clampV(0.5f),
            right, bottom,  back,          clampU(1), clampV(0.0f),

            // Top - Front Left Triangle
             left, top,  back,  clampU(twoThird), clampV(0.5f),
             left, top, front,         clampU(1), clampV(0.5f),
            right, top, front,         clampU(1), clampV(1.0f),

            // Top - Back Right Triangle
             left, top,  back,  clampU(twoThird), clampV(0.5f),
            right, top,  back,  clampU(twoThird), clampV(1.0f),
            right, top, front,         clampU(1), clampV(1.0f),
        };

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }
    else if (vertexMode == POSITION_NORMAL_UV) {
        // Reserve space for each vertex coordinate
        // each face will get 2 triangle made of 3 vertexes with 8 attribute values 
        int floatsPerVertexNormalUV = floatsPerVertex + floatsPerNormal + floatsPerUV;
        int vertexBufferResize = 6 * 2 * 3 * floatsPerVertexNormalUV;
        vertexBuffer.resize(vertexBufferResize);

        // Reserve space for each vertex coordinate index
        // each face will get 2 triangle made of 3 vertexes
        int elementBufferResize = 6 * 2 * 3;
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
        vertexBuffer.at(floatsPerVertexNormalUV * 0 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 1 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 2 + 7) = clampV(1.0f);

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
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 7) = clampV(1.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 7) = clampV(1.0f);


        // Front - Bottom Left Triangle
        vertex1 = glm::vec3(left, top, front);
        vertex2 = glm::vec3(left, bottom, front);
        vertex3 = glm::vec3(right, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 6 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 7 + 7) = clampV(0.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 8 + 7) = clampV(0.0f);


        // Front - Top Right Triangle
        vertex1 = glm::vec3(left, top, front);
        vertex2 = glm::vec3(right, top, front);
        vertex3 = glm::vec3(right, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 9 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 10 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 11 + 7) = clampV(0.0f);


        // Left - Bottom Back Triangle
        vertex1 = glm::vec3(left, top, back);
        vertex2 = glm::vec3(left, bottom, back);
        vertex3 = glm::vec3(left, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 12 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 13 + 7) = clampV(0.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 14 + 7) = clampV(0.0f);


        // Left - Top Front Triangle
        vertex1 = glm::vec3(left, top, back);
        vertex2 = glm::vec3(left, top, front);
        vertex3 = glm::vec3(left, bottom, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 15 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 16 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 17 + 7) = clampV(0.0f);


        // Back - Bottom Right Triangle
        vertex1 = glm::vec3(right, top, back);
        vertex2 = glm::vec3(right, bottom, back);
        vertex3 = glm::vec3(left, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 18 + 7) = clampV(1.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 19 + 7) = clampV(1.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 20 + 7) = clampV(0.5f);


        // Back - Top Left Triangle
        vertex1 = glm::vec3(right, top, back);
        vertex2 = glm::vec3(left, top, back);
        vertex3 = glm::vec3(left, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 21 + 7) = clampV(1.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 22 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 6) = clampU(oneThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 23 + 7) = clampV(0.5f);


        // Right - Bottom Front Triangle
        vertex1 = glm::vec3(right, top, front);
        vertex2 = glm::vec3(right, bottom, front);
        vertex3 = glm::vec3(right, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 24 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 25 + 7) = clampV(0.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 26 + 7) = clampV(0.0f);


        // Right - Top Back Triangle
        vertex1 = glm::vec3(right, top, front);
        vertex2 = glm::vec3(right, top, back);
        vertex3 = glm::vec3(right, bottom, back);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 27 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 28 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 29 + 7) = clampV(0.0f);


        // Top - Front Left Triangle
        vertex1 = glm::vec3(left, top, back);
        vertex2 = glm::vec3(left, top, front);
        vertex3 = glm::vec3(right, top, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 30 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 31 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 32 + 7) = clampV(1.0f);


        // Top - Back Right Triangle
        vertex1 = glm::vec3(left, top, back);
        vertex2 = glm::vec3(right, top, back);
        vertex3 = glm::vec3(right, top, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 33 + 7) = clampV(0.5f);

        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 6) = clampU(twoThird);
        vertexBuffer.at(floatsPerVertexNormalUV * 34 + 7) = clampV(1.0f);

        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 35 + 7) = clampV(1.0f);

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
     }
    else {
        throw "Vertex mode not implemented yet.";
    }
}
