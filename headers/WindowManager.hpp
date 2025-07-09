#pragma once

#include <vector>
#include <memory>
#include <map>
#include <cstring>
#include "Window.hpp"
#include "Utils.h"
#include "WindowPtr.hpp"

namespace tmk
{
    class WindowManager
    {
    public:
        WindowManager(const WindowManager &obj) = delete;

        static WindowManager *get_instance(void)
        {
            static WindowManager *instance = new WindowManager();
            return instance;
        }

        template <typename T>
        WindowPtr<T> create_window(WindowSize wsize, WindowId father)
        {
            if (!std::is_base_of<Window, T>::value)
            {
                throw std::runtime_error("Type should derive from Window!");
            }

            auto window = WindowPtr<T>(wsize);
            WindowId id = window->get_id();
            window_map_.emplace(id, window);

            if (father > 0)
            {
                auto father_win = window_map_.find(father)->second;
                father_win->add_child(id);
            }

            return window;
        }

        void delete_window(WindowPtr<Window> &window)
        {
        }

        void render_buffer(WindowId id)
        {
            auto window = window_map_.find(id)->second;
            auto wsize = window->get_size();
            auto wbuffer = window->get_buffer();
            auto wchildren = window->get_children();

            for (int h = 0; h < wsize.height; ++h)
            {
                for (int w = 0; w < wsize.width; ++w)
                {
                    buffer_[(wsize.y + h) * width_ + (wsize.x + w)] =
                        wbuffer[h * wsize.width + w];
                }
            }

            for (auto child : wchildren)
            {
                render_buffer(child);
            }
        }

        void render(WindowId id)
        {
            this->render_buffer(id);

            // Need to do this because std::wcout << buffer_ generates weird chars at the end
            for(size_t i = 0; i < width_ * height_; ++i)
            {
                std::wcout << buffer_[i];
            }
        }

        void set_visible(std::shared_ptr<Window> window)
        {
        }

    private:
        int width_;
        int height_;
        std::map<WindowId, WindowPtr<Window>> window_map_;
        std::shared_ptr<wchar_t[]> buffer_;

        WindowManager()
            : width_(Utils::get_term_width()), height_(Utils::get_term_height())
        {
            this->buffer_ = std::make_shared<wchar_t[]>(this->width_ * this->height_);

            for (unsigned int i = 0; i < this->width_ * this->height_; ++i)
                this->buffer_[i] = U_SPACE;
        }

        ~WindowManager()
        {
        }
    };
}
