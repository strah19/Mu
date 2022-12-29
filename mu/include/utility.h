#ifndef UTILITY_H
#define UTILITY_H

#include <string>

namespace Mu {
    class Time {
    public:
        static float GetTime();
    };

    void SetIcon(const char* path);
    std::string GetNameOfPath(const std::string& path);
}

#endif // !UTILITY_H