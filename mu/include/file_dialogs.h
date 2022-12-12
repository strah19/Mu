#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include "platform.h"
#include <string>

namespace Mu {    
	class FileDialogs {
    public:
        static std::string Open(const char* filter);
        static std::string Save(const char* filter);
	
        static bool BrowseFolder(char* out, int max_size, const char* starting_dir);
	};
}

#endif // !FILE_DIALOG_H