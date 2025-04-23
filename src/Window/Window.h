#pragma once
#include "../Logger/Logger.h"
#include "../../vendor/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

namespace Lambda{
    struct WindowProps{
    public:
        std::string title;
        uint32_t width;
        uint32_t height;
        bool resizable;
        bool running;
    };

    class Renderer;

    class Window{
    public:
        Window(const std::string& title, uint32_t width, uint32_t height, bool resizable = false);
        std::string getTitle() const;
        int getWidth() const;
        int getHeight() const;
        void pollEvents() const;
        GLFWwindow* getNativeWindow() const;
        bool isRunning() const;
        void swapBuffer() const;
        ~Window();
        friend class Renderer;
    private:
        void ShutDown();
        GLFWwindow* m_nativeWindow;
        WindowProps m_prop;
        inline static uint8_t m_windowCnt = 0;
    };
}