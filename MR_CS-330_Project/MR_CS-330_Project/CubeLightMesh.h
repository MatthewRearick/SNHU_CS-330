#pragma once
// CubeMesh.h
#pragma once

#include "Mesh.h"
#include "CubeMesh.h"

/**
 * Class representing a CubeLightMesh object with transformations.
 * Intended to be used for lighting objects
 */
class CubeLightMesh : public CubeMesh {
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
     * @param lightRange The range of the light.
     * @param ambientStrength The ambient light strength.
     * @param diffuseStrength The ambient light strength.
     * @param specularStrength The ambient light strength.
     */
    CubeLightMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1, float width = 1, float height = 1, float length = 1, 
        float lightRange = 100, float ambientStrength = 0.05f, float diffuseStrength = 0.8f, float specularStrength = 1.0f);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     * @param width The width of the cube from left to right (x-axis).
     * @param height The height of the cube from top to bottom (y-axis).
     * @param length The lenght of the cube from front to back (z-axis).
     * @param lightRange The range of the light.
     * @param ambientStrength The ambient light strength.
     * @param diffuseStrength The ambient light strength.
     * @param specularStrength The ambient light strength.
     */
    CubeLightMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1, float width = 1, float height = 1, float length = 1, 
        float lightRange = 100, float ambientStrength = 0.05f, float diffuseStrength = 0.8f, float specularStrength = 1.0f);


    // ##################
    // # Getter methods #
    // ##################


    /**
     * Get the ambientStrength of the light.
     *
     * @return The ambientStrength.
     */
    const float getAmbientStrength() const;

    /**
     * Get the diffuseStrength of the light.
     *
     * @return The diffuseStrength.
     */
    const float getDiffuseStrength() const;

    /**
     * Get the specularStrength of the light.
     *
     * @return The specularStrength.
     */
    const float getSpecularStrength() const;

    /**
     * Get the attenuationConstant of the light.
     *
     * @return The attenuationConstant.
     */
    const float getAttenuationConstant() const;

    /**
     * Get the attenuationLinear of the light.
     *
     * @return The attenuationLinear.
     */
    const float getAttenuationLinear() const;

    /**
     * Get the attenuationQuadratic of the light.
     *
     * @return The attenuationQuadratic.
     */
    const float getAttenuationQuadratic() const;


    // ##################
    // # Setter methods #
    // ##################


    /**
     * Set the ambientStrength of the light.
     *
     * @param ambientStrength The ambientStrength of the light.
     */
    void setAmbientStrength(float ambientStrength);

    /**
     * Set the diffuseStrength of the light.
     *
     * @param diffuseStrength The diffuseStrength of the light.
     */
    void setDiffuseStrength(float diffuseStrength);

    /**
     * Set the specularStrength of the light.
     *
     * @param  specularStrength The specularStrength of the light.
     */
    void setSpecularStrength(float specularStrength);


    // #################
    // # Other methods #
    // #################

    /**
     * Gives the light a glowing effect by fading in and out
     *
     * @param The fade amount.
     * @param minGlow the maximum value to fade down to
     * @param maxGlow the maximum value to fade up to
     */
    void glowFade(float fadeAmount, float minGLow, float maxGlow);

    /**
     * Gives the light a glowing effect by fading in and out
     *
     * @param The shift amount.
     * @param redPercent the percent of the color shift to apply to this color
     * @param greenPercent the percent of the color shift to apply to this color
     * @param bluePercent the percent of the color shift to apply to this color
     */
    void glowColorShift(float shiftAmount, float redPercent, float greenPercent, float bluePercent);



private:
    // #################
    // # Other methods #
    // #################


    /**
     * Calculate the attenuation Constant, Linear, and Quadratic.
     */
    const void calculateAttenuationParameters();


    // #############
    // # Variables #
    // #############

    float lightRange;               // The range of the light
    float ambientStrength;          // The ambient light strength
    float diffuseStrength;          // The diffuse light strength
    float specularStrength;         // The specular light strength
    float attenuationConstant;      // The constant for calculating attenuation
    float attenuationLinear;        // The linear for calculating attenuation
    float attenuationQuadratic;     // The quadratic for calculating attenuation
    bool glowFadeOut = true;        // If the glowFade is currently decreasing
    bool glowRedShiftOut = true;    // If the red glowColorShift is currently decreasing
    bool glowGreenShiftOut = true;  // If the green glowColorShift is currently decreasing
    bool glowBlueShiftOut = true;   // If the blue glowColorShift is currently decreasing
};