#include "command.h"

Direction getDirectionFromKeyboard(int key) {
    switch (key) {
        case GLFW_KEY_LEFT:
        case GLFW_KEY_A:
            return Direction::LEFT;
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_D:
            return Direction::RIGHT;
        case GLFW_KEY_UP:
        case GLFW_KEY_W:
            return Direction::UP;
        case GLFW_KEY_DOWN:
        case GLFW_KEY_S:
            return Direction::DOWN;
        default:
            return Direction::NONE;
    }
}