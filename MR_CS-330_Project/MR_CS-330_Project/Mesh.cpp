#include "Mesh.h"
#include <random>


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


Mesh::Mesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId)
	: vertexMode(vertexMode), unitOfMeasure(unitOfMeasure), floatsPerVertex(0), floatsPerColor(0), floatsPerNormal(0), floatsPerUV(0), stride(0), shaderProgramId(shaderProgramId)
{
	if (vertexMode == POSITION_COLOR){
		floatsPerVertex = DEFAULT_FLOATS_PER_VERTEX;
		floatsPerColor = DEFAULT_FLOATS_PER_COLOR;
	}
	else if (vertexMode == POSITION_COLOR_UV) {
		floatsPerVertex = DEFAULT_FLOATS_PER_VERTEX;
		floatsPerColor = DEFAULT_FLOATS_PER_COLOR;
		floatsPerUV = DEFAULT_FLOATS_PER_UV;
	}
	else if (vertexMode == POSITION_UV) {
		floatsPerVertex = DEFAULT_FLOATS_PER_VERTEX;
		floatsPerUV = DEFAULT_FLOATS_PER_UV;
	}
	else if (vertexMode == POSITION_NORMAL_UV) {
		floatsPerVertex = DEFAULT_FLOATS_PER_VERTEX;
		floatsPerNormal = DEFAULT_FLOATS_PER_NORMAL;
		floatsPerUV = DEFAULT_FLOATS_PER_UV;
	}

	stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);

	setScale(scale);
	setRotation(rotationDegrees, rotationOrder);
	setTranslation(translation);
	setTextureUClamp(glm::vec2(0.0f, 1.0f));
	setTextureVClamp(glm::vec2(0.0f, 1.0f));
	setColor(DEFAULT_COLOR);
}

Mesh::Mesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId)
	: Mesh(vertexMode, unitOfMeasure, Mesh::DEFAULT_SCALE_VEC3, DEFAULT_ROTATION_DEGREES_VEC3, DEFAULT_TRANSLATION_VEC3, DEFAULT_ROTATION_ORDER, shaderProgramId)
{
}


// ##################
// # Getter methods #
// ##################


const VertexMode Mesh::getVertexMode() const
{
	return vertexMode;
}

const UnitOfMeasure Mesh::getUnitOfMeasure() const
{
	return unitOfMeasure;
}

const std::vector<GLfloat> Mesh::getVertexBuffer() const
{
	return vertexBuffer;
}

const std::vector<GLushort> Mesh::getElementBuffer() const
{
	return elementBuffer;
}

const GLsizeiptr Mesh::getVertexBufferSize() const
{
	// The number of items in the buffer multiplied by the size of each item
	return vertexBuffer.size() * sizeof(GLfloat);
}

const GLsizeiptr Mesh::getElementBufferSize() const
{
	// The number of items in the buffer multiplied by the size of each item
	return elementBuffer.size() * sizeof(GLushort);
}

const GLsizei Mesh::getVertexBufferCount() const
{
	// The number of items in the buffer divided by the number of items per vertex
	return vertexBuffer.size() / (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV);
}

const GLsizei Mesh::getElementBufferCount() const
{
	// The number of items in the buffer
	return elementBuffer.size();
}

const GLuint Mesh::getFloatsPerVertex() const
{
	return floatsPerVertex;
}

const GLuint Mesh::getFloatsPerColor() const
{
	return floatsPerColor;
}

const GLuint Mesh::getFloatsPerNormal() const
{
	return floatsPerNormal;
}

const GLuint Mesh::getFloatsPerUV() const
{
	return floatsPerUV;
}

const GLuint Mesh::getStride() const
{
	return stride;
}

const GLuint Mesh::getVBO() const
{
	return vbo;
}

const GLuint Mesh::getEBO() const
{
	return ebo;
}

const GLuint Mesh::getVAO() const
{
	return vao;
}

const glm::mat4 Mesh::getScale() const
{
	return scale;
}

const glm::mat4 Mesh::getRotation() const
{
	return rotation;
}

const glm::mat4 Mesh::getTranslation() const
{
	return translation;
}

const glm::mat4 Mesh::getModel() const
{
	// Standard formulat for Model
	return translation * rotation * scale;
}

const GLuint Mesh::getShaderProgramId() const
{
	return shaderProgramId;
}

const std::vector<GLuint> Mesh::getTextureIds() const
{
	return textureIds;
}

