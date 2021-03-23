#ifndef GLITTER_SHAPE_HPP
#define GLITTER_SHAPE_HPP

#include "glApi.hpp"

#define EPSILON 0.01

class Shape {
public:
    Shape();
    Shape(glm::mat4 model);
    void setPositionVBO(std::vector<glm::vec2> const & VBO);
    void setColorVBO(std::vector<glm::vec3> const & VBO);
    void setIBO(std::vector<uint> const & IBO);
    void draw(GLenum mode);
    virtual void generateVAOFromMatrix();
    std::vector<glm::vec2> getSquareOriginsPositions();
    std::vector<glm::vec2> getSquarePositions();
    void setSquarePositions(const std::vector<glm::vec2> & squarePositions);


  protected:
    VAO _vao;
    /* vector of the positions of the left hand corners of squares that composed a shape*/
    std::vector<glm::vec2> _squareOriginsPositions;
    std::vector<glm::vec2> _squarePositions;


    glm::vec3 generateRandomColorVector();
    int positionInVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO);
    void print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec);
    void print3dVBO(const std::vector<glm::vec3> &VBO, int sizeOfVec);
    void printIBO(const std::vector<uint>& IBO);

    glm::vec4 coordinateToHomogene(glm::vec2 normalCoordinates);
    glm::vec2 homogeneToCoordinate(glm::vec4 homogeneCoordinates);

    std::vector<glm::vec4> coordinateToHomogeneV(const std::vector<glm::vec2>& normalCoordinates);
    std::vector<glm::vec2> homogeneToCoordinateV(const std::vector<glm::vec4>& homogeneCoordinates);

    std::vector<glm::vec4> matrixTransformation(std::vector<glm::vec4> homCoord, glm::mat4 model);
    void changeSquarePosition(glm::mat4 model);

};


#endif //GLITTER_SHAPE_HPP
