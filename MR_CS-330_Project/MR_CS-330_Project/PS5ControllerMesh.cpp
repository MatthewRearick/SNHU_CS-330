#include "PS5ControllerMesh.h"
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


PS5ControllerMesh::PS5ControllerMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), sectorCount(sectorCount)
{
}

PS5ControllerMesh::PS5ControllerMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), sectorCount(sectorCount)
{
}


// #################
// # Other methods #
// #################



void PS5ControllerMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }

    // Measurements in cm
    float bodyWidth = 14.0f;
    float bodyHeight = 4.0f;
    float bodyLength = 6.5f;
    float bodyCurveRadius = bodyHeight / 2.0f;
    float handleRadius = bodyCurveRadius;
    float handleLength = 6.5f;
    float joyStickOpeningRadius = 0.8f;
    float joyStickOpeningThickness = 0.4f;
    float joyStickStemHeight = 0.6f;
    float joyStickStemRadius = 0.4f;
    float joyStickTopHeight = 0.4f;
    float joyStickTopRadius = 0.9f;

    std::vector<Mesh*> meshes;

    // Controller Body
    CubeMesh controllerBody(vertexMode, UnitOfMeasure::CENTIMETER, -1, (bodyWidth - bodyCurveRadius * 2.0f), bodyHeight, (bodyLength - bodyCurveRadius));
    controllerBody.translateMesh(0.0f, 0.0f, 0.0f);
    controllerBody.rotateMesh(-10.0f, 0.0f, 0.0f);
    controllerBody.setColor(color);
    controllerBody.generateVertices();
    controllerBody.translateMeshPreVAO();
    meshes.push_back(&controllerBody);

    CylinderMesh leftBodyCurve(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius, (bodyLength - bodyCurveRadius - 0.001f), 100);
    leftBodyCurve.setTextureUClamp(glm::vec2(0.0f, 2.0f / 3.0f));
    leftBodyCurve.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    leftBodyCurve.translateMesh(-(bodyWidth / 2.0f - bodyCurveRadius), 0.0f, 0.0f);
    leftBodyCurve.rotateMesh(-100.0f, 180.0f, 0.0f);
    leftBodyCurve.setColor(color);
    leftBodyCurve.generateVertices();
    leftBodyCurve.translateMeshPreVAO();
    meshes.push_back(&leftBodyCurve);

    CylinderMesh rightBodyCurve(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius, (bodyLength - bodyCurveRadius - 0.001f), 100);
    rightBodyCurve.setTextureUClamp(glm::vec2(0.0f, 2.0f / 3.0f));
    rightBodyCurve.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    rightBodyCurve.translateMesh((bodyWidth / 2.0f - bodyCurveRadius), 0.0f, 0.0f);
    rightBodyCurve.rotateMesh(-100.0f, 180.0f, 0.0f);
    rightBodyCurve.setColor(color);
    rightBodyCurve.generateVertices();
    rightBodyCurve.translateMeshPreVAO();
    meshes.push_back(&rightBodyCurve);

    CylinderMesh frontBodyCurve(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius, (bodyWidth - bodyCurveRadius * 2.0f), 100);
    frontBodyCurve.setTextureUClamp(glm::vec2(1.0f / 3.0f, 1.0f));
    frontBodyCurve.translateMesh(0.0f, 0.4f, (bodyLength / 2.0f - 1.0f));
    frontBodyCurve.rotateMesh(0.0f, -80.0f, -90.0f, RotationOrder::Y_Z_X);
    frontBodyCurve.setColor(color);
    frontBodyCurve.generateVertices();
    frontBodyCurve.translateMeshPreVAO();
    meshes.push_back(&frontBodyCurve);

    // Controller Handles
    CylinderMesh leftHandle(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius, (handleLength - handleRadius), 100);
    leftHandle.setTextureUClamp(glm::vec2(0.0f, 2.0f / 3.0f));
    leftHandle.translateMesh(-(bodyWidth / 2.0f - bodyCurveRadius + 0.45f), 0.0f, (bodyLength / 2.0f + (handleLength - handleRadius) / 2.0f - 1.7f));
    leftHandle.rotateMesh(-80.0f, 180.0f, -15.0f, RotationOrder::Y_Z_X);
    leftHandle.setColor(color);
    leftHandle.generateVertices();
    leftHandle.translateMeshPreVAO();
    meshes.push_back(&leftHandle);

    SphereMesh leftHandleEnd(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius - 0.001f, sectorCount, sectorCount);
    leftHandleEnd.setTextureUClamp(glm::vec2(0.0f, 2.0f / 3.0f));
    leftHandleEnd.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    leftHandleEnd.translateMesh(-(bodyWidth / 2.0f - bodyCurveRadius + 1.0f), -0.3561f, (bodyLength / 2.0f + (handleLength - handleRadius) / 2.0f + 0.32f));
    leftHandleEnd.rotateMesh(10.0f, 0.0f, 0.0f);
    leftHandleEnd.setColor(color);
    leftHandleEnd.generateVertices();
    leftHandleEnd.translateMeshPreVAO();
    meshes.push_back(&leftHandleEnd);

    CylinderMesh rightHandle(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius, (handleLength - handleRadius), 100);
    rightHandle.setTextureUClamp(glm::vec2(0.0f, 2.0f / 3.0f));
    rightHandle.translateMesh((bodyWidth / 2.0f - bodyCurveRadius + 0.45f), 0.0f, (bodyLength / 2.0f + (handleLength - handleRadius) / 2.0f - 1.7f));
    rightHandle.rotateMesh(-80.0f, 180.0f, 15.0f, RotationOrder::Y_Z_X);
    rightHandle.setColor(color);
    rightHandle.generateVertices();
    rightHandle.translateMeshPreVAO();
    meshes.push_back(&rightHandle);

    SphereMesh rightHandleEnd(vertexMode, UnitOfMeasure::CENTIMETER, -1, bodyCurveRadius - 0.001f, sectorCount, sectorCount);
    rightHandleEnd.setTextureUClamp(glm::vec2(0.0f, 2.0f / 3.0f));
    rightHandleEnd.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    rightHandleEnd.translateMesh((bodyWidth / 2.0f - bodyCurveRadius + 1.0f), -0.3561f, (bodyLength / 2.0f + (handleLength - handleRadius) / 2.0f + 0.32f));
    rightHandleEnd.rotateMesh(10.0f, 0.0f, 0.0f);
    rightHandleEnd.setColor(color);
    rightHandleEnd.generateVertices();
    rightHandleEnd.translateMeshPreVAO();
    meshes.push_back(&rightHandleEnd);

    // Controller Joysticks
    TorusMesh leftJoystickOpening(vertexMode, UnitOfMeasure::CENTIMETER, -1, joyStickOpeningRadius + joyStickOpeningThickness, joyStickOpeningRadius, sectorCount, sectorCount);
    leftJoystickOpening.setTextureUClamp(glm::vec2(0.0f, 1.0f / 3.0f));
    leftJoystickOpening.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    leftJoystickOpening.translateMesh(-(bodyWidth / 6.0f), bodyHeight - 1.7f, bodyLength / 1.5f - (joyStickOpeningRadius + joyStickOpeningThickness) / 2.0f - 1.8f);
    leftJoystickOpening.rotateMesh(-100.0f, 0.0f, 0.0f);
    leftJoystickOpening.setColor(color);
    leftJoystickOpening.generateVertices();
    leftJoystickOpening.translateMeshPreVAO();
    meshes.push_back(&leftJoystickOpening);

    CylinderMesh leftJoystickStem(vertexMode, UnitOfMeasure::CENTIMETER, -1, joyStickStemRadius, joyStickStemHeight, sectorCount);
    leftJoystickStem.setTextureUClamp(glm::vec2(0.0f, 1.0f / 3.0f));
    leftJoystickStem.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    leftJoystickStem.translateMesh(-(bodyWidth / 6.0f), bodyHeight - 1.7f + joyStickStemHeight / 2.0f, bodyLength / 1.5f - (joyStickOpeningRadius + joyStickOpeningThickness) / 2.0f - 1.85f);
    leftJoystickStem.rotateMesh(-10.0f, 0.0f, 0.0f);
    leftJoystickStem.setColor(color);
    leftJoystickStem.generateVertices();
    leftJoystickStem.translateMeshPreVAO();
    meshes.push_back(&leftJoystickStem);

    CylinderMesh leftJoystickTop(vertexMode, UnitOfMeasure::CENTIMETER, -1, joyStickTopRadius, joyStickTopHeight, sectorCount);
    leftJoystickTop.setTextureUClamp(glm::vec2(0.0f, 1.0f / 3.0f));
    leftJoystickTop.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    leftJoystickTop.translateMesh(-(bodyWidth / 6.0f), bodyHeight - 1.7f + joyStickStemHeight + joyStickTopHeight / 2.0f, bodyLength / 1.5f - (joyStickOpeningRadius + joyStickOpeningThickness) / 2.0f - 1.95f);
    leftJoystickTop.rotateMesh(-10.0f, 0.0f, 0.0f);
    leftJoystickTop.setColor(color);
    leftJoystickTop.generateVertices();
    leftJoystickTop.translateMeshPreVAO();
    meshes.push_back(&leftJoystickTop);

    TorusMesh rightJoystickOpening(vertexMode, UnitOfMeasure::CENTIMETER, -1, joyStickOpeningRadius + joyStickOpeningThickness, joyStickOpeningRadius, sectorCount, sectorCount);
    rightJoystickOpening.setTextureUClamp(glm::vec2(0.0f, 1.0f / 3.0f));
    rightJoystickOpening.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    rightJoystickOpening.translateMesh((bodyWidth / 6.0f), bodyHeight - 1.7f, bodyLength / 1.5f - (joyStickOpeningRadius + joyStickOpeningThickness) / 2.0f - 1.8f);
    rightJoystickOpening.rotateMesh(-100.0f, 0.0f, 0.0f);
    rightJoystickOpening.setColor(color);
    rightJoystickOpening.generateVertices();
    rightJoystickOpening.translateMeshPreVAO();
    meshes.push_back(&rightJoystickOpening);

    CylinderMesh rightJoystickStem(vertexMode, UnitOfMeasure::CENTIMETER, -1, joyStickStemRadius, joyStickStemHeight, sectorCount);
    rightJoystickStem.setTextureUClamp(glm::vec2(0.0f, 1.0f / 3.0f));
    rightJoystickStem.setTextureVClamp(glm::vec2(0.0f, 0.5f));
    rightJoystickStem.translateMesh((bodyWidth / 6.0f), bodyHeight - 1.7f + joyStickStemHeight / 2.0f, bodyLength / 1.5f - (joyStickOpeningRadius + joyStickOpeningThickness) / 2.0f - 1.85f);
    rightJoystickStem.rotateMesh(-10.0f, 0.0f, 0.0f);
    rightJoystickStem.setColor(color);
    rightJoystickStem.generateVertices();
    rightJoystickStem.translateMeshPreVAO();
    meshes.push_back(&rightJoystickStem);

    CylinderMesh rightJoystickTop(vertexMode, UnitOfMeasure::CENTIMETER, -1, joyStickTopRadius, joyStickTopHeight, sectorCount);
    rightJoystickTop.setTextureUClamp(glm::vec2(0.0f, 1.0f / 3.0f));
    rightJoystickTop.setTextureVClamp(glm::vec2(0.5f, 1.0f));
    rightJoystickTop.translateMesh((bodyWidth / 6.0f), bodyHeight - 1.7f + joyStickStemHeight + joyStickTopHeight / 2.0f, bodyLength / 1.5f - (joyStickOpeningRadius + joyStickOpeningThickness) / 2.0f - 1.95f);
    rightJoystickTop.rotateMesh(-10.0f, 0.0f, 0.0f);
    rightJoystickTop.setColor(color);
    rightJoystickTop.generateVertices();
    rightJoystickTop.translateMeshPreVAO();
    meshes.push_back(&rightJoystickTop);


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

    // Transform the headset to start at a resting position where the origin is at the center and bottom of the mesh
    translateMesh(0.0f, 2.35f, 0.0f);

    if (unitOfMeasure == UnitOfMeasure::INCH) {
        float cmToInch = 0.394f;
        scaleMesh(cmToInch, cmToInch, cmToInch);
    }

    translateMeshPreVAO();
}