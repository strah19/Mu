#include "project.h"
#include "log.h"

namespace Iota {
	Project::Project() { }

	bool Project::InitializeProject(const char* path) {
		m_path = path;
		
		m_project_dir = std::filesystem::path(m_path);
		if (std::filesystem::exists(m_project_dir)) 
			m_working = true;
		
		m_name = m_path.substr(m_path.find_last_of("/\\") + 1);

		return m_working;
	}

	void Project::CloseProject() {
		m_working = false;
	}
}