#include "glwidget.h"

#include "cs123_lib/resourceloader.h"
#include "cs123_lib/errorchecker.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>


#define PI 3.14159265f

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent), m_angleX(0), m_angleY(0.5f), m_zoom(10.f)
{}

GLWidget::~GLWidget()
{
    glDeleteTextures(1,&m_program);
}

void GLWidget::initializeGL() {
    ResourceLoader::initializeGlew();
    resizeGL(width(), height());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Set the color to set the screen when the color buffer is cleared.
    glClearColor((float)130.0/255.0, (float)206/255.0, (float)255.0/255.0, 0.0f);

    m_program = ResourceLoader::createShaderProgram(":/shaders/terrain.vert", ":/shaders/terrain.frag");
   // m_textureprogramID = ResourceLoader::createShaderProgram(":/shaders/skybox.vert", ":/shaders/skybox.frag");
    m_terrain.init(m_program);
    initializeGLFragmentShaders();
    rebuildMatrices();
}
void GLWidget::initializeGLFragmentShaders() {

    QImage image(":/shaders/cliff.jpg");

    glGenTextures(1,&m_textureID);
    glBindTexture(GL_TEXTURE_2D,m_textureID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MODULATE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
//    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind shader program.
    glUseProgram(m_program);

    // Set uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glBindTexture(m_textureID,GL_TEXTURE_2D);
    // Draw terrain.
    m_terrain.draw();

    // Unbind shader program.
    //glUseProgram(0);
//    glUseProgram(m_textureprogramID);
//    m_terrain.draw();

    glUseProgram(0);
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    rebuildMatrices();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    m_angleX += 10 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->delta() / 100.f;
    rebuildMatrices();
}

void GLWidget::rebuildMatrices() {
    m_model = glm::mat4(1.f);
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));
    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    update();
}
