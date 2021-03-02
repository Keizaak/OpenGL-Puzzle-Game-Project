#ifndef GLITTER_SIMPLEAPP_HPP
#define GLITTER_SIMPLEAPP_HPP

#include "Application.hpp"
#include "glApi.hpp"

struct GLFWwindow;

class SimpleApp : public Application {
public:
  SimpleApp(int windowWidth, int windowHeight);
  void setCallbacks() override;
  void renderFrame() override;
  void update() override;
  static void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight);

private:
  VAO m_vao;           ///<  A complete VAO
  Program m_program;   ///< A program used in both parts.

  void makeA2DCorner();
};


#endif // GLITTER_SIMPLEAPP_HPP
