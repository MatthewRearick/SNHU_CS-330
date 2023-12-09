// Mesh.h
#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Enum for VertexMode
enum VertexMode {
    POSITION_COLOR,         // (x, y, z, r, g, b, a)
    POSITION_UV,            // (x, y, z, u, v)
    POSITION_COLOR_UV,      // (x, y, z, r, g, b, a, u, v)
    POSITION_NORMAL_UV      // (x, y, z, nx, ny, nz, u, v)
};

// Enum for UnitOfMeasure
enum UnitOfMeasure {
    CENTIMETER,
    INCH
};


// Enum for RotationOrder
// Determines the order the rotations are applied in
enum RotationOrder {
    Z_Y_X,
    Z_X_Y,
    Y_Z_X,
    Y_X_Z,
    X_Y_Z,
    X_Z_Y
};

/**
 * Class representing a Mesh object with transformations.
 */
class Mesh {
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
     */
    Mesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale = DEFAULT_SCALE_VEC3, glm::vec3 rotationDegrees = DEFAULT_ROTATION_DEGREES_VEC3,
        glm::vec3 translation = DEFAULT_TRANSLATION_VEC3, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER, GLuint shaderProgramId = -1);

    /**
     * Mesh constructor with no initial transformation.
     *
     * @param vertexMode Mode for vertex attributes in VBO.
     * @param unitOfMeasure Unit of measure for metric to imperial scaling.
     * @param shaderProgramId The ID of the shader program for rendering.
     */
    Mesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId = -1);


    // ##################
    // # Getter methods #
    // ##################


    /**
     * Get the VertexMode of the Mesh.
     *
     * @return The VertexMode.
     */
    const VertexMode getVertexMode() const;

    /**
     * Get the UnitOfMeasure of the Mesh.
     *
     * @return The UnitOfMeasure.
     */
    const UnitOfMeasure getUnitOfMeasure() const;

    /**
     * Get the vertex buffer of the Mesh.
     *
     * @return The vertex buffer as a vector of GLfloat.
     */
    const std::vector<GLfloat> getVertexBuffer() const;

    /**
     * Get the element buffer of the Mesh.
     *
     * @return The element buffer as a vector of GLushort.
     */
    const std::vector<GLushort> getElementBuffer() const;

    /**
     * Get the size of the vertex buffer.
     *
     * @return The size of the vertex buffer as GLsizeiptr.
     */
    const GLsizeiptr getVertexBufferSize() const;

    /**
     * Get the size of the element buffer.
     *
     * @return The size of the element buffer as GLsizeiptr.
     */
    const GLsizeiptr getElementBufferSize() const;

    /**
     * Get the count of vertex sets (not the count of values in the vector).
     *
     * @return The count of vertex sets as GLsizei.
     */
    const GLsizei getVertexBufferCount() const;

    /**
     * Get the count of elements in the element buffer.
     *
     * @return The count of elements in the element buffer as GLsizei.
     */
    const GLsizei getElementBufferCount() const;

    /**
     * Get the number of values per vertex attribute.
     *
     * @return The number of values per vertex as GLuint.
     */
    const GLuint getFloatsPerVertex() const;

    /**
     * Get the number of values per color attribute.
     *
     * @return The number of values per color as GLuint.
     */
    const GLuint getFloatsPerColor() const;

    /**
     * Get the number of values per normal attribute.
     *
     * @return The number of values per normal as GLuint.
     */
    const GLuint getFloatsPerNormal() const;

    /**
     * Get the number of values per UV attribute.
     *
     * @return The number of values per UV as GLuint.
     */
    const GLuint getFloatsPerUV() const;

    /**
     * Get the stride (number of floats between vertices) of the Mesh.
     *
     * @return The stride as GLuint.
     */
    const GLuint getStride() const;

    /**
     * Get the Vertex Buffer Object (VBO) of the Mesh.
     *
     * @return The VBO as GLuint.
     */
    const GLuint getVBO() const;

    /**
     * Get the Element Buffer Object (EBO) of the Mesh.
     *
     * @return The EBO as GLuint.
     */
    const GLuint getEBO() const;

    /**
     * Get the Vertex Array Object (VAO) of the Mesh.
     *
     * @return The VAO as GLuint.
     */
    const GLuint getVAO() const;

    /**
     * Get the scale matrix of the Mesh.
     *
     * @return The scale matrix as a glm::mat4.
     */
    const glm::mat4 getScale() const;

    /**
     * Get the rotation matrix of the Mesh.
     *
     * @return The rotation matrix as a glm::mat4.
     */
    const glm::mat4 getRotation() const;

    /**
     * Get the translation matrix of the Mesh.
     *
     * @return The translation matrix as a glm::mat4.
     */
    const glm::mat4 getTranslation() const;

    /**
     * Get the model matrix (combination of translation, rotation, and scale) of the Mesh.
     *
     * @return The model matrix as a glm::mat4.
     */
    const glm::mat4 getModel() const;

    /**
     * Get the ID of the shader program for rendering.
     *
     * @return The ID of the shader program for rendering.
     */
    const GLuint getShaderProgramId() const;

    /**
     * Get the IDs of the textures for rendering.
     *
     * @return The IDs of the textures for rendering.
     */
    const std::vector<GLuint> getTextureIds() const;

    /**
     * Get the Min and Max clamp values for texture U coordniate clamping for subsection of texture use
     *
     * @return The Min and Max clamp values for texture U coordniate clamping for subsection of texture use
     */
    const glm::vec2 getTextureUClamp() const;

    /**
     * Get the Min and Max clamp values for texture V coordniate clamping for subsection of texture use
     *
     * @return The Min and Max clamp values for texture V coordniate clamping for subsection of texture use
     */
    const glm::vec2 getTextureVClamp() const;


    /**
     * Get the color to use when rendering with a color mode
     *
     * @return The color to use when rendering with a color mode.
     */
    const glm::vec4 getColor() const;
    

    // ##################
    // # Setter methods #
    // ##################


    /**
     * Set the mesh scale per axis.
     *
     * @param xScale Scale factor along the X-axis.
     * @param yScale Scale factor along the Y-axis.
     * @param zScale Scale factor along the Z-axis.
     */
    void setScale(float xScale, float yScale, float zScale);

    /**
     * Set the mesh scale using a scale vector.
     *
     * @param scale Scale vector.
     */
    void setScale(glm::vec3 scale);

    /**
     * Set the mesh rotation in degrees per axis.
     *
     * @param xRotationDegrees Rotation angle along the X-axis in degrees.
     * @param yRotationDegrees Rotation angle along the Y-axis in degrees.
     * @param zRotationDegrees Rotation angle along the Z-axis in degrees.
     * @param rotationOrder Order of rotations to be applied.
     */
    void setRotation(float xRotationDegrees, float yRotationDegrees, float zRotationDegrees, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER);

    /**
     * Set the mesh rotation using a rotation vector.
     *
     * @param rotationDegrees Rotation vector per axis in degrees.
     * @param rotationOrder Order of rotations to be applied.
     */
    void setRotation(glm::vec3 rotationDegrees, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER);

    /**
     * Set the mesh translation to a specific location.
     *
     * @param xTranslation Translation along the X-axis.
     * @param yTranslation Translation along the Y-axis.
     * @param zTranslation Translation along the Z-axis.
     */

    // Set the mesh translation to a specific location
    void setTranslation(float xTranslation, float yTranslation, float zTranslation);

    /**
     * Set the mesh translation using a translation vector.
     *
     * @param translation Translation vector.
     */
    void setTranslation(glm::vec3 translation);

    /**
     * Set the Min and Max clamp values for texture U coordniate clamping for subsection of texture use
     *
     * @param textureUClamp (Min, Max).
     */
    void setTextureUClamp(glm::vec2 textureUClamp);

    /**
     * Set the Min and Max clamp values for texture V coordniate clamping for subsection of texture use
     *
     * @param textureVClamp (Min, Max).
     */
    void setTextureVClamp(glm::vec2 textureVClamp);


    /**
     * Set the color to use when rendering with a color mode
     * Must be used before generating vertices
     *
     * @param color The color to use when rendering with a color mode.
     */
    void setColor(glm::vec4 color);


    // #################
    // # Other methods #
    // #################


    /**
     * Scale the mesh per axis, applies on top of the existing scale.
     *
     * @param xScale Scale factor along the X-axis.
     * @param yScale Scale factor along the Y-axis.
     * @param zScale Scale factor along the Z-axis.
     */
    void scaleMesh(float xScale, float yScale, float zScale);

    /**
     * Scale the mesh using a scale vector, applies on top of the existing scale.
     *
     * @param scale Scale vector.
     */
    void scaleMesh(glm::vec3 scale);

    /**
     * Rotate the mesh in degrees per axis, applies on top of the existing rotation.
     *
     * @param xRotationDegrees Rotation angle along the X-axis in degrees.
     * @param yRotationDegrees Rotation angle along the Y-axis in degrees.
     * @param zRotationDegrees Rotation angle along the Z-axis in degrees.
     * @param rotationOrder Order of rotations to be applied.
     */
    void rotateMesh(float xRotationDegrees, float yRotationDegrees, float zRotationDegrees, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER);

    /**
     * Rotate the mesh using a rotation vector, applies on top of the existing rotation.
     *
     * @param rotationDegrees Rotation vector per axis in degrees.
     * @param rotationOrder Order of rotations to be applied.
     */
    void rotateMesh(glm::vec3 rotationDegrees, RotationOrder rotationOrder = DEFAULT_ROTATION_ORDER);

    /**
     * Translate the mesh to an offset of the existing translation per axis.
     *
     * @param xTranslation Translation along the X-axis.
     * @param yTranslation Translation along the Y-axis.
     * @param zTranslation Translation along the Z-axis.
     */
    void translateMesh(float xTranslation, float yTranslation, float zTranslation);

    /**
     * Translate the mesh using a translation vector, applies an offset to the existing translation.
     *
     * @param translation Translation vector.
     */
    void translateMesh(glm::vec3 translation);

    /**
     * Transform the mesh before creating VAO.
     * Applies a permanent transformation directly to the vertecies
     * Resets the scale, rotation, and translation to default base values
     */
    const void translateMeshPreVAO();

    /**
     * Destroy the VBO, EBO, and VAO objects.
     */
    void destroyMesh();

    /**
     * Generate the Vertex Array Object from the VBO and EBO.
     * Must be used before rendering.
     */
    void generateVAO();

    /**
     * Add a new texture layer to the Mesh.
     *
     * @param textureId The  of the texture for rendering.
     */
    void addTextureID(GLuint textureId);


    // #############
    // # Variables #
    // #############


    // Class constants
    static constexpr GLuint DEFAULT_FLOATS_PER_VERTEX = 3;
    static constexpr GLuint DEFAULT_FLOATS_PER_COLOR = 4;
    static constexpr GLuint DEFAULT_FLOATS_PER_NORMAL = 3;
    static constexpr GLuint DEFAULT_FLOATS_PER_UV = 2;
    static constexpr RotationOrder DEFAULT_ROTATION_ORDER = Z_Y_X;
    static constexpr glm::vec3 DEFAULT_SCALE_VEC3 = glm::vec3(1.0f, 1.0f, 1.0f);
    static constexpr glm::vec3 DEFAULT_ROTATION_DEGREES_VEC3 = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 DEFAULT_TRANSLATION_VEC3 = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec4 DEFAULT_COLOR = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f);       // No default color, will use reandom color

