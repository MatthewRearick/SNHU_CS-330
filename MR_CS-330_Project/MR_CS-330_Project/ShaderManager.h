// ShaderManager.h
#pragma once

#include "Mesh.h"


/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

/**
 * Class representing a CubeMesh object with transformations.
 */
class ShaderManager {
public:
    // ######################
    // # Singleton Instance #
    // ######################


    /**
     * Get the Shader Manager Singleton Instance     *
     */
    static ShaderManager& getInstance();

    // Prevent copying and assignment
    ShaderManager(const ShaderManager&) = delete;
    void operator=(const ShaderManager&) = delete;


    // #################
    // # Other methods #
    // #################


    /**
     * Create a shader program in the provided programId reference based on the vertex mode
     * Position;    location = 0
     * Color;       location = 1
     * Normal;      location = 2
     * Texture UV;  location = 3
     *
     * @param programId Reference to create the program id in
     * @param vertexMode Mode for vertex attributes in VBO of Mesh to use shader program.
     */
    bool createShaderProgram(GLuint& programId, VertexMode vertexMode);

    /**
     * Create a shader program in the provided programId reference based on the provided Vertex and Fragment shaders
     *
     * @param vtxShaderSource Vertex shader source pointer
     * @param fragShaderSource Fragment shader source pointer
     * @param programId Reference to create the program id in
     */
    bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);

private:
    // ################
    // # Constructors #
    // ################


    /**
     * ShaderManager constructor.
     */
    ShaderManager();


    // #################
    // # Other methods #
    // #################


    /**
     * Create a shader program in the provided programId reference for Position and Color
     * Position;    location = 0
     * Color;       location = 1
     * (x, y, z, r, g, b, a)
     *
     * @param programId Reference to create the program id in
     */
    bool createShaderProgramPositionColor(GLuint& programId);

    /**
     * Create a shader program in the provided programId reference for Position and Texture UV
     * Position;    location = 0
     * Texture UV;  location = 3
     * (x, y, z, u, v)
     *
     * @param programId Reference to create the program id in
     */
    bool createShaderProgramPositionUV(GLuint& programId);

    /**
     * Create a shader program in the provided programId reference for Position, Color, and Texture UV
     * Position;    location = 0
     * Color;       location = 1
     * Texture UV;  location = 3
     * (x, y, z, r, g, b, a, u, v)
     *
     * @param programId Reference to create the program id in
     */
    bool createShaderProgramPositionColorUV(GLuint& programId);

    /**
     * Create a shader program in the provided programId reference for Position, Normal, and Texture UV
     * Position;    location = 0
     * Normal;      location = 2
     * Texture UV;  location = 3
     * (x, y, z, nx, ny, nz, u, v)
     *
     * @param programId Reference to create the program id in
     */
    bool createShaderProgramPositionNormalUV(GLuint& programId);
};
