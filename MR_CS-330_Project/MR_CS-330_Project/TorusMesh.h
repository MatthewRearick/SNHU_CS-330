// TorusMesh.h
#pragma once

#include "Mesh.h"

/**
 * Class representing a TorusMesh object with transformations.
 */
class TorusMesh : public Mesh {
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
     * @param outerRadius The distance from the center to the outside of the tube.
     * @param innerRadius The distance from the center to the inside of the tube.
     * @param slices The number of lines of longitude, these are slices parallel to the z-axis and go around the tube the short way (through the hole).
     * @param stacks The number of lines of latitude plus 1, these lines are perpendicular to the z-axis and go around the tube the long way (arouind the hole).
     */
    TorusMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float outerRadius = 0.5f, float innerRadius = 0.25f, int slices = 100, int stacks = 100);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param outerRadius The distance from the center to the outside of the tube.
     * @param innerRadius The distance from the center to the inside of the tube.
     * @param slices The number of lines of longitude, these are slices parallel to the z-axis and go around the tube the short way (through the hole).
     * @param stacks The number of lines of latitude plus 1, these lines are perpendicular to the z-axis and go around the tube the long way (arouind the hole).
     */
    TorusMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float outerRadius = 0.5f, float innerRadius = 0.25f, int slices = 100, int stacks = 100);


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

    float outerRadius;  // The distance from the center to the outside of the tube
    float innerRadius;  // The distance from the center to the inside of the tube
    int slices;         // The number of lines of longitude, these are slices parallel to the z-axis and go around the tube the short way (through the hole)
    int stacks;         // The number of lines of latitude plus 1, these lines are perpendicular to the z-axis and go around the tube the long way (arouind the hole)
};