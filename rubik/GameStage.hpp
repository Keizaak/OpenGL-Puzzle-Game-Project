#ifndef __GAME_STAGE_H__
#define __GAME_STAGE_H__

#include "RubikLogic.hpp"
#include "RubikRenderer.hpp"
#include "TextPrinter.hpp"

/// Interface for game stages (eg start menu, playing stage, gameover screen,...)
class GameStage {
public:
  /// Renders a single frame
  virtual void renderFrame() = 0;

  /// Updates the time related members
  virtual void update() = 0;

  /// Returns the next game stage
  virtual std::unique_ptr<GameStage> nextStage() const = 0;

  /// Handle queue of key events
  virtual void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) = 0;

  /// Window resize callback
  virtual void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight) = 0;

  /// Destructor
  virtual ~GameStage() {}
};

/// The start menu
class StartMenuStage final : public GameStage {
public:
  StartMenuStage();
  ~StartMenuStage();

  void renderFrame() override;

  void update() override;

  void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) override {}

  void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight) override;

  std::unique_ptr<GameStage> nextStage() const override;

private:
  std::unique_ptr<TextPrinter> m_text;
  RubikRenderer m_renderer;
};

/// The actual playing stage
class PlayingStage final : public GameStage {
public:
  PlayingStage() : m_displayHelp(false), m_helper(800, 800)
  {
    const glm::vec3 textColor(1, 0, 0);
    const glm::vec4 fillColor(0.5, 0.5, 0.5, 0.8);
    const unsigned int fontSize = 3;
    m_renderer.deform(false);
    m_helper.printText("1,2,3: rotate faces", 0, 0, fontSize, textColor, fillColor);
    m_helper.printText("\x1a,\x1b  : rotate cube ", 0, 1, fontSize, textColor, fillColor);
    m_helper.printText("       horizonttaly", 0, 2, fontSize, textColor, fillColor);
    m_helper.printText("\x12    : rotate cube ", 0, 3, fontSize, textColor, fillColor);
    m_helper.printText("       vertically  ", 0, 4, fontSize, textColor, fillColor);
  }

  void renderFrame() override;

  void update() override;

  void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) override;

  void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight) override;

  std::unique_ptr<GameStage> nextStage() const override;

private:
  void turnClockwise(unsigned int faceID);

private:
  RubikRenderer m_renderer; ///< the rubik's cube renderer
  RubikState m_state;       ///< the rubik's cube state
  TextPrinter m_helper;
  bool m_displayHelp;
};

/// game over menu
class GameOverStage final : public GameStage {
public:
  GameOverStage();

  ~GameOverStage() { glClearColor(1, 1, 1, 1); }

  void renderFrame() override;

  void update() override {}

  void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) override {}

  void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight) override;

  std::unique_ptr<GameStage> nextStage() const override;

private:
  std::unique_ptr<TextPrinter> m_text;
};

#endif // !defined(__GAME_STAGE_H__)
