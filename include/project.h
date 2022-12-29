#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <filesystem>

namespace Iota {
	class Project {
	public:
		Project() = default;

		bool InitializeProject(const char* path);
		void CloseProject();
		bool DoesPathExist();

		inline void SetName(const std::string& name) { m_name = name; }
		inline std::filesystem::path* GetDirectory() { return &m_project_dir; }
		inline bool const Working() const { return m_working; }
		inline std::string const GetPath() { return m_path; }
		inline std::string const GetName() { return m_name; }
	private:
		std::filesystem::path m_project_dir;
		
		std::string m_path;
		std::string m_name;

		bool m_working = false;
	};
}

#endif // !PROJECT_H