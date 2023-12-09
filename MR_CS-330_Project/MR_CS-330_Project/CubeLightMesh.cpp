#include "CubeMesh.h"
#include "CubeLightMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################

CubeLightMesh::CubeLightMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float width, float height, float length,
	float lightRange, float ambientStrength, float diffuseStrength, float specularStrength)
	: CubeMesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId, width, height, length), lightRange(lightRange), ambientStrength(ambientStrength), diffuseStrength(diffuseStrength),
	specularStrength(specularStrength)
{
	calculateAttenuationParameters();
}

CubeLightMesh::CubeLightMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float width, float height, float length, float lightRange, float ambientStrength, float diffuseStrength, float specularStrength)
	: CubeMesh(vertexMode, unitOfMeasure, shaderProgramId, width, height, length), lightRange(lightRange), ambientStrength(ambientStrength), diffuseStrength(diffuseStrength), specularStrength(specularStrength)
{
	calculateAttenuationParameters();
}


// #################
// # Other methods #
// #################


const float CubeLightMesh::getAmbientStrength() const
{
	return ambientStrength;
}

const float CubeLightMesh::getDiffuseStrength() const
{
	return diffuseStrength;
}

const float CubeLightMesh::getSpecularStrength() const
{
	return specularStrength;
}

const float CubeLightMesh::getAttenuationConstant() const
{
	return attenuationConstant;
}

const float CubeLightMesh::getAttenuationLinear() const
{
	return attenuationLinear;
}

const float CubeLightMesh::getAttenuationQuadratic() const
{
	return attenuationQuadratic;
}


// ##################
// # Setter methods #
// ##################


void CubeLightMesh::setAmbientStrength(float ambientStrength)
{
	this->ambientStrength = ambientStrength;
}

void CubeLightMesh::setDiffuseStrength(float diffuseStrength)
{
	this->diffuseStrength = diffuseStrength;
}

void CubeLightMesh::setSpecularStrength(float specularStrength)
{
	this->specularStrength = specularStrength;
}


// #################
// # Other methods #
// #################



void CubeLightMesh::glowFade(float fadeAmount, float minGLow, float maxGlow)
{
	if (glowFadeOut) {
		fadeAmount *= -1;
	}

	diffuseStrength += fadeAmount;

	if (diffuseStrength < minGLow) {
		diffuseStrength = minGLow;
		glowFadeOut = !glowFadeOut;
	}
	else if (diffuseStrength > maxGlow) {
		diffuseStrength = maxGlow;
		glowFadeOut = !glowFadeOut;
	}
}

void CubeLightMesh::glowColorShift(float shiftAmount, float redPercent, float greenPercent, float bluePercent)
{
	float redShiftAmount = shiftAmount * redPercent;
	float greenShiftAmount = shiftAmount * greenPercent;
	float blueShiftAmount = shiftAmount * bluePercent;

	if (glowRedShiftOut) {
		redShiftAmount *= -1;
	}
	color.r += redShiftAmount;

	if (glowGreenShiftOut) {
		greenShiftAmount *= -1;
	}
	color.g += greenShiftAmount;

	if (glowBlueShiftOut) {
		blueShiftAmount *= -1;
	}
	color.b += blueShiftAmount;

	if (color.r < 0.0f) {
		color.r = 0.0f;
		glowRedShiftOut = !glowRedShiftOut;
	}
	else if (color.r > 1.0f) {
		color.r = 1.0f;
		glowRedShiftOut = !glowRedShiftOut;
	}

	if (color.g < 0.0f) {
		color.g = 0.0f;
		glowGreenShiftOut = !glowGreenShiftOut;
	}
	else if (color.g > 1.0f) {
		color.g = 1.0f;
		glowGreenShiftOut = !glowGreenShiftOut;
	}

	if (color.b < 0.0f) {
		color.b = 0.0f;
		glowBlueShiftOut = !glowBlueShiftOut;
	}
	else if (color.b > 1.0f) {
		color.b = 1.0f;
		glowBlueShiftOut = !glowBlueShiftOut;
	}
}


// ###################
// #                 #
// # Private methods #
// #                 #
// ###################


const void CubeLightMesh::calculateAttenuationParameters()
{
	attenuationConstant = 1.0f;
	// Rough approximate formula for linear
	attenuationLinear = 4.6905f * (pow(lightRange, -1.01f));
	// Rought approximate formula for quadratic
	attenuationQuadratic = 82.445 * (pow(lightRange, -2.019));
}