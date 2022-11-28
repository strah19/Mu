#ifndef RENDERER_COMMANDS_H
#define RENDERER_COMMANDS_H

#include <stdint.h>

namespace Mu {
    class RendererCommands {
    public:
        static void Initialize();
        static void Clear(float r, float g, float b, float a);
        static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    };
} 

#endif // !RENDERER_COMMANDS_H