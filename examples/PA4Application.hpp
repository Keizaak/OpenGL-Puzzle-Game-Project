#ifndef __PA_APPLICATION_H__
#define __PA_APPLICATION_H__
#include <memory>
struct GLFWwindow;
#include "Application.hpp"
#include "glApi.hpp"

class PA4Application : public Application {
public:
  PA4Application(int windowWidth, int windowHeight);
  void setCallbacks() override;
  static void usage(std::string & shortDescritpion, std::string & synopsis, std::string & description);

public:
  static unsigned int part; ///< Part 1 or 2 of the tutorial

private:
  void renderFrame() override;
  void update() override;
  static void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight);
  static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
  void continuousKey();
  void computeView(bool reset = false);

private:
  class RenderObject {
  public:
    RenderObject() = delete;
    RenderObject(const RenderObject &) = delete;

    static std::shared_ptr<RenderObject> createCheckerBoardCubeInstance(const glm::mat4 & modelWorld);
    /**
     * @brief creates an instance from a wavefront file and modelWorld matrix
     * @param objname the filename of the wavefront file
     * @param modelWorld the matrix transform between the object (a.k.a model) space and the world space
     * @return the created RenderObject as a smart pointer
     */
    static std::shared_ptr<RenderObject> createWavefrontInstance(const std::string & objname, const glm::mat4 & modelWorld);

    /**
     * @brief Draw this VAO
     */
    void draw(GLenum mode = GL_TRIANGLES);

    /**
     * @brief update the program MVP uniform variable
     * @param prog the target program
     * @param proj the projection matrix
     * @param view the worldView matrix
     */
    void updateProgram(Program & prog) const;

    void update(const glm::mat4 & proj, const glm::mat4 & view);

  private:
    RenderObject(const glm::mat4 & modelWorld);
    RenderObject(const std::string & objname, const glm::mat4 & modelWorld);
    void loadWavefront(const std::string & objname);
    static std::vector<GLubyte> makeCheckerBoard();

  private:
    std::vector<std::shared_ptr<VAO>> m_vaos;
    glm::mat4 m_mw; ///< modelWorld matrix
    std::vector<std::shared_ptr<Program>> m_programs;
    std::vector<std::shared_ptr<Texture>> m_diffuseTextures;
    std::unique_ptr<Sampler> m_colormap;
  };

private:
  std::vector<std::shared_ptr<RenderObject>> m_objects; ///< render objects
  glm::mat4 m_proj;                                     ///< Projection matrix
  glm::mat4 m_view;                                     ///< worldView matrix
  float m_eyePhi;                                       ///< Camera position longitude angle
  float m_eyeTheta;                                     ///< Camera position latitude angle
  float m_currentTime;                                  ///< elapsed time since first frame
  float m_deltaTime;                                    ///< elapsed time since last frame
};

#endif // !defined(__PA_APPLICATION_H__)