protected:    
    // #################
    // # Other methods #
    // #################

    
    /**
     * Get the rotation in a specific order.
     *
     * @param xRotation X-axis rotation matrix.
     * @param yRotation Y-axis rotation matrix.
     * @param zRotation Z-axis rotation matrix.
     * @param rotationOrder Order of rotations to be applied.
     * @return The resulting rotation matrix.
     */
    const glm::mat4 getRotationInOrder(glm::mat4 xRotation, glm::mat4 yRotation, glm::mat4 zRotation, RotationOrder rotationOrder);

    /**
     * Get a random color vector
     * Alpha is left at 1.0f
     *
     * @return The resulting color vector 4.
     */
    const glm::vec4 getRandomColor();

    /**
     * Clamps a U texture coordinate value
     *
     * @param uValue The U value to clamp.
     * 
     * @return The clamped U value
     */
    const GLfloat clampU(GLfloat uValue);

    /**
     * Clamps a V texture coordinate value
     *
     * @param uValue The V value to clamp.
     *
     * @return The clamped V value
     */
    const GLfloat clampV(GLfloat vValue);


    /**
     * Generates a normal for a given triangle (set of 3 vetices)
     *
     * @param vertex1 The first vertex.
     * @param vertex2 The second vertex.
     * @param vertex3 The third vertex.
     *
     * @return The normal for a given triangle
     */
    const glm::vec3 calculateNormal(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3);


    // ####################
    // # Abstract methods #
    // ####################
    
    
    /**
     * Generate the vertex and element buffer.
     * Must be used before generateVAO().
     * All texture clamping must be performed befor using this method
     */
    virtual void generateVertices() = 0;


    // #############
    // # Variables #
    // #############


    VertexMode vertexMode;                  // Expected mode for vertex attributes in vbo
    UnitOfMeasure unitOfMeasure;            // Unit of measure for metric to imperial scaling
    std::vector<GLfloat> vertexBuffer;      // Store vertecies (and attributes) to create vbo with
    std::vector<GLushort> elementBuffer;    // Store elements to create ebo with
    GLuint floatsPerVertex;                 // The number of values per vertex attribute
    GLuint floatsPerColor;                  // The number of values per color attribute
    GLuint floatsPerNormal;                 // The number of values per Normal attribute
    GLuint floatsPerUV;                     // The number of values per UV attribute
    GLuint stride;                          // Number of floats between vertecies
    GLuint vbo;                             // Vertex Buffer Object
    GLuint ebo;                             // Element (Index) Buffer Object
    GLuint vao;                             // Vertex Array Object
    glm::mat4 scale;                        // The scale matrix for the mesh
    glm::mat4 rotation;                     // The rotation matrix for the mesh
    glm::mat4 translation;                  // The translation matrix for the mesh
    GLuint shaderProgramId;                 // The ID of the shader program for rendering
    std::vector<GLuint> textureIds;         // The IDs of the textures for rendering
    glm::vec2 textureUClamp;                // Min and Max clamp values for texture U coordniate clamping for subsection of texture use
    glm::vec2 textureVClamp;                // Min and Max clamp values for texture V coordniate clamping for subsection of texture use
    GLfloat textureUClampRatio;             // Ratio of clamp for texture U coordnitate clamping
    GLfloat textureVClampRatio;             // Ratio of clamp for texture U coordnitate clamping
    glm::vec4 color;                        // The color to use when rendering with a color mode
};