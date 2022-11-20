#include "../../pch.h"
#include "GlfwWindow.h"
#include "../Engine.h"
#include "../graphics/Vulkan/Instance.h"
#include "../graphics/Vulkan/Surface.h"

#include <iostream>

GlfwWindow::GlfwWindow(int width, int height, const std::string &title) : m_keys()
{
    glfwInit();

    static auto errorCallback = [](int error, const char *description)
    {
      // why not cerr?
      // #error is printed in hex because GLFW defines the codes as hex values
      std::cout << "GLFW error 0x" << std::hex << error << ": " << description
                << std::endl;
    };
    glfwSetErrorCallback(errorCallback);

    switch (Engine::getGraphicsConfig().getApi())
    {
    case GraphicsAPI::Undefined:
        throw;
    case GraphicsAPI::OpenGL:
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        break;
    case GraphicsAPI::Vulkan:
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        break;
    }

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        // we'll not print any information, because if GLFW fails, it will print the error.
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
    glfwSetFramebufferSizeCallback(m_window, glfwFramebufferSizeCallback);
}

void GlfwWindow::makeContextCurrent() const
{
    glfwMakeContextCurrent(m_window);
}

void GlfwWindow::loadGL() const
{
    gladLoadGL(glfwGetProcAddress);
}

void GlfwWindow::swapInterval(int interval) const
{
    glfwSwapInterval(interval);
}

std::vector<const char *> GlfwWindow::getRequiredExtensions() const
{
    u32 extCount = 0;

    const char **extensions = glfwGetRequiredInstanceExtensions(&extCount);

    return {extensions, extensions + extCount};
}

bool GlfwWindow::createSurface(const vk::Instance &instance, vk::Surface &surfaceKhr)
{
    if (glfwCreateWindowSurface(instance, m_window, nullptr, &surfaceKhr.surface) != VK_SUCCESS)
        return false;

    return true;
}

bool GlfwWindow::isOpen() const
{
    return !glfwWindowShouldClose(m_window);
}

void GlfwWindow::close() const
{
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}

void GlfwWindow::destroy() const
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GlfwWindow::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

void GlfwWindow::pollEvents() const
{
    glfwPollEvents();
}

int GlfwWindow::getWidth() const
{
    int width;
    glfwGetWindowSize(m_window, &width, nullptr);
    return width;
}

int GlfwWindow::getHeight() const
{
    int height;
    glfwGetWindowSize(m_window, nullptr, &height);
    return height;
}

bool GlfwWindow::getKey(Key key)
{
    int glfwKey = m_glfwKeyMapper.map(key);
    return m_keys[glfwKey];
}

glm::vec2 GlfwWindow::getCursorPosition()
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return {x, y};
}

bool GlfwWindow::getMouseButton(int mouseButton)
{
    return m_mouseButtons[mouseButton];
}

void GlfwWindow::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    auto *win = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
    if (key < 0) return;

    win->m_onInput(key, action);

    switch (action)
    {
    case GLFW_PRESS:
        win->m_keys[key] = true;
        break;
    case GLFW_RELEASE:
        win->m_keys[key] = false;
        break;
    default:
        break;
    }
}

void GlfwWindow::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto *win = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
    switch (action)
    {
    case GLFW_PRESS:
        win->m_mouseButtons[button] = true;
        break;
    case GLFW_RELEASE:
        win->m_mouseButtons[button] = false;
        break;
    default:
        break;
    }
}

void GlfwWindow::glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    auto *win = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
    win->m_onResize(width, height);
}

IWindow::ResizeEvent::IType &GlfwWindow::getOnResize()
{
    return m_onResize;
}

IWindow::InputEvent::IType &GlfwWindow::getOnInput()
{
    return m_onInput;
}
