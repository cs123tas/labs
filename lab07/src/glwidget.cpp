#include "glwidget.h"

#include "cs123_lib/resourceloader.h"
#include "cs123_lib/errorchecker.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include "settings.h"

#include "openglshape.h"
#include "gl/textures/Texture2D.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "sphere.h"

using namespace CS123::GL;

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_width(width()), m_height(height()),
      m_phongProgram(0), m_textureProgram(0), m_horizontalBlurProgram(0), m_verticalBlurProgram(0),
      m_quad(nullptr), m_sphere(nullptr),
      m_blurFBO1(nullptr), m_blurFBO2(nullptr),
      m_particlesFBO1(nullptr), m_particlesFBO2(nullptr),
      m_firstPass(true), m_evenPass(true), m_numParticles(5000),
      m_angleX(-0.5f), m_angleY(0.5f), m_zoom(4.f)
{
}

GLWidget::~GLWidget()
{
    glDeleteVertexArrays(1, &m_particlesVAO);
}

void GLWidget::initializeGL() {
    ResourceLoader::initializeGlew();
    glEnable(GL_DEPTH_TEST);

    // Set the color to set the screen when the color buffer is cleared.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create shader programs.
    m_phongProgram = ResourceLoader::createShaderProgram(
                ":/shaders/phong.vert", ":/shaders/phong.frag");
    m_textureProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/texture.frag");
    m_horizontalBlurProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/horizontalBlur.frag");
    m_verticalBlurProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/verticalBlur.frag");
    m_particleUpdateProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/particles_update.frag");
    m_particleDrawProgram = ResourceLoader::createShaderProgram(
                ":/shaders/particles_draw.vert", ":/shaders/particles_draw.frag");

    // Initialize sphere OpenGLShape.
    std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;
    m_sphere = std::make_unique<OpenGLShape>();
    m_sphere->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->setAttribute(ShaderAttrib::NORMAL, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->buildVAO();

    // TODO: [Task 6] Fill in the positions and UV coordinates to draw a fullscreen quad
    // We've already set the vertex attributes for you, so be sure to follow those specifications
    // (triangle strip, 4 vertices, position followed by UVs)

    std::vector<GLfloat> quadData ={
        -1,1,0,0, 1,
        -1,-1,0, 0,0,
        1,1,0, 1,1,
        1,-1,0,1,0


//        1,1,0,1,1,
//        -1,1,0,1,-1
    };
    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    // We will use this VAO to draw our particles' triangles.
    // It doesn't need any data associated with it, so we don't have to make a full VAO instance
    glGenVertexArrays(1, &m_particlesVAO);
    // TODO [Task 13] Create m_particlesFBO1 and 2 with std::make_shared
    m_particlesFBO1 = std::make_unique<FBO>(2,FBO::DEPTH_STENCIL_ATTACHMENT::NONE,m_numParticles,1,TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,TextureParameters
                                            ::FILTER_METHOD::NEAREST,GL_FLOAT);
    m_particlesFBO2 = std::make_unique<FBO>(2,FBO::DEPTH_STENCIL_ATTACHMENT::NONE,m_numParticles,1,TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,TextureParameters
                                            ::FILTER_METHOD::NEAREST,GL_FLOAT);
    GLint firstPassloc = glGetUniformLocation(m_particleUpdateProgram,"firstPass");
    GLint numloc = glGetUniformLocation(m_particleUpdateProgram,"numParticles");
    GLint posloc = glGetUniformLocation(m_particleUpdateProgram,"prevPos");
    GLint velloc = glGetUniformLocation(m_particleUpdateProgram,"prevVel");
    glUniform1f(firstPassloc,m_firstPass);
    glUniform1i(numloc,m_numParticles);
    glUniform1i(posloc,0);
    glUniform1i(velloc,1);
    m_quad->draw();
    // Print the max FBO dimension.
    GLint maxRenderBufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &maxRenderBufferSize);
    std::cout << "Max FBO size: " << maxRenderBufferSize << std::endl;
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    switch (settings.mode) {
        case MODE_BLUR:
            drawBlur();
            break;
        case MODE_PARTICLES:
            drawParticles();
            update();
            break;
    }
}

