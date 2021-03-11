#include "Piece.hpp"

Piece::Piece(Piece_Type type)
    : _type(type), _vao(2)
{
    _topLeftPosition = glm::vec2(0, 0);
    _angle = 0;
    //generateVAOFromMatrix(_type);
}

Piece::Piece(glm::vec2 position, Piece_Type type, int angle)
    : _topLeftPosition(position), _type(type), _angle(angle), _vao(2)
{
    //generateVAOFromMatrix(_type);
}

void Piece::clockwiseRotate() {
    _angle = (_angle + 90) % 360;
}

void Piece::counterClockwiseRotate() {
    _angle = (_angle - 90) % 360;
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

int Piece::getAngle() {
    return _angle;
}

glm::vec2 Piece::getPosition() {
    return _topLeftPosition;
}

void Piece::setVBO(int index, std::vector<glm::vec2> VBO) {
    _vao.setVBO(index, VBO);
}

void Piece::setIBO(std::vector<uint> IBO) {
    _vao.setIBO(IBO);
}

void Piece::draw(GLenum mode) {
    _vao.draw(mode);
}

glm::vec3 generateRandomColor() {
    glm::vec3 color;
    for (int i = 0; i < 3; i++) {

    }
}


void Piece::print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec) {
  std::cout << "{";
  std::vector<glm::vec2>::size_type j;
  for (auto & i : VBO)
  {
    std::cout << "{";
    for (j = (std::vector<glm::vec2>::size_type)0; j < (std::vector<glm::vec2>::size_type)sizeOfVec; j++ )
    {
      std::cout << i[j];
      if(j != (std::vector<glm::vec2>::size_type)sizeOfVec -1) {
        std::cout << ',';
      }
    }
    std::cout << "}," << std::endl;
  }
  std::cout << "}" << std::endl;
}

bool Piece::isAlreadyIn2dVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO) {

  for (auto & i : VBO)
  {
    if (abs(point[0]-i[0]) < EPSILON && abs(point[1]-i[1]) < EPSILON) {
      return true;
    }
  }
  return false;
}

void Piece::generateVAOFromMatrix (Piece_Type pieceType) {
  std::string lineBuffer;
  std::ifstream file("../game/piece/MatrixFile.txt");
  if (!file.is_open()){
    std::cerr << "Error: " << strerror(errno) << std::endl;
  }

  std::vector<glm::vec2> positionVBO;
  std::vector<glm::vec2> tilesOccupied;
  std::vector<uint> IBO;

  int lineNumber = 0;
  int firstLine = 4 * (int)pieceType;

  while (lineNumber != firstLine && getline(file, lineBuffer)) {
    std::cout<< "Ligne n°" << lineNumber << " = " << lineBuffer << std::endl;
    lineNumber++;
  }

  if (lineNumber != firstLine) {
    std::cout << "Reading file error" << std::endl;
  }

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

  print2dVBO(tilesOccupied, 2);

  glm::vec2 vecToPushBack;
  for (auto & i : tilesOccupied) {
    std::cout <<"new tile" << std::endl;
    for (unsigned int deltax = 0; deltax <=1; deltax++) {

      for (unsigned int deltay = 0; deltay <=1; deltay++) {
        if (width > height) {
          vecToPushBack = glm::vec2(
              (i[0] + (float)deltax)*(2.0/width) - 1,
              ((i[1] + (float)deltay)*(2.0/width) - 1  +  (((float)width-(float)height)/(float)width)  )
              );
        } else {
          vecToPushBack = glm::vec2(
              ((i[0] + (float)deltax)*(2.0/height) - 1  +  (((float)height-(float)width)/(float)height)  ),
              (i[1] + (float)deltay)*(2.0/height) - 1
          );
        }

        if(!isAlreadyIn2dVBO(vecToPushBack, positionVBO)) {
          std::cout<< vecToPushBack[0] << " " << vecToPushBack[1] << std::endl;
          positionVBO.emplace_back(vecToPushBack);
        }
      }
    }
  }
  _vao.setVBO(0, positionVBO);
  print2dVBO(positionVBO, 2);
}
