#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>
#include <vector>
#include <cstdarg>
#include <sstream>

#include "file.h"

namespace Mu {
	class LogCommand {
	public:
		LogCommand() : command_name("") { }
		virtual void RunCommand(va_list& args, const char* input) = 0;
		virtual void ProcessArgs(va_list& args) = 0;

		inline const std::string GetCommand() const { return command_name; }
		inline const std::string GetCommandOutput() const { return command_output; }

		inline void SetCommand(const std::string& command_name) { this->command_name = command_name; };
		inline void SetCommandOutput(const std::string& command_output) { this->command_output = command_output; };
	protected:
		std::string command_name;
		std::string command_output;
	};

	typedef LogCommand* (__stdcall* CreateLogCommandFn)(void);

	class TimestampLogCommand : public LogCommand {
	public:
		TimestampLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new TimestampLogCommand(); }
	};

	class UserLogCommand : public LogCommand {
	public:
		UserLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new UserLogCommand(); }
	};

	class LeftLogCommand : public LogCommand {
	public:
		LeftLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new LeftLogCommand(); }
	};

	enum ColorCode {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_YELLOW = 33,
		FG_DEFAULT = 37,

		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};

	class ColorController {
	public:
		ColorController() : code(FG_DEFAULT) { }

		inline void SetColor(ColorCode code) { this->code = code; }

		friend std::ostream& operator<<(std::ostream& os, const ColorController& controller) {
			return os << "\033[" << controller.code << "m";
		}
	private:
		ColorCode code;
	};

	class ColorLogCommand : public LogCommand {  
	public:
		ColorLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new ColorLogCommand(); }
	private:
		ColorController color_controller;
	};

	class LogFormat {
	public:
		void Initialize(const char* format, ...);
		std::vector<LogCommand*>& GetCommands() { return commands; }
	private:
		std::string LookForCommand(const std::string& input, size_t index);
		void NewCommand(const std::string& name, va_list& args);

		std::vector<LogCommand*> commands;
	};

	class Logger {
	public:
		void SetLogFormat(LogFormat* log_format);
		void Log(const char* fmt, ...); 
	private:
		LogFormat* formatter;
	};

	using LogEntries = std::vector<std::string>;
	void InitializeLoggingSystem();
	LogEntries* GetLogEntries();

	class Logs {
	public:
		static void InitializeLoggers();

		static Logger& GetLogError();
		static Logger& GetLogWarning();
		static Logger& GetLogDef();
		static Logger& GetLogDefGood();
	};
}

#define MU_LOG_ERROR(...) Mu::Logs::GetLogError().Log(__VA_ARGS__)
#define MU_LOG_WARNING(...) Mu::Logs::GetLogWarning().Log(__VA_ARGS__)
#define MU_LOG(...) Mu::Logs::GetLogDef().Log(__VA_ARGS__)
#define MU_LOG_GOOD(...) Mu::Logs::GetLogDefGood().Log(__VA_ARGS__)

#endif // !LOGGER_H