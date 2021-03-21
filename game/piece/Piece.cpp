#include "Piece.hpp"

Piece::Piece(Piece_Type type)
    : _type(type)
{
    _topLeftPosition = glm::vec2(0, 0);
    _angle = 0;
    _transVect = glm::vec2(0, 0);
    generateVAOFromMatrix();
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

int Piece::getAngle() const {
    return _angle;
}

glm::vec2 Piece::getPosition() {
    return _topLeftPosition;
}

void Piece::setTopLeftPosition(glm::vec2 topLeftPosition){
  _topLeftPosition = topLeftPosition;
}

void Piece::generateVAOFromMatrix () {
    std::string lineBuffer;
    std::ifstream file("../game/piece/pieces_file.txt");
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
    int firstLine = 4 * (int)_type;

    while (lineNumber != firstLine && getline(file, lineBuffer)) {
        lineNumber++;
    }
    if (lineNumber != firstLine) {
        std::cout << "Reading file error" << std::endl;
    }
    int minx = 4, maxx = 0, miny = 4, maxy = 0;
    for (lineNumber = 0; lineNumber <= 3; lineNumber++) {
        getline(file, lineBuffer);
        for (int columnNumber = 0; columnNumber <= 3; columnNumber++) {
            if (lineBuffer.at(columnNumber) == '1') {
                tilesOccupied.emplace_back(columnNumber,lineNumber);

                if (lineNumber < miny) {
                    miny = lineNumber;
                }
                if (lineNumber > maxy) {
                    maxy = lineNumber;
                }
                if (columnNumber < minx) {
                    minx = columnNumber;
                }
                if (columnNumber > maxx) {
                    maxx = columnNumber;
                }
            }
        }
    }
    file.close();
    int height = maxy - miny + 1;
    int width = maxx - minx + 1;
    glm::vec2 vecToPushBackinPositionVBO;
    for (auto & i : tilesOccupied) {
        for (unsigned int deltax = 0; deltax <=1; deltax++) {
            for (unsigned int deltay = 0; deltay <=1; deltay++) {
                vecToPushBackinPositionVBO = glm::vec2(2 * (i[0] + (float)deltax) - 1 ,-(2 * (i[1] + (float)deltay) - 1));
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
    _vao.setVBO(0, positionVBO);
    _vao.setVBO(1, colorVBO);
    _vao.setIBO(IBO);
}

const glm::vec2 & Piece::getTransVect() const
{
  return _transVect;
}
void Piece::setTransVect(const glm::vec2 & transVect)
{
  _transVect = transVect;
}