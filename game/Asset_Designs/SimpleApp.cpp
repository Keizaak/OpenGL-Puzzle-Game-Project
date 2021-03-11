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

  float angle = glm::pi<float>();
  glm::mat4 model1 = glm::scale(glm::mat4(1.), glm::vec3(1.1, 1.1, 1.1));
  model1 = glm::translate(model1, glm::vec3(0.5, 0.5, 0));

  std::shared_ptr<Piece> corner(new Piece(positionsCorner[0],BIG_L,0));
  corner->clockwiseRotate();
  glm::mat4 model2 = createRotationAroundAnchor(corner);
  makeA2DShape(corner,positionsCorner,yellow,model2,iboCorner);

  std::shared_ptr<Piece> rod(new Piece(positionsRod[0],RECTANGLE_3X1,0));
  glm::mat4 model3 = createRotationAroundAnchor(rod);
  makeA2DShape(rod,positionsRod,red,model3,iboRod);

  std::shared_ptr<Piece> smallRod(new Piece(positionsSmallRod[0],RECTANGLE_1X2,0));
  glm::mat4 model4 = createRotationAroundAnchor(smallRod);
  makeA2DShape(smallRod,positionsSmallRod,blue,model4,iboSmallRod);
}

glm::mat4 createRotationAroundAnchor(std::shared_ptr<Piece> piece){
  float newAngle = static_cast<float>(piece->getAngle())*2*glm::pi<float>()/360.;

  glm::mat4 rotationMatrix = glm::translate(glm::mat4(1.), glm::vec3(piece->getPosition(), 0));

  rotationMatrix = glm::rotate(rotationMatrix, newAngle, glm::vec3(0, 0, 1));
  rotationMatrix = glm::translate(rotationMatrix, glm::vec3(-piece->getPosition(), 0));

  return rotationMatrix;
}

void SimpleApp::makeA2DShape(std::shared_ptr<Piece> piece,  std::vector<glm::vec2> positions, glm::vec3 color, glm::mat4 model, std::vector<uint> ibo){
  piece->setPositionVBO(positions);

  std::vector<glm::vec3> colors;
  for(long unsigned int i=0; i< positions.size(); i++) {
    colors.push_back(color);
  }

  piece->setColorVBO(colors);
  piece->setIBO(ibo);

  m_pieces.push_back(RenderObject::createInstance(m_program, piece, model));
}

void SimpleApp::renderFrame() {
  glClear(GL_COLOR_BUFFER_BIT);

  for (const auto &piece : m_pieces) {
    piece->draw();
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
    m_piece->draw(mode);
    m_prog->unbind();
  }
}

void SimpleApp::RenderObject::updateProgram() const {
  m_prog->setUniform("M", m_mw);
}
