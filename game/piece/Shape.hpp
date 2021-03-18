#ifndef GLITTER_SHAPE_HPP
#define GLITTER_SHAPE_HPP

#include "glApi.hpp"

class Shape {
public:
    Shape();
    void setPositionVBO(std::vector<glm::vec2> const & VBO);
    void setColorVBO(std::vector<glm::vec3> const & VBO);
    void setIBO(std::vector<uint> const & IBO);
    void draw(GLenum mode);
    virtual void generateVAOFromMatrix();
protected:
    VAO _vao;
    glm::vec3 generateRandomColorVector();
    int positionInVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO);
    void print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec);
    void print3dVBO(const std::vector<glm::vec3> &VBO, int sizeOfVec);
    void printIBO(const std::vector<uint>& IBO);
};


#endif //GLITTER_SHAPE_HPP
