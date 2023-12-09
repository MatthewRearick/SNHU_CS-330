#include "FrustumMesh.h"


// ##################
// #                #
// # Public methods #
// #                #
// ##################


// ################
// # Constructors #
// ################


FrustumMesh::FrustumMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, glm::vec3 scale, glm::vec3 rotationDegrees, glm::vec3 translation, RotationOrder rotationOrder, GLuint shaderProgramId, float topRadius, float bottomRadius, float height, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, scale, rotationDegrees, translation, rotationOrder, shaderProgramId), topRadius(topRadius), bottomRadius(bottomRadius), height(height), sectorCount(sectorCount)
{
}

FrustumMesh::FrustumMesh(VertexMode vertexMode, UnitOfMeasure unitOfMeasure, GLuint shaderProgramId, float topRadius, float bottomRadius, float height, int sectorCount)
    : Mesh(vertexMode, unitOfMeasure, shaderProgramId), topRadius(topRadius), bottomRadius(bottomRadius), height(height), sectorCount(sectorCount)
{
}


// #################
// # Other methods #
// #################



void FrustumMesh::generateVertices()
{
    // Initialize color for color modes
    if (color == DEFAULT_COLOR) {
        color = getRandomColor();
    }
    float r = color.r;
    float g = color.g;
    float b = color.b;
    float a = color.a;

    // Define relative offsets from width, height, and length
    float top = height / 2.0f;
    float bottom = -top;

    if (vertexMode == POSITION_COLOR) {
        int floatsPerVertexColor = floatsPerVertex + floatsPerColor;

        // Random number variable
        glm::vec4 color;

        // Reserve space for each vertex coordinate and color value for the top and bottom 
        // center points as well as all sector points on the top and bottom
        int vertexBufferResize = (sectorCount * 2 + 2) * floatsPerVertexColor;
        vertexBuffer.resize(vertexBufferResize);

        // Reserve space for each vertex coordinate index for the top, bottom, upper side, 
        // and lower side triangles, 3 vertices per triangle, 4 triagnles per sector
        int elementBufferResize = 3 * 4 * sectorCount;
        elementBuffer.resize(elementBufferResize);

        // Indices for center vertices
        int topCenterIndex = 0;
        int bottomCenterIndex = sectorCount + 1;

        // Top center vertex
        // Set vertex buffer x, y, and z for current sector
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor) = 0.0f;
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor + 1) = top;
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor + 2) = 0.0f;
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor + 3) = r;
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor + 4) = g;
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor + 5) = b;
        vertexBuffer.at(topCenterIndex * floatsPerVertexColor + 6) = a;

        // Bottom center vertex
        // Set vertex buffer x, y, and z for current sector
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor) = 0.0f;
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor + 1) = bottom;
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor + 2) = 0.0f;
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor + 3) = r;
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor + 4) = g;
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor + 5) = b;
        vertexBuffer.at(bottomCenterIndex * floatsPerVertexColor + 6) = a;

        for (int i = 0; i < sectorCount; ++i) {
            // Calculate vertex coordinate for sector
            glm::vec3 topVertex = calculateSectorVertex(i, topRadius);
            glm::vec3 bottomVertex = calculateSectorVertex(i, bottomRadius);

            // Top circle vertices
            // Set vertex buffer x, y, and z for current sector
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor) = topVertex.x;
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor + 1) = top;
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor + 2) = topVertex.z;
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor + 3) = r;
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor + 4) = g;
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor + 5) = b;
            vertexBuffer.at((topCenterIndex + i + 1) * floatsPerVertexColor + 6) = a;

            // Bottom circle vertices
            // Set vertex buffer x, y, and z for current sector
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor) = bottomVertex.x;
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor + 1) = bottom;
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor + 2) = bottomVertex.z;
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor + 3) = r;
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor + 4) = g;
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor + 5) = b;
            vertexBuffer.at((bottomCenterIndex + i + 1) * floatsPerVertexColor + 6) = a;

            int topIndex;
            int nextTopIndex;
            int bottomIndex;
            int nextBottomIndex;

            if (i == 0) {
                topIndex = sectorCount;
                nextTopIndex = topCenterIndex + 1;
                bottomIndex = sectorCount * 2 + 1;
                nextBottomIndex = bottomCenterIndex + 1;
            }
            else {
                topIndex = i;
                nextTopIndex = i + 1;
                bottomIndex = i + sectorCount + 1;
                nextBottomIndex = bottomCenterIndex + i + 1;
            }

            // index offset for index sections
            // Number of sectors * number of sectors indexed * number of indices per trianble + 1 for the next value + 1 for the 0 index
            int offset1 = sectorCount * 1 * 3;
            int offset2 = sectorCount * 2 * 3;
            int offset3 = sectorCount * 3 * 3;

            // Top circle indices
            elementBuffer.at(i * 3) = topCenterIndex;
            elementBuffer.at(i * 3 + 1) = topIndex;
            elementBuffer.at(i * 3 + 2) = nextTopIndex;

            // Top side indices
            elementBuffer.at(i * 3 + offset1) = topIndex;
            elementBuffer.at(i * 3 + 1 + offset1) = nextTopIndex;
            elementBuffer.at(i * 3 + 2 + offset1) = bottomIndex;

            // Bottom side indices
            elementBuffer.at(i * 3 + offset2) = bottomIndex;
            elementBuffer.at(i * 3 + 1 + offset2) = nextBottomIndex;
            elementBuffer.at(i * 3 + 2 + offset2) = nextTopIndex;

            // Bottom circle indices
            elementBuffer.at(i * 3 + offset3) = bottomCenterIndex;
            elementBuffer.at(i * 3 + 1 + offset3) = bottomIndex;
            elementBuffer.at(i * 3 + 2 + offset3) = nextBottomIndex;
        }
    }
    else if (vertexMode == POSITION_UV) {
        // Define relative offsets from width, height, and length
        int floatsPerVertexUV = floatsPerVertex + floatsPerUV;

        // Reserve space for each vertex coordinate and UV value for the top and bottom 
        // Each triangle will utilize its own cooridinates in order to specify unique UV mapping
        // Meaning vertex coordinates will repeate rather than being shared between joining triangles
        // Each sector will be made up of 5 triangles (one top, three sides, one bottom)
        int vertexBufferResize = 3 * 5 * sectorCount * floatsPerVertexUV;
        vertexBuffer.resize(vertexBufferResize);

        // Reserve space for each vertex coordinate index for the top, bottom, upper side, 
        // and lower side triangles, 3 vertices per triangle, 4 triagnles per sector
        int elementBufferResize = 3 * 5 * sectorCount;
        elementBuffer.resize(elementBufferResize);

        // Indices for triangle index offsets
        int topIndexOffset = 3 * 0 * sectorCount * floatsPerVertexUV + 0;
        int leftSideIndexOffset = 3 * 1 * sectorCount * floatsPerVertexUV + 0;
        int middleSideIndexOffset = 3 * 2 * sectorCount * floatsPerVertexUV + 0;
        int rightSideIndexOffset = 3 * 3 * sectorCount * floatsPerVertexUV + 0;
        int bottomIndexOffset = 3 * 4 * sectorCount * floatsPerVertexUV + 0;

        for (int i = 0; i < sectorCount; ++i) {
            // Calculate vertex coordinate for sector
            glm::vec3 topVertex = calculateSectorVertex(i, topRadius);
            glm::vec3 nextTopVertex = calculateSectorVertex(i + 1, topRadius);
            glm::vec3 bottomVertex = calculateSectorVertex(i, bottomRadius);
            glm::vec3 nextBottomVertex = calculateSectorVertex(i + 1, bottomRadius);
            glm::vec3 middleBottomVertex = (bottomVertex + nextBottomVertex) * 0.5f;

            // Get angles in reverse order due to top being inside out
            glm::vec2 topUV = calculateSectorUV(sectorCount - i, true);
            glm::vec2 nextTopUV = calculateSectorUV(sectorCount - i - 1, true);

            GLfloat sideU = static_cast<float>(sectorCount - i) / static_cast<float>(sectorCount);
            GLfloat nextSideU = static_cast<float>(sectorCount - i - 1) / static_cast<float>(sectorCount);
            GLfloat middleSideU = (sideU + nextSideU) * 0.5f;

            glm::vec2 bottomUV = calculateSectorUV(i, false);
            glm::vec2 nextBottomUV = calculateSectorUV(i + 1, false);

            // Top circle vertices and texture uv
            // Vertex 1 - Center
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 0) = 0.0f;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 1) = top;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 2) = 0.0f;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 3) = clampU(0.25f);
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 4) = clampV(0.75f);

            // Vertex 2 - Top
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 5) = topVertex.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 6) = top;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 7) = topVertex.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 8) = clampU(topUV.x);
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 9) = clampV(topUV.y);

            // Vertex 3 - Next Top
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 10) = nextTopVertex.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 11) = top;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 12) = nextTopVertex.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 13) = clampU(nextTopUV.x);
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexUV * 3 + 14) = clampV(nextTopUV.y);


            // Left side (lower left triangle) vertices and texture uv
            // Vertex 1 - Top
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 0) = topVertex.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 1) = top;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 2) = topVertex.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 3) = clampU(sideU);
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 4) = clampV(0.5f);

            // Vertex 2 - Middle Bottom
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 5) = middleBottomVertex.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 6) = bottom;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 7) = middleBottomVertex.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 8) = clampU(middleSideU);
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 9) = clampV(0.0f);

            // Vertex 2 - Bottom
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 10) = bottomVertex.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 11) = bottom;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 12) = bottomVertex.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 13) = clampU(sideU);
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexUV * 3 + 14) = clampV(0.0f);


            // Top side (upper middle triangle) vertices and texture uv
            // Vertex 1 - Top
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 0) = topVertex.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 1) = top;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 2) = topVertex.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 3) = clampU(sideU);
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 4) = clampV(0.5f);

            // Vertex 2 - Middle Bottom
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 5) = middleBottomVertex.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 6) = bottom;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 7) = middleBottomVertex.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 8) = clampU(middleSideU);
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 9) = clampV(0.0f);

            // Vertex 3 - Next Top
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 10) = nextTopVertex.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 11) = top;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 12) = nextTopVertex.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 13) = clampU(nextSideU);
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexUV * 3 + 14) = clampV(0.5f);


            // Right side (lower right triangle) vertices and texture uv
            // Vertex 1 - Next Top
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 0) = nextTopVertex.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 1) = top;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 2) = nextTopVertex.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 3) = clampU(nextSideU);
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 4) = clampV(0.5f);

            // Vertex 2 - Middle Bottom
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 5) = middleBottomVertex.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 6) = bottom;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 7) = middleBottomVertex.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 8) = clampU(middleSideU);
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 9) = clampV(0.0f);

            // Vertex 2 - Next Bottom
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 10) = nextBottomVertex.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 11) = bottom;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 12) = nextBottomVertex.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 13) = clampU(nextSideU);
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexUV * 3 + 14) = clampV(0.0f);


            // Bottom circle vertices and texture uv
            // Vertex 1 - Center
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 0) = 0.0f;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 1) = bottom;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 2) = 0.0f;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 3) = clampU(0.75f);
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 4) = clampV(0.75f);

            // Vertex 2 - Sector
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 5) = bottomVertex.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 6) = bottom;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 7) = bottomVertex.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 8) = clampU(bottomUV.x);
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 9) = clampV(bottomUV.y);

            // Vertex 3 - Next Sector
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 10) = nextBottomVertex.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 11) = bottom;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 12) = nextBottomVertex.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 13) = clampU(nextBottomUV.x);
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexUV * 3 + 14) = clampV(nextBottomUV.y);
        }

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }


    else if (vertexMode == POSITION_NORMAL_UV) {
        int floatsPerVertexNormalUV = floatsPerVertex + floatsPerNormal + floatsPerUV;

        // Reserve space for each vertex coordinate and UV value for the top and bottom 
        // Each triangle will utilize its own cooridinates in order to specify unique UV mapping
        // Meaning vertex coordinates will repeate rather than being shared between joining triangles
        // Each sector will be made up of 5 triangles (one top, three sides, one bottom)
        int vertexBufferResize = 3 * 5 * sectorCount * floatsPerVertexNormalUV;
        vertexBuffer.resize(vertexBufferResize);

        // Reserve space for each vertex coordinate index for the top, bottom, upper side, 
        // and lower side triangles, 3 vertices per triangle, 4 triagnles per sector
        int elementBufferResize = 3 * 5 * sectorCount;
        elementBuffer.resize(elementBufferResize);

        // Indices for triangle index offsets
        int topIndexOffset = 3 * 0 * sectorCount * floatsPerVertexNormalUV + 0;
        int leftSideIndexOffset = 3 * 1 * sectorCount * floatsPerVertexNormalUV + 0;
        int middleSideIndexOffset = 3 * 2 * sectorCount * floatsPerVertexNormalUV + 0;
        int rightSideIndexOffset = 3 * 3 * sectorCount * floatsPerVertexNormalUV + 0;
        int bottomIndexOffset = 3 * 4 * sectorCount * floatsPerVertexNormalUV + 0;

        glm::vec3 normal;

        for (int i = 0; i < sectorCount; ++i) {
            // Calculate vertex coordinate for sector
            glm::vec3 topCenterVertex = glm::vec3(0.0f, top, 0.0f);
            glm::vec3 topVertex = calculateSectorVertex(i, topRadius);
            glm::vec3 nextTopVertex = calculateSectorVertex(i + 1, topRadius);
            glm::vec3 bottomVertex = calculateSectorVertex(i, bottomRadius);
            glm::vec3 bottomCenterVertex = glm::vec3(0.0f, bottom, 0.0f);
            glm::vec3 nextBottomVertex = calculateSectorVertex(i + 1, bottomRadius);
            glm::vec3 middleBottomVertex = (bottomVertex + nextBottomVertex) * 0.5f;

            topVertex.y = top;
            nextTopVertex.y = top;
            bottomVertex.y = bottom;
            nextBottomVertex.y = bottom;
            middleBottomVertex.y = bottom;

            // Get angles in reverse order due to top being inside out
            glm::vec2 topUV = calculateSectorUV(sectorCount - i, true);
            glm::vec2 nextTopUV = calculateSectorUV(sectorCount - i - 1, true);

            GLfloat sideU = static_cast<float>(sectorCount - i) / static_cast<float>(sectorCount);
            GLfloat nextSideU = static_cast<float>(sectorCount - i - 1) / static_cast<float>(sectorCount);
            GLfloat middleSideU = (sideU + nextSideU) * 0.5f;

            glm::vec2 bottomUV = calculateSectorUV(i, false);
            glm::vec2 nextBottomUV = calculateSectorUV(i + 1, false);


            // Top circle vertices and texture uv
            normal = calculateNormal(topCenterVertex, topVertex, nextTopVertex);

            // Vertex 1 - Center
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 0) = topCenterVertex.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 1) = topCenterVertex.y;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 2) = topCenterVertex.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 3) = normal.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 4) = normal.y;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 5) = normal.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 6) = clampU(0.25f);
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 7) = clampV(0.75f);

            // Vertex 2 - Top
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 8) = topVertex.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 9) = topVertex.y;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 10) = topVertex.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 11) = normal.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 12) = normal.y;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 13) = normal.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 14) = clampU(topUV.x);
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 15) = clampV(topUV.y);

            // Vertex 3 - Next Top
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 16) = nextTopVertex.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 17) = nextTopVertex.y;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 18) = nextTopVertex.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 19) = normal.x;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 20) = normal.y;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 21) = normal.z;
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 22) = clampU(nextTopUV.x);
            vertexBuffer.at(topIndexOffset + i * floatsPerVertexNormalUV * 3 + 23) = clampV(nextTopUV.y);


            // Left side (lower left triangle) vertices and texture uv
            normal = calculateNormal(topVertex, middleBottomVertex, bottomVertex);
            // Vertex 1 - Top
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 0) = topVertex.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 1) = topVertex.y;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 2) = topVertex.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 3) = normal.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 4) = normal.y;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 5) = normal.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 6) = clampU(sideU);
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 7) = clampV(0.5f);

            // Vertex 2 - Middle Bottom
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 8) = middleBottomVertex.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 9) = middleBottomVertex.y;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 10) = middleBottomVertex.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 11) = normal.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 12) = normal.y;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 13) = normal.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 14) = clampU(middleSideU);
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 15) = clampV(0.0f);

            // Vertex 2 - Bottom
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 16) = bottomVertex.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 17) = bottomVertex.y;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 18) = bottomVertex.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 19) = normal.x;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 20) = normal.y;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 21) = normal.z;
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 22) = clampU(sideU);
            vertexBuffer.at(leftSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 23) = clampV(0.0f);


            // Top side (upper middle triangle) vertices and texture uv
            normal = calculateNormal(topVertex, middleBottomVertex, nextTopVertex);
            // Vertex 1 - Top
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 0) = topVertex.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 1) = topVertex.y;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 2) = topVertex.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 3) = normal.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 4) = normal.y;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 5) = normal.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 6) = clampU(sideU);
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 7) = clampV(0.5f);

            // Vertex 2 - Middle Bottom
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 8) = middleBottomVertex.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 9) = middleBottomVertex.y;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 10) = middleBottomVertex.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 11) = normal.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 12) = normal.y;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 13) = normal.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 14) = clampU(middleSideU);
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 15) = clampV(0.0f);

            // Vertex 3 - Next Top
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 16) = nextTopVertex.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 17) = nextTopVertex.y;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 18) = nextTopVertex.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 19) = normal.x;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 20) = normal.y;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 21) = normal.z;
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 22) = clampU(nextSideU);
            vertexBuffer.at(middleSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 23) = clampV(0.5f);


            // Right side (lower right triangle) vertices and texture uv
            normal = calculateNormal(nextTopVertex, middleBottomVertex, nextBottomVertex);
            // Vertex 1 - Next Top
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 0) = nextTopVertex.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 1) = nextTopVertex.y;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 2) = nextTopVertex.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 3) = normal.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 4) = normal.y;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 5) = normal.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 6) = clampU(nextSideU);
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 7) = clampV(0.5f);
            
            // Vertex 2 - Middle Bottom
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 8) = middleBottomVertex.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 9) = middleBottomVertex.y;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 10) = middleBottomVertex.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 11) = normal.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 12) = normal.y;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 13) = normal.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 14) = clampU(middleSideU);
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 15) = clampV(0.0f);

            // Vertex 2 - Next Bottom
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 16) = nextBottomVertex.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 17) = nextBottomVertex.y;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 18) = nextBottomVertex.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 19) = normal.x;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 20) = normal.y;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 21) = normal.z;
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 22) = clampU(nextSideU);
            vertexBuffer.at(rightSideIndexOffset + i * floatsPerVertexNormalUV * 3 + 23) = clampV(0.0f);


            // Bottom circle vertices and texture uv
            normal = calculateNormal(bottomCenterVertex, bottomVertex, nextBottomVertex);
            // Vertex 1 - Center
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 0) = bottomCenterVertex.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 1) = bottomCenterVertex.y;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 2) = bottomCenterVertex.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 3) = normal.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 4) = normal.y;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 5) = normal.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 6) = clampU(0.75f);
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 7) = clampV(0.75f);

            // Vertex 2 - Sector
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 8) = bottomVertex.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 9) = bottomVertex.y;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 10) = bottomVertex.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 11) = normal.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 12) = normal.y;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 13) = normal.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 14) = clampU(bottomUV.x);
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 15) = clampV(bottomUV.y);

            // Vertex 3 - Next Sector
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 16) = nextBottomVertex.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 17) = nextBottomVertex.y;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 18) = nextBottomVertex.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 19) = normal.x;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 20) = normal.y;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 21) = normal.z;
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 22) = clampU(nextBottomUV.x);
            vertexBuffer.at(bottomIndexOffset + i * floatsPerVertexNormalUV * 3 + 23) = clampV(nextBottomUV.y);
        }

        for (size_t i = 0; i < elementBuffer.size(); ++i) {
            elementBuffer.at(i) = static_cast<unsigned int>(i);
        }
    }
    else {
        throw "Vertex mode not implemented yet.";
    }
}

