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
  class RenderObject {
  public:
    RenderObject() = delete;
    RenderObject(const RenderObject &) = delete;

    /**
     * @brief creates an instance from a vao and modelView matrix
     * @param program  a GLSL program
     * @param vao  the VAO to be instanciated
     * @param modelWorld the matrix transform between the object (a.k.a model) space and the camera (a.k.a view) space
     * @return the created InstancedVAO as a smart pointer
     */
    static std::shared_ptr<RenderObject> createInstance(const std::shared_ptr<Program> & prog, const std::shared_ptr<VAO> & vao, const glm::mat4 & modelWorld);

    /**
     * @brief Draw this VAO
     */
    void draw(GLenum mode = GL_TRIANGLES) const;

  private:
    RenderObject(const std::shared_ptr<Program> & prog, const std::shared_ptr<VAO> & vao, const glm::mat4 & modelView);

    /**
     * @brief update the program M uniform variable
     */
    void updateProgram() const;

  private:
    std::shared_ptr<Program> m_prog; ///< VAO
    std::shared_ptr<VAO> m_vao;      ///< VAO
    glm::mat4 m_mw;                  ///< modelWorld matrix
  };

private:
  class Shape {
  public:
    std::string _name;
    std::vector<glm::vec2> _positions;
    glm::vec4 _color;
    std::vector<uint> _ibo;
    };

private:
  std::vector<std::shared_ptr<RenderObject>> m_vaos; ///< List of instanced VAOs (VAO + modelView matrix)
  std::shared_ptr<Program> m_program;                ///< A GLSL progam

  void makeA2DShape(std::vector<glm::vec2>, glm::vec4 , std::vector<uint>,glm::mat4);
};

#endif // GLITTER_SIMPLEAPP_HPP
