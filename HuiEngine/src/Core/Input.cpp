#include "Input.h"
#include "vk_engine.h"

#include <GLFW/glfw3.h>

namespace Huiluna {

    bool Input::IsKeyPressed(KeyCode keycode)
    {
        auto window = ComputeShaderApplication::GetWindow();
        auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
        return state == GLFW_PRESS;
    }

    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto window = ComputeShaderApplication::GetWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = ComputeShaderApplication::GetWindow();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return (float)x;
    }

    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return (float)y;
    }

}