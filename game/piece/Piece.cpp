#include <glm/gtc/matrix_transform.hpp>
#include "Piece.hpp"

Piece::Piece(Piece_Type type, float scale)
    : _type(type), _scale(scale)
{
    _angle = 0;
    _scale = 1;
    _translationVector = glm::vec2(0, 0);
    _model = glm::mat4(1);
    generateVAOFromMatrix();
}

glm::mat4 Piece::getModel() {
    return _model;
}

void Piece::setScale(float scale) {
    _scale = scale;
    changeModelMatrix();
}

void Piece::clockwiseRotate() {
    _angle = (_angle - 90) % 360;
    changeModelMatrix();
}

void Piece::counterClockwiseRotate() {
    _angle = (_angle + 90) % 360;
    changeModelMatrix();
}

void Piece::move(Direction direction) {
    switch (direction) {
        case UP:
            _translationVector.y += 2 * _scale;
            break;
        case RIGHT:
            _translationVector.x += 2 * _scale;
            break;
        case DOWN:
            _translationVector.y -= 2 * _scale;
            break;
        case LEFT:
            _translationVector.x -= 2 * _scale;
            break;
        default:
            throw "Error: incorrect direction";
    }
    changeModelMatrix();
}

float Piece::convertAngleToRadian() {
    return static_cast<float>(_angle * 2 * M_PI /360.);
}

void Piece::changeModelMatrix() {
    _model = glm::translate(glm::mat4(1), glm::vec3(_translationVector, 0));
    _model = glm::rotate(_model, convertAngleToRadian(),glm::vec3(0, 0, 1));
    _model = glm::scale(_model, glm::vec3(_scale, _scale, 1));
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