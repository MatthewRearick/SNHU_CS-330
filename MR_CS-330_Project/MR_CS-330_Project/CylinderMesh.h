// CylinderMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a CylinderMesh object with transformations.
 */
class CylinderMesh : public Mesh {
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
     * @param radius The radius of the Cylinder (x-axis & z-axis).
     * @param height The height of the Cylinder from top to bottom (y-axis).
     * @param sectorCount The number of sectors around the cylinder (greater = more triangles and detail).
     */
    CylinderMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float radius = 1, float height = 1, int sectorCount = 100);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param radius The radius of the Cylinder (x-axis & z-axis).
     * @param height The height of the Cylinder from top to bottom (y-axis).
     * @param sectorCount The number of sectors around the cylinder (greater = more triangles and detail).
     */
    CylinderMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float radius = 1, float height = 1, int sectorCount = 100);


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

    float radius;       // The radius of the Cylinder
    float height;       // The height of the Cylinder from top to bottom (y-axis)
    int sectorCount;    // The number of sectors around the cylinder (greater = more triangles and detail)
};