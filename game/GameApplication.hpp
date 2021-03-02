#ifndef GLITTER_GAMEAPPLICATION_HPP
#define GLITTER_GAMEAPPLICATION_HPP

#include "Application.hpp"
#include "glApi.hpp"
#include "./Asset_Designs/simple_shapes.hpp"

struct GLFWwindow;

class GameApplication : public Application {
public:
    GameApplication(int windowWidth, int windowHeight);
    void setCallbacks() override;
    void renderFrame() override;
    void update() override;
    static void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight);

private:
    VAO m_vao;           ///<  A complete VAO
    Program m_program;   ///< A program used in both parts.

    void makeA2DSquare();
};


#endif //GLITTER_GAMEAPPLICATION_HPP
