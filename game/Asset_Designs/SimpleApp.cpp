#include "SimpleApp.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.hpp"
#include "../keyboard/command.h"


SimpleApp::SimpleApp(int windowWidth, int windowHeight)
        : Application(windowWidth, windowHeight, "Puzzle Game"),
          m_program(new Program("shaders/game.v.glsl", "shaders/game.f.glsl")) {

    GLFWwindow * window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    resize(window, windowWidth, windowHeight);

    std::shared_ptr<Piece> corner(new Piece(BIG_L));
    corner->setScale(0.05);
    makeA2DShape(corner);
}

void SimpleApp::makeA2DShape(std::shared_ptr<Piece> piece) {
    _currentPiece = RenderObject::createInstance(m_program, piece);
    m_pieces.push_back(_currentPiece);
}

void SimpleApp::renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT);

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

SimpleApp::RenderObject::RenderObject(const std::shared_ptr<Program> &prog, const std::shared_ptr<Piece> &piece) : m_prog(prog), m_piece(piece) {}

std::shared_ptr<SimpleApp::RenderObject>
SimpleApp::RenderObject::createInstance(const std::shared_ptr<Program> &prog, const std::shared_ptr<Piece> &piece) {
    return std::shared_ptr<RenderObject>(new RenderObject(prog, piece));
}

void SimpleApp::RenderObject::draw(GLenum mode) const {
    if (m_piece and m_prog) {
        m_prog->bind();
        updateProgram();
        m_piece->draw(mode);
        m_prog->unbind();
    }
}

void SimpleApp::RenderObject::updateProgram() const {
    m_prog->setUniform("M", m_piece->getModel());
}

std::shared_ptr<Piece> SimpleApp::RenderObject::getPiece() {
    return m_piece;
}