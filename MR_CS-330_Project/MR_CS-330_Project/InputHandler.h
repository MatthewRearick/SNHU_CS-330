// InputHandler.h
#pragma once

#include <GLFW/glfw3.h>
#include "camera.h"

/**
 * Class representing a CubeMesh object with transformations.
 */
class InputHandler {
public:
    // ################
    // # Constructors #
    // ################


    /**
     * InputHandler constructor.
     *
     * @param camera Reference to Camera instance.
     */
    InputHandler(Camera& camera);

    /**
     * Set the mesh rotation in degrees per axis.
     *
     * @param window Rotation Reference to GLFWindow instance.
     * @param deltaTime Time delta since last call.
     */
    void processInput(GLFWwindow* window, float deltaTime);

    /**
     * Set the mesh rotation in degrees per axis.
     *
     * @param xpos X position of mouse.
     * @param ypos Y position of mouse.
     */
    void mousePositionCallback(double xpos, double ypos);

    /**
     * Set the mesh rotation in degrees per axis.
     *
     * @param xoffset X offset of mousewheel movement.
     * @param yoffset Y offset of mousewheel movement.
     */
    void mouseScrollCallback(double xoffset, double yoffset);

private:

    // #############
    // # Variables #
    // #############

    Camera& camera;             // Reference to Camera instance
    bool pKeyPressed = false;   // "P" keypress toggle
    float gLastX = 0.0f;        // Last mouse x position
    float gLastY = 0.0f;        // Last mouse y position
    bool gFirstMouse = true;    // Initial mouse movement flag
};
