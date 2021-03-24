#include <fstream>
#include "Shape.hpp"

Shape::Shape() : _vao(2) {}

Shape::Shape(glm::mat4 model) : _vao(2) {
    generateVAOFromMatrix();
    _squareOriginsPositions = _squarePositions;
    changeSquarePosition(model);
}

glm::vec3 Shape::generateRandomColorVector() {
    glm::vec3 colorVector;
    colorVector = {1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX};
    return colorVector;
}

int Shape::positionInVBO(glm::vec2 point, const std::vector<glm::vec2> &VBO) {
    int index = 0;
    for (auto &i : VBO) {
        if (abs(point[0] - i[0]) < EPSILON && abs(point[1] - i[1]) < EPSILON) {
            return index;
        }
        index++;
    }
    return -1;
}

void Shape::generateVAOFromMatrix() {
    std::string lineBuffer;
    std::ifstream file("../game/piece/shapes_file.txt");
    if (!file.is_open()) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
    std::vector<glm::vec2> positionVBO;
    std::vector<glm::vec3> colorVBO;
    std::vector<glm::vec2> tilesOccupied;
    std::vector<uint> IBO;
    int index = 0;
    int square_index[4];
    glm::vec3 colorVector = {0.5, 0.5, 0.5};
    int lineNumber = 0;
    int firstLine = 0;

    while (lineNumber != firstLine && getline(file, lineBuffer)) {
        lineNumber++;
    }
    if (lineNumber != firstLine) {
        std::cout << "Reading file error" << std::endl;
    }
    int minx = 10, maxx = 0, miny = 10, maxy = 0;
    for (lineNumber = 0; lineNumber <= 9; lineNumber++) {
        getline(file, lineBuffer);
        for (int columnNumber = 0; columnNumber <= 9; columnNumber++) {
            if (lineBuffer.at(columnNumber) == '1') {
                tilesOccupied.emplace_back(columnNumber, lineNumber);

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

    glm::vec2 vecToPushBackinPositionVBO;
    for (auto &i : tilesOccupied) {
        for (unsigned int deltax = 0; deltax <= 1; deltax++) {
            for (unsigned int deltay = 0; deltay <= 1; deltay++) {

                vecToPushBackinPositionVBO = glm::vec2(2 * (i[0] + (float) deltax) - 1,
                                                       -(2 * (i[1] + (float) deltay) - 1));
                if (deltax == 0 && deltay == 0) {
                    _squarePositions.emplace_back(
                            glm::vec2(2 * (i[0] + (float) deltax) - 1, -(2 * (i[1] + (float) deltay) - 1)));
                }
                if (positionInVBO(vecToPushBackinPositionVBO, positionVBO) != -1) {
                    square_index[2 * deltax + deltay] = positionInVBO(vecToPushBackinPositionVBO, positionVBO);
                }
                if (positionInVBO(vecToPushBackinPositionVBO, positionVBO) == -1) {
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

void Shape::draw(GLenum mode) {
    _vao.draw(mode);
}

glm::vec4 Shape::coordinateToHomogene(glm::vec2 normalCoordinates) {
    return glm::vec4(normalCoordinates, 0, 1);
}

glm::vec2 Shape::homogeneToCoordinate(glm::vec4 homogeneCoordinates) {
    float w = homogeneCoordinates[3];
    return glm::vec2(homogeneCoordinates[0] / w, homogeneCoordinates[1] / w);
}

std::vector<glm::vec2> Shape::getSquarePositions() {
    return _squarePositions;
}

std::vector<glm::vec4> Shape::coordinateToHomogeneV(const std::vector<glm::vec2> &normalCoordinates) {
    std::vector<glm::vec4> res;

    for (auto &o : normalCoordinates) {
        res.push_back(coordinateToHomogene(o));
    }

    return res;
}

std::vector<glm::vec2> Shape::homogeneToCoordinateV(const std::vector<glm::vec4> &homogeneCoordinates) {
    std::vector<glm::vec2> res;
    for (auto &o : homogeneCoordinates) {
        res.push_back(homogeneToCoordinate(o));
    }
    return res;
}

std::vector<glm::vec4> Shape::matrixTransformation(const std::vector<glm::vec4> & vect, glm::mat4 model) {
    std::vector<glm::vec4> res;
    for (auto &i : vect) {
        glm::vec4 tmp = model * i;
        res.push_back(tmp);
    }
    return res;
}

void Shape::changeSquarePosition(glm::mat4 model) {
    std::vector<glm::vec4> homCoord = matrixTransformation(coordinateToHomogeneV(_squareOriginsPositions), model);
    _squarePositions = homogeneToCoordinateV(homCoord);
}
