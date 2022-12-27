#include "terminal.h"
#include <thread>
#include <stdarg.h>
#include "log.h"

namespace Iota {
    std::vector<Command> gcommands;
    
    void GlobalCommands::AddCommand(const Command& command) {
        gcommands.push_back(command);
    }

    CommandOutput echo_cmd(const std::vector<CommandArg>& args) {
        CommandOutput output;
        for (auto& arg : args)
            output.output += arg.str;
        return output;
    }

    CommandOutput add_cmd(const std::vector<CommandArg>& args) {
        CommandOutput output;
        double num = 0;
        for (auto& arg : args)
            num += arg.num;
        output.output = std::to_string(num);
        return output;
    }

    Terminal::Terminal() {
        GlobalCommands::AddCommand(Command("echo", echo_cmd));
        GlobalCommands::AddCommand(Command("add", { { CommandType::NUM }, { CommandType::NUM } }, add_cmd, CommandType::NUM));
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

        bool get_name = false;
        while (getline(iss, input_after_space, ' ')) {
            if (!get_name) {
                name = input_after_space;
                get_name = true;
            }
            else
                command_args.push_back(input_after_space);
        }

        std::vector<CommandArg> send_args;
        for (auto& cmd_arg : command_args) 
            send_args.push_back(CommandArg(cmd_arg.c_str()));

        if (m_input != "history") {
            CommandOutput out;
            for (auto& cmd : gcommands) {
                if (cmd.name == name) {
                    if (send_args.size() < cmd.arg_types.size() || (send_args.size() > cmd.arg_types.size() && !cmd.variable)) {
                        m_output.push_back(NUM_ARGS_ERROR);
                        break;
                    }

                    for (int i = 0; i < cmd.arg_types.size(); i++) {
                        if (cmd.arg_types[i] == CommandType::NUM) {
                            if (IsNumber(send_args[i].str)) {
                                send_args[i].type = CommandType::NUM;
                                send_args[i].num = std::stod(send_args[i].str);
                            }
                            else {
                                m_output.push_back(NUM_TYPE_ERROR);
                                break;
                            }
                        }
                    }

                    if (cmd.variable) {
                        for (int i = cmd.arg_types.size(); i < send_args.size(); i++) {
                            if (cmd.variable_type == CommandType::NUM) {
                                if (IsNumber(send_args[i].str)) {
                                    send_args[i].type = CommandType::NUM;
                                    send_args[i].num = std::stod(send_args[i].str);
                                }
                                else {
                                    m_output.push_back(NUM_TYPE_ERROR);
                                    break;
                                }
                            }
                        }
                    }

                    if (out.error == NO_ERROR)
                        out = cmd.fn(send_args);
                }
            }
            if (out.error == NO_ERROR)
                m_output.push_back(out.output);
            else
                m_output.push_back(out.error);

            return;
        }

        for (auto& entry : m_history) 
            m_output.push_back(entry);  
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
}