const glm::vec2 Mesh::getTextureUClamp() const
{
	return textureUClamp;
}

const glm::vec2 Mesh::getTextureVClamp() const
{
	return textureVClamp;
}

const glm::vec4 Mesh::getColor() const
{
	return color;
}


// ##################
// # Setter methods #
// ##################


void Mesh::setScale(float xScale, float yScale, float zScale)
{
	setScale(glm::vec3(xScale, yScale, zScale));
}

void Mesh::setScale(glm::vec3 scale)
{
	this->scale = glm::scale(scale);
}

void Mesh::setRotation(float xRotationDegrees, float yRotationDegrees, float zRotationDegrees, RotationOrder rotationOrder)
{
	setRotation(glm::vec3(xRotationDegrees, yRotationDegrees, zRotationDegrees), rotationOrder);
}

void Mesh::setRotation(glm::vec3 rotationDegrees, RotationOrder rotationOrder)
{
	// Individually get the rotation matrices of each axis
	glm::mat4 xRotation = glm::rotate(glm::radians(rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 yRotation = glm::rotate(glm::radians(rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 zRotation = glm::rotate(glm::radians(rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Get the combined rotation matrix based on the desired rotation order
	this->rotation = getRotationInOrder(xRotation, yRotation, zRotation, rotationOrder);
}

void Mesh::setTranslation(float xTranslation, float yTranslation, float zTranslation)
{
	setTranslation(glm::vec3(xTranslation, yTranslation, zTranslation));
}

void Mesh::setTranslation(glm::vec3 translation)
{
	this->translation = glm::translate(translation);
}

void Mesh::setTextureUClamp(glm::vec2 textureUClamp)
{
	GLfloat clampMin = textureUClamp.x;
	GLfloat clampMax = textureUClamp.y;
	GLfloat clampRange = clampMax - clampMin;
	this->textureUClampRatio = clampRange / 1.0f;
	this->textureUClamp = textureUClamp;
}

void Mesh::setTextureVClamp(glm::vec2 textureVClamp)
{
	GLfloat clampMin = textureVClamp.x;
	GLfloat clampMax = textureVClamp.y;
	GLfloat clampRange = clampMax - clampMin;
	this->textureVClampRatio = clampRange / 1.0f;
	this->textureVClamp = textureVClamp;
}

void Mesh::setColor(glm::vec4 color)
{
	this->color = color;
}


// #################
// # Other methods #
// #################


void Mesh::scaleMesh(float xScale, float yScale, float zScale)
{
	scaleMesh(glm::vec3(xScale, yScale, zScale));
}

void Mesh::scaleMesh(glm::vec3 scale)
{
	this->scale = glm::scale(scale) * this->scale;
}

void Mesh::rotateMesh(float xRotationDegrees, float yRotationDegrees, float zRotationDegrees, RotationOrder rotationOrder)
{
	rotateMesh(glm::vec3(xRotationDegrees, yRotationDegrees, zRotationDegrees), rotationOrder);
}

void Mesh::rotateMesh(glm::vec3 rotationDegrees, RotationOrder rotationOrder)
{
	// Individually get the rotation matrices of each axis
	glm::mat4 xRotation = glm::rotate(glm::radians(rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 yRotation = glm::rotate(glm::radians(rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 zRotation = glm::rotate(glm::radians(rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Get the combined rotation matrix based on the desired rotation order
	this->rotation = getRotationInOrder(xRotation, yRotation, zRotation, rotationOrder) * this->rotation;
}

void Mesh::translateMesh(float xTranslation, float yTranslation, float zTranslation)
{
	translateMesh(glm::vec3(xTranslation, yTranslation, zTranslation));
}

void Mesh::translateMesh(glm::vec3 translation)
{
	this->translation = glm::translate(translation) * this->translation;
}

const void Mesh::translateMeshPreVAO()
{
	glm::mat4 model = getModel();

	if (vertexMode != POSITION_NORMAL_UV) {
		// Transform each vertex in the vertexBuffer by the model matrix transformation
		for (size_t i = 0; i < vertexBuffer.size(); i += (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV)) {
			glm::vec4 vertex(vertexBuffer[i], vertexBuffer[i + 1], vertexBuffer[i + 2], 1.0f);
			vertex = model * vertex;

			vertexBuffer[i] = vertex.x;
			vertexBuffer[i + 1] = vertex.y;
			vertexBuffer[i + 2] = vertex.z;
		}
	}
	else if (vertexMode == POSITION_NORMAL_UV) {
		// Transform each vertex and normal in the vertexBuffer by the model matrix transformation
		for (size_t i = 0; i < vertexBuffer.size(); i += (floatsPerVertex + floatsPerColor + floatsPerNormal + floatsPerUV)) {
			glm::vec4 vertex(vertexBuffer[i], vertexBuffer[i + 1], vertexBuffer[i + 2], 1.0f);
			glm::vec3 vertexNormal(vertexBuffer[i + 3], vertexBuffer[i + 4], vertexBuffer[i + 5]);

			vertex = model * vertex;
			vertexNormal = glm::mat3(glm::transpose(glm::inverse(model))) * vertexNormal;

			vertexBuffer[i] = vertex.x;
			vertexBuffer[i + 1] = vertex.y;
			vertexBuffer[i + 2] = vertex.z;

			vertexBuffer[i + 3] = vertexNormal.x;
			vertexBuffer[i + 4] = vertexNormal.y;
			vertexBuffer[i + 5] = vertexNormal.z;
		}
	}

	// Reset the transformation values to default to account for the pre-VAO transformation after creating a VAO
	setScale(DEFAULT_SCALE_VEC3);
	setRotation(DEFAULT_ROTATION_DEGREES_VEC3);
	setTranslation(DEFAULT_TRANSLATION_VEC3);
}

void Mesh::destroyMesh()
{
	// Clear the GPU objects
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
}

void Mesh::generateVAO()
{
	glGenVertexArrays(1, &vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, getVertexBufferSize(), vertexBuffer.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU


	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getElementBufferSize(), elementBuffer.data(), GL_STATIC_DRAW);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	if (floatsPerColor != 0)
	{
		glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
		glEnableVertexAttribArray(1);
	}

	if (floatsPerNormal != 0)
	{
		glVertexAttribPointer(2, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerColor)));
		glEnableVertexAttribArray(2);
	}

	if (floatsPerUV != 0)
	{
		glVertexAttribPointer(3, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerNormal)));
		glEnableVertexAttribArray(3);
	}

	// Deactivate the Vertex Array Object
	glBindVertexArray(0);
}

void Mesh::addTextureID(GLuint textureId)
{
	textureIds.push_back(textureId);
}


// ###################
// #                 #
// # Private methods #
// #                 #
// ###################


const glm::mat4 Mesh::getRotationInOrder(glm::mat4 xRotation, glm::mat4 yRotation, glm::mat4 zRotation, RotationOrder rotationOrder)
{
	// Matrices are applied in reverse multiplication order
	// Identify the proper order to apply rotation in
	switch (rotationOrder) {
	case Z_Y_X:
		return xRotation * yRotation * zRotation;
		break;
	case Z_X_Y:
		return yRotation * xRotation * zRotation;
		break;
	case Y_Z_X:
		return xRotation * zRotation * yRotation;
		break;
	case Y_X_Z:
		return zRotation * xRotation * yRotation;
		break;
	case X_Y_Z:
		return zRotation * yRotation * xRotation;
		break;
	case X_Z_Y:
		return yRotation * zRotation * xRotation;
		break;
	default:
		return xRotation * yRotation * zRotation;
		break;
	}
}

const glm::vec4 Mesh::getRandomColor()
{

	// Random number generation
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	// Random r, g, and b
	return glm::vec4(distribution(gen), distribution(gen), distribution(gen), 1.0f);
}

const GLfloat Mesh::clampU(GLfloat uValue)
{
	GLfloat clampMin = textureUClamp.x;
	return uValue * textureUClampRatio + clampMin;
}

const GLfloat Mesh::clampV(GLfloat vValue)
{
	GLfloat clampMin = textureVClamp.x;
	return vValue * textureVClampRatio + clampMin;
}

const glm::vec3 Mesh::calculateNormal(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3)
{
	glm::vec3 edge1 = vertex2 - vertex1;
	glm::vec3 edge2 = vertex3 - vertex1;

	// Calculate the cross product of the edges to get the normal vertexector
	glm::vec3 normal = glm::cross(edge1, edge2);

	// Ensure the normal points away from the origin (0, 0, 0)
	if (glm::dot(normal, -vertex1) >= 0.0f) {
		normal = -normal;
	}

	return glm::normalize(normal);
}
