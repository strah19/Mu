#include "terminal.h"
#include <thread>
#include <stdarg.h>
#include "mu.h"

namespace Iota {
    std::vector<Command> gcommands;
    
    void GlobalCommands::AddCommand(const Command& command) {
        gcommands.push_back(command);
    }

    CommandOutput ReturnWithError(const char* error) {
        CommandOutput output;
        output.error = error;
        return output;
    }

    CommandOutput echo_cmd(const std::vector<CommandArg>& args) {
        CommandOutput output;
        for (auto& arg : args)
            output.output += arg.raw;
        return output;
    }

    CommandOutput add_cmd(const std::vector<CommandArg>& args) {
        CommandOutput output;
        double num = 0;
        for (auto& arg : args) {
            if (!arg.IsNumber()) return ReturnWithError(NUM_TYPE_ERROR);
            num += arg.num;
        }
        output.output = std::to_string(num);
        return output;
    }

    CommandOutput lua_cmd(const std::vector<CommandArg>& args) {
        CommandOutput output;
        Mu::Script script;
        const char* error = script.RunScript(args[0].str);
        output.output = (error) ? error : "successfully ran lua program.";
        return output;
    }

    Terminal::Terminal() {
        GlobalCommands::AddCommand(Command("echo", echo_cmd));
        GlobalCommands::AddCommand(Command("lua",  lua_cmd));
        GlobalCommands::AddCommand(Command("add",  add_cmd));
    }

    bool IsNumber(const std::string & str) {
        auto result = double();
        auto i = std::istringstream(str);

        i >> result;      

        return !i.fail() && i.eof();
    }  

    void Terminal::Execute() {
        AddToHistory(m_input);
        ClearOutputBuffer();

        std::vector<std::string> command_args;
        std::istringstream iss(m_input);
        std::string input_after_space;
        std::string name;

        // Get the name of the command and the arguents into strings
        bool get_name = false;
        while (getline(iss, input_after_space, ' ')) {
            if (!get_name) {
                name = input_after_space;
                get_name = true;
            }
            else
                command_args.push_back(input_after_space);
        }

        // Put raw argument strings into CommandArg objects that will be sent to command function
        std::vector<CommandArg> send_args;
        for (auto& cmd_arg : command_args) 
            send_args.push_back(CommandArg(cmd_arg.c_str()));

        // Checks if command is help and exits process
        if (m_input == "help") {
            m_output.push_back("Terminal Commands:");
            for (auto& cmd : gcommands) {
                m_output.push_back(cmd.name);
            }
            return;
        }
        else if (m_input != "history") {
            CommandOutput output;
            output.error = UNKNOWN_CMD_ERROR;
            for (auto& cmd : gcommands) {
                if (cmd.name == name) {
                    output.error = NO_ERROR;

                    for (int i = 0; i < command_args.size(); i++) {
                        if (IsNumber(send_args[i].str)) {
                            send_args[i].type = CommandType::NUM;
                            send_args[i].num = std::stod(send_args[i].str);
                        }
                        else {
                            send_args[i].type = CommandType::STR;
                            send_args[i].str = send_args[i].str;
                        }
                    }

                    output = cmd.fn(send_args);
                }
            }

            m_output.push_back(m_input);
            if (!output.error)
                m_output.push_back(output.output);
            else
                m_output.push_back(output.error);
        }
        else {
            for (auto& entry : m_history) 
                m_output.push_back(entry);  
        }
    }

    void Terminal::FormattedOutput(const char* fmt, ...) {
        char buf[MAX_OUTPUT_LEN];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, MAX_OUTPUT_LEN, fmt, args);
        buf[MAX_OUTPUT_LEN - 1] = 0;
        va_end(args);
        m_output.push_back(buf);
    }

    void Terminal::AddToHistory(const std::string& entry) {
        if (entry != "history") m_history.push_back(entry);
    }
}