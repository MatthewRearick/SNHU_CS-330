#include <iostream>         // cout, cerr
#include <random>
#include <string>
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Image loading Utility functions
#include <map>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h" // Camera class
#include "InputHandler.h" // Input Handler class
#include "ShaderManager.h" // Shader Manager class

// Primitive Meshes
#include "PyramidMesh.h"
#include "CubeMesh.h"
#include "CubeLightMesh.h"
#include "PlaneMesh.h"
#include "CylinderMesh.h"
#include "FrustumMesh.h"
#include "SphereMesh.h"
#include "TorusMesh.h"

// Complex Meshes
#include "SodaCanMesh.h"
#include "PS5ControllerMesh.h"
#include "HeadsetMesh.h"
#include "BackScratcherMesh.h"

using namespace std; // Standard namespace

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Matt Rearick - Project One"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 1028;
    const int WINDOW_HEIGHT = 720;

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Texture id
    GLuint gTextureId;
    // Shader program
    GLuint gProgramId;

    // camera
    Camera gCamera(glm::vec3(0.0f, 65.0f, 70.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, -45.0f);
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // key state tracking
    bool pKeyPressed = false;

    // input handler
    InputHandler gInput(gCamera);

    // shader manager
    ShaderManager& gShaderManager = ShaderManager::getInstance();
    // store shader program id mapping by vertex mode
    std::map<VertexMode, GLuint> programIds;

    // texture file path storage
    const char* texFilename;

    // list of meshes in scene
    std::vector<Mesh*> sceneMeshes;

    // list of mesh lights in scene
    std::vector<CubeLightMesh*> sceneMeshLights;

    // Temp directional light struct
    // TODO: move into class for managing scene directional lighting
    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    DirectionalLight directionalLight;
}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void URenderMeshObject(Mesh& mesh);
void UDestroyShaderProgram(GLuint programId);
bool UCreateTexture(const char* filename, GLuint& textureId);



// TODO, move texture management to class
// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}




