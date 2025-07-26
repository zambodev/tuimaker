#pragma once

#include <tuimaker/Window.hpp>

namespace tmk
{
    template <class T>
    class WindowPtr
    {
    public:
        typedef std::atomic<uint64_t> InstanceCounter;

        WindowPtr()
        {
            instance_ = nullptr;
            counter_ = nullptr;
        }

        template <class... Args>
        WindowPtr(Args... args)
        {
            instance_ = new T(args...);
            counter_ = new InstanceCounter(0);
            ++(*counter_);
        }

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

        ~WindowPtr()
        {
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

        WindowPtr<T> &operator=(const WindowPtr<T> &obj)
        {
            instance_ = obj.instance_;
            counter_ = obj.counter_;
            ++(*counter_);

            return *this;
        }

        T *operator->()
        {
            return instance_;
        }

        template <class U = T>
        U *get(void)
        {
            if (!std::is_base_of<T, U>::value && !std::is_same<U, T>::value)
            {
                throw std::runtime_error("Not a parent type!");
            }

            return dynamic_cast<U *>(instance_);
        }

        InstanceCounter *get_counter(void) const
        {
            return counter_;
        }

    private:
        T *instance_ = nullptr;
        InstanceCounter *counter_ = nullptr;
    };
}