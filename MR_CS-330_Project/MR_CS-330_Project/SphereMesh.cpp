#include "SphereMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


SphereMesh::SphereMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float radius, int slices, int stacks)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), radius(radius), slices(slices), stacks(stacks)
{
}

SphereMesh::SphereMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float radius, int slices, int stacks)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), radius(radius), slices(slices), stacks(stacks)
{
}


// #################
// # Other methods #
// #################


void SphereMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }
    float r = color.r;
    float g = color.g;
    float b = color.b;
    float a = color.a;

    int numVertices = (slices + 1) * (stacks + 1); // Total number of vertices in the mesh

    // Allocate buffer space for vertices, normals, and texture coordinates
    int floatsPerAttribute = floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV;
    vertexBuffer.resize(floatsPerAttribute * numVertices);

    // Allocate buffer space for indices
    elementBuffer.resize(2 * slices * stacks * 3);

    float horizontalAngleIncrement = 2 * glm::pi<float>() / slices; // Angle increment in the horizontal direction
    float verticalAngleIncrement = glm::pi<float>() / stacks; // Angle increment in the vertical direction
    float horizontalAngle, verticalAngle, x, y, z; // Variables for angles and Cartesian coordinates
    int vertexBufferIndex = 0; // Index for the vertex buffer

    // Generate vertices
    for (int stack = 0; stack <= stacks; stack++) {
        verticalAngle = -glm::pi<float>() / 2 + stack * verticalAngleIncrement; // Calculate the vertical angle
        for (int slice = 0; slice <= slices; slice++) {
            horizontalAngle = slice * horizontalAngleIncrement; // Calculate the horizontal angle

            // Calculate Cartesian coordinates using spherical coordinates
            x = glm::cos(horizontalAngle) * glm::cos(verticalAngle);
            y = glm::sin(horizontalAngle) * glm::cos(verticalAngle);
            z = glm::sin(verticalAngle);

            // Position
            vertexBuffer[vertexBufferIndex++] = radius * x; // x-coordinate of vertex
            vertexBuffer[vertexBufferIndex++] = radius * y; // y-coordinate of vertex
            vertexBuffer[vertexBufferIndex++] = radius * z; // z-coordinate of vertex

            // Color
            if (floatsPerColor > 0) {
                vertexBuffer[vertexBufferIndex++] = r; // r-component of color
                vertexBuffer[vertexBufferIndex++] = g; // g-component of color
                vertexBuffer[vertexBufferIndex++] = b; // b-component of color
                vertexBuffer[vertexBufferIndex++] = a; // a-component of color
            }

            // Normal
            if (floatsPerNormal > 0) 
            {
                vertexBuffer[vertexBufferIndex++] = x; // x-component of normal
                vertexBuffer[vertexBufferIndex++] = y; // y-component of normal
                vertexBuffer[vertexBufferIndex++] = z; // z-component of normal
            }

            // Texture Coordinates
            if (floatsPerUV > 0) {
                vertexBuffer[vertexBufferIndex++] = clampU(static_cast<float>(slice) / static_cast<float>(slices)); // u texture coordinate
                vertexBuffer[vertexBufferIndex++] = clampV(static_cast<float>(stack) / static_cast<float>(stacks)); // v texture coordinate
            }
        }
    }

    int elementBufferIndex = 0; // Index for the element buffer

    // Generate indices
    for (int stack = 0; stack < stacks; stack++) {
        int row1 = stack * (slices + 1);
        int row2 = (stack + 1) * (slices + 1);
        for (int slice = 0; slice < slices; slice++) {
            // Define indices to create triangles for the mesh
            elementBuffer[elementBufferIndex++] = row1 + slice;
            elementBuffer[elementBufferIndex++] = row2 + slice + 1;
            elementBuffer[elementBufferIndex++] = row2 + slice;
            elementBuffer[elementBufferIndex++] = row1 + slice;
            elementBuffer[elementBufferIndex++] = row1 + slice + 1;
            elementBuffer[elementBufferIndex++] = row2 + slice + 1;
        }
    }
}
