#ifndef GLITTER_PIECE_HPP
#define GLITTER_PIECE_HPP

#include <fstream>
#include <glApi.hpp>
#include <glm/vec2.hpp>

#define EPSILON 0.01 /* for equality between 2 floats in the VBO */

/*
class Piece_Type final {
public:
  static Piece_Type SQUARE_1x1=Piece_Type("SQUARE_1x1");
  static Piece_Type SQUARE_2x2("SQUARE_2x2");

private:
  Piece_Type(std::string piece_name);

private:
  VAO vao;

};


enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

class Piece {
public:
    Piece(Piece_Type *type);
    Piece(glm::vec2 position, Piece_Type *type, Direction orientation);


    void clockwiseRotate();
    void counterClockwiseRotate();
    void move(Direction direction);

    Direction getOrientation();
    glm::vec2 getPosition();

private:
    glm::vec2 _topLeftPosition;
    Piece_Type* _type;
    Direction _orientation;
};

inline Direction & operator++(Direction & state, int);
inline Direction & operator--(Direction & type, int);

*/

void print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec);
bool isAlreadyIn2dVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO);
int generateVAOFromMatrix (/*Piece_Type pieceType*/);








#endif //GLITTER_PIECE_HPP
