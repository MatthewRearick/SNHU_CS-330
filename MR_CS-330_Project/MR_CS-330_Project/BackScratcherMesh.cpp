#include "BackScratcherMesh.h"
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


BackScratcherMesh::BackScratcherMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), sectorCount(sectorCount)
{
}

BackScratcherMesh::BackScratcherMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), sectorCount(sectorCount)
{
}


// #################
// # Other methods #
// #################



void BackScratcherMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }

    // Measurements in cm
    float rubberGripRadius = 0.6f;
    float rubberGripLength = 11.5f;
    float tube1Radius = 0.45f;
    float tube1Length = 3.7f;
    float tube2Radius = 0.4f;
    float tube2Length = 12.2f;
    float tube3Radius = 0.35f;
    float tube3Length = 12.2f;
    float tube4Radius = 0.3f;
    float tube4Length = 6.1f;
    float pawConnectorRadius = 0.5f;
    float pawConnectorLength = 0.7f;
    float pawRadius = 3.0f;
    float pawThickness = 0.6f;
    float clawWidth = 0.3f;
    float clawHeight = 0.3f;
    float clawLength = 1.0f;
    float clawTipLength = 0.3f;

    float meshLength = 0.0f;

    std::vector<Mesh*> meshes;

    // Handle
    CylinderMesh rubberGrip(vertexMode, UnitOfMeasure::CENTIMETER, -1, rubberGripRadius, rubberGripLength, sectorCount);
    rubberGrip.setTextureUClamp(glm::vec2(0.5f, 1.0f));
    rubberGrip.translateMesh(0.0f, meshLength + (rubberGripLength / 2.0f), 0.0f);
    rubberGrip.setColor(color);
    rubberGrip.generateVertices();
    rubberGrip.translateMeshPreVAO();
    meshes.push_back(&rubberGrip);
    meshLength += rubberGripLength;

    CylinderMesh tube1(vertexMode, UnitOfMeasure::CENTIMETER, -1, tube1Radius, tube1Length, sectorCount);
    tube1.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    tube1.translateMesh(0.0f, meshLength + (tube1Length / 2.0f), 0.0f);
    tube1.setColor(color);
    tube1.generateVertices();
    tube1.translateMeshPreVAO();
    meshes.push_back(&tube1);
    meshLength += tube1Length;

    CylinderMesh tube2(vertexMode, UnitOfMeasure::CENTIMETER, -1, tube2Radius, tube2Length, sectorCount);
    tube2.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    tube2.translateMesh(0.0f, meshLength + (tube2Length / 2.0f), 0.0f);
    tube2.setColor(color);
    tube2.generateVertices();
    tube2.translateMeshPreVAO();
    meshes.push_back(&tube2);
    meshLength += tube2Length;

    CylinderMesh tube3(vertexMode, UnitOfMeasure::CENTIMETER, -1, tube3Radius, tube3Length, sectorCount);
    tube3.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    tube3.translateMesh(0.0f, meshLength + (tube3Length / 2.0f), 0.0f);
    tube3.setColor(color);
    tube3.generateVertices();
    tube3.translateMeshPreVAO();
    meshes.push_back(&tube3);
    meshLength += tube3Length;

    CylinderMesh tube4(vertexMode, UnitOfMeasure::CENTIMETER, -1, tube4Radius, tube4Length, sectorCount);
    tube4.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    tube4.translateMesh(0.0f, meshLength + (tube4Length / 2.0f), 0.0f);
    tube4.setColor(color);
    tube4.generateVertices();
    tube4.translateMeshPreVAO();
    meshes.push_back(&tube4);
    meshLength += tube4Length;

    CylinderMesh pawConnector(vertexMode, UnitOfMeasure::CENTIMETER, -1, pawConnectorRadius, pawConnectorLength, sectorCount);
    pawConnector.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    pawConnector.translateMesh(0.0f, meshLength + (pawConnectorLength / 2.0f), 0.0f);
    pawConnector.setColor(color);
    pawConnector.generateVertices();
    pawConnector.translateMeshPreVAO();
    meshes.push_back(&pawConnector);
    meshLength += pawConnectorLength;

    // Head
    CylinderMesh paw(vertexMode, UnitOfMeasure::CENTIMETER, -1, pawRadius, pawThickness, sectorCount);
    paw.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    paw.translateMesh(0.0f, meshLength + pawRadius, 0.0f);
    paw.rotateMesh(90.0f, 0.0f, 0.0f);
    paw.setColor(color);
    paw.generateVertices();
    paw.translateMeshPreVAO();
    meshes.push_back(&paw);
    meshLength += (pawRadius * 2.0f);

    CubeMesh clawCenter(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawHeight, clawLength);
    clawCenter.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawCenter.translateMesh(0.0f, meshLength - (clawHeight / 2.0f), (pawThickness / 2.0f) + (clawLength / 2.0f));
    clawCenter.setColor(color);
    clawCenter.generateVertices();
    clawCenter.translateMeshPreVAO();
    meshes.push_back(&clawCenter);

    PyramidMesh clawCenterTip(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawTipLength, clawHeight);
    clawCenterTip.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawCenterTip.rotateMesh(90.0f, 0.0f, 0.0f);
    clawCenterTip.translateMesh(0.0f, meshLength - (clawHeight / 2.0f), (pawThickness / 2.0f) + clawLength + (clawTipLength / 2.0f));
    clawCenterTip.setColor(color);
    clawCenterTip.generateVertices();
    clawCenterTip.translateMeshPreVAO();
    meshes.push_back(&clawCenterTip);

    CubeMesh clawLeft1(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawHeight, clawLength);
    clawLeft1.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawLeft1.rotateMesh(0.0f, 0.0f, 25.0f);
    clawLeft1.translateMesh(-(pawRadius / 2.5f), meshLength - (clawHeight / 2.0f) - 0.25f, (pawThickness / 2.0f) + (clawLength / 2.0f));
    clawLeft1.setColor(color);
    clawLeft1.generateVertices();
    clawLeft1.translateMeshPreVAO();
    meshes.push_back(&clawLeft1);

    PyramidMesh clawLeft1Tip(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawTipLength, clawHeight);
    clawLeft1Tip.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawLeft1Tip.rotateMesh(90.0f, 0.0f, 25.0f, RotationOrder::X_Y_Z);
    clawLeft1Tip.translateMesh(-(pawRadius / 2.5f), meshLength - (clawHeight / 2.0f) - 0.25f, (pawThickness / 2.0f) + clawLength + (clawTipLength / 2.0f));
    clawLeft1Tip.setColor(color);
    clawLeft1Tip.generateVertices();
    clawLeft1Tip.translateMeshPreVAO();
    meshes.push_back(&clawLeft1Tip);

    CubeMesh clawLeft2(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawHeight, clawLength);
    clawLeft2.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawLeft2.rotateMesh(0.0f, 0.0f, 50.0f);
    clawLeft2.translateMesh(-(pawRadius / 1.25f - 0.25f), meshLength - (clawHeight / 2.0f) - 1.0f, (pawThickness / 2.0f) + (clawLength / 2.0f));
    clawLeft2.setColor(color);
    clawLeft2.generateVertices();
    clawLeft2.translateMeshPreVAO();
    meshes.push_back(&clawLeft2);

    PyramidMesh clawLeftTip2Tip(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawTipLength, clawHeight);
    clawLeftTip2Tip.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawLeftTip2Tip.rotateMesh(90.0f, 0.0f, 50.0f, RotationOrder::X_Y_Z);
    clawLeftTip2Tip.translateMesh(-(pawRadius / 1.25f - 0.25f), meshLength - (clawHeight / 2.0f) - 1.0f, (pawThickness / 2.0f) + clawLength + (clawTipLength / 2.0f));
    clawLeftTip2Tip.setColor(color);
    clawLeftTip2Tip.generateVertices();
    clawLeftTip2Tip.translateMeshPreVAO();
    meshes.push_back(&clawLeftTip2Tip);

    CubeMesh clawRight1(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawHeight, clawLength);
    clawRight1.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawRight1.rotateMesh(0.0f, 0.0f, -25.0f);
    clawRight1.translateMesh((pawRadius / 2.5f), meshLength - (clawHeight / 2.0f) - 0.25f, (pawThickness / 2.0f) + (clawLength / 2.0f));
    clawRight1.setColor(color);
    clawRight1.generateVertices();
    clawRight1.translateMeshPreVAO();
    meshes.push_back(&clawRight1);

    PyramidMesh clawRight1Tip(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawTipLength, clawHeight);
    clawRight1Tip.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawRight1Tip.rotateMesh(90.0f, 0.0f, -25.0f, RotationOrder::X_Y_Z);
    clawRight1Tip.translateMesh((pawRadius / 2.5f), meshLength - (clawHeight / 2.0f) - 0.25f, (pawThickness / 2.0f) + clawLength + (clawTipLength / 2.0f));
    clawRight1Tip.setColor(color);
    clawRight1Tip.generateVertices();
    clawRight1Tip.translateMeshPreVAO();
    meshes.push_back(&clawRight1Tip);

    CubeMesh clawRight2(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawHeight, clawLength);
    clawRight2.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawRight2.rotateMesh(0.0f, 0.0f, -50.0f);
    clawRight2.translateMesh((pawRadius / 1.25f - 0.25f), meshLength - (clawHeight / 2.0f) - 1.0f, (pawThickness / 2.0f) + (clawLength / 2.0f));
    clawRight2.setColor(color);
    clawRight2.generateVertices();
    clawRight2.translateMeshPreVAO();
    meshes.push_back(&clawRight2);

    PyramidMesh clawRightTip2Tip(vertexMode, UnitOfMeasure::CENTIMETER, -1, clawWidth, clawTipLength, clawHeight);
    clawRightTip2Tip.setTextureUClamp(glm::vec2(0.0f, 0.5f));
    clawRightTip2Tip.rotateMesh(90.0f, 0.0f, -50.0f, RotationOrder::X_Y_Z);
    clawRightTip2Tip.translateMesh((pawRadius / 1.25f - 0.25f), meshLength - (clawHeight / 2.0f) - 1.0f, (pawThickness / 2.0f) + clawLength + (clawTipLength / 2.0f));
    clawRightTip2Tip.setColor(color);
    clawRightTip2Tip.generateVertices();
    clawRightTip2Tip.translateMeshPreVAO();
    meshes.push_back(&clawRightTip2Tip);




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
    translateMesh(0.0f, -(meshLength / 2.0f), 0.0f);
    translateMeshPreVAO();

    rotateMesh(0.0f, 90.0f, 90.2f, RotationOrder::Y_Z_X);
    translateMesh(0.0f, rubberGripRadius - 0.05f, 0.0f);

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