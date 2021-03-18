#ifndef GLITTER_PIECE_HPP
#define GLITTER_PIECE_HPP

#include <fstream>
#include "glApi.hpp"
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
    LEFT,
    NONE
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
    void setPositionVBO(std::vector<glm::vec2> const & VBO);
    void setColorVBO(std::vector<glm::vec3> const & VBO);
    void setIBO(std::vector<uint> IBO);
    void setTopLeftPosition(glm::vec2 topLeftPosition);
    void draw(GLenum mode);
    void verifVAO();

private:
    glm::vec2 _topLeftPosition;
    Piece_Type _type;
    int _angle;
    VAO _vao;
    glm::vec2 _transVect;

  public:
    const glm::vec2 & getTransVect() const;
    void setTransVect(const glm::vec2 & transVect);

  private:
    glm::vec3 generateRandomColorVector();
    void print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec);
    void print3dVBO(const std::vector<glm::vec3> &VBO, int sizeOfVec);
    void printIBO(const std::vector<uint>& IBO);
    int positionInVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO);
  public:
    void generateVAOFromMatrix();
};

/* tmp pour teste la suite */




//void generateVAOFromMatrixv2 (Piece_Type pieceType);
//void printMatrix4x4(bool matrix[4][4]);

#endif //GLITTER_PIECE_HPP
