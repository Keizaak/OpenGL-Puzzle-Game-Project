#ifndef GLITTER_PIECE_HPP
#define GLITTER_PIECE_HPP

#include <fstream>
#include <glApi.hpp>
#include <glm/vec2.hpp>

#define EPSILON 0.01 /* for equality between 2 floats in the VBO */

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
    LEFT
};

class Piece {
public:
    Piece(Piece_Type type);
    Piece(glm::vec2 position, Piece_Type type, int angle);

    void clockwiseRotate();
    void counterClockwiseRotate();
    void move(Direction direction);

    int getAngle();
    glm::vec2 getPosition();
    void setVBO(int index, std::vector<glm::vec2> VBO);
    void setIBO(std::vector<uint> IBO);
    void draw(GLenum mode);

private:
    glm::vec2 _topLeftPosition;
    Piece_Type _type;
    int _angle;
    VAO _vao;

    glm::vec3 generateRandomColor();
    void print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec);
    bool isAlreadyIn2dVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO);
    void generateVAOFromMatrix(Piece_Type pieceType);
};

#endif //GLITTER_PIECE_HPP
