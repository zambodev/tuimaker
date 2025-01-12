#pragma once

#include <vector>
#include <memory>
#include "Window.hpp"


namespace tmk
{
    class WindowManager
    {
        public:
            struct WindowNode
            {
                std::shared_ptr<Window>                     window;
                std::shared_ptr<Window>                     father;
                std::vector<std::shared_ptr<WindowNode>>    children;

                WindowNode(std::shared_ptr<Window> wptr, std::shared_ptr<Window> fptr)
                    : window(wptr), father(fptr) {}
            };
 
            WindowManager(const WindowManager& obj) = delete;
            static WindowManager* GetInstance(void)
            {
                static WindowManager* instance = new WindowManager();
                return instance;
            }

            std::shared_ptr<Window> AddWindow(WindowSize wsize, std::shared_ptr<Window> father);
            void RemoveWindow(std::shared_ptr<Window> window);
            void Render(void);
            std::shared_ptr<Window> GetWindow(WindowId id);
            void SetVisible(std::shared_ptr<Window> window);
        
        private:
            int                         width;
            int                         height;
            std::shared_ptr<wchar_t[]>  buffer;
            std::shared_ptr<WindowNode> root;

            WindowManager();
            ~WindowManager();
            void RenderChildren(std::shared_ptr<WindowNode> wnode);
            std::shared_ptr<WindowNode> FindNode(std::shared_ptr<WindowNode> upperNode, WindowId id);
    };
}
