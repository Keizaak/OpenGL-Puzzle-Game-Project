#ifndef GLITTER_PIECE_HPP
#define GLITTER_PIECE_HPP

#include <fstream>
#include "glApi.hpp"
#include <glm/vec2.hpp>
#include "Shape.hpp"

enum Piece_Type {
    SQUARE_1X1 = 0,
    RECTANGLE_1X2,
    SMALL_V,
    RECTANGLE_3X1,
    RECTANGLE_4X1,
    SMALL_L,
    SMALL_T,
    Z,
    SQUARE_2X2,
    BIG_L,
    Y,
    N,
    BIG_V,
    P,
    U,
    S
};


enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

class Piece : public Shape {
public:
    explicit Piece(Piece_Type type, float scale);

    glm::mat4 getModel();
    void setScale(float scale);

    void clockwiseRotate();
    void counterClockwiseRotate();
    void move(Direction direction);
    float convertAngleToRadian();
    void generateVAOFromMatrix() override;

private:
    Piece_Type _type;
    int _angle;
    float _scale;
    glm::vec2 _translationVector;
    glm::mat4 _model;
    bool _isWellPlaced;

    void changeModelMatrix();
    void changeSquarePosition() override;

    std::vector<glm::vec4> matrixTransformation(std::vector<glm::vec4> vect);
};

#endif //GLITTER_PIECE_HPP
