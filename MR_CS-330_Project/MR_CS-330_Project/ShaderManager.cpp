#include "ShaderManager.h"
#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ######################
// # Singleton Instance #
// ######################


ShaderManager& ShaderManager::getInstance()
{
    static ShaderManager instance;
    return instance;
}


// #################
// # Other methods #
// #################


bool ShaderManager::createShaderProgram(GLuint& programId, VertexMode vertexMode)
{
    switch (vertexMode) {
    case POSITION_COLOR:
        return createShaderProgramPositionColor(programId);
        break;
    case POSITION_UV:
        return createShaderProgramPositionUV(programId);
        break;
    case POSITION_COLOR_UV:
        return createShaderProgramPositionColorUV(programId);
        break;
    case POSITION_NORMAL_UV:
        return createShaderProgramPositionNormalUV(programId);
        break;
    default:
        throw "Vertex mode not implemented yet.";
        break;
    }
}

bool ShaderManager::createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


// ###################
// #                 #
// # Private methods #
// #                 #
// ###################


// ################
// # Constructors #
// ################


ShaderManager::ShaderManager() {}


// #################
// # Other methods #
// #################


bool ShaderManager::createShaderProgramPositionColor(GLuint& programId)
{
    /* Vertex Shader Source Code*/
    const GLchar* vertexShaderSource = GLSL(460,
        layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
        layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1

        out vec4 vertexColor; // variable to transfer color data to the fragment shader

        //Global variables for the  transform matrices
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
            vertexColor = color; // references incoming color data
        }
    );


    /* Fragment Shader Source Code*/
    const GLchar* fragmentShaderSource = GLSL(460,
        in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

        out vec4 fragmentColor;

        void main()
        {
            fragmentColor = vec4(vertexColor);
        }
    );

    return createShaderProgram(vertexShaderSource, fragmentShaderSource, programId);
}

bool ShaderManager::createShaderProgramPositionUV(GLuint& programId)
{
    /* Vertex Shader Source Code*/
    const GLchar* vertexShaderSource = GLSL(460,
        layout(location = 0) in vec3 position;
        layout(location = 3) in vec2 textureCoordinate;

        out vec2 vertexTextureCoordinate;


        //Global variables for the transform matrices
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
            vertexTextureCoordinate = textureCoordinate;
        }
    );


    /* Fragment Shader Source Code*/
    const GLchar* fragmentShaderSource = GLSL(460,
        in vec2 vertexTextureCoordinate;

        out vec4 fragmentColor;

        uniform sampler2D uTextureBase;
        uniform sampler2D uTextureOverlay;
        uniform bool enableTextureOverlay;

        void main()
        {
            vec4 textureBase = texture(uTextureBase, vertexTextureCoordinate);
            if (enableTextureOverlay) {
                vec4 textureOverlay = texture(uTextureOverlay, vertexTextureCoordinate);
                fragmentColor = mix(textureBase, textureOverlay, textureOverlay.a);
            }
            else {
                fragmentColor = textureBase;
            }
        }
    );

    return createShaderProgram(vertexShaderSource, fragmentShaderSource, programId);
}

bool ShaderManager::createShaderProgramPositionColorUV(GLuint& programId)
{
    return false;
}

bool ShaderManager::createShaderProgramPositionNormalUV(GLuint& programId)
{
    /* Vertex Shader Source Code*/
    const GLchar* vertexShaderSource = GLSL(460,
        layout(location = 0) in vec3 aPos;
        layout(location = 2) in vec3 aNormal;
        layout(location = 3) in vec2 aTexCoords;

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoords;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoords = aTexCoords;

            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    );


    /* Fragment Shader Source Code*/
    const GLchar* fragmentShaderSource = GLSL(460,
        out vec4 FragColor;

        struct Material {
            sampler2D diffuse;
            sampler2D specular;
            float shininess;
        };

        struct DirLight {
            vec3 direction;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        struct PointLight {
            vec3 position;

            float constant;
            float linear;
            float quadratic;

            vec3 color;

            float ambientStrength;
            float diffuseStrength;
            float specularStrength;
        };

        struct SpotLight {
            vec3 position;
            vec3 direction;
            float cutOff;
            float outerCutOff;

            float constant;
            float linear;
            float quadratic;

            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoords;

        const int MAX_POINT_LIGHTS = 100;

        uniform vec3 viewPos;
        uniform DirLight dirLight;
        uniform int pointLightCount;
        uniform PointLight pointLights[MAX_POINT_LIGHTS];
        uniform SpotLight spotLight;
        uniform Material material;

        // function prototypes
        vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
        vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
        vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

        void main()
        {
            // properties
            vec3 norm = normalize(Normal);
            vec3 viewDir = normalize(viewPos - FragPos);

            // == =====================================================
            // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
            // For each phase, a calculate function is defined that calculates the corresponding color
            // per lamp. In the main() function we take all the calculated colors and sum them up for
            // this fragment's final color.
            // == =====================================================
            // phase 1: directional lighting
            vec3 result = CalcDirLight(dirLight, norm, viewDir);
            // phase 2: point lights
            for (int i = 0; i < pointLightCount; i++)
                result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
            // phase 3: spot light
            result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

            FragColor = vec4(result, 1.0);
        }

        // calculates the color when using a directional light.
        vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
        {
            vec3 lightDir = normalize(-light.direction);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            // combine results
            vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
            return (ambient + diffuse + specular);
        }

        // calculates the color when using a point light.
        vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            // attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            // combine results
            vec3 ambient = light.color * light.ambientStrength * vec3(texture(material.diffuse, TexCoords));
            vec3 diffuse = light.color * light.diffuseStrength * diff * vec3(texture(material.diffuse, TexCoords));
            vec3 specular = light.color * light.specularStrength * spec * vec3(texture(material.specular, TexCoords));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            return (ambient + diffuse + specular);
        }

        // calculates the color when using a spot light.
        vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            // attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            // spotlight intensity
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
            // combine results
            vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
            ambient *= attenuation * intensity;
            diffuse *= attenuation * intensity;
            specular *= attenuation * intensity;
            return (ambient + diffuse + specular);
        }
    );

    return createShaderProgram(vertexShaderSource, fragmentShaderSource, programId);
}
