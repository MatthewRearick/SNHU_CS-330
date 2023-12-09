#include "SodaCanMesh.h"
#include "CubeMesh.h"
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


SodaCanMesh::SodaCanMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float radius, float height, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), radius(radius), height(height), sectorCount(sectorCount)
{
}

SodaCanMesh::SodaCanMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float radius, float height, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), radius(radius), height(height), sectorCount(sectorCount)
{
}


// #################
// # Other methods #
// #################



void SodaCanMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }

    // Standard 12oz can
    // base height  = 0.74cm
    // body height  = 10.3cm
    // top height   = 1.16cm
    // ring heigh   = 0.2cm
    // total height = 12.4cm
    // Get part height ratio to get part height
    float standardCanHeight = 12.4f;
    float baseHeight = (0.74f / standardCanHeight) * height;
    float bodyHeight = (10.3f / standardCanHeight) * height;
    float topHeight = (1.16f / standardCanHeight) * height;
    float ringHeight = (0.2f / standardCanHeight) * height;

    // base & top radius = 2.65cm
    // body radius       = 3.31cm
    // Get the ratio of the radius for the smaller base and top radius as well as the tab radius
    float smallRadius = (2.6f / 3.31f) * radius;
    float ringRadius = (2.65f / 3.31f) * radius;
    float tabRadius = (0.5f / 3.31f) * radius;
    float tabLength = tabRadius * 2.5f;

    std::vector<Mesh*> meshes;

    FrustumMesh canBase(vertexMode, UnitOfMeasure::CENTIMETER, -1, radius, smallRadius, baseHeight, sectorCount);
    canBase.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    canBase.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    canBase.translateMesh(0.0f, -bodyHeight / 2 - baseHeight / 2, 0.0f);
    canBase.setColor(color);
    canBase.generateVertices();
    canBase.translateMeshPreVAO();
    meshes.push_back(&canBase);

    CylinderMesh canBody(vertexMode, UnitOfMeasure::CENTIMETER, -1, radius, bodyHeight, sectorCount);
    canBody.setTextureVClamp(glm::vec2(0.0f, 227.0f / 256.0f));
    canBody.setColor(color);
    canBody.generateVertices();
    meshes.push_back(&canBody);

    FrustumMesh canTop(vertexMode, UnitOfMeasure::CENTIMETER, -1, smallRadius, radius, topHeight, sectorCount);
    canTop.setTextureVClamp(glm::vec2(227.0f / 512.0f, 285.0f / 512.0f));
    canTop.translateMesh(0.0f, bodyHeight / 2 + topHeight / 2, 0.0f);
    canTop.setColor(color);
    canTop.generateVertices();
    canTop.translateMeshPreVAO();
    meshes.push_back(&canTop);

    // TODO: Implement canRing as torus
    CylinderMesh canRing(vertexMode, UnitOfMeasure::CENTIMETER, -1, ringRadius, ringHeight, sectorCount);
    canRing.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    canRing.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    canRing.translateMesh(0.0f, bodyHeight / 2 + topHeight + ringHeight / 2, 0.0f);
    canRing.setColor(color);
    canRing.generateVertices();
    canRing.translateMeshPreVAO();
    meshes.push_back(&canRing);

    int verticesAdded = 0;

    for (Mesh* meshPtr : meshes) {
        for (GLfloat vertexValue : meshPtr->getVertexBuffer()) {
            vertexBuffer.push_back(vertexValue);
        }

        for (GLuint elementValue : meshPtr->getElementBuffer()) {
            elementBuffer.push_back(elementValue + verticesAdded);
        }

        verticesAdded = getVertexBufferCount();
    }

    // Store any existing transformations
    glm::mat4 scaleTemp = scale;
    glm::mat4 rotationTemp = rotation;
    glm::mat4 translationTemp = translation;

    // Reset any existing transformations
    setScale(Mesh::DEFAULT_SCALE_VEC3);
    setRotation(Mesh::DEFAULT_ROTATION_DEGREES_VEC3);
    setTranslation(Mesh::DEFAULT_TRANSLATION_VEC3);

    float meshHeightOffset = baseHeight + bodyHeight / 2.0f;
    translateMesh(0.0f, meshHeightOffset, 0.0f);
    translateMeshPreVAO();

    // Set back any existing transformations
    scale = scaleTemp;
    rotation = rotationTemp;
    translation = translationTemp;
}