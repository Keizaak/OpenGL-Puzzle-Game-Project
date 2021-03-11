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

void Piece::setColorVBO(std::vector<glm::vec3> const & VBO) {
    _vao.setVBO(1, VBO);
}

void Piece::setPositionVBO(std::vector<glm::vec2> const & VBO) {
    _vao.setVBO(0, VBO);
}

void Piece::setIBO(std::vector<uint> IBO) {
    _vao.setIBO(IBO);
}

void Piece::draw() {
    _vao.draw();
}



glm::vec3 Piece::generateRandomColorVector() {
  glm::vec3 colorVector;
  colorVector = {0.5,1,1};

  /*
  for (int i = 0; i < 3; i++) {

  }
   */
  return colorVector;
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

void Piece::print3dVBO(const std::vector<glm::vec3> &VBO, int sizeOfVec) {
  std::cout << "{";
  std::vector<glm::vec3>::size_type j;
  for (auto & i : VBO)
  {
    std::cout << "{";
    for (j = (std::vector<glm::vec3>::size_type)0; j < (std::vector<glm::vec3>::size_type)sizeOfVec; j++ )
    {
      std::cout << i[j];
      if(j != (std::vector<glm::vec3>::size_type)sizeOfVec -1) {
        std::cout << ',';
      }
    }
    std::cout << "}," << std::endl;
  }
  std::cout << "}" << std::endl;
}

void Piece::printIBO(const std::vector<uint>& IBO) {
  std::cout << "{";
  for (auto & i : IBO)
  {
    std::cout << i << ", ";
  }
  std::cout << "}" << std::endl;
}

int Piece::positionInVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO) {
  int index = 0;
  for (auto & i : VBO)
  {
    if (abs(point[0]-i[0]) < EPSILON && abs(point[1]-i[1]) < EPSILON) {
      return index;
    }
    index++;
  }
  return -1;
}


void Piece::generateVAOFromMatrix (Piece_Type pieceType) {
  std::string lineBuffer;
  std::ifstream file("../game/piece/MatrixFile.txt");
  if (!file.is_open()){
    std::cerr << "Error: " << strerror(errno) << std::endl;
  }

  std::vector<glm::vec2> positionVBO;
  std::vector<glm::vec3> colorVBO;
  std::vector<glm::vec2> tilesOccupied;
  std::vector<uint> IBO;
  int index = 0;
  int square_index[4];

  glm::vec3 colorVector = generateRandomColorVector();

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

  glm::vec2 vecToPushBackinPositionVBO;
  for (auto & i : tilesOccupied) {

    for (unsigned int deltax = 0; deltax <=1; deltax++) {

      for (unsigned int deltay = 0; deltay <=1; deltay++) {

        if (width > height) {
          vecToPushBackinPositionVBO = glm::vec2(
              (i[0] + (float)deltax)*(2.0/width) - 1,
              ((i[1] + (float)deltay)*(2.0/width) - 1  +  (((float)width-(float)height)/(float)width)  )
          );
        } else {
          vecToPushBackinPositionVBO = glm::vec2(
              ((i[0] + (float)deltax)*(2.0/height) - 1  +  (((float)height-(float)width)/(float)height)  ),
              (i[1] + (float)deltay)*(2.0/height) - 1
          );
        }


        std::cout<< vecToPushBackinPositionVBO[0] << " " << vecToPushBackinPositionVBO[1] << std::endl;

        if(positionInVBO(vecToPushBackinPositionVBO, positionVBO) != -1) {
          square_index[2 * deltax + deltay] = positionInVBO(vecToPushBackinPositionVBO, positionVBO);
        }

        if(positionInVBO(vecToPushBackinPositionVBO, positionVBO) == -1) {
          positionVBO.emplace_back(vecToPushBackinPositionVBO);
          square_index[2 * deltax + deltay] = index;
          index++;
          colorVBO.emplace_back(colorVector);
        }
      }

    }
    IBO.emplace_back(square_index[0]);
    IBO.emplace_back(square_index[1]);
    IBO.emplace_back(square_index[2]);
    IBO.emplace_back(square_index[1]);
    IBO.emplace_back(square_index[2]);
    IBO.emplace_back(square_index[3]);
  }
  //_vao.setVBO(0, positionVBO);
  //_vao.setVBO(1,colorVBO);
  //_vao.setIBO(IBO);
  std::cout << "VBO position =" << std::endl;
  print2dVBO(positionVBO, 2);
  std::cout << "VBO couleur =" << std::endl;
  print3dVBO(colorVBO, 3);
  std::cout << "IBO =" << std::endl;
  printIBO(IBO);
  std::cout << "La couleur doit être cyan" << std::endl;
}

