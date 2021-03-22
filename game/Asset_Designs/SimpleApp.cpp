#include "SimpleApp.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../keyboard/command.h"
#include "utils.hpp"

SimpleApp::SimpleApp(int windowWidth, int windowHeight)
    : Application(windowWidth, windowHeight, "Puzzle Game"), m_program(new Program("shaders/game.v.glsl", "shaders/game.f.glsl")), _currentPieceIndex(1), nb_piece(0)
{

  GLFWwindow * window = glfwGetCurrentContext();
  glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
  resize(window, windowWidth, windowHeight);

  _scale = 0.05;

  std::shared_ptr<Shape> shape(new Shape());

  makeA2DShape(shape);

  for (int i = 0; i < 4; i++) {
    makeA2DShape(std::shared_ptr<Piece>(new Piece(BIG_V, _scale)));
    makeA2DShape(std::shared_ptr<Piece>(new Piece(SQUARE_2X2, _scale)));
    makeA2DShape(std::shared_ptr<Piece>(new Piece(SMALL_T, _scale)));
    if (i < 2) {
      makeA2DShape(std::shared_ptr<Piece>(new Piece(RECTANGLE_4X1, _scale)));
    }
  }

  if (m_pieces.size() != 0) {
    nb_piece = m_pieces.size();
    _currentPieceIndex = nb_piece - 1;
    _currentPiece = givePointerInVector();
  }
}

std::shared_ptr<SimpleApp::RenderObject> SimpleApp::givePointerInVector()
{
  return m_pieces[_currentPieceIndex];
}

void SimpleApp::makeA2DShape(std::shared_ptr<Piece> piece)
{
  m_pieces.push_back(RenderObject::createInstance(m_program, piece));
}

void SimpleApp::makeA2DShape(std::shared_ptr<Shape> shape)
{
  m_shapes.push_back(RenderObject::createInstance(m_program, shape, _scale));
}

void SimpleApp::renderFrame()
{
  glClear(GL_COLOR_BUFFER_BIT);

  for (const auto & shape : m_shapes) {
    shape->draw();
  }

  for (const auto & piece : m_pieces) {
    piece->draw();
  }
}

void SimpleApp::update()
{
  glm::mat4 view(1.);
  glm::mat4 proj(1.);

  m_program->bind();
  m_program->setUniform("V", view);
  m_program->setUniform("P", proj);
  m_program->unbind();
}

void SimpleApp::resize(GLFWwindow *, int framebufferWidth, int framebufferHeight)
{
  glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void SimpleApp::setCallbacks()
{
  GLFWwindow * window = glfwGetCurrentContext();
  glfwSetFramebufferSizeCallback(window, SimpleApp::resize);
  glfwSetKeyCallback(window, SimpleApp::keyCallback);
}

void SimpleApp::keyCallback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/)
{
  SimpleApp & app = *static_cast<SimpleApp *>(glfwGetWindowUserPointer(window));

  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_Q || key == GLFW_KEY_RIGHT_SHIFT) {
      app.rotatePiece(1);
    } else if (key == GLFW_KEY_E || key == GLFW_KEY_KP_1) {
      app.rotatePiece(0);
    } else if (key == GLFW_KEY_O) {
      if (app.nb_piece > 1) {
        app.changeCurrentPiece(2);
      }
    } else if (key == GLFW_KEY_P) {
      if (app.nb_piece > 1) {
        app.changeCurrentPiece(1);
      }
    } else {
      Direction direction = getDirectionFromKeyboard(key);
      if (direction != Direction::NONE) {
        app.movePiece(direction);
      }
    }
  }
}

void SimpleApp::rotatePiece(int direction)
{
  if (direction == 0) {
    _currentPiece->getPiece()->clockwiseRotate();
  } else if (direction == 1) {
    _currentPiece->getPiece()->counterClockwiseRotate();
  }
  allShapesCollision();
}

void SimpleApp::movePiece(Direction direction)
{
  _currentPiece->getPiece()->move(direction);
  allShapesCollision();
}

void SimpleApp::changeCurrentPiece(int mode)
{
  if (mode == 1) {
    int tmp = (_currentPieceIndex - 1) % nb_piece;

    if (tmp < 0){
      tmp += nb_piece;
    }
    _currentPieceIndex = tmp;
    _currentPiece = givePointerInVector();
  }
  if (mode == 2) {
    _currentPieceIndex = (_currentPieceIndex + 1) % nb_piece;
    _currentPiece = givePointerInVector();
  }
}
bool SimpleApp::squareCollision(glm::vec2 square1, glm::vec2 square2) {
  return (abs(square1[0] - square2[0]) < EPSILON && abs(square1[1] - square2[1]) < EPSILON);
}

bool SimpleApp::shapeCollision(const std::shared_ptr<Shape>& shape1, const std::shared_ptr<Shape>& shape2) {
  bool collision = false;
  for (auto & i : shape1->getSquarePositions()) {
    for (auto & j : shape2->getSquarePositions()) {
      collision = (collision || squareCollision(i, j));
    }
  }
  return collision;
}

void displayCoordinates(std::vector<glm::vec2> vector)
{
  for (auto & o : vector) {
    std::cout << "(" << o[0] << ", " << o[1] << ")" << std::endl;
  }
}

bool SimpleApp::allShapesCollision()
{
  int counter = 0;
  bool collision = false;
  std::shared_ptr<Piece> Piece2;
  std::shared_ptr<Piece> currentPiece = _currentPiece->getPiece();
  for (auto &o : m_pieces) {
    Piece2 = o->getPiece();
    if (Piece2 != currentPiece) {
      bool tmp = shapeCollision(currentPiece, Piece2);
      collision = (collision || tmp);
      if (tmp) {
        std::cout << "Piece n° " << counter << std::endl;
        displayCoordinates(Piece2->getSquarePositions());
      }
    }
    counter ++;
  }
  return collision;
}

SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> & prog, const std::shared_ptr<Piece> & piece) : m_prog(prog), m_piece(piece) {}
SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> & prog, const std::shared_ptr<Shape> & shape, float scale) : m_prog(prog), m_shape(shape), _scale(scale) {}

std::shared_ptr<SimpleApp::RenderObject> SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> & prog, const std::shared_ptr<Piece> & piece)
{
  return std::shared_ptr<RenderObject>(new RenderObject(prog, piece));
}

std::shared_ptr<SimpleApp::RenderObject> SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> & prog, const std::shared_ptr<Shape> & shape, float scale)
{
  return std::shared_ptr<RenderObject>(new RenderObject(prog, shape, scale));
}

void SimpleApp::RenderObject::draw(GLenum mode) const
{

  if (m_prog) {
    m_prog->bind();
    updateProgram();
    if (m_piece) {
      m_piece->draw(mode);
    } else if (m_shape) {
      m_shape->draw(mode);
    } else {
      std::cerr << "No object to draw" << std::endl;
    }
    m_prog->unbind();
  }
}

void SimpleApp::RenderObject::updateProgram() const
{
  if (m_piece) {
    m_prog->setUniform("M", m_piece->getModel());
  } else {
    m_prog->setUniform("M", glm::scale(glm::mat4(1), glm::vec3(_scale, _scale, 1)));
  }
}

std::shared_ptr<Piece> SimpleApp::RenderObject::getPiece()
{
  return m_piece;
}
