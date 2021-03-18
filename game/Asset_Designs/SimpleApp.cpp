#include "SimpleApp.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.hpp"
#include "../keyboard/command.h"


SimpleApp::SimpleApp(int windowWidth, int windowHeight)
        : Application(windowWidth, windowHeight, "Puzzle Game"),
          m_program(new Program("shaders/game.v.glsl", "shaders/game.f.glsl"))
{

    std::shared_ptr<Piece> corner(new Piece(BIG_L));
    makeA2DShape(corner,glm::mat4(1));
}

glm::mat4 createTranslate(std::shared_ptr<Piece> piece, Direction direction){
    glm::vec2 directionVect;
    switch (direction) {
        case UP:
            directionVect = glm::vec2(0.0, 0.33);
            break;
        case RIGHT:
            directionVect = glm::vec2(0.33, 0.0);
            break;
        case DOWN:
            directionVect = glm::vec2(0.0, -0.33);
            break;
        case LEFT:
            directionVect = glm::vec2(-0.33, 0.0);
            break;
        default:
            throw "Error: incorrect direction";
    }

    piece->setTransVect(piece->getTransVect() + directionVect);
    glm::mat4 res = glm::translate(glm::mat4(1),glm::vec3(piece->getTransVect(),0.));

    piece->setTopLeftPosition(piece->getPosition() + directionVect);

    return res;
}

glm::mat4 createRotationAroundAnchor(std::shared_ptr<Piece> piece, glm::mat4 model){
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

void SimpleApp::makeA2DShape(std::shared_ptr<Piece> piece, glm::mat4 model){
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
    glfwSetKeyCallback(window, SimpleApp::keyCallback);
}

void SimpleApp::keyCallback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/) {
    SimpleApp &app = *static_cast<SimpleApp *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q || key == GLFW_KEY_RIGHT_SHIFT) {
            std::cout << "Counterclockwise rotation" << std::endl;
        } else if (key == GLFW_KEY_E || key == GLFW_KEY_KP_1 ) {
            std::cout << "Clockwise rotation" << std::endl;
        } else {
            Direction direction = getDirectionFromKeyboard(key);
            if (direction != Direction::NONE) {
                //current_piece.move(direction);
                std::cout << "Moved: " << direction << std::endl;
            }
        }
    }
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