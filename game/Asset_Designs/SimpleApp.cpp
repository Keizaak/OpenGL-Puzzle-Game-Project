#include "SimpleApp.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.hpp"


SimpleApp::SimpleApp(int windowWidth, int windowHeight)
    : Application(windowWidth, windowHeight, "Puzzle Game"),
      m_program(new Program("shaders/game.v.glsl", "shaders/game.f.glsl"))
{
  std::vector<glm::vec2> positionsCorner = {{-0.5, 0.5},
                                      {0.16, 0.5},
                                      {0.16, 0.16},
                                      {-0.17, 0.16},
                                      {-0.17, -0.5},
                                      {-0.5, -0.5},
                                      {-0.5,0.16}};

  std::vector<glm::vec2> positionsRod = {{0.16, 0.5},
                                      {0.5, 0.5},
                                      {0.5, -0.5},
                                      {0.16, -0.5}};

  std::vector<glm::vec2> positionsSmallRod = {{-0.17, 0.16},
                                      {0.16, 0.16},
                                      {0.16, -0.5},
                                      {-0.17, -0.5}};

  glm::vec4 yellow = {1,1,0,1};
  glm::vec4 red = {1,0,0,1};
  glm::vec4 blue = {0,0,1,1};

  std::vector<uint> iboCorner = {0, 1, 2,
                           2, 6, 0,
                           3, 4, 5,
                           5, 6, 3};
  std::vector<uint> iboRod = {0, 1, 2,
                           2, 3, 0};
  std::vector<uint> iboSmallRod = {0, 1, 2,
                           2, 3, 0};

  makeA2DShape(positionsCorner,yellow,iboCorner);
  makeA2DShape(positionsRod,red,iboRod);
  makeA2DShape(positionsSmallRod,blue,iboSmallRod);
}

void SimpleApp::makeA2DShape(std::vector<glm::vec2> positions, glm::vec4 color, std::vector<uint> ibo)
{
  std::vector<glm::vec4> colors;
  for(int i=0; i< positions.size(); i++) {
    colors.push_back(color);
  }

  glm::mat4 mw(1);

  std::shared_ptr<VAO> vao(new VAO(2));
  vao->setVBO(0,positions);
  vao->setVBO(1,colors);
  vao->setIBO(ibo);

  m_vaos.push_back(RenderObject::createInstance(m_program, vao, mw));
}

void SimpleApp::renderFrame() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  for (const auto &vao : m_vaos) {
    vao->draw();
  }
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

SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> &prog, const std::shared_ptr<VAO> &vao,
                                           const glm::mat4 &modelWorld) : m_prog(prog), m_vao(vao), m_mw(modelWorld) {}

std::shared_ptr<SimpleApp::RenderObject>
SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> &prog, const std::shared_ptr<VAO> &vao,
                                             const glm::mat4 &modelView) {
  return std::shared_ptr<RenderObject>(new RenderObject(prog, vao, modelView));
}

void SimpleApp::RenderObject::draw(GLenum mode) const {
  if (m_vao and m_prog) {
    m_prog->bind();
    updateProgram();
    m_vao->draw(mode);
    m_prog->unbind();
  }
}

void SimpleApp::RenderObject::updateProgram() const {
  m_prog->setUniform("M", m_mw);
}
