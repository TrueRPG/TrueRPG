#ifndef RPG_IWINDOW_H
#define RPG_IWINDOW_H

#include "glm/vec2.hpp"
#include "../../utils/Event.hpp"
#include "../input/Key.h"

class IWindow
{
public:
    using InputEvent = Event<int, int>;
    using ResizeEvent = Event<int, int>;

    virtual ~IWindow() = default;

    virtual bool isOpen() const = 0;

    virtual void close() const = 0;

    virtual void destroy() const = 0;

    virtual void swapBuffers() const = 0;

    virtual void pollEvents() const = 0;

    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

    virtual bool getKey(Key key) = 0;

    virtual bool getMouseButton(int mouseButton) = 0;

    virtual glm::vec2 getCursorPosition() = 0;

    virtual ResizeEvent::IType &getOnResize() = 0;
    virtual InputEvent::IType &getOnInput() = 0;
};

#endif // RPG_IWINDOW_H
