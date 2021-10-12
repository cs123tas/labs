#include "glfragwidget.h"

#include "settings.h"

#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>
#include "glwidget.h"
#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <math.h>
#include <iostream>
#include "sphere.h"

GLFragWidget::GLFragWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_square(nullptr),
      m_solidProgramID(0),
      m_gradientProgramID(0),
      m_textureProgramID(0),
      m_textureID(0),
      m_program(0),
      m_sphere(nullptr),
      m_timer(this),
      m_fps(60.0f),
      m_increment(0)
{
    // Set up 60 FPS draw loop.
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_timer.start(1000.0f / m_fps);
    m_tick = 1.0;
    m_angle = 0.f;
}

GLFragWidget::~GLFragWidget()
{
    // TODO: Clean up GPU memory. (Task 15)
    // begin ta solution
    glDeleteTextures(1, &m_textureID);
    // end ta solution
}


void GLFragWidget::initializeGL() {
    initializeGLFragmentShaders();
}

void GLFragWidget::initializeGLFragmentShaders() {
    ResourceLoader::initializeGlew();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    // Creates the three shader programs.
    m_solidProgramID = ResourceLoader::createShaderProgram(":/shaders/solid.vert", ":/shaders/solid.frag");
    m_gradientProgramID = ResourceLoader::createShaderProgram(":/shaders/gradient.vert", ":/shaders/gradient.frag");
    m_textureProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/texture.frag");

    // Smart pointer!
    m_square = std::make_unique<OpenGLShape>();


    // TODO: Interleave positions and colors in the array used to intialize m_square (Task 11)
    // TODO: Interleave UV-coordinates along with positions and colors in your VBO (Task 15)
    std::vector<float> coordinates = {-.5, .5, 0,
                                       -.5, -.5, 0,
                                      .5, .5, 0,
                                      .5, -.5, 0};
    // TODO: update the stride (last argument to setVertexData) when adding info to square (Task 11 & 15)
    m_square->setVertexData(&coordinates[0], coordinates.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, coordinates.size() / 3);
    m_square->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);

    // TODO: Don't forget to add the color attribute similar to how you do for the position above (Task 11)

    m_square->buildVAO();

    // TODO: Initialize texture map.  Follow the handout for specific instructions. (Task 14)
    QImage image(":/images/ostrich.jpg");

}

void GLFragWidget::paintGLFragmentShaders() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (settings.shaderProgram) {
        case SOLID_SHADER_PROGRAM:
            // TODO: Use m_solidProgramID as the program. (Task 9)

            // TODO: Set the uniform's value to a color other than white. (Task 10)

            // TODO: Draw the square, and then unbind the program. (Task 9)

            break;
        case GRADIENT_SHADER_PROGRAM:
            // TODO: Draw the square using m_gradientProgramID. (Task 13)

            // TODO: Draw the square, and then unbind the program. (Task 13)

            break;
        case TEXTURE_SHADER_PROGRAM:
            // TODO: Use m_textureProgramID. (Task 16)


            // TODO: Bind the texture. (Task 16)


            // TODO: Draw the square and unbind the program. (Task 16)

            break;
    }

}

/** Repaints the canvas. Called 60 times per second. */
void GLFragWidget::tick()
{
    m_tick += 1;

    m_angle += M_PI / 60.f;

    if(m_angle >= 2 * M_PI) {
        m_angle = 0.f;
    }

    update();
}

void GLFragWidget::paintGL() {
    paintGLFragmentShaders();
}

void GLFragWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

