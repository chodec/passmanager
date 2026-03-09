#include <iostream>
#include <string>
#include "cli/parser.hpp"
#include "core/storage.hpp"
#include "core/app.hpp"


int main() {
    std::string storage = storage::initStorage();
    std::string masterPassword = storage::initMasterPassword();
    auto getMasterPassword = storage::showMasterPassword(masterPassword);
    if(getMasterPassword.value() == "0" || !getMasterPassword.has_value()){
        app::setMasterPassword(masterPassword);
    }

    std::string enteredPassword = app::logIn(masterPassword);
    if(enteredPassword.empty()){
        std::cout << "passmanager> Password does not match." << std::endl;
        return 0;
    }

    while (true) {
        std::cout << "passmanager>";
        std::string input;
        std::getline(std::cin, input);

        if(input == "q" || input == "quit") {
            break;
        }

        cli::Command cmd = cli::parseArguments(input);
        if (cmd.type == cli::CommandType::Unknown) {
            cli::printHelp();
        }
        else if (cmd.type == cli::CommandType::Add) {
            app::handleAdd(storage, enteredPassword);
        }
        else if (cmd.type == cli::CommandType::Show){
            app::handleGet(storage, enteredPassword);
        }
        else if (cmd.type == cli::CommandType::List){
            app::handleList(storage, enteredPassword);
        }
        else if (cmd.type == cli::CommandType::Remove){
            app::handleRemove(storage, enteredPassword);
        }
        else if (cmd.type == cli::CommandType::Edit){
            app::handleEdit(storage, enteredPassword);
        }
    }
    return 0;
}
