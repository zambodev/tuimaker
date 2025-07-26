#pragma once

#include <vector>
#include <memory>
#include <map>
#include <cstring>
#include <print>
#include <tuimaker/Window.hpp>
#include <tuimaker/InputBox.hpp>
#include <tuimaker/Utils.hpp>
#include <tuimaker/WindowPtr.hpp>

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

            std::wcout << "\033[?25l\033[0;0H";
            for (uint64_t i = 0; i < width_ * height_; ++i)
                std::wcout << buffer_[i];

            std::fflush(stdout);
        }

        void set_visible(std::shared_ptr<Window> window)
        {
        }

        void select_window(WindowId id)
        {
            selected_win_ = window_map_.find(id)->second;
            selected_win_->select(true);
        }

        void input(void)
        {
            if (!selected_win_.get())
                return;

            fd_set sigfd;
            struct timeval tv;

            FD_ZERO(&sigfd);
            FD_SET(0, &sigfd);

            tv.tv_sec = 0;
            tv.tv_usec = 10000; // 10ms

            if (!select(1, &sigfd, NULL, NULL, &tv))
                return;

            char c = getchar();

            selected_win_.get<InputBox>()->write_char(c);
        }

    private:
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

        template <class T1, class T2>
        bool is_sub_of(void)
        {
            return std::is_base_of<T1, T2>::value;
        }

        int width_;
        int height_;
        WindowPtr<Window> selected_win_;
        std::map<WindowId, WindowPtr<Window>> window_map_;
        std::shared_ptr<wchar_t[]> buffer_;
    };
}
