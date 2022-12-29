#include "project.h"
#include "mu.h"

namespace Iota {
	bool Project::InitializeProject(const char* path) {
		m_path = path;
		m_project_dir = std::filesystem::path(m_path);
		m_name = Mu::GetNameOfPath(m_path);
		m_working = DoesPathExist();

		return m_working;
	}

	bool Project::DoesPathExist() {
		return (std::filesystem::exists(m_project_dir)); 
	}
	
	void Project::CloseProject() {
		m_working = false;
	}
}