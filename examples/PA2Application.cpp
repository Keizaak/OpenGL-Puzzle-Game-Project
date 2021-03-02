#define GLM_FORCE_RADIANS

#include "PA2Application.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "utils.hpp"

PA2Application::PA2Application(int windowWidth, int windowHeight)
        : Application(windowWidth, windowHeight, "Application for PA2"), m_vao(2),
          m_program("shaders/simple3d.v.glsl", "shaders/simple3d.f.glsl"), m_currentTime(0) {
    makeA3dCube();
}

void PA2Application::makeA3dCube() {
    /*
    std::vector<glm::vec2> positions = {
        {-0.5, -0.5},
        {0.5, -0.5},
        {0.5, 0.5},
        {-0.5, 0.5}
    };
    std::vector<glm::vec3> colors = {{1, 1, 0}, {1, 1, 0}, {1, 1, 0}, {1, 1, 0}}; // a single yellow color
    std::vector<uint> ibo = {0, 1, 2, 0, 2, 3};
    m_vao.setVBO(0, positions);
    m_vao.setVBO(1, colors);
    m_vao.setIBO(ibo);
    */
    glm::vec3 a = {-0.5, 0.5, 0.5};
    glm::vec3 b = {0.5, 0.5, 0.5};
    glm::vec3 c = {-0.5, -0.5, 0.5};
    glm::vec3 d = {0.5, -0.5, 0.5};
    glm::vec3 e = {-0.5, 0.5, -0.5};
    glm::vec3 f = {-0.5, -0.5, -0.5};
    glm::vec3 g = {0.5, 0.5, -0.5};
    glm::vec3 h = {0.5, -0.5, -0.5};
    /*
    std::vector<glm::vec3> positions = {
        a, b, c, d,
        a, b, h, g,
        a, d, e, g,
        d, c, f, e,
        b, c, f, h,
        e, f, g, h
    };
    */
    std::vector<glm::vec3> positions = {
            c, d, b, a,
            f, h, g, e,
            c, d, h, f,
            a, b, g, e,
            f, c, a, e,
            h, d, b, g
    };

    glm::vec3 red = {1, 0, 0};
    glm::vec3 green = {0, 1, 0};
    glm::vec3 blue = {0, 0, 1};
    glm::vec3 magenta = {1, 0, 1};
    glm::vec3 cyan = {0, 1, 1};
    glm::vec3 yellow = {1, 1, 0};
    std::vector<glm::vec3> colors = {
            red, red, red, red,
            green, green, green, green,
            blue, blue, blue, blue,
            magenta, magenta, magenta, magenta,
            cyan, cyan, cyan, cyan,
            yellow, yellow, yellow, yellow
    };

    std::vector<uint> ibo = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
    };

    m_vao.setVBO(0, positions);
    m_vao.setVBO(1, colors);
    m_vao.setIBO(ibo);

}

void PA2Application::renderFrame() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program.bind();

    float angle = 0.5f * m_currentTime;
    glm::mat4 m = glm::rotate(glm::mat4(1.), angle, glm::vec3(0.5, 0.5, 0.5));
    m = glm::scale(m, glm::vec3(0.5, 0.5, 0.5));
    m = glm::translate(m, glm::vec3(-0.5, -0.5, -0.5));
    glm::mat4 v(1.);
    glm::mat4 p(1.);

    m_program.setUniform("M", m);
    m_program.setUniform("V", v);
    m_program.setUniform("P", p);
    m_vao.draw();

    m = glm::translate(m, glm::vec3(1, 1, 1));
    m_program.setUniform("M", m);
    m_program.setUniform("V", v);
    m_program.setUniform("P", p);
    m_vao.draw();

    m_program.unbind();
}

void PA2Application::update() {
    m_currentTime = glfwGetTime();
    m_program.bind();
    m_program.setUniform("time", m_currentTime);
    m_program.unbind();
}

void PA2Application::resize(GLFWwindow *, int framebufferWidth, int framebufferHeight) {
    glViewport(0, 0, framebufferWidth, framebufferHeight);
}

void PA2Application::setCallbacks() {
    GLFWwindow *window = glfwGetCurrentContext();
    glfwSetFramebufferSizeCallback(window, PA2Application::resize);
}

void PA2Application::usage(std::string &shortDescritpion, std::string &synopsis, std::string &description) {
    shortDescritpion = "Application for programming assignment 2";
    synopsis = "pa2";
    description = "  An application on GLSL uniform variables and true 3D meshes.\n";
}
