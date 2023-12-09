// PyramidMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a PyramidMesh object with transformations.
 */
class PyramidMesh : public Mesh {
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
     * @param width The width of the base from left to right (x-axis).
     * @param height The height from the center of the base to the peak of the pyramid (y-axis).
     * @param length The lenght of the base from front to back (z-axis).
     */
    PyramidMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float width = 1, float height = 1, float length = 1);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param width The width of the base from left to right (x-axis).
     * @param height The height from the center of the base to the peak of the pyramid (y-axis).
     * @param length The lenght of the base from front to back (z-axis).
     */
    PyramidMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float width = 1, float height = 1, float length = 1);


    // #############################
    // # Abstract method overrides #
    // #############################


    /**
     * Generate the vertex and element buffer.
     * Must be used before generateVAO().
     * All texture clamping must be performed befor using this method
     */
    void generateVertices() override;

private:

    // #############
    // # Variables #
    // #############

    float width;    // The width of the base from left to right (x-axis)
    float height;   // The height from the center of the base to the peak of the pyramid (y-axis)
    float length;   // The lenght of the base from front to back (z-axis)
};