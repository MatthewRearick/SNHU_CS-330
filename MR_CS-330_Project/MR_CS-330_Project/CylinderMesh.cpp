#include "CylinderMesh.h"
#include "FrustumMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


CylinderMesh::CylinderMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation,
    RotationOrder rotationOrder, GLuint shaderProgramId, float radius, float height, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), radius(radius), height(height), sectorCount(sectorCount)
{
}

CylinderMesh::CylinderMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float radius, float height, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure,shaderProgramId), radius(radius), height(height), sectorCount(sectorCount)
{
}


// #################
// # Other methods #
// #################



void CylinderMesh::generateVertices()
{
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }

    // Utilize building a Frustum with a matching top and bottom radius to create a cylinder
    FrustumMesh cylinderFrustum(vertexMode, unitOfMeasure, shaderProgramId, radius, radius, height, sectorCount);
    cylinderFrustum.setTextureUClamp(textureUClamp);
    cylinderFrustum.setTextureVClamp(textureVClamp);
    cylinderFrustum.setColor(color);
    cylinderFrustum.generateVertices();

    const std::vector<GLfloat> sourceVertexBuffer = cylinderFrustum.getVertexBuffer();
    const std::vector<GLushort> sourceElementBuffer = cylinderFrustum.getElementBuffer();

    vertexBuffer.resize(sourceVertexBuffer.size());
    elementBuffer.resize(sourceElementBuffer.size());

    for (size_t i = 0; i < sourceVertexBuffer.size(); ++i) {
        vertexBuffer.at(i) = sourceVertexBuffer.at(i);
    }

    for (size_t i = 0; i < sourceElementBuffer.size(); ++i) {
        elementBuffer.at(i) = sourceElementBuffer.at(i);
    }
}
