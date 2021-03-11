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

  glm::vec3 yellow = {1,1,0};
  glm::vec3 red = {1,0,0};
  glm::vec3 blue = {0,0,1};



  std::vector<uint> iboCorner = {0, 1, 2,
                           2, 6, 0,
                           3, 4, 5,
                           5, 6, 3};
  std::vector<uint> iboRod = {0, 1, 2,
                           2, 3, 0};
  std::vector<uint> iboSmallRod = {0, 1, 2,
                           2, 3, 0};

  glm::mat4 model0(1.);

  float angle = 3.14;
  glm::mat4 model1 = glm::scale(glm::mat4(1.), glm::vec3(1.1, 1.1, 1.1));
  model1 = glm::translate(model1, glm::vec3(0.5, 0.5, 0));

  glm::vec2 anchor = positionsCorner[0];

  glm::mat4 model2 = createRotationAroundAnchor(anchor,angle);
//  model2 = glm::scale(model2, glm::vec3(1.1, 1.1, 1.1));

  // Le vecteur de rotation doit se situer sur le centre
//  makeA2DShape(positionsCorner,yellow,iboCorner,model2);
//  makeA2DShape(positionsRod,red,iboRod,model0);
//  makeA2DShape(positionsSmallRod,blue,iboSmallRod,model0);

  std::shared_ptr<Piece> corner(new Piece(SQUARE_1X1));
  makeA2DShape(corner,positionsCorner,yellow,model0);
}

glm::mat4 createRotationAroundAnchor(glm::vec2 anchor, float angle){
  glm::mat4 rotationMatrix = glm::translate(glm::mat4(1.), glm::vec3(anchor, 0));
  rotationMatrix = glm::rotate(rotationMatrix, angle, glm::vec3(0, 0, 1));
  rotationMatrix = glm::translate(rotationMatrix, glm::vec3(-anchor, 0));

  return rotationMatrix;
}

//void SimpleApp::makeA2DShape(std::vector<glm::vec2> positions, glm::vec3 color, std::vector<uint> ibo, glm::mat4 model)
//{
//  std::vector<glm::vec3> colors;
//  for(int i=0; i< positions.size(); i++) {
//    colors.push_back(color);
//  }
//
//  std::shared_ptr<VAO> vao(new VAO(2));
//  vao->setVBO(0,positions);
//  vao->setVBO(1,colors);
//  vao->setIBO(ibo);
//
//  m_pieces.push_back(RenderObject::createInstance(m_program, vao, model));
//}

void SimpleApp::makeA2DShape(std::shared_ptr<Piece> piece,  std::vector<glm::vec2> positions, glm::vec3 color, glm::mat4 model){
  piece->setVBO(0,positions);

  std::vector<glm::vec3> colors;
  for(int i=0; i< positions.size(); i++) {
    colors.push_back(color);
  }

  piece->setVBO(1,colors);

  m_pieces.push_back(RenderObject::createInstance(m_program, piece, model));
}

void SimpleApp::renderFrame() {
  glClear(GL_COLOR_BUFFER_BIT);

  for (const auto &vao : m_pieces) {
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

SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> &prog, const std::shared_ptr<Piece> &piece,
                                           const glm::mat4 &modelWorld) : m_prog(prog), m_piece(piece), m_mw(modelWorld) {}

std::shared_ptr<SimpleApp::RenderObject>
SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> &prog, const std::shared_ptr<Piece> &piece,
                                             const glm::mat4 &modelView) {
  return std::shared_ptr<RenderObject>(new RenderObject(prog, piece, modelView));
}

void SimpleApp::RenderObject::draw(GLenum mode) const {
  if (m_piece and m_prog) {
    m_prog->bind();
    updateProgram();
    glm::mat4 view(1.);
    glm::mat4 proj(1.);
    m_prog->setUniform("V", view);
    m_prog->setUniform("P", proj);
    m_piece->draw();
    m_prog->unbind();
  }
}

void SimpleApp::RenderObject::updateProgram() const {
  m_prog->setUniform("M", m_mw);
}
