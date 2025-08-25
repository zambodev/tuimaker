#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <unordered_map>
#include <cstring>
#include <utility>
#include <mutex>
#include <algorithm>
#ifdef __linux__
#include <termios.h>
#elif _WIN32
#include <winsock2.h>
#include <windows.h>
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
#ifdef __linux__
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term_);
#elif _WIN32
            WSACleanup();
            DWORD mode = 0;
            GetConsoleMode(term_, &mode);
            SetConsoleMode(term_, (mode & ENABLE_ECHO_INPUT));
#endif

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
            window_stack_.push_front(id);

            if (std::is_same<Button, T>::value)
                button_map_.emplace(window.template get<Button>()->get_key(), window);

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
            for (auto it = window_stack_.rbegin(); it != window_stack_.rend(); ++it)
            {
                auto window_it = window_map_.find(*it);
                if (window_it == window_map_.end())
                    continue;

                auto window = window_it->second;
                auto size = window->get_size();

                for (uint64_t x = 0; x < size.width; ++x)
                    for (uint64_t y = 0; y < size.height; ++y)
                        buffer_[(size.y + y) * width_ + (size.x + x)] = window->get_char_at(x, y);
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

            auto it = std::find(window_stack_.begin(), window_stack_.end(), id);
            if (it == window_stack_.end())
                return;

            window_stack_.erase(it);
            window_stack_.push_front(*it);
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

            for (unsigned int i = 0; i < width_ * height_; ++i)
                buffer_[i].character = TChar::U_SPACE;

#ifdef __linux__
            // Buffered input off
            tcgetattr(STDIN_FILENO, &old_term_);
            term_ = old_term_;
            term_.c_cc[VMIN] = 1;
            term_.c_cc[VTIME] = 0;
            term_.c_lflag &= (~ICANON & ~ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &term_);
#elif _WIN32
            WSAStartup(MAKEWORD(2, 2), &wsa_data_);
            term_ = GetStdHandle(STD_INPUT_HANDLE);
            DWORD mode = 0;
            GetConsoleMode(term_, &mode);
            SetConsoleMode(term_, mode & (~ENABLE_ECHO_INPUT));
#endif
        }

        mutable std::mutex mtx_;
        int width_;
        int height_;
#ifdef __linux__
        struct termios old_term_;
        struct termios term_;
#elif _WIN32
        WSADATA wsa_data_;
        HANDLE term_;
#endif
        WindowPtr<Window> root_win_;
        WindowPtr<Window> selected_win_;
        std::deque<Window::Id> window_stack_;
        std::unordered_map<Window::Id, WindowPtr<Window>> window_map_;
        std::unordered_map<char, WindowPtr<Button>> button_map_;
        std::shared_ptr<TChar[]> buffer_;
    };
}
