#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <filesystem>

namespace Iota {
	class Project {
	public:
		Project();

		inline void SetName(const std::string& name) { m_name = name; }
		bool InitializeProject(const char* path);
		void CloseProject();

		std::filesystem::path* GetDirectory() { return &m_project_dir; }
		inline bool const Working() const { return m_working; }
		inline std::string const GetPath() { return m_path; }
		inline std::string const GetName() { return m_name; }
	private:
		std::string m_path;
		std::string m_name;

		bool m_working = false;
		std::filesystem::path m_project_dir;
	};
}

#endif // !PROJECT_H