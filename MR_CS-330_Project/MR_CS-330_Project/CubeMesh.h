// CubeMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a CubeMesh object with transformations.
 */
class CubeMesh : public Mesh {
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
     * @param width The width of the cube from left to right (x-axis).
     * @param height The height of the cube from top to bottom (y-axis).
     * @param length The lenght of the cube from front to back (z-axis).
     */
    CubeMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float width = 1, float height = 1, float length = 1);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param width The width of the cube from left to right (x-axis).
     * @param height The height of the cube from top to bottom (y-axis).
     * @param length The lenght of the cube from front to back (z-axis).
     */
    CubeMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float width = 1, float height = 1, float length = 1);


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

    float width;    // The width of the cube from left to right (x-axis)
    float height;   // The height of the cube from top to bottom (y-axis)
    float length;   // The lenght of the cube from front to back (z-axis)
};