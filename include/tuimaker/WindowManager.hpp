#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <unordered_map>
#include <cstring>
#include <utility>
#include <mutex>
#ifdef __linux__
#include <termios.h>
#elif _WIN32
#include <conio.h>
#endif
#include <tuimaker/Window.hpp>
#include <tuimaker/InputBox.hpp>
#include <tuimaker/TermUtils.hpp>
#include <tuimaker/WindowPtr.hpp>
#include <tuimaker/Button.hpp>

namespace tmk
{
    /**
     * @class WindowManager
     * @brief Window manager singleton class
     *
     */
    class WindowManager
    {
    public:
        WindowManager(const WindowManager &obj) = delete;

        /**
         * @brief Destroy the Window Manager object
         *
         */
        ~WindowManager()
        {
            // Buffered input on
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term_);

            // Show cursor
            std::wcout << L"\x1b[?25h";
        }

        /**
         * @brief Get the instance object
         *
         * @return std::shared_ptr<WindowManager>
         */
        static auto get_instance(void) -> std::shared_ptr<WindowManager>
        {
            static auto instance = std::shared_ptr<WindowManager>(new WindowManager());

            return instance;
        }

        /**
         * @brief Create a window object
         *
         * @tparam T Window type
         * @tparam Args Window parameters types
         * @param title Window title
         * @param wsize Window size
         * @param conf Window configuration
         * @param args Window arguments
         * @return WindowPtr<T>
         */
        template <typename T, class... Args>
        auto create_window(const std::string &title, const Window::Size &wsize, const Window::Conf &conf, Args &&...args) -> WindowPtr<T>
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (!std::is_base_of<Window, T>::value)
            {
                throw std::runtime_error("Type should derive from Window!");
            }

            auto window = WindowPtr<T>(title, wsize, conf, std::forward<Args>(args)...);
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

        /**
         * @brief Delete window
         *
         * @param window Window instance
         */
        auto delete_window(WindowPtr<Window> &window) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            std::wcout << L"\x1b[?25h";
        }

        /**
         * @brief Render the screen buffer to screen
         *
         * @param show_cursor
         */
        auto render(const bool &show_cursor = false) -> void
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
            std::wcout << L"\x1b[?25l\x1b[0;0H";
            // Need to print char by char to avoid weird chars at the end
            for (uint64_t i = 0; i < width_ * height_; ++i)
                std::wcout << buffer_[i];
            if (show_cursor)
                selected_win_->show_cursor();
            // Flush the buffer for instant render
            std::fflush(stdout);
        }

        /**
         * @brief Set a window on top
         *
         * @param id Window id
         */
        auto set_on_top(Window::Id id) -> void
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

        /**
         * @brief Set root window
         *
         * @param id Window id
         */
        auto set_root(Window::Id id) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (auto it = window_map_.find(id); it != window_map_.end())
                root_win_ = it->second;
        }

        /**
         * @brief Select a window
         *
         * @param id Window id
         */
        auto select_window(Window::Id id) -> void
        {
            std::lock_guard<std::mutex> lock(mtx_);

            if (auto it = window_map_.find(id); it != window_map_.end())
            {
                selected_win_ = it->second;
                selected_win_->select(true);
            }
        }

        /**
         * @brief Get user text input
         * Send the user input to selected window
         */
        auto input(void) -> void
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

#ifdef __linux__
            char c = getchar();
#elif _WIN32
            char c = getch();
#endif

            selected_win_.get<InputBox>()->write_char(c);
        }

        /**
         * @brief Get command text input
         * Run the corresponding key -> command
         */
        auto command(void) -> void
        {
            char c = 0;

            { // Mutex lock
                std::lock_guard<std::mutex> lock(mtx_);

                fd_set sigfd;
                struct timeval tv;

                FD_ZERO(&sigfd);
                FD_SET(0, &sigfd);

                tv.tv_sec = 0;
                tv.tv_usec = 10000; // 10ms

                if (!select(1, &sigfd, NULL, NULL, &tv))
                    return;

#ifdef __linux__
                c = getchar();
#elif _WIN32
                c = getch();
#endif
            } // Mutex lock end

            if (auto it = button_map_.find(c); it != button_map_.end())
                it->second();
        }

    private:
        /**
         * @brief Construct a new Window Manager object
         *
         */
        WindowManager()
        {
            std::tie(width_, height_) = TermUtils::get_term_size();
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
