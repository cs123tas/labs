#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "openglshape.h"

class Terrain {
public:
    Terrain();

    void init(GLuint m_program);
    void draw();

private:
    float randValue(int row, int col);
    glm::vec3 getPosition(int row, int col);
    glm::vec3 getNormal(int row, int col);
    float getNoiseValue(int row, int col, float octave,float frequency, float persistence);
    glm::vec3 generate_biome(glm::vec3 position, int row, int col);
    std::unique_ptr<OpenGLShape> m_shape,m_square;
    const float m_numRows, m_numCols;
};

#endif // TERRAIN_H
