#include "parser.hpp"
#include <iostream>

namespace cli {

std::string toLower(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return data;
}

// TODO: Implement printHelp function
// Print usage information for the tool
// Handle specific command help if command parameter provided
void printHelp(const std::string& command){ 
    if (command.empty()){
        std::cout << "Type <add> to save new password" << std::endl;
        std::cout << "Type <edit> to update site info" << std::endl;
        std::cout << "Type <remove> to remove site info" << std::endl;
        std::cout << "Type <list> to show all sites" << std::endl;
        std::cout << "Type <show> to display your password" << std::endl;
    } else if(toLower(command) == "add"){
        std::cout << "Add - store password to the manager" << std::endl;
        std::cout << "Type <add>" << std::endl;
        std::cout << "site     - where it belongs" << std::endl;
        std::cout << "username - your username" << std::endl;
        std::cout << "password - your password" << std::endl;
    } else if(toLower(command) == "edit"){
        std::cout << "Edit - edit password / site / username to the manager" << std::endl;
        std::cout << "Type <edit>" << std::endl;
        std::cout << "site     - where it belongs" << std::endl;
        std::cout << "username - your username" << std::endl;
        std::cout << "password - your password" << std::endl;
    } else if(toLower(command) == "remove"){
        std::cout << "Remove - remove site from the manager" << std::endl;
        std::cout << "Type <remove>" << std::endl;
        std::cout << "site     - which you want to remove" << std::endl;
    } else if(toLower(command) == "list"){
        std::cout << "List - Show all sites from the manager" << std::endl;
        std::cout << "Type - <list>" << std::endl;
    } else if(toLower(command) == "show"){
        std::cout << "Show - Show password from the manager" << std::endl;
        std::cout << "Type <show>" << std::endl;
        std::cout << "site     - which you want to show" << std::endl;
    }
};


// TODO: Implement parseArguments function
// Parse argc/argv into Command struct
// Handle: help, add, list, get, remove, edit commands
// Parse flags: --site/-s, --user/-u, --pass/-p, --show
// Validate required arguments for each command

Command parseArguments(std::string argc){
    Command cmd;
    if(argc == ""){
        std::cout << "You forgot the command" << std::endl;
        cmd.type = Unknown;
        return cmd;
    }

    if (toLower(argc) == "add"){
        cmd.type = CommandType::Add;
    }

    else if (toLower(argc) == "edit"){
        cmd.type = CommandType::Edit;      
    }

    else if (toLower(argc) == "remove"){
        cmd.type = CommandType::Remove;
    }
    
    else if (toLower(argc) == "list"){
        cmd.type = CommandType::List;
    }

    else if (toLower(argc) == "show"){
        cmd.type = CommandType::Show;
    }

    else if (toLower(argc) == "help"){
        cmd.type = CommandType::Help;
    }
   
    else {
        cmd.type = CommandType::Unknown;
    }
    
    return cmd;
};
} // namespace cli