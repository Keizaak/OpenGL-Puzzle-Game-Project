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
    explicit Piece(Piece_Type type);
    Piece(glm::vec2 position, Piece_Type type, int angle);

    void clockwiseRotate();
    void counterClockwiseRotate();
    void move(Direction direction);

    int getAngle() const;
    glm::vec2 getPosition();
    void setTopLeftPosition(glm::vec2 topLeftPosition);
    const glm::vec2 & getTransVect() const;
    void setTransVect(const glm::vec2 & transVect);
    void generateVAOFromMatrix() override;

private:
    glm::vec2 _topLeftPosition;
    Piece_Type _type;
    int _angle;
    glm::vec2 _transVect;
};

#endif //GLITTER_PIECE_HPP
