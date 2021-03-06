#include "Piece.hpp"
/*
Piece::Piece(Piece_Type* type)
    : _type(type)
{
    _topLeftPosition = glm::vec2(0, 0);
    _orientation = UP;
}

Piece::Piece(glm::vec2 position, Piece_Type* type, Direction orientation)
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
 */

void printVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec) {
  std::cout << "{";
  std::vector<glm::vec2>::size_type j;
  for (auto & i : VBO)
  {
    std::cout << "{";
    for (j = 0; j < sizeOfVec; j++ )
    {
      std::cout << i[j];
      if(j != sizeOfVec -1) {
        std::cout << ',';
      }
    }
    std::cout << "}," << std::endl;
  }
  std::cout << "}" << std::endl;
}

bool isAlreadyInVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO, int sizeOfVec) {

  for (auto & i : VBO)
  {
    if (abs(point[0]-i[0]) < EPSILON && abs(point[1]-i[1]) < EPSILON) {
      return true;
    }
  }
  return false;
}

int generateVAOFromMatrix (/*Piece_Type pieceType*/)
{
  std::string lineBuffer;
  /* @TODO chemin relatif ? */
  std::ifstream file("/home/nathan/CLionProjects/opengl-project-puzzle-game/game/piece/MatrixFile.txt");
  if (!file.is_open()){
    std::cerr << "Error: " << strerror(errno) << std::endl;
  }
  std::vector<glm::vec2> positionVbo;
  std::vector<glm::vec2> tilesOccupied;

  /*
  positionVBO.emplace_back(glm::vec2(0,0));
  positionVBO.emplace_back(glm::vec2(0,1));
  positionVBO.emplace_back(glm::vec2(5,6));
  printVBO(positionVBO, 2);
   */





  // VAO vao(2); /* 2 ?? 1 ?? */
  int lineNumber = 0;
  int firstLine = 8; /* @todo à déterminer en fonction de pieceType dans un second temps*/





  while (lineNumber != firstLine && getline(file, lineBuffer)) {
    std::cout<< "Ligne n°" << lineNumber << " = " << lineBuffer << std::endl;
    lineNumber++;
  }

  if (lineNumber != firstLine) {
    std::cout << "Reading file error" << std::endl;
  }
  /* en faire une fonction à part ??? */

  int minx = 4, maxx = 0, miny = 4, maxy = 0;

  for (lineNumber = 0; lineNumber <= 3; lineNumber++) {
    getline(file, lineBuffer);
    std::cout<< "Ligne n°" << lineNumber << " de la matrice qui nous intéresse = " << lineBuffer << std::endl;

    for (int columnNumber = 0; columnNumber <= 3; columnNumber++) {
      std::cout<< "Colonne n°" << columnNumber << std::endl;

      std::cout<< "En ("<< columnNumber << ";" << lineNumber <<"), on a : " << lineBuffer.at(columnNumber) << std::endl;
      if (lineBuffer.at(columnNumber) == '1') {

        tilesOccupied.emplace_back(columnNumber,lineNumber);

        if (lineNumber < miny) {
          std::cout << "new min y" << std::endl;
          miny = lineNumber;
        }
        if (lineNumber > maxy) {
          std::cout << "new max y" << std::endl;
          maxy = lineNumber;
        }
        if (columnNumber < minx) {
          std::cout << "new min x" << std::endl;
          minx = columnNumber;
        }
        if (columnNumber > maxx) {
          std::cout << "new max x" << std::endl;
          maxx = columnNumber;
        }
        }
    }

  }

  file.close();

  int height = maxy - miny + 1;
  int width = maxx - minx + 1;
  std::cout << "height = " << height << std::endl;
  std::cout << "width = " << width << std::endl;
  int maxInHeightAndWidth = std::max(height,width);
  std::cout << "min = " << maxInHeightAndWidth << std::endl;


  printVBO(tilesOccupied, 2);

  glm::vec2 vecToPushBack;
  /* careful : search columns */
  for (auto & i : tilesOccupied)
  {
    std::cout <<"new tile" << std::endl;
    for (unsigned int deltax = 0; deltax <=1; deltax++) {

      for (unsigned int deltay = 0; deltay <=1; deltay++) {
        /* @todo normalization problem when height != width */
        vecToPushBack = glm::vec2((i[0] + (float)deltax) *(2.0/maxInHeightAndWidth) - 1, (i[1] + (float)deltay) *(2.0/maxInHeightAndWidth) - 1);
        if(!isAlreadyInVBO(vecToPushBack, positionVbo, 2)) {
          std::cout<< vecToPushBack[0] << " " << vecToPushBack[1] << std::endl;
          positionVbo.emplace_back(vecToPushBack);
        }
      }
    }
  }
  printVBO(positionVbo, 2);




  return 0;
}

  /*----------------------------------*/
