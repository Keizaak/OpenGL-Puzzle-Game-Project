#include "Piece.hpp"

Piece::Piece(Piece_Type type)
    : _type(type)
{
    _topLeftPosition = glm::vec2(0, 0);
    _orientation = UP;
}

Piece::Piece(glm::vec2 position, Piece_Type type, Direction orientation)
    : _topLeftPosition(position), _type(type), _orientation(orientation)
{
}

inline Direction & operator++(Direction & state, int) {
    const int i = static_cast<int>(state)+1;
    state = static_cast<Direction>((i) % 4);
    return state;
}

inline Direction & operator--(Direction & type, int) {
    const int i = static_cast<int>(type)-1;

    if (i < 0) {
        type = static_cast<Direction>(3);
    } else {
        type = static_cast<Direction>((i) % 4);
    }
    return type;
}

void Piece::clockwiseRotate() {
    _orientation++;
}

void Piece::counterClockwiseRotate() {
    _orientation--;
}

void Piece::move(Direction direction) {
    switch (direction) {
        case UP:
            _topLeftPosition.y--;
            break;
        case RIGHT:
            _topLeftPosition.x++;
            break;
        case DOWN:
            _topLeftPosition.y++;
            break;
        case LEFT:
            _topLeftPosition.x--;
            break;
        default:
            throw "Error: incorrect direction";
    }
}

Direction Piece::getOrientation() {
    return _orientation;
}

glm::vec2 Piece::getPosition() {
    return _topLeftPosition;
}
