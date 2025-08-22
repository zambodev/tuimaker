#pragma once

#include <utility>
#include <tuimaker/Window.hpp>

namespace tmk
{
    /**
     * @class WindowPtr
     * @brief Window shared pointer class
     *
     * @tparam T
     */
    template <class T>
    class WindowPtr
    {
    public:
        /**
         * @brief Instance counter
         * Atomic integer to count how many instances active of the same window
         */
        typedef std::atomic<uint64_t> InstanceCounter;

        /**
         * @brief Construct a new emtpy Window Ptr object
         *
         */
        WindowPtr()
        {
            instance_ = nullptr;
            counter_ = nullptr;
        }

        /**
         * @brief Construct a new Window Ptr object
         *
         * @tparam Args Window arguments types
         * @param title Window title
         * @param wsize Window size
         * @param conf Window configuration
         * @param args Window arguments
         */
        template <class... Args>
        WindowPtr(const std::string &title, const Window::Size &wsize, const Window::Conf &conf, Args &&...args)
        {
            instance_ = new T(title, wsize, conf, std::forward<Args>(args)...);
            counter_ = new InstanceCounter(0);
            ++(*counter_);
        }

        /**
         * @brief Window templated compy constructor
         *
         * @tparam U Source window type
         * @param ptr Window object
         */
        template <class U>
        WindowPtr(WindowPtr<U> &ptr)
        {
            if (!std::is_base_of<T, U>::value && !std::is_same<U, T>::value)
            {
                throw std::runtime_error("Not a derived type!");
            }

            instance_ = dynamic_cast<T *>(ptr.get());
            counter_ = ptr.get_counter();
            ++(*counter_);
        }

        /**
         * @brief Destroy the Window Ptr object
         *
         */
        ~WindowPtr()
        {
            if (!counter_)
                return;

            if (*counter_ == 1)
            {
                delete instance_;
                delete counter_;
            }
            else
            {
                --(*counter_);
            }
        }

        /**
         * @brief Operator= overload
         *
         * @param obj Window object
         * @return WindowPtr<T>&
         */
        auto operator=(const WindowPtr<T> &obj) -> WindowPtr<T> &
        {
            instance_ = obj.instance_;
            counter_ = obj.counter_;
            ++(*counter_);

            return *this;
        }

        /**
         * @brief Operator() overload
         * Call window as a function from the pointer wrapper
         * @return auto
         */
        auto operator()(void)
        {
            (*instance_)();
        }

        /**
         * @brief Operator-> overload
         *
         * @return T*
         */
        auto operator->() -> T *
        {
            return instance_;
        }

        /**
         * @brief Get raw pointer
         *
         * @tparam U Destination type
         * @return U*
         */
        template <class U = T>
        auto get(void) -> U *
        {
            if (!std::is_base_of<T, U>::value && !std::is_same<U, T>::value)
            {
                throw std::runtime_error("Not a parent type!");
            }

            return dynamic_cast<U *>(instance_);
        }

        /**
         * @brief Get the window pointer counter
         *
         * @return InstanceCounter*
         */
        auto get_counter(void) const -> InstanceCounter *
        {
            return counter_;
        }

    private:
        T *instance_ = nullptr;
        InstanceCounter *counter_ = nullptr;
    };
}