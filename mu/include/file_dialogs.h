#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include "platform.h"
#include <string>

#define MAX_FILENAME_LEN 256

namespace Mu {    
	class FileDialogs {
    public:
        static std::string Open(const char* filter);
        static std::string Save(const char* filter, const char* name);
	
        static bool BrowseFolder(char* out, int max_size, const char* starting_dir);
	};
}

#endif // !FILE_DIALOG_H