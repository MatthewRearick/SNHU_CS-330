#include "HeadsetMesh.h"
#include "PyramidMesh.h"
#include "CubeMesh.h"
#include "CubeLightMesh.h"
#include "PlaneMesh.h"
#include "CylinderMesh.h"
#include "FrustumMesh.h"
#include "SphereMesh.h"
#include "TorusMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


HeadsetMesh::HeadsetMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), sectorCount(sectorCount)
{
}

HeadsetMesh::HeadsetMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), sectorCount(sectorCount)
{
}


// #################
// # Other methods #
// #################



void HeadsetMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }

    // Measurements in cm
    float frameThickness = 1.0f;
    float frameDepth = 3.75f;
    float upperHeadbandSegmentWidth = 4.8f;
    float upperHeadbandFoamRadius = 0.8f;
    float lowerHeadbandSegmentWidth = 7.2f;
    float connectorRadius = 0.55f;
    float connectorHeight = 1.5f;
    float upperEarpieceWidth = 2.5f;
    float outterEarpieceRadius = 4.0f;
    float outterEarpieceThickness = 2.5f;
    float innerEarpieceRadius = 4.15f;
    float innerEarpieceThickness = 2.5f;

    std::vector<Mesh*> meshes;

    // Headband Center
    CubeMesh upperHeadbandCenter(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperHeadbandSegmentWidth, frameDepth, frameThickness);
    upperHeadbandCenter.setTextureUClamp(glm::vec2(2.0f / 7.0f, 5.0f / 7.0f));
    upperHeadbandCenter.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    upperHeadbandCenter.rotateMesh(0.0f, 180.0f, 0.0f);
    upperHeadbandCenter.translateMesh(0.0f, 0.0f, -10.75f + (frameThickness /2.0f));
    upperHeadbandCenter.setColor(color);
    upperHeadbandCenter.generateVertices();
    upperHeadbandCenter.translateMeshPreVAO();
    meshes.push_back(&upperHeadbandCenter);

    CylinderMesh upperHeadbandCenterFoam(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperHeadbandFoamRadius, upperHeadbandSegmentWidth, sectorCount);
    upperHeadbandCenterFoam.setTextureUClamp(glm::vec2(0.0f, 1.0f / 7.0f));
    upperHeadbandCenterFoam.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    upperHeadbandCenterFoam.translateMesh(0.0f, 0.0f, frameThickness / 2.0f);
    upperHeadbandCenterFoam.rotateMesh(0.0f, 0.0f, 90.0f);
    upperHeadbandCenterFoam.setColor(color);
    upperHeadbandCenterFoam.generateVertices();
    upperHeadbandCenterFoam.translateMeshPreVAO();
    upperHeadbandCenterFoam.translateMesh(0.0f, 0.0f, -10.75f + (frameThickness / 2.0f));    upperHeadbandCenterFoam.translateMeshPreVAO();
    upperHeadbandCenterFoam.translateMeshPreVAO();
    meshes.push_back(&upperHeadbandCenterFoam);

    // Headband Left
    CubeMesh upperHeadbandLeft(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperHeadbandSegmentWidth, frameDepth, frameThickness);
    upperHeadbandLeft.setTextureUClamp(glm::vec2(3.0f / 7.0f, 6.0f / 7.0f));
    upperHeadbandLeft.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    upperHeadbandLeft.rotateMesh(0.0f, 207, 0.0f);
    upperHeadbandLeft.translateMesh(-(upperHeadbandSegmentWidth - 0.5f), 0.0f, -9.71f + (frameThickness / 2.0f));
    upperHeadbandLeft.setColor(color);
    upperHeadbandLeft.generateVertices();
    upperHeadbandLeft.translateMeshPreVAO();
    meshes.push_back(&upperHeadbandLeft);

    CylinderMesh upperHeadbandLeftFoam(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperHeadbandFoamRadius, upperHeadbandSegmentWidth, sectorCount);
    upperHeadbandLeftFoam.setTextureUClamp(glm::vec2(0.0f, 1.0f / 7.0f));
    upperHeadbandLeftFoam.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    upperHeadbandLeftFoam.translateMesh(0.0f, 0.0f, frameThickness / 2.0f);
    upperHeadbandLeftFoam.rotateMesh(0.0f, 0.0f, 90.0f);
    upperHeadbandLeftFoam.setColor(color);
    upperHeadbandLeftFoam.generateVertices();
    upperHeadbandLeftFoam.translateMeshPreVAO();
    upperHeadbandLeftFoam.rotateMesh(0.0f, 27.0f, 0.0f);
    upperHeadbandLeftFoam.translateMesh(-(upperHeadbandSegmentWidth - 0.5f), 0.0f, -9.71f + (frameThickness / 2.0f));
    upperHeadbandLeftFoam.translateMeshPreVAO();
    meshes.push_back(&upperHeadbandLeftFoam);

    CubeMesh lowerHeadbandLeft(vertexMode, UnitOfMeasure::CENTIMETER, -1, lowerHeadbandSegmentWidth, frameDepth, frameThickness);
    lowerHeadbandLeft.setTextureUClamp(glm::vec2(4.0f / 7.0f, 1.0f));
    lowerHeadbandLeft.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    lowerHeadbandLeft.rotateMesh(0.0f, 234.0f, 0.0f);
    lowerHeadbandLeft.translateMesh(-(upperHeadbandSegmentWidth + 3.58f), 0.0f, -5.86f + (frameThickness / 2.0f));
    lowerHeadbandLeft.setColor(color);
    lowerHeadbandLeft.generateVertices();
    lowerHeadbandLeft.translateMeshPreVAO();
    meshes.push_back(&lowerHeadbandLeft);

    // Headband Right
    CubeMesh upperHeadbandRight(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperHeadbandSegmentWidth, frameDepth, frameThickness);
    upperHeadbandRight.setTextureUClamp(glm::vec2(1.0f / 7.0f, 4.0f / 7.0f));
    upperHeadbandRight.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    upperHeadbandRight.rotateMesh(0.0f, -207, 0.0f);
    upperHeadbandRight.translateMesh((upperHeadbandSegmentWidth - 0.5f), 0.0f, -9.71f + (frameThickness / 2.0f));
    upperHeadbandRight.setColor(color);
    upperHeadbandRight.generateVertices();
    upperHeadbandRight.translateMeshPreVAO();
    meshes.push_back(&upperHeadbandRight);

    CylinderMesh upperHeadbandRightFoam(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperHeadbandFoamRadius, upperHeadbandSegmentWidth, sectorCount);
    upperHeadbandRightFoam.setTextureUClamp(glm::vec2(0.0f, 1.0f / 7.0f));
    upperHeadbandRightFoam.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    upperHeadbandRightFoam.translateMesh(0.0f, 0.0f, frameThickness / 2.0f);
    upperHeadbandRightFoam.rotateMesh(0.0f, 0.0f, 90.0f);
    upperHeadbandRightFoam.setColor(color);
    upperHeadbandRightFoam.generateVertices();
    upperHeadbandRightFoam.translateMeshPreVAO();
    upperHeadbandRightFoam.rotateMesh(0.0f, -27.0f, 0.0f);
    upperHeadbandRightFoam.translateMesh((upperHeadbandSegmentWidth - 0.5f), 0.0f, -9.71f + (frameThickness / 2.0f));
    upperHeadbandRightFoam.translateMeshPreVAO();
    meshes.push_back(&upperHeadbandRightFoam);

    CubeMesh lowerHeadbandRight(vertexMode, UnitOfMeasure::CENTIMETER, -1, lowerHeadbandSegmentWidth, frameDepth, frameThickness);
    lowerHeadbandRight.setTextureUClamp(glm::vec2(0.0f, 3.0f / 7.0f));
    lowerHeadbandRight.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    lowerHeadbandRight.rotateMesh(0.0f, -234.0f, 0.0f);
    lowerHeadbandRight.translateMesh((upperHeadbandSegmentWidth + 3.58f), 0.0f, -5.86f + (frameThickness / 2.0f));
    lowerHeadbandRight.setColor(color);
    lowerHeadbandRight.generateVertices();
    lowerHeadbandRight.translateMeshPreVAO();
    meshes.push_back(&lowerHeadbandRight);

    // Headband connectors
    CylinderMesh headbandConnectorLeft(vertexMode, UnitOfMeasure::CENTIMETER, -1, connectorRadius, connectorHeight, sectorCount);
    headbandConnectorLeft.setTextureUClamp(glm::vec2(0.0f, 3.0f / 7.0f));
    headbandConnectorLeft.setTextureVClamp(glm::vec2(0.25f, 0.5f));
    headbandConnectorLeft.rotateMesh(0.0f, 80.0f, 90.0f);
    headbandConnectorLeft.translateMesh(-10.3f, 0.0f, -2.1f);
    headbandConnectorLeft.setColor(color);
    headbandConnectorLeft.generateVertices();
    headbandConnectorLeft.translateMeshPreVAO();
    meshes.push_back(&headbandConnectorLeft);

    CylinderMesh headbandConnectorRight(vertexMode, UnitOfMeasure::CENTIMETER, -1, connectorRadius, connectorHeight, sectorCount);
    headbandConnectorRight.setTextureUClamp(glm::vec2(0.0f, 3.0f / 7.0f));
    headbandConnectorRight.setTextureVClamp(glm::vec2(0.25f, 0.5f));
    headbandConnectorRight.rotateMesh(0.0f, -80.0, 90.0f);
    headbandConnectorRight.translateMesh(10.3f, 0.0f, -2.1f);
    headbandConnectorRight.setColor(color);
    headbandConnectorRight.generateVertices();
    headbandConnectorRight.translateMeshPreVAO();
    meshes.push_back(&headbandConnectorRight);

    // Upper Earpiece
    CubeMesh upperEarpieceLeft(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperEarpieceWidth, frameDepth, frameThickness);
    upperEarpieceLeft.setTextureUClamp(glm::vec2(0.0f, 3.0f / 7.0f));
    upperEarpieceLeft.setTextureVClamp(glm::vec2(0.25f, 0.5f));
    upperEarpieceLeft.rotateMesh(0.0f, 286.0f, 0.0f);
    upperEarpieceLeft.translateMesh(-(upperHeadbandSegmentWidth + 5.4f), 0.0f, -1.05f + (frameThickness / 2.0f));
    upperEarpieceLeft.setColor(color);
    upperEarpieceLeft.generateVertices();
    upperEarpieceLeft.translateMeshPreVAO();
    meshes.push_back(&upperEarpieceLeft);

    CubeMesh upperEarpieceRight(vertexMode, UnitOfMeasure::CENTIMETER, -1, upperEarpieceWidth, frameDepth, frameThickness);
    upperEarpieceRight.setTextureUClamp(glm::vec2(0.0f, 3.0f / 7.0f));
    upperEarpieceRight.setTextureVClamp(glm::vec2(0.25f, 0.5f));
    upperEarpieceRight.rotateMesh(0.0f, -286.0f, 0.0f);
    upperEarpieceRight.translateMesh((upperHeadbandSegmentWidth + 5.4f), 0.0f, -1.05f + (frameThickness / 2.0f));
    upperEarpieceRight.setColor(color);
    upperEarpieceRight.generateVertices();
    upperEarpieceRight.translateMeshPreVAO();
    meshes.push_back(&upperEarpieceRight);

    // Earpieces
    CylinderMesh outterEarpieceLeft(vertexMode, UnitOfMeasure::CENTIMETER, -1, outterEarpieceRadius, outterEarpieceThickness, sectorCount);
    outterEarpieceLeft.setTextureUClamp(glm::vec2(3.0f / 7.0f, 5.0f / 7.0f));
    outterEarpieceLeft.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    outterEarpieceLeft.rotateMesh(0.0f, 90.0f, 0.0f);
    outterEarpieceLeft.rotateMesh(0.0f, 10.0f, 90.0f);
    outterEarpieceLeft.translateMesh(-(8.8f), 0.0f, 3.9f);
    outterEarpieceLeft.setColor(color);
    outterEarpieceLeft.generateVertices();
    outterEarpieceLeft.translateMeshPreVAO();
    meshes.push_back(&outterEarpieceLeft);

    TorusMesh innerEarpieceLeft(vertexMode, UnitOfMeasure::CENTIMETER, -1, innerEarpieceRadius, innerEarpieceRadius - innerEarpieceThickness, sectorCount, sectorCount);
    innerEarpieceLeft.setTextureUClamp(glm::vec2(1.0f / 7.0f, 3.0f / 7.0f));
    innerEarpieceLeft.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    innerEarpieceLeft.rotateMesh(0.0f, 100.0f, 0.0f);
    innerEarpieceLeft.translateMesh(-(8.2f - innerEarpieceThickness / 2.0f), 0.0f, 3.57f);
    innerEarpieceLeft.setColor(color);
    innerEarpieceLeft.generateVertices();
    innerEarpieceLeft.translateMeshPreVAO();
    meshes.push_back(&innerEarpieceLeft);

    CylinderMesh outterEarpieceRight(vertexMode, UnitOfMeasure::CENTIMETER, -1, outterEarpieceRadius, outterEarpieceThickness, sectorCount);
    outterEarpieceRight.setTextureUClamp(glm::vec2(5.0f / 7.0f, 1.0f));
    outterEarpieceRight.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    outterEarpieceRight.rotateMesh(0.0f, -90.0f, 180.0f);
    outterEarpieceRight.rotateMesh(0.0f, -10.0f, 90.0f);
    outterEarpieceRight.translateMesh((8.8f), 0.0f, 3.9f);
    outterEarpieceRight.setColor(color);
    outterEarpieceRight.generateVertices();
    outterEarpieceRight.translateMeshPreVAO();
    meshes.push_back(&outterEarpieceRight);

    TorusMesh innerEarpieceRight(vertexMode, UnitOfMeasure::CENTIMETER, -1, innerEarpieceRadius, innerEarpieceRadius - innerEarpieceThickness, sectorCount, sectorCount);
    innerEarpieceRight.setTextureUClamp(glm::vec2(1.0f / 7.0f, 3.0f / 7.0f));
    innerEarpieceRight.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    innerEarpieceRight.rotateMesh(0.0f, -100.0f, 0.0f);
    innerEarpieceRight.translateMesh((8.2f - innerEarpieceThickness / 2.0f), 0.0f, 3.57f);
    innerEarpieceRight.setColor(color);
    innerEarpieceRight.generateVertices();
    innerEarpieceRight.translateMeshPreVAO();
    meshes.push_back(&innerEarpieceRight);



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

    // Transform the headset to start at a resting position where the origin is at the center and bottom of the mesh
    translateMesh(0.0f, 3.45f, 0.0f);
    rotateMesh(-8.55f, 0.0f, 0.0f);

    if (unitOfMeasure == UnitOfMeasure::INCH) {
        float cmToInch = 0.394f;
        scaleMesh(cmToInch, cmToInch, cmToInch);
    }

    translateMeshPreVAO();

    // Set back any existing transformations
    scale = scaleTemp;
    rotation = rotationTemp;
    translation = translationTemp;
}