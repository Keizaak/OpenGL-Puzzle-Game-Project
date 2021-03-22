#include <fstream>
#include "Shape.hpp"

#define EPSILON 0.01 /* for equality between 2 floats in the VBO */

Shape::Shape() : _vao(2)
{
  generateVAOFromMatrix();
  yolo = 1;
}

void Shape::setColorVBO(std::vector<glm::vec3> const & VBO) {
    _vao.setVBO(1, VBO);
}

void Shape::setPositionVBO(std::vector<glm::vec2> const & VBO) {
    _vao.setVBO(0, VBO);
}

void Shape::setIBO(std::vector<uint> const & IBO) {
    _vao.setIBO(IBO);
}


glm::vec3 Shape::generateRandomColorVector() {
    glm::vec3 colorVector;
    colorVector = {0.5,0.5,0.5};
    /* @TODO gérer l'aléatoire + hsv format */
    //colorVector = {rand()/RAND_MAX,1,1};
    return colorVector;
}

int Shape::positionInVBO(glm::vec2 point /* @todo RENAME */, const std::vector<glm::vec2>& VBO) {
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

void Shape::print2dVBO(const std::vector<glm::vec2> &VBO, int sizeOfVec) {
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

void Shape::print3dVBO(const std::vector<glm::vec3> &VBO, int sizeOfVec) {
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

void Shape::printIBO(const std::vector<uint>& IBO) {
    std::cout << "{";
    for (auto & i : IBO)
    {
        std::cout << i << ", ";
    }
    std::cout << "}" << std::endl;
}

void Shape::generateVAOFromMatrix () {
    std::string lineBuffer;
    std::ifstream file("../game/piece/shapes_file.txt");
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
    std::cout << "Color VBO" << std::endl;
    print3dVBO(colorVBO, 3);
    std::cout << "Color VBO" << std::endl;
    _vao.setVBO(0, positionVBO);
    _vao.setVBO(1, colorVBO);
    _vao.setIBO(IBO);

}

void Shape::draw(GLenum mode) {
    _vao.draw(mode);
}