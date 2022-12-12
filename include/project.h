#ifndef PROJECT_H
#define PROJECT_H

#include <string>

namespace Iota {
	class Project {
	public:
		Project();

		inline std::string const GetPath() { return m_path; }
		inline std::string const GetName() { return m_name; }
	private:
		std::string m_path;
		std::string m_name;
	};
}

#endif // !PROJECT_H