#include "TorusMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


TorusMesh::TorusMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float outerRadius, float innerRadius, int slices, int stacks)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), outerRadius(outerRadius), innerRadius(innerRadius), slices(slices), stacks(stacks)
{
}

TorusMesh::TorusMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float outerRadius, float innerRadius, int slices, int stacks)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), outerRadius(outerRadius), innerRadius(innerRadius), slices(slices), stacks(stacks)
{
}


// #################
// # Other methods #
// #################


void TorusMesh::generateVertices() {
    // Initialize Torus color for Torus modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }
    float r = color.r;
    float g = color.g;
    float b = color.b;
    float a = color.a;

    int numVertices = (slices + 1) * (stacks + 1); // Total number of vertices in the torus mesh

    // Allocate buffer space for vertices, normals, and texture coordinates
    int floatsPerAttribute = floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV;
    vertexBuffer.resize(floatsPerAttribute * numVertices);

    // Allocate buffer space for indices
    elementBuffer.resize(2 * slices * stacks * 3);

    float horizontalAngleIncrement = 2 * glm::pi<float>() / slices; // Angle increment in the horizontal direction
    float verticalAngleIncrement = 2 * glm::pi<float>() / stacks; // Angle increment in the vertical direction
    float centerRadius = (innerRadius + outerRadius) / 2;
    float tubeRadius = outerRadius - centerRadius;
    float horizontalAngle, verticalAngle, horizontalAngleCos, horizontalAngleSin, verticalAngleCos, verticalAngleSin, x, y, z; // Variables for angles and Cartesian coordinates
    int vertexBufferIndex = 0; // Index for the vertex buffer

    // Generate vertices
    for (int stack = 0; stack <= stacks; stack++) {
        verticalAngle = -glm::pi<float>() + stack * verticalAngleIncrement; // Calculate the vertical angle
        verticalAngleCos = glm::cos(verticalAngle);
        verticalAngleSin = glm::sin(verticalAngle);
        for (int slice = 0; slice <= slices; slice++) {
            horizontalAngle = slice * horizontalAngleIncrement; // Calculate the horizontal angle

            horizontalAngleCos = glm::cos(horizontalAngle);
            horizontalAngleSin = glm::sin(horizontalAngle);

            // Calculate torus Cartesian coordinates using parametric equations
            x = (centerRadius + tubeRadius * horizontalAngleCos) * verticalAngleCos;
            y = (centerRadius + tubeRadius * horizontalAngleCos) * verticalAngleSin;
            z = tubeRadius * horizontalAngleSin;

            // Position
            vertexBuffer[vertexBufferIndex++] = x;  // x-coordinate of vertex
            vertexBuffer[vertexBufferIndex++] = y;  // y-coordinate of vertex
            vertexBuffer[vertexBufferIndex++] = z;  // z-coordinate of vertex

            // Color
            if (floatsPerColor > 0) {
                vertexBuffer[vertexBufferIndex++] = r; // Red component of color
                vertexBuffer[vertexBufferIndex++] = g; // Green component of color
                vertexBuffer[vertexBufferIndex++] = b; // Blue component of color
                vertexBuffer[vertexBufferIndex++] = a; // Alpha component of color
            }

            // Normal
            if (floatsPerNormal > 0)
            {
                // Calculate normals (using simplified version, adjust if needed)
                vertexBuffer[vertexBufferIndex++] = horizontalAngleCos * verticalAngleCos;  // x-component of normal
                vertexBuffer[vertexBufferIndex++] = horizontalAngleCos * verticalAngleSin;  // y-component of normal
                vertexBuffer[vertexBufferIndex++] = horizontalAngleSin;                       // z-component of normal
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
            // Define indices to create triangles for the torus mesh
            elementBuffer[elementBufferIndex++] = row1 + slice;
            elementBuffer[elementBufferIndex++] = row2 + slice + 1;
            elementBuffer[elementBufferIndex++] = row2 + slice;
            elementBuffer[elementBufferIndex++] = row1 + slice;
            elementBuffer[elementBufferIndex++] = row1 + slice + 1;
            elementBuffer[elementBufferIndex++] = row2 + slice + 1;
        }
    }
}
