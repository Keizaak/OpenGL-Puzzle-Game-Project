#include "Piece.hpp"
#include <iostream>

int main() {
//    Piece piece(RECTANGLE_1x2);
//
//    /*
//    std::cout << piece.getOrientation() << std::endl;
//    piece.clockwiseRotate();
//    std::cout << piece.getOrientation() << std::endl;
//    piece.counterClockwiseRotate();
//    piece.counterClockwiseRotate();
//    std::cout << piece.getOrientation() << std::endl;
//    */
//
//    piece.move(UP);
//    piece.move(RIGHT);
//    std::cout << piece.getPosition().x << " " << piece.getPosition().y << std::endl;

    /* test getVAOFromMatrix */

  VAO vao(2);
  Piece piece(Piece_Type::BIG_L);
  piece.generateVAOFromMatrix(Piece_Type::BIG_L);


    return 0;
}
