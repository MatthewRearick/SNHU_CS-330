#include "PlaneMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


PlaneMesh::PlaneMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float width, float length)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), width(width), length(length)
{
}

PlaneMesh::PlaneMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float width, float length)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), width(width), length(length)
{
}


// #################
// # Other methods #
// #################



void PlaneMesh::generateVertices()
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
    float front = length / 2.0f;
    float back = -front;
    float center = 0.0f;

    if (vertexMode == POSITION_COLOR) {
        vertexBuffer = {
            // Vertex Positions (x, y, z),   Colors (r, g, b, a)
            // Bottom Corners
             left, center, front,   r, g, b, a, // Left, Bottom, Front
             left, center,  back,   r, g, b, a, // Left, Bottom, Back
            right, center,  back,   r, g, b, a, // Right, Bottom, Back
            right, center, front,   r, g, b, a, // Right, Bottom, Front
        };

        elementBuffer = {
            0, 1, 2,    // Triangle 1 - Center: Left, Back
            0, 3, 2,    // Triangle 2 - Center: Right, Front
        };
    }
    else if (vertexMode == POSITION_UV) {

        // Reserve space for each vertex coordinate index
        // two triangles are needed at 3 vertices each
        int elementBufferResize = 2 * 3;
        elementBuffer.resize(elementBufferResize);
        vertexBuffer = {
            // Vertex Positions (x, y, z), -  Texture UV (u, v)
            // Back Left Triangle
             left, center, front,      clampU(0), clampV(0),
             left, center,  back,      clampU(0), clampV(1.0f),
            right, center,  back,   clampU(1.0f), clampV(1.0f),

            // Front Right Triangle
             left, center, front,      clampU(0), clampV(0),
            right, center, front,   clampU(1.0f), clampV(0),
            right, center,  back,   clampU(1.0f), clampV(1.0f),
        };

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }
    else if (vertexMode == POSITION_NORMAL_UV) {
        // Reserve space for each vertex coordinate
        // each face will get 2 triangle made of 3 vertexes with 8 attribute values 
        int floatsPerVertexNormalUV = floatsPerVertex + floatsPerNormal + floatsPerUV;
        int vertexBufferResize = 2 * 3 * floatsPerVertexNormalUV;
        vertexBuffer.resize(vertexBufferResize);

        // Reserve space for each vertex coordinate index
        // each face will get 2 triangle made of 3 vertexes
        int elementBufferResize = 2 * 3;
        elementBuffer.resize(elementBufferResize);

        glm::vec3 vertex1;
        glm::vec3 vertex2;
        glm::vec3 vertex3;
        glm::vec3 normal;
        // Vertex Positions (x, y, z), - Normal (nx, ny, nz) -  Texture UV (u, v)
        // Bottom - Back Left Triangle
        vertex1 = glm::vec3(left, center, front);
        vertex2 = glm::vec3(left, center, back);
        vertex3 = glm::vec3(right, center, back);
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
        vertex1 = glm::vec3(right, center, back);
        vertex2 = glm::vec3(right, center, front);
        vertex3 = glm::vec3(left, center, front);
        normal = calculateNormal(vertex1, vertex2, vertex3);

        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 0) = vertex1.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 1) = vertex1.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 2) = vertex1.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 3 + 7) = clampV(1);

        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 0) = vertex2.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 1) = vertex2.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 2) = vertex2.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 6) = clampU(1);
        vertexBuffer.at(floatsPerVertexNormalUV * 4 + 7) = clampV(0);

        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 0) = vertex3.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 1) = vertex3.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 2) = vertex3.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 3) = normal.x;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 4) = normal.y;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 5) = normal.z;
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 6) = clampU(0);
        vertexBuffer.at(floatsPerVertexNormalUV * 5 + 7) = clampV(0);

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }
    else {
        throw "Vertex mode not implemented yet.";
    }

}