void GLWidget::drawBlur() {
    m_blurFBO1->bind();
    // TODO: [Task 1] Do drawing here!
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_phongProgram);
    GLint uniformLocView = glGetUniformLocation(m_phongProgram, "view");
    GLint uniformLocProj = glGetUniformLocation(m_phongProgram, "projection");
    GLint uniformLoc = glGetUniformLocation(m_phongProgram, "model");
   glUniformMatrix4fv(uniformLocView,1,GL_FALSE,glm::value_ptr(m_view));
   glUniformMatrix4fv(uniformLocProj,1,GL_FALSE,glm::value_ptr(m_projection));
   glm::mat4x4 id =glm::translate(glm::vec3(0.0f,1.2f,0.0f));
   glUniformMatrix4fv(uniformLoc,1,GL_FALSE,glm::value_ptr(id));

    //       [Task 1.5] Call glViewport so that the viewport is the right size
   glViewport(0,0,m_width,m_height);
   m_sphere->draw();

    //       [Task 5b] Bind m_blurFBO1
    //       [Task 8] Bind m_blurFBO1's color texture
   m_blurFBO1->unbind();

    //       [Task 7] Unbind m_blurFBO1 and render a full screen quad
   //glUseProgram(0);



   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glViewport(0,0,m_width,m_height);
   m_blurFBO2->bind();
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   m_blurFBO1->getColorAttachment(0).bind();
   //glUseProgram(m_textureProgram);
   glUseProgram(m_horizontalBlurProgram);
   m_quad->draw();
   m_blurFBO2->unbind();
   m_blurFBO2->getColorAttachment(0).bind();
   glUseProgram(m_verticalBlurProgram);
   m_quad->draw();
    //       [Task 11] Bind m_blurFBO2

}

void GLWidget::drawParticles() {
    auto prevFBO = m_evenPass ? m_particlesFBO1 : m_particlesFBO2;
    auto nextFBO = m_evenPass ? m_particlesFBO2 : m_particlesFBO1;
    float firstPass = m_firstPass ? 1.0f : 0.0f;

    // TODO [Task 14] Move the particles from prevFBO to nextFBO while updating them
    nextFBO->bind();
    glUseProgram(m_particleUpdateProgram);
    glActiveTexture(GL_TEXTURE0);
    prevFBO->getColorAttachment(0).bind();
    glActiveTexture(GL_TEXTURE1);
    prevFBO->getColorAttachment(1).bind();
    // TODO [Task 17] Draw the particles from nextFBO
    nextFBO->unbind();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_particleDrawProgram);
    setParticleViewport();
    glActiveTexture(GL_TEXTURE0);
    nextFBO->getColorAttachment(0).bind();
    glActiveTexture(GL_TEXTURE1);
    nextFBO->getColorAttachment(1).bind();
    GLint uniformLocPos = glGetUniformLocation(m_particleDrawProgram, "prevPos");
    glUniform1i(uniformLocPos,0);
    GLint uniformLocVel = glGetUniformLocation(m_particleDrawProgram, "prevVel");
    glUniform1i(uniformLocVel,1);
    GLint uniformLocNum = glGetUniformLocation(m_particleDrawProgram, "numParticles");
    glUniform1i(uniformLocNum,m_numParticles);
    glBindVertexArray(m_particlesVAO);
    glDrawArrays(GL_TRIANGLES,uniformLocNum,3*m_numParticles);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    m_firstPass = false;
    m_evenPass = !m_evenPass;
}

// This is called at the beginning of the program between initializeGL and
// the first paintGL call, as well as every time the window is resized.
void GLWidget::resizeGL(int w, int h) {
    m_width = w;
    m_height = h;

    // TODO: [Task 5] Initialize FBOs here, with dimensions m_width and m_height.
   m_blurFBO1 = std::make_unique<FBO>(1,FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY,m_width,m_height);

    //       [Task 12] Pass in TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE as the last parameter
    m_blurFBO2 = std::make_unique<FBO>(1,FBO::DEPTH_STENCIL_ATTACHMENT::NONE,m_width,m_height);

    rebuildMatrices();
}

// Sets the viewport to ensure that {0,0} is always in the center of the viewport
// in clip space, and to ensure that the aspect ratio is 1:1
void GLWidget::setParticleViewport() {
    int maxDim = std::max(m_width, m_height);
    int x = (m_width - maxDim) / 2.0f;
    int y = (m_height - maxDim) / 2.0f;
    glViewport(x, y, maxDim, maxDim);
}

/// Mouse interaction code below.

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    m_angleX += 3 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 3 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->delta() / 100.f;
    rebuildMatrices();
}

void GLWidget::rebuildMatrices() {
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));

    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    update();
}
