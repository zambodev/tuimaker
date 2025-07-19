#pragma once

#include <tuimaker/Window.hpp>

namespace tmk
{
    template <class T>
    class WindowPtr
    {
    public:
        typedef std::atomic<size_t> InstanceCounter;

        template <class... Args>
        WindowPtr(Args... args)
        {
            instance_ = dynamic_cast<Window *>(new T(args...));
            counter_ = new InstanceCounter(0);
            ++(*counter_);
        }

        template <class U>
        WindowPtr(WindowPtr<U> &ptr)
        {
            if (!std::is_base_of<T, U>::value)
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

        T *operator->()
        {
            return dynamic_cast<T *>(instance_);
        }

        T *get(void) const
        {
            return dynamic_cast<T *>(instance_);
        }

        InstanceCounter *get_counter(void) const
        {
            return counter_;
        }

    private:
        Window *instance_;
        InstanceCounter *counter_;
    };
}