const glm::vec3 FrustumMesh::calculateSectorVertex(int SectorIndex, float radius)
{
    // Calculation for sector angle in radians
    float sectorAngle = static_cast<float>(SectorIndex) / sectorCount * 2.0f * glm::pi<float>();

    // Calculations for x and z based on sector angle (y is filled with a default value)
    float x = radius * glm::cos(sectorAngle);
    float y = 0.0f;
    float z = radius * glm::sin(sectorAngle);

    return glm::vec3(x, y, z);
}

const glm::vec2 FrustumMesh::calculateSectorUV(int SectorIndex, bool topCircle)
{
    float uOffset;
    float vOffset;
    float uvRadius = 0.25f;
    float rotationOffset;

    if (topCircle) {
        uOffset = 0.25f;
        rotationOffset = (glm::pi<float>() * 0.5f);
    }
    else {
        uOffset = 0.75f;
        rotationOffset = (glm::pi<float>() * -0.5f);
    }

    vOffset = 0.75f;

    // Calculation for sector angle in radians
    float sectorAngle = static_cast<float>(SectorIndex) / sectorCount * 2.0f * glm::pi<float>() + rotationOffset;

    // Calculations for x and z based on sector angle (y is filled with a default value)
    float u = uvRadius * glm::cos(sectorAngle) + uOffset;
    float v = uvRadius * glm::sin(sectorAngle) + vOffset;

    return glm::vec2(u, v);
}