int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the shader programs
    if (!gShaderManager.createShaderProgram(programIds[POSITION_COLOR], VertexMode::POSITION_COLOR))
        return EXIT_FAILURE;
    if (!gShaderManager.createShaderProgram(programIds[POSITION_UV], VertexMode::POSITION_UV))
        return EXIT_FAILURE;
    if (!gShaderManager.createShaderProgram(programIds[POSITION_NORMAL_UV], VertexMode::POSITION_NORMAL_UV))
        return EXIT_FAILURE;

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(programIds[POSITION_UV]);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(programIds[POSITION_UV], "uTextureBase"), 0);
    // We set the texture overlay as texture unit 1
    glUniform1i(glGetUniformLocation(programIds[POSITION_UV], "uTextureOverlay"), 1);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(programIds[POSITION_NORMAL_UV]);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(programIds[POSITION_NORMAL_UV], "uTexture"), 0);

    // Sets the background color of the window to Sky Blue (it will be implicitely used by glClear)
    glClearColor(0.43f, 0.71f, 0.72f, 1.0f);

    // Directional Light
    // Replicate angle and color of actual light scene is based on
    // Far away single room ceiling light
    directionalLight.direction = glm::vec3(1.0F, -0.75, -0.5F);
    directionalLight.ambient = glm::vec3(0.075f, 0.075f, 0.05625f);
    directionalLight.diffuse = glm::vec3(0.6f, 0.6f, 0.45f);
    directionalLight.specular = glm::vec3(0.25f, 0.25f, 0.1875F);

    // ###################
    // Create Scene Meshes
    // ###################
    
    // Create Plane for scene with texture
    PlaneMesh tablePlane(VertexMode::POSITION_NORMAL_UV, UnitOfMeasure::CENTIMETER, programIds[POSITION_NORMAL_UV], 75.5f, 47.7f);
    tablePlane.setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    tablePlane.generateVertices();
    texFilename = "../resources/textures/table_top_1024x1024.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    tablePlane.addTextureID(gTextureId);
    texFilename = "../resources/textures/table_top_specular_1024x1024.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    tablePlane.addTextureID(gTextureId);
    tablePlane.generateVAO();
    sceneMeshes.push_back(&tablePlane);                

    // Create Mountain Dew Can for scene with texture
    SodaCanMesh mtnDewCan(VertexMode::POSITION_NORMAL_UV, UnitOfMeasure::CENTIMETER, programIds[POSITION_NORMAL_UV], 3.31f, 12.4f, 50);
    mtnDewCan.setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    mtnDewCan.generateVertices();
    mtnDewCan.translateMesh(22.0f, 0.0f, -13.0f);
    mtnDewCan.rotateMesh(0.0f, 120.0f, 0.0f, Mesh::DEFAULT_ROTATION_ORDER);
    texFilename = "../resources/textures/mtn_dew_zero_can_2048x2048.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    mtnDewCan.addTextureID(gTextureId);
    texFilename = "../resources/textures/mtn_dew_zero_can_specular_2048x2048.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    mtnDewCan.addTextureID(gTextureId);
    mtnDewCan.generateVAO();
    sceneMeshes.push_back(&mtnDewCan);    

    // Create PS5 Controller for scene with texture
    PS5ControllerMesh ps5Controller(VertexMode::POSITION_NORMAL_UV, UnitOfMeasure::CENTIMETER, programIds[POSITION_NORMAL_UV], 50);
    ps5Controller.setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    ps5Controller.generateVertices();
    ps5Controller.rotateMesh(0.0f, -2.0f, 0.0f);
    texFilename = "../resources/textures/ps5_black_controller_1536x1024.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    ps5Controller.addTextureID(gTextureId);
    texFilename = "../resources/textures/ps5_black_controller_specular_1536x1024.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    ps5Controller.addTextureID(gTextureId);
    ps5Controller.generateVAO();
    sceneMeshes.push_back(&ps5Controller);

    // Create Turtle Beach Headset for scene with texture
    HeadsetMesh turtleBeachHeadset(VertexMode::POSITION_NORMAL_UV, UnitOfMeasure::CENTIMETER, programIds[POSITION_NORMAL_UV], 50);
    turtleBeachHeadset.setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    turtleBeachHeadset.generateVertices();
    turtleBeachHeadset.translateMesh(-20.0f, 0.0f, -5.0f);
    turtleBeachHeadset.rotateMesh(0.0f, 35.0f, 0.0f);
    texFilename = "../resources/textures/turtlebeach_blue_headset_3584x2048.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    turtleBeachHeadset.addTextureID(gTextureId);
    texFilename = "../resources/textures/turtlebeach_blue_headset_specular_3584x2048.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    turtleBeachHeadset.addTextureID(gTextureId);
    turtleBeachHeadset.generateVAO();
    sceneMeshes.push_back(&turtleBeachHeadset);

    // Create Bear Back Scratcher for scene with texture
    BackScratcherMesh bearBackScratcher(VertexMode::POSITION_NORMAL_UV, UnitOfMeasure::CENTIMETER, programIds[POSITION_NORMAL_UV], 50);
    bearBackScratcher.setColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    bearBackScratcher.generateVertices();
    bearBackScratcher.translateMesh(0.0, 0.0f, 15.0f);
    bearBackScratcher.rotateMesh(0.0f, -2.0f, 0.0f);
    texFilename = "../resources/textures/back_scratcher_simple_512x256.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    bearBackScratcher.addTextureID(gTextureId);
    texFilename = "../resources/textures/back_scratcher_simple_specular_512x256.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    bearBackScratcher.addTextureID(gTextureId);
    bearBackScratcher.generateVAO();
    sceneMeshes.push_back(&bearBackScratcher);

    // Create a TV
    PlaneMesh tvPlane(VertexMode::POSITION_NORMAL_UV, UnitOfMeasure::CENTIMETER, programIds[POSITION_NORMAL_UV], 138.0f, 83.0f);
    tvPlane.translateMesh(-10.0f, 55.0f, -90.0f);
    tvPlane.rotateMesh(90.0f, 0.0f, 0.0f, Mesh::DEFAULT_ROTATION_ORDER);
    texFilename = "../resources/textures/z_tv_1024x1024.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    tvPlane.addTextureID(gTextureId);
    texFilename = "../resources/textures/z_tv_specular_1024x1024.png";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    tvPlane.addTextureID(gTextureId);
    tvPlane.generateVertices();
    tvPlane.generateVAO();
    sceneMeshes.push_back(&tvPlane);


    // #########################
    // Create Scene Point Lights
    // #########################

    std::vector<CubeLightMesh*>tvGlows;

    // Create a TV lights
    CubeLightMesh tvGlow(VertexMode::POSITION_COLOR, UnitOfMeasure::CENTIMETER, programIds[POSITION_COLOR], 2.0f, 2.0f, 2.0f, 100.0f, 2.0f, 10.0f, 0.5f);
    tvGlow.translateMesh(-45.0f, 75.0f, -90.0f);
    tvGlow.setColor(glm::vec4(0.5f, .5f, 1.0f, 1.0f));
    tvGlow.generateVertices();
    tvGlow.generateVAO();
    //sceneMeshes.push_back(&tvGlow);      // Turn off render to hide point light cube and leave lights only
    sceneMeshLights.push_back(&tvGlow);
    tvGlows.push_back(&tvGlow);

    CubeLightMesh tvGlow2(VertexMode::POSITION_COLOR, UnitOfMeasure::CENTIMETER, programIds[POSITION_COLOR], 2.0f, 2.0f, 2.0f, 100.0f, 2.0f, 10.0f, 0.5f);
    tvGlow2.translateMesh(25.0f, 75.0f, -90.0f);
    tvGlow2.setColor(glm::vec4(0.5f, .5f, 1.0f, 1.0f));
    tvGlow2.generateVertices();
    tvGlow2.generateVAO();
    //sceneMeshes.push_back(&tvGlow2);      // Turn off render to hide point light cube and leave lights only
    sceneMeshLights.push_back(&tvGlow2);
    tvGlows.push_back(&tvGlow2);

    // Create a TV lights
    CubeLightMesh tvGlow3(VertexMode::POSITION_COLOR, UnitOfMeasure::CENTIMETER, programIds[POSITION_COLOR], 2.0f, 2.0f, 2.0f, 100.0f, 2.0f, 10.0f, 0.5f);
    tvGlow3.translateMesh(-45.0f, 35.0f, -90.0f);
    tvGlow3.setColor(glm::vec4(0.5f, .5f, 1.0f, 1.0f));
    tvGlow3.generateVertices();
    tvGlow3.generateVAO();
    //sceneMeshes.push_back(&tvGlow3);      // Turn off render to hide point light cube and leave lights only
    sceneMeshLights.push_back(&tvGlow3);
    tvGlows.push_back(&tvGlow3);

    CubeLightMesh tvGlow4(VertexMode::POSITION_COLOR, UnitOfMeasure::CENTIMETER, programIds[POSITION_COLOR], 2.0f, 2.0f, 2.0f, 100.0f, 2.0f, 10.0f, 0.5f);
    tvGlow4.translateMesh(25.0f, 35.0f, -90.0f);
    tvGlow4.setColor(glm::vec4(0.5f, .5f, 1.0f, 1.0f));
    tvGlow4.generateVertices();
    tvGlow4.generateVAO();
    //sceneMeshes.push_back(&tvGlow4);      // Turn off render to hide point light cube and leave lights only
    sceneMeshLights.push_back(&tvGlow4);
    tvGlows.push_back(&tvGlow4);

    // Create an Off-White hallway light
    CubeLightMesh hallwayLight(VertexMode::POSITION_COLOR, UnitOfMeasure::CENTIMETER, programIds[POSITION_COLOR], 2.0f, 2.0f, 2.0f, 300.0f, 0.5f, 5.0f, 0.1f);
    hallwayLight.translateMesh(40.0f, 40.0f, 50.0f);
    hallwayLight.setColor(glm::vec4(1.0f, 1.0f, 0.9f, 1.0f));
    hallwayLight.generateVertices();
    hallwayLight.generateVAO();
    sceneMeshes.push_back(&hallwayLight);
    sceneMeshLights.push_back(&hallwayLight);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // input
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;
        gInput.processInput(gWindow, gDeltaTime);

        // Enable z-depth
        glEnable(GL_DEPTH_TEST);

        // Clear the frame and z buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render objects
        for (Mesh* mesh : sceneMeshes) {
            URenderMeshObject(*mesh);
        }

        // Random number generation for glowing effects
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> glowFadeDistribution(-15.0f, 20.0f);
        std::uniform_real_distribution<float> glowShiftDistribution(0.01f, 3.0f);
        std::uniform_real_distribution<float> colorDistribution(0.0f, 1.0f);

        // Fade light intensity in and out
        float tvGlowFadeAmount = glowFadeDistribution(gen) * gDeltaTime;

        // Shift colors around
        float tvGlowColorShiftAmount = glowShiftDistribution(gen) * gDeltaTime;
        float tvGlowColorShiftRedPercent = colorDistribution(gen);
        float tvGlowColorShiftGreenPercent = colorDistribution(gen);
        float tvGlowColorShiftBluePercent = colorDistribution(gen);

        for (CubeLightMesh* tvGlow : tvGlows) {
            tvGlow->glowFade(tvGlowFadeAmount, 0.0f, 10.0f);
            tvGlow->glowColorShift(tvGlowColorShiftAmount, tvGlowColorShiftRedPercent, tvGlowColorShiftGreenPercent, tvGlowColorShiftBluePercent);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
        glfwPollEvents();
    }

    // Release mesh data
    for (Mesh* mesh : sceneMeshes) {
        mesh->destroyMesh();
    }

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);


    glfwSetCursorPosCallback(*window, [](GLFWwindow* window, double xpos, double ypos) {
        gInput.mousePositionCallback(xpos, ypos);
        }
    );

    glfwSetScrollCallback(*window, [](GLFWwindow* window, double xoffset, double yoffset) {
        gInput.mouseScrollCallback(xoffset, yoffset);
        }
    );

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// Functioned called to render a frame
// TODO: setup renderer class to manage render process and modularize rendering options based on vertex mode
void URenderMeshObject(Mesh& mesh)
{
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = mesh.getModel();

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    glm::mat4 projection;

    // Creates a perspective projection
    if (gCamera.CameraProjectionMode == PERSPECTIVE)
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 300.0f);
    else
        projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 300.0f);

    // Set the shader to be used
    glUseProgram(mesh.getShaderProgramId());

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(mesh.getShaderProgramId(), "model");
    GLint viewLoc = glGetUniformLocation(mesh.getShaderProgramId(), "view");
    GLint projLoc = glGetUniformLocation(mesh.getShaderProgramId(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(mesh.getVAO());

    if (mesh.getVertexMode() == POSITION_UV) {
        GLint enableTextureOverlayLoc = glGetUniformLocation(mesh.getShaderProgramId(), "enableTextureOverlay");
        std::vector<GLuint> textureIds = mesh.getTextureIds();

        if (textureIds.size() == 1) {
            glUniform1i(enableTextureOverlayLoc, false);
        }
        else {
            glUniform1i(enableTextureOverlayLoc, true);
        }

        for (int i = 0; i < textureIds.size(); ++i) {
            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textureIds.at(i));
        }
    }
    else if (mesh.getVertexMode() == POSITION_NORMAL_UV) {
        // Temp Camera View Position
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "viewPos"),
            gCamera.Position.x, gCamera.Position.y, gCamera.Position.z
        );

        // Temp Material Settings
        // TODO: Make dynamic to Mesh
      glUniform1i(
            glGetUniformLocation(mesh.getShaderProgramId(), "material.diffuse"),
            0
        );
        glUniform1i(
            glGetUniformLocation(mesh.getShaderProgramId(), "material.specular"),
            1
        );
        glUniform1f(
            glGetUniformLocation(mesh.getShaderProgramId(), "material.shininess"),
            32.0f
        );


        // Temp Directional Light
        // TODO: Make a dynamic Directional Light Object
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "dirLight.direction"),
            directionalLight.direction.x, directionalLight.direction.y, directionalLight.direction.z
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "dirLight.ambient"),
            directionalLight.ambient.x, directionalLight.ambient.y, directionalLight.ambient.z
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "dirLight.diffuse"),
            directionalLight.diffuse.x, directionalLight.diffuse.y, directionalLight.diffuse.z
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "dirLight.specular"),
            directionalLight.specular.x, directionalLight.specular.y, directionalLight.specular.z
        );


        // Temp Point Lights
        // TODO: Make dynamic point light its own class to handle any mesh for the visual indicator
        // not just a cube
        std::vector<CubeLightMesh*> meshLights = sceneMeshLights;

        glUniform1i(
            glGetUniformLocation(mesh.getShaderProgramId(), "pointLightCount"),
            meshLights.size()
        );

        for (size_t i = 0; i < meshLights.size(); ++i) {
            CubeLightMesh& meshLight = *meshLights.at(i);
            glm::vec3 lightPosition = glm::vec3(meshLight.getTranslation()[3]);
            glm::vec4 lightColor = meshLight.getColor();
            float ambientStrength = meshLight.getAmbientStrength();
            float diffuseStrength = meshLight.getDiffuseStrength();
            float specularStrength = meshLight.getSpecularStrength();
            float constant = meshLight.getAttenuationConstant();
            float linear = meshLight.getAttenuationLinear();
            float quadratic = meshLight.getAttenuationQuadratic();
            std::string index = std::to_string(i);

            glUniform3f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].position").c_str()),
                lightPosition.x, lightPosition.y, lightPosition.z
            );
            glUniform3f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].color").c_str()),
                lightColor.r, lightColor.g, lightColor.b
            );
            glUniform1f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].ambientStrength").c_str()),
                ambientStrength
            );
            glUniform1f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].diffuseStrength").c_str()),
                diffuseStrength
            );
            glUniform1f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].specularStrength").c_str()),
                specularStrength
            );
            glUniform1f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].constant").c_str()),
                constant
            );
            glUniform1f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].linear").c_str()),
                linear
            );
            glUniform1f(
                glGetUniformLocation(mesh.getShaderProgramId(), ("pointLights[" + index + "].quadratic").c_str()),
                quadratic
            );
        }


        // Temp Spot Light - Camera Light
        // TODO: Make dynamic to any spot light
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.position"),
            gCamera.Position.x, gCamera.Position.y, gCamera.Position.z
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.direction"),
            gCamera.Front.x, gCamera.Front.y, gCamera.Front.z
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.ambient"),
            0.0f, 0.0f, 0.0f
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.diffuse"),
            1.0f, 1.0f, 1.0f
        );
        glUniform3f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.specular"),
            1.0f, 1.0f, 1.0f
        );
        glUniform1f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.constant"),
            1.0f
        );
        glUniform1f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.linear"),
            0.09f
        );
        glUniform1f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.quadratic"),
            0.032f
        );
        glUniform1f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.cutOff"),
            glm::cos(glm::radians(12.5f))
        );
        glUniform1f(
            glGetUniformLocation(mesh.getShaderProgramId(), "spotLight.outerCutOff"),
            glm::cos(glm::radians(15.0f))
        );

        // Texture maps
        std::vector<GLuint> textureIds = mesh.getTextureIds();

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIds.at(0));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureIds.at(1));
    }

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, mesh.getElementBuffer().size(), GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object and texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

