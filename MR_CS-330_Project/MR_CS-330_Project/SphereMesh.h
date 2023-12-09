// SphereMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a SphereMesh object with transformations.
 */
class SphereMesh : public Mesh {
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
     * @param radius The radius of the Sphere.
     * @param slices The number of slices around the Sphere (greater = more triangles and detail).
     * @param stacks The number of stacks down the Sphere (greater = more triangles and detail).
     */
    SphereMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float radius = 0.5f, int slices = 32, int stacks = 16);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param radius The radius of the Sphere.
     * @param slices The number of slices around the Sphere (greater = more triangles and detail).
     * @param stacks The number of stacks down the Sphere (greater = more triangles and detail).
     */
    SphereMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float radius = 0.5f, int slices = 32, int stacks = 16);


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

    float radius;   // The radius of the Sphere
    int slices;     // The number of slices around the Sphere (greater = more triangles and detail)
    int stacks;     // The number of stacks down the Sphere (greater = more triangles and detail)
};