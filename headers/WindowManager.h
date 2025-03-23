#pragma once

#include <vector>
#include <memory>
#include "Window.h"


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
                : window(wptr), father(fptr)
                {}
            };
 
            WindowManager(const WindowManager& obj) = delete;
            static WindowManager* get_instance(void)
            {
                static WindowManager* instance = new WindowManager();
                return instance;
            }

            std::shared_ptr<Window> add_window(WindowSize wsize, std::shared_ptr<Window> father);
            void delete_window(std::shared_ptr<Window> window);
            void render(void);
            std::shared_ptr<Window> get_window(WindowId id);
            void set_visible(std::shared_ptr<Window> window);
        
        private:
            int                         width;
            int                         height;
            std::shared_ptr<wchar_t[]>  buffer;
            std::shared_ptr<WindowNode> root;

            WindowManager();
            ~WindowManager();
            void render_children(std::shared_ptr<WindowNode> wnode);
            std::shared_ptr<WindowNode> find_node(std::shared_ptr<WindowNode> upperNode, WindowId id);
    };
}
