#ifndef GLITTER_SIMPLEAPP_HPP
#define GLITTER_SIMPLEAPP_HPP

#include "Application.hpp"
#include "glApi.hpp"
#include "../piece/Piece.hpp"

struct GLFWwindow;

class SimpleApp : public Application {
public:
    SimpleApp(int windowWidth, int windowHeight);
    void setCallbacks() override;
    void renderFrame() override;
    void update() override;
    static void resize(GLFWwindow * window, int framebufferWidth, int framebufferHeight);

private:
    class RenderObject {
    public:
        RenderObject() = delete;
        RenderObject(const RenderObject &) = delete;

        /**
         * @brief creates an instance from a vao and modelView matrix
         * @param program  a GLSL program
         * @param vao  the VAO to be instanciated
         * @param modelWorld the matrix transform between the object (a.k.a model) space and the camera (a.k.a view) space
         * @return the created InstancedVAO as a smart pointer
         */
        static std::shared_ptr<RenderObject> createInstance(const std::shared_ptr<Program> & prog, const std::shared_ptr<Piece> & piece);
        static std::shared_ptr<RenderObject> createInstance(const std::shared_ptr<Program> & prog, const std::shared_ptr<Shape> & shape, float scale);

      /**
         * @brief Draw this VAO
         */
        void draw(GLenum mode = GL_TRIANGLES) const;

        std::shared_ptr<Piece> getPiece();
        std::shared_ptr<Shape> getShape();

    private:
        RenderObject(const std::shared_ptr<Program> & prog, const std::shared_ptr<Piece> & piece);
        RenderObject(const std::shared_ptr<Program> & prog, const std::shared_ptr<Shape> & shape, float scale);

        /**
         * @brief update the program M uniform variable
         */
        void updateProgram() const;

    private:
        std::shared_ptr<Program> m_prog; ///< Program
        std::shared_ptr<Piece> m_piece;   ///< Piece
        std::shared_ptr<Shape> m_shape;
        float _scale;
    };

private:
    std::vector<std::shared_ptr<RenderObject>> m_pieces; ///< List of instanced Pieces (VAO + modelView matrix)
    std::vector<std::shared_ptr<RenderObject>> m_shapes; ///< list of instanced Shapes (only VAO)
    std::shared_ptr<Program> m_program;                ///< A GLSL progam
    std::shared_ptr<RenderObject> _currentPiece;
    glm::mat4 _shape_model;
    int _currentPieceIndex;
    int nb_piece;
    float _scale;

    void makeA2DShape(std::shared_ptr<Piece> piece);
    void makeA2DShape(std::shared_ptr<Shape> shape);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void rotatePiece(int direction);
    void movePiece(Direction direction);
    std::shared_ptr<RenderObject> givePointerInVector();
    void changeCurrentPiece(int mode);

    bool squareCollision(glm::vec2 square1, glm::vec2 square2);
    bool shapeCollision(const std::shared_ptr<Shape>& shape1, const std::shared_ptr<Shape>& shape2);
    //check if shape1 is superposed on shape2
    bool shapeSuperposition(const std::shared_ptr<Shape>& shape1, const std::shared_ptr<Shape>& shape2);
    bool allPiecesCollision();
    bool currentPieceShapeSuperposition();

    void updateIsWellPlaced();
    bool verifVictory();


};

#endif // GLITTER_SIMPLEAPP_HPP