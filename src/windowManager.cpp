#include <vector>
#include "windowManager.hpp"
#include "utils.hpp"


void WindowManager::addWindow(Window& window)
{
    Window *tmp = &window;
    int depth = UTILS::getWindowDepth(tmp);

    if(this->m_visibilityLayerList.size() < depth)
        for(int i = 0; i < depth; ++i)
            this->m_visibilityLayerList.emplace_back(new std::vector<Window*>);

    this->m_visibilityLayerList.at(depth - 1)->emplace_back(tmp);
}

void WindowManager::removeWindow(Window& window)
{

}