/* tentative de générérer les VBOs autrement, à garder sous le code au cas ou*/
/*
void printMatrix4x4(bool matrix[4][4]) {
  for (int i=0; i<=3; i++) {
    for (int j=0; j<=3; j++) {
      std::cout << matrix[i][j];
    }
    std::cout << std::endl;
  }
}
void generateVAOFromMatrixv2 (Piece_Type pieceType) {
  std::string lineBuffer;
  std::ifstream file("../game/piece/MatrixFile.txt");
  if (!file.is_open()){
    std::cerr << "Error: " << strerror(errno) << std::endl;
  }

  std::vector<glm::vec2> positionVBO;
  std::vector<glm::vec3> colorVBO;
  bool matrix [4][4];
  std::vector<uint> IBO;

  glm::vec3 colorVector = generateRandomColorVector();

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

        matrix[columnNumber][lineNumber] = true;

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

      else {
        matrix[columnNumber][lineNumber] = false;
      }
    }

  }

  file.close();

  int height = maxy - miny + 1;
  int width = maxx - minx + 1;
  std::cout << "height = " << height << std::endl;
  std::cout << "width = " << width << std::endl;

  printMatrix4x4(matrix);

  glm::vec2 vecToPushBackinPositionVBO;

  for (unsigned int j = 0; j <=3; j++) {
    for (unsigned int i = 0; i <=3; i++) {
      std::cout << "New i =" << i << std::endl;
      if (matrix[j][i]) {

        for (unsigned int k = 0; k <=1; k++) {
          if (width > height) {
              vecToPushBackinPositionVBO = glm::vec2(
                  (i + k) * (2.0 / width) - 1,
                  ((j) * (2.0 / width) - 1 + (((float)width - (float)height) / (float)width)));
              std::cout << vecToPushBackinPositionVBO[0] << " " << vecToPushBackinPositionVBO[1];

              if (isAlreadyIn2dVBO(vecToPushBackinPositionVBO, positionVBO)) {
                std::cout << " IS IN VBO " << std::endl;
              }
              if (!isAlreadyIn2dVBO(vecToPushBackinPositionVBO, positionVBO)) {
                std::cout << " IS NOT IN VBO " << std::endl;
                positionVBO.emplace_back(vecToPushBackinPositionVBO);
                colorVBO.emplace_back(colorVector);
              }

          } else {
            for (unsigned int l = 0; l <=1; l++) {
              vecToPushBackinPositionVBO = glm::vec2(
                  ((i + k + l)*(2.0/height) - 1  +  (((float)height-(float)width)/(float)height)  ),
                  (j + k + l) *(2.0/height) - 1
              );
              if (!positionInVBO(vecToPushBackinPositionVBO, positionVBO)) {
                std::cout << vecToPushBackinPositionVBO[0] << " " << vecToPushBackinPositionVBO[1] << std::endl;
                positionVBO.emplace_back(vecToPushBackinPositionVBO);
                colorVBO.emplace_back(colorVector);
              }
            }
          }
        }
      }
    }
  }
  //_vao.setVBO(0, positionVBO);
  //_vao.setVBO(1,colorVBO);
  //print2dVBO(positionVBO, 2);
  //print3dVBO(colorVBO, 3);
  std::cout << "La couleur doit être cyan" << std::endl;
}

 */

