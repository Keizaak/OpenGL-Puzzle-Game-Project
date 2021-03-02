#include "SimpleApp.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.hpp"


SimpleApp::SimpleApp(int windowWidth, int windowHeight)
    : Application(windowWidth, windowHeight, "Puzzle Game"),
      m_vao(2), m_program("shaders/game.v.glsl", "shaders/game.f.glsl")
{
  makeA2DCorner();
  makeA2DRod();
}

void SimpleApp::makeA2DCorner()
{
  std::vector<glm::vec2> positions = {{-0.5, 0.5},
                                      {0.16, 0.5},
                                      {0.16, 0.16},
                                      {-0.17, 0.16},
                                      {-0.17, -0.5},
                                      {-0.5, -0.5},
                                      {-0.5,0.16}};

  glm::vec4 yellow = {1,0,0,1};

  std::vector<glm::vec4> colors = {yellow,
                                   yellow,
                                   yellow,
                                   yellow,
                                   yellow,
                                   yellow,
                                   yellow};

  std::vector<uint> ibo = {0, 1, 2,
                           2, 6, 0,
                           3, 4, 5,
                           5, 6, 3};

  m_vao.setVBO(0,positions);
  m_vao.setVBO(1,colors);
  m_vao.setIBO(ibo);
}

void SimpleApp::makeA2DRod()
{
  std::vector<glm::vec2> positions = {{-0.5, 0.5},
                                      {0.16, 0.5},
                                      {0.16, 0.16},
                                      {-0.17, 0.16},
                                      {-0.17, -0.5},
                                      {-0.5, -0.5},
                                      {-0.5,0.16}};

  glm::vec4 yellow = {1,1,0,1};

  std::vector<glm::vec4> colors = {yellow,
                                   yellow,
                                   yellow,
                                   yellow,
                                   yellow,
                                   yellow,
                                   yellow};

  std::vector<uint> ibo = {0, 1, 2,
                           2, 6, 0,
                           3, 4, 5,
                           5, 6, 3};

  m_vao.setVBO(0,positions);
  m_vao.setVBO(1,colors);
  m_vao.setIBO(ibo);
}

void SimpleApp::renderFrame() {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  m_program.bind();

  m_vao.draw();

  m_program.unbind();
}

void SimpleApp::update() {

}

void SimpleApp::resize(GLFWwindow *, int framebufferWidth, int framebufferHeight) {
  glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void SimpleApp::setCallbacks() {
  GLFWwindow *window = glfwGetCurrentContext();
  glfwSetFramebufferSizeCallback(window, SimpleApp::resize);
}
