#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <map>
#include <cstring>
#include <tuimaker/Window.hpp>
#include <tuimaker/InputBox.hpp>
#include <tuimaker/TermUtils.hpp>
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
        WindowPtr<T> create_window(WindowSize wsize)
        {
            if (!std::is_base_of<Window, T>::value)
            {
                throw std::runtime_error("Type should derive from Window!");
            }

            auto window = WindowPtr<T>(wsize);
            WindowId id = window->get_id();
            window_map_.emplace(id, window);
            window_stack_.push_front(id);

            return window;
        }

        void delete_window(WindowPtr<Window> &window)
        {
            //! Implement
        }

        void render(void)
        {
            // Fill the frame buffer
            for (auto it = window_stack_.rbegin(); it != window_stack_.rend(); ++it)
            {
                this->render_buffer(*it);
            }

            // Hide curor
            std::wcout << "\033[?25l\033[0;0H";
            // Need to print char by char to avoid weird chars at the end
            for (uint64_t i = 0; i < width_ * height_; ++i)
                std::wcout << buffer_[i];
            // Flush the buffer for instant render
            std::fflush(stdout);
        }

        void set_on_top(WindowId id)
        {
            selected_win_ = window_map_.find(id)->second;
            auto it = std::find(window_stack_.begin(), window_stack_.end(), id);

            if (it == window_stack_.end())
            {
                //! Error
            }

            window_stack_.push_front(*it);
            window_stack_.erase(it);
        }

        void set_root(WindowId id)
        {
            root_win_ = window_map_.find(id)->second;
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
        void render_buffer(WindowId id)
        {
            auto window = window_map_.find(id)->second;
            auto wsize = window->get_size();
            auto wbuffer = window->get_buffer();

            for (int h = 0; h < wsize.height; ++h)
            {
                for (int w = 0; w < wsize.width; ++w)
                {
                    buffer_[(wsize.y + h) * width_ + (wsize.x + w)] =
                        wbuffer[h * wsize.width + w];
                }
            }
        }

        WindowManager()
            : width_(TermUtils::get_term_width()), height_(TermUtils::get_term_height())
        {
            this->buffer_ = std::make_shared<wchar_t[]>(this->width_ * this->height_);

            for (unsigned int i = 0; i < this->width_ * this->height_; ++i)
                this->buffer_[i] = TChar::U_SPACE;
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
        WindowPtr<Window> root_win_;
        WindowPtr<Window> selected_win_;
        std::map<WindowId, WindowPtr<Window>> window_map_;
        std::deque<WindowId> window_stack_;
        std::shared_ptr<wchar_t[]> buffer_;
    };
}
