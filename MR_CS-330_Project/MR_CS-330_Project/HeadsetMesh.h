#pragma once
// HeadsetMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a HeadsetMesh object with transformations.
 */
class HeadsetMesh : public Mesh {
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
     * @param sectorCount The number of sectors around curved controller sections (greater = more triangles and detail).
     */
    HeadsetMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, int sectorCount = 100);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param sectorCount The number of sectors around curved controller sections (greater = more triangles and detail).
     */
    HeadsetMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, int sectorCount = 100);


    // #############################
    // # Abstract method overrides #
    // #############################


    /**
     * Generate the vertex and element buffer.
     * Must be used before generateVAO().
     */
    void generateVertices() override;

private:
    // #############
    // # Variables #
    // #############

    int sectorCount;    // The number of sectors around curved controller sections (greater = more triangles and detail)
};