#include "window.hpp"


Window::Window(int x, int y, int width, int height) : Box(x, y, width, height) {
    std::wcout << L"Creating Window!\n";
}

Window::~Window() {

}

void Window::setWidth(int width) {
    this->width = width;
}

int Window::getWidth(void) {
    return this->width;
}

void Window::setHeight(int height) {
    this->height = height;
}

int Window::getHeight(void) {
    return this->height;
}

