#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <unordered_map>
#include <cstring>
#include <utility>
#include <mutex>
#include <termios.h>
#include <tuimaker/Window.hpp>
#include <tuimaker/InputBox.hpp>
#include <tuimaker/TermUtils.hpp>
#include <tuimaker/WindowPtr.hpp>
#include <tuimaker/Button.hpp>

namespace tmk
{
    class WindowManager
    {
    public:
        WindowManager(const WindowManager &obj) = delete;

        ~WindowManager()
        {
            // Buffered input on
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term_);

            // Show cursor
            std::wcout << L"\e[?25h";
        }

        static std::shared_ptr<WindowManager> get_instance(void)
        {
            static auto instance = std::shared_ptr<WindowManager>(new WindowManager());

            return instance;
        }

        template <typename T, class... Args>
        WindowPtr<T> create_window(Window::Size wsize, Args &&...args)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (!std::is_base_of<Window, T>::value)
            {
                throw std::runtime_error("Type should derive from Window!");
            }

            auto window = WindowPtr<T>(wsize, std::forward<Args>(args)...);
            Window::Id id = window->get_id();
            window_map_.emplace(id, window);

            if (std::is_same<Button, T>::value)
            {
                button_map_.emplace(window.template get<Button>()->get_key(), window);
            }

            for (uint64_t h = 0; h < wsize.height; ++h)
                for (uint64_t w = 0; w < wsize.width; ++w)
                    id_show_layer_[(wsize.y + h) * width_ + (wsize.x + w)] = id;

            return window;
        }

        void delete_window(WindowPtr<Window> &window)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            std::wcout << L"\e[?25h";
        }

        void render(const bool &show_cursor = false)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            // Fill the frame buffer
            //! Implement something better, this is temporary
            for (uint64_t h = 0; h < height_; ++h)
            {
                for (uint64_t w = 0; w < width_; ++w)
                {
                    if (auto it = window_map_.find(id_show_layer_[h * width_ + w]); it != window_map_.end())
                    {
                        auto window = it->second;
                        auto wsize = window->get_size();
                        buffer_[h * width_ + w] = window->get_char_at(w - wsize.x, h - wsize.y);
                    }
                }
            }

            // Hide curor
            std::wcout << L"\e[?25l\e[0;0H";
            // Need to print char by char to avoid weird chars at the end
            for (uint64_t i = 0; i < width_ * height_; ++i)
                std::wcout << buffer_[i];
            if (show_cursor)
                selected_win_->show_cursor();
            // Flush the buffer for instant render
            std::fflush(stdout);
        }

        void set_on_top(Window::Id id)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (auto it = window_map_.find(id); it != window_map_.end())
            {
                selected_win_ = it->second;
                auto wsize = selected_win_->get_size();

                for (uint64_t h = 0; h < wsize.height; ++h)
                    for (uint64_t w = 0; w < wsize.width; ++w)
                        id_show_layer_[(wsize.y + h) * width_ + (wsize.x + w)] = id;
            }
        }

        void set_root(Window::Id id)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (auto it = window_map_.find(id); it != window_map_.end())
                root_win_ = it->second;
        }

        void select_window(Window::Id id)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (auto it = window_map_.find(id); it != window_map_.end())
            {
                selected_win_ = it->second;
                selected_win_->select(true);
            }
        }

        void input(void)
        {
            std::lock_guard<std::mutex> lock(mtx_);

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

        void command(void)
        {
            std::lock_guard<std::mutex> lock(mtx_);

            fd_set sigfd;
            struct timeval tv;

            FD_ZERO(&sigfd);
            FD_SET(0, &sigfd);

            tv.tv_sec = 0;
            tv.tv_usec = 10000; // 10ms

            if (!select(1, &sigfd, NULL, NULL, &tv))
                return;

            char c = getchar();

            if (auto it = button_map_.find(c); it != button_map_.end())
                it->second();
        }

    private:
        void render_buffer(Window::Id id)
        {
            if (auto it = window_map_.find(id); it != window_map_.end())
            {
                auto window = it->second;
                auto wsize = window->get_size();
                auto wbuffer = window->get_buffer();

                for (int h = 0; h < wsize.height; ++h)
                    for (int w = 0; w < wsize.width; ++w)
                        buffer_[(wsize.y + h) * width_ + (wsize.x + w)] = wbuffer[h * wsize.width + w];
            }
        }

        WindowManager()
            : width_(TermUtils::get_term_width()), height_(TermUtils::get_term_height())
        {
            buffer_ = std::make_shared<TChar[]>(width_ * height_);
            id_show_layer_ = std::make_unique<Window::Id[]>(width_ * height_);

            for (unsigned int i = 0; i < width_ * height_; ++i)
            {
                buffer_[i].character = TChar::U_SPACE;
                id_show_layer_[i] = 0;
            }

            // Buffered input off
            tcgetattr(STDIN_FILENO, &old_term_);
            term_ = old_term_;
            term_.c_cc[VMIN] = 1;
            term_.c_cc[VTIME] = 0;
            term_.c_lflag &= (~ICANON & ~ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &term_);
        }

        template <class T1, class T2>
        bool is_sub_of(void)
        {
            return std::is_base_of<T1, T2>::value;
        }

        mutable std::mutex mtx_;
        int width_;
        int height_;
        struct termios old_term_;
        struct termios term_;
        WindowPtr<Window> root_win_;
        WindowPtr<Window> selected_win_;
        std::unordered_map<Window::Id, WindowPtr<Window>> window_map_;
        std::unordered_map<char, WindowPtr<Button>> button_map_;
        std::unique_ptr<Window::Id[]> id_show_layer_;
        std::shared_ptr<TChar[]> buffer_;
    };
}
