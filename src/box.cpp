#include "box.hpp"


Box::Box(int x, int y, int width, int height) {
    std::cout << "Creating Box!\n";
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->buffer = (wchar_t *)calloc(width * height, sizeof(wchar_t));

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
    setlocale(LC_ALL, "");
    for(int i = 0; i < this->width; ++i) {
        this->buffer[i] = hbar;
        this->buffer[this->width * (this->height - 1) + i] = hbar;
    }

    std::wcout << L"\u251b";

    std::wcout << L"\x1b[s\x1b[0;0H";
    for(int i = 0; i < this->width * this->height; ++i) {
		std::wcout << this->buffer[i];
    }
}
