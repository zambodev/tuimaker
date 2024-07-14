#include "box.hpp"


Box::Box(int x, int y, int width, int height) {
    std::wcout << L"Creating Box!\n";
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->buffer = new wchar_t[width * height];

    for(int i = 0; i < width * height; ++i)
        this->buffer[i] = u'\u0020';

    this->draw();
}

Box::~Box() {
    delete this->buffer;
}

void Box::setIsSelected(bool isSelected) {
    this->isSelected = isSelected;
}

bool Box::getIsSelected(void) {
    return this->isSelected;
}

void Box::setIsSelectable(bool isSelectable) {
    this->isSelectable = isSelectable;
}

bool Box::getIsSelectable(void) {
    return this->isSelectable;
}

void Box::setIsWritable(bool isWritable) {
    this->isWritable = isWritable;
}

bool Box::getIsWritable(void) {
    return this->isWritable;
}

void Box::draw(void) {
    for(int i = 0; i < this->width; ++i) {
        this->buffer[i] = hbar;
        this->buffer[this->width * (this->height - 1) + i] = hbar;
    }

    std::wcout << L"\x1b[s\x1b[0;0H" << this->buffer << L"\x1b[s\x1b[10;0H";
}
