#ifndef GLITTER_PIECE_HPP
#define GLITTER_PIECE_HPP

#include <glm/vec2.hpp>

enum Piece_Type {
    SQUARE_1x1,
    SQUARE_2x2,
    RECTANGLE_1x2,
    RECTANGLE_1x3,
    RECTANGLE_1x4,
    RECTANGLE_2x3
};

enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

class Piece {
public:
    Piece(Piece_Type type);
    Piece(glm::vec2 position, Piece_Type type, Direction orientation);
    void clockwiseRotate();
    void counterClockwiseRotate();
    void move(Direction direction);

    Direction getOrientation();
    glm::vec2 getPosition();

private:
    glm::vec2 _topLeftPosition;
    Piece_Type _type;
    Direction _orientation;
};

inline Direction & operator++(Direction & state, int);
inline Direction & operator--(Direction & type, int);


#endif //GLITTER_PIECE_HPP
