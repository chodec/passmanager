#include "app.hpp"
#include "storage.hpp"
#include <unistd.h>

namespace app {

int validateInput(std::string input){
    if(input == ""){
        std::cout << "passmanager>You have enter some value" << std::endl;
        return 1;
    }
    if(input == "s" || input == "u" || input == "p"){
        return 0;
    }
    return 0;
}

// TODO: Implement run function
// Use switch on command.type to call appropriate handler
// Return handler's result

// TODO: Implement handleAdd function
// Validate required fields (site, username, password)
// Print stub message for now
// Return 0 for success, 1 for error
int handleAdd(std::string filename, std::string masterPassword) {
    storage::PasswordEntry entry;
    std::cout << "passmanager>Enter site: ";
    std::string site;
    std::getline(std::cin, site);

    if(validateInput(site) != 0) {
        return 1;
    }
    entry.site = site;

    std::cout << "passmanager>Enter username: ";
    std::string username;
    std::getline(std::cin, username);

    if(validateInput(username) != 0) {
        return 1;
    }
    entry.username = username;

    std::string pass = getpass("passmanager>Enter password: ");

    if(validateInput(pass) != 0) {
        return 1;
    }
    entry.password = pass;
    storage::savePassword(entry,filename, masterPassword);

    return 0;
}

// TODO: Implement handleList function
// Print stub message
// Return 0
int handleList(std::string filename, std::string masterPassword) {
    storage::showSites(filename, masterPassword);
    return 0;
}

// TODO: Implement handleGet function
// Validate site is provided
// Print stub message
// Return 0 for success, 1 for error
    int handleGet(std::string filename, std::string masterPassword) {
        std::cout << "passmanager>Enter site: ";
        std::string site;
        std::getline(std::cin, site);
            if(validateInput(site) != 0) {
            return 1;
        }
        storage::showPassword(filename, site, masterPassword);
        return 0;
    }

// TODO: Implement handleRemove function
// Validate site is provided
// Print stub message
// Return 0 for success, 1 for error
    int handleRemove(std::string filename, std::string masterPassword) {
        std::cout << "passmanager>Enter site: ";
        std::string site;
        std::getline(std::cin, site);
            if(validateInput(site) != 0) {
            return 1;
        }
        storage::removeSite(filename, site, masterPassword);
        return 0;
    }


// TODO: Implement handleEdit function
// Validate site is provided
// Print stub message
// Return 0 for success, 1 for error
    int handleEdit(std::string filename, std::string masterPassword){
        std::cout << "passmanager>Enter site: ";
        std::string site;
        std::getline(std::cin, site);
            if(validateInput(site) != 0) {
            return 1;
        }

        std::cout << "passmanager> " << "Your accout details: "<< std::endl;   
        int val = storage::getSiteDetails(filename, site, masterPassword);

        if(val != 0){
            std::cout << "passmanager> " << "Could not find your site"<< std::endl;
            return 1;
        }

        std::cout << "passmanager> " << "Do you want to change (s)ite, (u)sername, (p)assword?";
        std::string change;
        std::getline(std::cin, change);
            if(validateInput(change) != 0) {
            return 1;
        }
        std::cout << "passmanager>Enter value to change " << change <<": ";
        std::string changeItem;
        std::getline(std::cin, changeItem);
            if(validateInput(changeItem) != 0) {
            return 1;
        }
        if (change == "s"){
            change = "site";
        } else if (change == "u"){
            change = "user";
        } else if (change == "p"){
            change = "password";
        }
        
        storage::editSiteDetails(filename, site, change, changeItem, masterPassword);
        std::cout << "passmanager> Value changed" << std::endl;

        return 0;
    }

    std::optional<std::string> getMastePassword(std::string filename) {
        auto master = storage::showMasterPassword(filename);
        return master;
    }

    int setMasterPassword(std::string filename) {
        std::cout << "passmanager> Iniciate master password: ";
        std::string masterpass;
        std::getline(std::cin, masterpass);
            if(validateInput(masterpass) != 0) { //TODO: pass rules
            return 1;
        }
        
        storage::setMasterPassword(filename,masterpass);
        return 0;
    }

    std::string logIn(std::string filename){
        std::cout << "passmanager> Enter password to Log In: ";
        std::string masterpass;
        std::getline(std::cin, masterpass);
            if(validateInput(masterpass) != 0) {
            return "";
        }

        int output = storage::logInMasterPassword(filename,masterpass);
        if(output == 1){
            return masterpass;
        } else if( output == 0){
            return "";
        }
        return "";
    }
} // namespace app
