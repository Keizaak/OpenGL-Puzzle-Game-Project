#include "simple_shapes.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils.hpp"

void makeA2DSimpleShapeWithFakeVAO()
{
  std::vector<glm::vec2> positions = {{-0.5, 0.5},
                                      {0.16, 0.5},
                                      {0.16, 0.16},
                                      {-0.17, 0.16},
                                      {-0.17, -0.5},
                                      {-0.5, -0.5} };

  m_vbo.setData(positions);
  m_fakevao.bind();
  m_vbo.bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, m_vbo.attributeSize(), m_vbo.attributeType(), GL_FALSE, 0, 0);
  m_vbo.unbind();
  m_fakevao.unbind();
}