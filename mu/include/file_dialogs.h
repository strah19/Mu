#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include "platform.h"
#include <string>

namespace Mu {
    #ifdef MU_PLATFORM_WINDOWS
    
	class FileDialogs {
    public:
        static std::string Open(const char* filter);
        static std::string Save(const char* filter);
    };

    #endif
}

#endif // !FILE_DIALOG_H