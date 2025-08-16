#pragma once

#include "utility"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"

namespace Huiluna {
    
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode keycode);

        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}