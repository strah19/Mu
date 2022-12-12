#include "file_dialogs.h"
#include "application.h"

#ifdef MU_PLATFORM_WINDOWS
#include <windows.h>      // For common windows data types and function headers
#include <shlobj.h>
#include <shobjidl.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#define MAX_FILENAME_LEN 256

namespace Mu {
	#ifdef MU_PLATFORM_WINDOWS
    std::string FileDialogs::Open(const char* filter) {
		OPENFILENAMEA ofn;
		
		CHAR szFile[260] = { 0 };
		CHAR currentDir[MAX_FILENAME_LEN] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		
		if (GetCurrentDirectoryA(MAX_FILENAME_LEN, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	std::string FileDialogs::Save(const char* filter) {
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[MAX_FILENAME_LEN] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow()->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(MAX_FILENAME_LEN, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		
		return std::string();
	}

	bool FileDialogs::BrowseFolder(char* out, int max_size, const char* starting_dir) {
		bool ret = false;
		IFileDialog* pfd;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd)))) {
			if (starting_dir) {
				PIDLIST_ABSOLUTE pidl;
				WCHAR wstarting_dir[MAX_PATH];
				WCHAR* wc = wstarting_dir;
				for (const char* c = starting_dir; *c && wc - wstarting_dir < MAX_PATH - 1; ++c, ++wc)
					*wc = *c == '/' ? '\\' : *c;
				*wc = 0;

				HRESULT hresult = ::SHParseDisplayName(wstarting_dir, 0, &pidl, SFGAO_FOLDER, 0);
				if (SUCCEEDED(hresult)) {
					IShellItem* psi;
					hresult = ::SHCreateShellItem(NULL, NULL, pidl, &psi);
					if (SUCCEEDED(hresult))
						pfd->SetFolder(psi);
					ILFree(pidl);
				}
			}

			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
				pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
			if (SUCCEEDED(pfd->Show(NULL))) {
				IShellItem* psi;
				if (SUCCEEDED(pfd->GetResult(&psi))) {
					WCHAR* tmp;
					if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &tmp))) {
						char* c = out;
						while (*tmp && c - out < max_size - 1) {
							*c = (char)*tmp;
							++c;
							++tmp;
						}
						*c = '\0';
						ret = true;
					}
					psi->Release();
				}
			}
			pfd->Release();
		}
		return ret;
	}

    #endif
} 