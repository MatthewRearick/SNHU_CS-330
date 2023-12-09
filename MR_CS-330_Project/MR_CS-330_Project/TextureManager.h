//// TextureManager.h
//#pragma once
//
//#include <iostream>         // cout, cerr
//#include <cstdlib>          // EXIT_FAILURE
//#include <GL/glew.h>
//
//#define STB_IMAGE_IMPLEMENTATION    
//#include <stb_image.h>
//
///**
// * Class representing a CubeMesh object with transformations.
// */
//class TextureManager {
//public:
//    // ######################
//    // # Singleton Instance #
//    // ######################
//
//
//    /**
//     * Get the Texture Manager Singleton Instance     *
//     */
//    static TextureManager& getInstance();
//
//    // Prevent copying and assignment
//    TextureManager(const TextureManager&) = delete;
//    void operator=(const TextureManager&) = delete;
//
//
//    // #################
//    // # Other methods #
//    // #################
//
//    // 
//    bool createTexture(const char* filename, GLuint& textureId);
//
//private:
//    // ################
//    // # Constructors #
//    // ################
//
//
//    /**
//     * TextureManager constructor.
//     */
//    TextureManager();
//
//
//    // #################
//    // # Other methods #
//    // #################
//
//
//    // Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
//    void flipImageVertically(unsigned char* image, int width, int height, int channels);
//};
