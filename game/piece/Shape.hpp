#ifndef GLITTER_SHAPE_HPP
#define GLITTER_SHAPE_HPP

#include "glApi.hpp"

#define EPSILON 0.0001

class Shape {
public:
    Shape();

    explicit Shape(glm::mat4 model);

    void draw(GLenum mode);

    virtual void generateVAOFromMatrix();

    std::vector<glm::vec2> getSquarePositions();

protected:
    VAO _vao;
    /* vector of the positions of the left hand corners of squares that composed a shape*/
    std::vector<glm::vec2> _squareOriginsPositions;
    std::vector<glm::vec2> _squarePositions;

    static glm::vec3 generateRandomColorVector();

    static int positionInVBO(glm::vec2 point, const std::vector<glm::vec2> &VBO);

    static glm::vec4 coordinateToHomogene(glm::vec2 normalCoordinates);

    static glm::vec2 homogeneToCoordinate(glm::vec4 homogeneCoordinates);

    static std::vector<glm::vec4> coordinateToHomogeneV(const std::vector<glm::vec2> &normalCoordinates);

    static std::vector<glm::vec2> homogeneToCoordinateV(const std::vector<glm::vec4> &homogeneCoordinates);

    static std::vector<glm::vec4> matrixTransformation(const std::vector<glm::vec4>& homCoord, glm::mat4 model);

    void changeSquarePosition(glm::mat4 model);

};

#endif //GLITTER_SHAPE_HPP
