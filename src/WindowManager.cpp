#include <vector>
#include <tuple>
#include <cstring>
#include <cassert>
#include <memory>
#include "../headers/WindowManager.h"
#include "../headers/Utils.h"


namespace tmk
{
    WindowManager::WindowManager()
    : width(Utils::get_term_width()), height(Utils::get_term_height())
    {
        this->buffer = std::make_shared<wchar_t[]>(this->width * this->height);

        for(unsigned int i = 0; i < this->width * this->height; ++i)
            this->buffer[i] = U_SPACE;
    }

    WindowManager::~WindowManager()
    {
    }

    void WindowManager::render_children(std::shared_ptr<WindowNode> wnode)
    {
        wnode->window->draw();
        
        const WindowSize& wsize = wnode->window->get_size();
        auto wbuffer = wnode->window->get_buffer();

        for(int h = 0; h < wsize.height; ++h)
        {
            for(int w = 0; w < wsize.width; ++w)
            {
                this->buffer[(wsize.y + h) * this->width + (wsize.x + w)] =
                    wbuffer[h * wsize.width + w];
            }
        }

        for(auto node : wnode->children)
            this->render_children(node);
    }

    std::shared_ptr<WindowManager::WindowNode> WindowManager::find_node(std::shared_ptr<WindowNode> upperNode, WindowId id)
    {
        if(upperNode == nullptr)
            return nullptr;

        if(upperNode->window->get_id() == id)
            return upperNode;

        for(auto node : upperNode->children)
            if(this->find_node(node, id) != nullptr)
                return node;
    
        return nullptr;
    }

    std::shared_ptr<Window> WindowManager::add_window(WindowSize wsize, std::shared_ptr<Window> father)
    {
        auto window = std::make_shared<Window>(wsize);
        
        if(root == nullptr)
        {
            this->root = std::make_shared<WindowNode>(window, father);
        }
        else
        {
            auto node = find_node(this->root, father->get_id());
            node->children.push_back(std::make_shared<WindowNode>(window, father));
        }
        this->set_visible(window);

        return window;
    }

    void WindowManager::delete_window(std::shared_ptr<Window> window)
    {
        
    }

    void WindowManager::render(void)
    {
        this->render_children(this->root);

        std::wcout << "\x1b[1J\x1b[0;0H";
        
        for(int i = 0; i < this->width * this->height; ++i)
            std::wcout << this->buffer[i];

        std::wcout << std::flush;
    }

    void WindowManager::set_visible(std::shared_ptr<Window> window)
    {
    }
}
