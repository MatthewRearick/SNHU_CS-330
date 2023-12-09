#include "InputHandler.h"
#include <iostream>


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


InputHandler::InputHandler(Camera& camera) : camera(camera) {}


// #################
// # Other methods #
// #################


void InputHandler::processInput(GLFWwindow* window, float deltaTime) 
{
    // Hardcoded base camera speed (will be converted to dynamic at a later time)
    static const float cameraSpeed = 50.0f;

    // Check for escape for quit action
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Check for keyboard movement inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);

    // Check for P key to be pressed, wait until release before repeating
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!pKeyPressed) {
            camera.ToggleProjectionMode();
            pKeyPressed = true;  // Set the flag to indicate 'P' was pressed
        }
    }
    else {
        pKeyPressed = false; // Reset the flag when 'P' is released
    }
}

void InputHandler::mousePositionCallback(double xpos, double ypos) 
{
    // Set inital position if it is the first call
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    // Get mouse movement offsets
    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    // Update positions
    gLastX = xpos;
    gLastY = ypos;

    // Pass mouse offsets to camera for processing
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void InputHandler::mouseScrollCallback(double xoffset, double yoffset) 
{
    // Pass mousewheel y offset to camera for processing
    camera.ProcessMouseScroll(yoffset);
}
