#include "GameApplication.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.hpp"

GameApplication::GameApplication(int windowWidth, int windowHeight)
    : Application(windowWidth, windowHeight, "Puzzle Game"),
      m_vao(2), m_program("shaders/game.v.glsl", "shaders/game.f.glsl")
{
    makeA2DSquare();
}

void GameApplication::makeA2DSquare() {
    std::vector<glm::vec2> positions = {{-0.5, -0.5}, {0.5, -0.5}, {0.5, 0.5}, {-0.5, 0.5}};
    std::vector<uint> ibo = {0, 1, 2, 0, 2, 3};
    m_vao.setVBO(0, positions);
    m_vao.setIBO(ibo);
}

void GameApplication::renderFrame() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program.bind();

    m_vao.draw();

    m_program.unbind();
}

void GameApplication::update() {

}

void GameApplication::resize(GLFWwindow *, int framebufferWidth, int framebufferHeight) {
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void GameApplication::setCallbacks() {
    GLFWwindow *window = glfwGetCurrentContext();
    glfwSetFramebufferSizeCallback(window, GameApplication::resize);
}
