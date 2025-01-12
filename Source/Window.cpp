#include "../Include/Window.hpp"
#include "../Include/WindowManager.hpp"
#include "../Include/Utils.hpp"


namespace tmk
{
    Window::Window(WindowSize wsize)
    : size(wsize),
      buffer(std::make_shared<wchar_t[]>(wsize.width * wsize.height)),
      id(Utils::GetProgressiveId()), isSelectable(false),
      isSelected(false), isWritable(false),
      cursorX(1), cursorY(1)
    {
        for(int i = 0; i < wsize.width * wsize.height; ++i)
            this->buffer[i] = U_SPACE;

        this->Draw();
    }

    Window::~Window()
    {
    }

    bool Window::operator==(Window &window)
    {
        if(this->id == window.GetId())
            return true;
        else
            return false;
    }

    const WindowSize &Window::GetSize(void) const
    {
        return this->size;
    }

    void Window::SetSelected(bool isSelected)
    {
        this->isSelected = isSelected;
    }

    bool Window::IsSelected(void) const
    {
        return this->isSelected;
    }

    void Window::SetSelectable(bool isSelectable)
    {
        this->isSelectable = isSelectable;
    }

    bool Window::IsSelectable(void) const
    {
        return this->isSelectable;
    }

    void Window::SetWritable(bool isWritable)
    {
        this->isWritable = isWritable;
    }

    bool Window::IsWritable(void) const
    {
        return this->isWritable;
    }

    int Window::GetId(void) const
    {
        return this->id;
    }

    wchar_t Window::GetCharAt(int x, int y) const
    {
        return this->buffer[y * this->size.width + x];
    }

    std::shared_ptr<wchar_t[]> Window::GetBuffer(void)
    {
        return this->buffer;
    }

    void Window::Draw(void)
    {
        // Top and bottom sides
        for(int i = 1; i < this->size.width - 1; ++i)
        {
            this->buffer[i] = U_BAR_HORIZONTAL;
            this->buffer[this->size.width * (this->size.height - 1) + i] = U_BAR_HORIZONTAL;
        }
        // Left and right sides
        for(int i = 1; i < this->size.height - 1; ++i)
        {
            this->buffer[i * this->size.width] = U_BAR_VERTICAL;
            this->buffer[i * this->size.width + (this->size.width - 1)] = U_BAR_VERTICAL;
        }
        // Corners
        this->buffer[0] = U_CRN_TOP_LEFT;
        this->buffer[this->size.width - 1] = U_CRN_TOP_RIGHT;
        this->buffer[(this->size.height - 1) * this->size.width] = U_CRN_BOTTOM_LEFT;
        this->buffer[(this->size.height - 1) * this->size.width + (this->size.width - 1)] = U_CRN_BOTTOM_RIGHT;
    }
}
