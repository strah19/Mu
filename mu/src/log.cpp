#include "log.h"
#include <ctime>

#define MAX_INPUT_SIZE 512

namespace Mu {
    void TimestampLogCommand::RunCommand(va_list& args, const char* input) {
        const std::time_t now = std::time(nullptr); 
        const std::tm calendar_time = *std::localtime(std::addressof(now));

        command_output = std::to_string(calendar_time.tm_hour) + ":" + std::to_string(calendar_time.tm_min) + ":" + std::to_string(calendar_time.tm_sec);
    }

    void TimestampLogCommand::ProcessArgs(va_list& args) { }

    void LogCommand::AddToOutput(std::string& output, std::vector<LogCommand*>& commands) {
        size_t pos = GetPosition();
        size_t size = GetCommandOutput().size();
        output.insert(GetPosition(), GetCommandOutput());
        for (auto& command : commands)
            if (pos < command->GetPosition())
                command->SetPosition(command->GetPosition() + size);
    }

    void UserLogCommand::RunCommand(va_list& args, const char* input) {
        char buffer[MAX_INPUT_SIZE];
        vsnprintf(buffer, MAX_INPUT_SIZE, input, args);
        va_end(args);

        SetCommandOutput(buffer);
    }

    void UserLogCommand::ProcessArgs(va_list& args) { }

    const std::pair<std::string, ColorCode> COLORS[] = {
        std::make_pair("{cR}", ColorCode::FG_RED),
        std::make_pair("{cDef}", ColorCode::FG_DEFAULT),
        std::make_pair("{cB}", ColorCode::FG_BLUE),
        std::make_pair("{cG}", ColorCode::FG_GREEN),
        std::make_pair("{cY}", ColorCode::FG_YELLOW),
    };

    void ColorLogCommand::RunCommand(va_list& args, const char* input) {
        for (auto& color : COLORS) {
            if (color.first == command_name) {
                color_controller.SetColor(color.second);
                break;
            }
        }

        std::stringstream ss;
        ss << color_controller;
        command_output = ss.str();
    }

    void ColorLogCommand::ProcessArgs(va_list& args) { }

    static std::map<std::string, CreateLogCommandFn> COMMANDS;

    void InitCommands();

    static LogEntries log_entries;
    void InitializeLoggingSystem() {
        InitCommands();
    }

    void InitCommands() {
        COMMANDS["{ts}"] = TimestampLogCommand::Create;
        COMMANDS["{l}"] = UserLogCommand::Create;

        for(auto& color : COLORS)
            COMMANDS[color.first] = ColorLogCommand::Create;
    }

    void FindAllOccurances(std::vector<size_t>& vec, std::string data, std::string to_search) {
        size_t pos = data.find(to_search);
        while (pos != std::string::npos) {
            vec.push_back(pos);
            pos = data.find(to_search, pos + to_search.size());
        }
    }

    void LogFormat::Initialize(const char* format, ...) {
        va_list args;
        va_start(args, format);
 
        std::string input = format;

        for (auto& current_command : COMMANDS) {
            std::vector<size_t> positions;
            FindAllOccurances(positions, input, current_command.first);
            for (auto& position : positions) {
                commands.push_back(current_command.second());
                commands.back()->SetCommand(current_command.first);
                commands.back()->SetPosition(position);
                commands.back()->ProcessArgs(args);
            }
        }

        for (auto& command : commands) {
            input.erase(command->GetPosition(), command->GetCommand().size());
            for (auto& pos_test : commands)
                if (command->GetPosition() < pos_test->GetPosition())
                    pos_test->SetPosition(pos_test->GetPosition() - command->GetCommand().size());
        }

        for (auto& command : commands) 
            command->GetResetingPosition();

        output = input;

        va_end(args);
	}

    void Logger::SetLogFormat(LogFormat* log_format) {
        formatter = log_format;
    }

    void Logger::Log(const char* fmt, ...) {
        if (formatter) {
            va_list args;
            va_start(args, fmt);
            std::string output = formatter->GetLeftOutput();

            for (auto& command : formatter->GetCommands())
                command->ResetPosition();

            for (auto& command : formatter->GetCommands()) {
                command->RunCommand(args, fmt);
                command->AddToOutput(output, formatter->GetCommands());
            }

            printf("%s", output.c_str());
            log_entries.push_back(output);
            va_end(args);
        }
    }

    static Logger error_log;
    static LogFormat error_format;

    static Logger warning_log;
    static LogFormat warning_format;

    static Logger def_log;
    static LogFormat def_format;

    static Logger def_log_good;
    static LogFormat def_format_good;

    void Logs::InitializeLoggers() {
        error_format.Initialize("{cR}Mu error[{ts}]: {l}{cDef}.\n");
        error_log.SetLogFormat(&error_format);

        warning_format.Initialize("{cY}Mu warning[{ts}]: {l}{cDef}.\n");
        warning_log.SetLogFormat(&warning_format);

        def_format.Initialize("{cDef}[{ts}]: {l}{cDef}.\n");
        def_log.SetLogFormat(&def_format);

        def_format_good.Initialize("{cG}[{ts}]: {l}{cDef}.\n");
        def_log_good.SetLogFormat(&def_format_good);
    }

	LogEntries* GetLogEntries() {
        return &log_entries;
    }

    Logger& Logs::GetLogError() { return error_log; }

    Logger& Logs::GetLogWarning() { return warning_log; }

    Logger& Logs::GetLogDef() { return def_log; }

    Logger& Logs::GetLogDefGood() { return def_log_good; }
}