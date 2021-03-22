#include "SimpleApp.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.hpp"
#include "../keyboard/command.h"


SimpleApp::SimpleApp(int windowWidth, int windowHeight)
        : Application(windowWidth, windowHeight, "Puzzle Game"),
          m_program(new Program("shaders/game.v.glsl", "shaders/game.f.glsl")), _currentPieceIndex(1) ,nb_piece(0) {

    GLFWwindow * window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    resize(window, windowWidth, windowHeight);

    std::shared_ptr<Shape> shape(new Shape());

    std::shared_ptr<Piece> corner(new Piece(BIG_L));
    std::shared_ptr<Piece> small_rod(new Piece(RECTANGLE_1X2));
    std::shared_ptr<Piece> big_rod(new Piece(RECTANGLE_3X1));
    corner->setScale(0.05);
    small_rod->setScale(0.05);
    big_rod->setScale(0.05);

    makeA2DShape(shape);

    //makeA2DShape(corner);
    //makeA2DShape(big_rod);
    //makeA2DShape(small_rod);

    if (m_pieces.size() != 0){
      _currentPiece = givePointerInVector();
      nb_piece = m_pieces.size();
    }
}

std::shared_ptr<SimpleApp::RenderObject> SimpleApp::givePointerInVector(){
  return m_pieces[_currentPieceIndex];
}

void SimpleApp::makeA2DShape(std::shared_ptr<Piece> piece) {
    m_pieces.push_back(RenderObject::createInstance(m_program, piece));
}

void SimpleApp::makeA2DShape(std::shared_ptr<Shape> shape) {
  m_shapes.push_back(RenderObject::createInstance(m_program, shape));
}

void SimpleApp::renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto &shape : m_shapes) {
      shape->draw();
    }

    for (const auto &piece : m_pieces) {
        piece->draw();
    }
}

void SimpleApp::update() {
    glm::mat4 view(1.);
    glm::mat4 proj(1.);

    m_program->bind();
    m_program->setUniform("V", view);
    m_program->setUniform("P", proj);
    m_program->unbind();
}

void SimpleApp::resize(GLFWwindow *, int framebufferWidth, int framebufferHeight) {
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void SimpleApp::setCallbacks() {
    GLFWwindow *window = glfwGetCurrentContext();
    glfwSetFramebufferSizeCallback(window, SimpleApp::resize);
    glfwSetKeyCallback(window, SimpleApp::keyCallback);
}

void SimpleApp::keyCallback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/) {
    SimpleApp &app = *static_cast<SimpleApp *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q || key == GLFW_KEY_RIGHT_SHIFT) {
            app.rotatePiece(1);
        } else if (key == GLFW_KEY_E || key == GLFW_KEY_KP_1) {
            app.rotatePiece(0);
        }else if (key == GLFW_KEY_SPACE){
          if (app.nb_piece > 1){
            app.changeCurrentPiece();
          }
        } else {
            Direction direction = getDirectionFromKeyboard(key);
            if (direction != Direction::NONE) {
                app.movePiece(direction);
            }
        }
    }

}

void SimpleApp::rotatePiece(int direction) {
    if (direction == 0) {
        _currentPiece->getPiece()->clockwiseRotate();
    } else if (direction == 1) {
        _currentPiece->getPiece()->counterClockwiseRotate();
    }
}

void SimpleApp::movePiece(Direction direction) {
    _currentPiece->getPiece()->move(direction);
}

void SimpleApp::changeCurrentPiece(){
    _currentPieceIndex = (_currentPieceIndex + 1) % nb_piece;
    _currentPiece = givePointerInVector();
};

SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> &prog, const std::shared_ptr<Piece> &piece) : m_prog(prog), m_piece(piece) {}
SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> &prog, const std::shared_ptr<Shape> &shape) : m_prog(prog), m_shape(shape) {}

std::shared_ptr<SimpleApp::RenderObject>
SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> &prog, const std::shared_ptr<Piece> &piece) {
    return std::shared_ptr<RenderObject>(new RenderObject(prog, piece));
}

std::shared_ptr<SimpleApp::RenderObject>
SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> &prog, const std::shared_ptr<Shape> &shape) {
  return std::shared_ptr<RenderObject>(new RenderObject(prog, shape));
}

void SimpleApp::RenderObject::draw(GLenum mode) const {

    if (m_prog) {
        m_prog->bind();
        updateProgram();
        if (m_piece){
          m_piece->draw(mode);
        }
        else if (m_shape){
         m_shape->draw(mode);
        }
        else{
          std::cerr << "No object to draw" << std::endl;
        }
        m_prog->unbind();
    }
}

void SimpleApp::RenderObject::updateProgram() const {
    if (m_piece){
      m_prog->setUniform("M", m_piece->getModel());
    }
    else{
      m_prog->setUniform("M", glm::scale(glm::mat4(1), glm::vec3(0.01, 0.01, 1)));
    }
}

std::shared_ptr<Piece> SimpleApp::RenderObject::getPiece() {
    return m_piece;
}