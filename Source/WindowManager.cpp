#include <vector>
#include <tuple>
#include <cstring>
#include <cassert>
#include "../Include/WindowManager.hpp"
#include "../Include/Utils.hpp"


namespace tmk
{
    WindowManager::WindowManager()
    : width(Utils::GetTerminalWidth()), height(Utils::GetTerminalHeight())
    {
        buffer = std::make_shared<wchar_t[]>(this->width * this->height);

        for(unsigned int i = 0; i < this->width * this->height; ++i)
            this->buffer[i] = U_SPACE;
    }

    WindowManager::~WindowManager()
    {
    }

    void WindowManager::RenderChildren(std::shared_ptr<WindowNode> wnode)
    {
        wnode->window->Draw();
        
        const WindowSize& wsize = wnode->window->GetSize();
        auto wbuffer = wnode->window->GetBuffer();

        for(int h = 0; h < wsize.height; ++h)
        {
            for(int w = 0; w < wsize.width; ++w)
            {
                this->buffer[(wsize.y + h) * this->width + (wsize.x + w)] =
                    wbuffer[h * wsize.width + w];
            }
        }

        for(auto node : wnode->children)
            this->RenderChildren(node);
    }

    std::shared_ptr<WindowManager::WindowNode> WindowManager::FindNode(std::shared_ptr<WindowNode> upperNode, WindowId id)
    {
        if(upperNode == nullptr)
            return nullptr;

        if(upperNode->window->GetId() == id)
            return upperNode;

        for(auto node : upperNode->children)
            if(this->FindNode(node, id) != nullptr)
                return node;
    
        return nullptr;
    }

    std::shared_ptr<Window> WindowManager::AddWindow(WindowSize wsize, std::shared_ptr<Window> father)
    {
        auto window = std::make_shared<Window>(wsize);
        
        if(root == nullptr)
        {
            this->root = std::make_shared<WindowNode>(window, father);
        }
        else
        {
            auto node = FindNode(this->root, father->GetId());
            node->children.push_back(std::make_shared<WindowNode>(window, father));
        }
        this->SetVisible(window);

        return window;
    }

    void WindowManager::RemoveWindow(std::shared_ptr<Window> window)
    {
        
    }

    void WindowManager::Render(void)
    {
        this->RenderChildren(this->root);

        std::wcout << "\x1b[1J\x1b[0;0H" << this->buffer;
    }

    void WindowManager::SetVisible(std::shared_ptr<Window> window)
    {
    }
}
