#pragma once
// FrustumMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a FrustumMesh object with transformations.
 */
class FrustumMesh : public Mesh {
public:
    // ################
    // # Constructors #
    // ################


    /**
     * Mesh constructor.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param scale Scale vector per axis.
     * @param rotationDegrees Rotation vector per axis in degrees.
     * @param translation Translation vector.
     * @param rotationOrder Order of rotations to be applied.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param topRadius The radius of the top of the Frustum (x-axis & z-axis).
     * @param bottomRadius The radius of the bottom of the Frustum (x-axis & z-axis).
     * @param height The height of the Frustum from top to bottom (y-axis).
     * @param sectorCount The number of sectors around the Frustum (greater = more triangles and detail).
     */
    FrustumMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float topRadius = 1, float bottomRadius = 1,
        float height = 1, int sectorCount = 100);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param topRadius The radius of the top of the Frustum (x-axis & z-axis).
     * @param bottomRadius The radius of the bottom of the Frustum (x-axis & z-axis).
     * @param height The height of the Frustum from top to bottom (y-axis).
     * @param sectorCount The number of sectors around the Frustum (greater = more triangles and detail).
     */
    FrustumMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float topRadius = 1, float bottomRadius = 1,
        float height = 1, int sectorCount = 100);


    // #############################
    // # Abstract method overrides #
    // #############################


    /**
     * Generate the vertex and element buffer.
     * Must be used before generateVAO().
     */
    void generateVertices() override;

private:
    // #################
    // # Other methods #
    // #################


    /**
     * Calculate the vertex based on the current sector.
     *
     * @param SectorIndex The index of the current sector.
     * @return The resulting vec3 of x, y, z values.
     */
    const glm::vec3 calculateSectorVertex(int SectorIndex, float radius);


    /**
     * Calculate the vertex based on the current sector.
     *
     * @param SectorIndex The index of the current sector.
     * @return The resulting vec3 of x, y, z values.
     */
    const glm::vec2 calculateSectorUV(int SectorIndex, bool topCircle);


    // #############
    // # Variables #
    // #############

    float topRadius;    // The radius of the top of the Frustum
    float bottomRadius; // The radius of the bottom of the Frustum
    float height;       // The height of the Frustum from top to bottom (y-axis)
    int sectorCount;    // The number of sectors around the Frustum (greater = more triangles and detail OR more sides)
};