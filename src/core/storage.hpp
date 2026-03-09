#pragma once

#include <string>
#include <nlohmann/json.hpp> 
#include <optional> 
#include <fstream>
#include <iostream>

namespace storage {

    struct PasswordEntry {
        std::string site;
        std::string username;
        std::string password; 
    };

    std::optional<std::ifstream> openFile(std::string filename);

    std::string initStorage(const std::string& filename = "passwords.json");

    std::string initMasterPassword(const std::string& filename = "masterPassword.json");

    int setMasterPassword(std::string filename, std::string password);
    
    std::optional<std::string> showMasterPassword(std::string filename);

    int logInMasterPassword(std::string filename, std::string password);
    
    void savePassword(PasswordEntry entry, std::string filename, std::string masterPassword);

    void showPassword(std::string filename, std::string site, std::string masterPassword);

    void showSites(std::string filename, std::string masterPassword);

    void removeSite(std::string filename, std::string site, std::string masterPassword);

    int getSiteDetails(std::string filename, std::string site, std::string masterPassword);

    void editSiteDetails(std::string filename, std::string site, std::string change, std::string changeItem, std::string masterPassword);

} //namespace storage