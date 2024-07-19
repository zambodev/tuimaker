#include <vector>
#include <tuple>
#include <cstring>
#include "windowManager.hpp"
#include "utils.hpp"


WindowManager::WindowManager()
{
    auto size = UTILS::getTerminalSize();

    this->m_Buffer = new wchar_t[std::get<0>(size) * std::get<1>(size)];
    this->m_BufferLayerMap = new int[std::get<0>(size) * std::get<1>(size)];
    
    for(int i = 0; i < std::get<0>(size) * std::get<1>(size); ++i)
    {
        this->m_Buffer[i] = U_SPACE;
        this->m_BufferLayerMap[i] = 1;
    }
}

WindowManager::~WindowManager()
{
    delete this->m_Buffer;
}

void WindowManager::addWindow(Window& window)
{
    Window *tmp = &window;
    int depth = UTILS::getWindowDepth(tmp);

    if(this->m_visibilityLayerList.size() < depth)
        for(int i = 0; i < depth; ++i)
            this->m_visibilityLayerList.emplace_back(new std::vector<Window*>);

    this->m_visibilityLayerList.at(depth - 1)->emplace_back(tmp);

    auto size = window.getSize();
    auto termSize = UTILS::getTerminalSize();
    for(int i = 0; i < std::get<3>(size); ++i)
        for(int j = 0; j < std::get<2>(size); ++j)
            this->m_BufferLayerMap[(std::get<1>(size) + i) * std::get<0>(termSize) + (std::get<0>(size) + j)] = depth;
}

void WindowManager::removeWindow(int id)
{

}

void WindowManager::render(void)
{
    std::vector<std::vector<Window*>*>::reverse_iterator layerItr;
    std::vector<Window*>::reverse_iterator windowItr;

    for(layerItr = this->m_visibilityLayerList.rbegin(); layerItr != this->m_visibilityLayerList.rend(); ++layerItr)
        for(windowItr = (*layerItr)->rbegin(); windowItr != (*layerItr)->rend(); ++windowItr)
            if((*windowItr)->isReady())
                (*windowItr)->show(this->m_Buffer, this->m_BufferLayerMap);

    std::wcout << L"\x1b[s\x1b[0;0H" << this->m_Buffer << L"\x1b[00;0H";
}
