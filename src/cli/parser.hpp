#pragma once

#include <string>
#include <optional>
#include <vector>
#include <algorithm>
#include <cctype>

namespace cli {

// TODO: Define CommandType enum
// Values: Help, Add, List, Remove, Edit, Show, Unknown

enum CommandType {
    Help,
    Add,
    Edit,
    Remove,
    List,
    Show,
    Unknown
};

// TODO: Define Command struct
// Fields: type, site, username, password

struct Command {
    CommandType type;
    std::optional<std::string> site;
    std::optional<std::string> username;
    std::optional<std::string> password;
};

// TODO: Declare parseArguments function
// Takes: int argc, char** argv
// Returns: Command
Command parseArguments(std::string argc);

// TODO: Declare printHelp function
// Takes: optional command name string
void printHelp(const std::string& command = "");

std::string toLower(std::string data);

} // namespace cli
