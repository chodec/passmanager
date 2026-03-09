#include "storage.hpp"
#include "crypto.hpp"

using json = nlohmann::json;

namespace storage {

    std::optional<std::ifstream> openFile(std::string filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "passmanager> Cannot open file " << filename << std::endl;
            return std::nullopt;
        }
        return file;        
    }

    std::string initStorage(const std::string& filename) {
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cout << "passmanager> Initializing new file " << filename << std::endl;
            std::ofstream ostm(filename);
            ostm << "[]";
        }
        return filename;
    }

    std::string initMasterPassword(const std::string& filename) {
        std::ifstream file(filename);
        if(!file.is_open()){
            std::ofstream ostm(filename);
            ostm << "{\"masterPassword\": \"0\"}";
        }
        return filename;    
    }

    std::optional<std::string> showMasterPassword(std::string filename) {
        auto file = openFile(filename);
        if(!file){
            return "0";
        }    
        json data = json::parse(*file);

        if(data.contains("masterPassword")){
            std::string master = data["masterPassword"]; 
            return master;   
        }
        return "0";
    }

    int setMasterPassword(std::string filename, std::string password) {
        auto file = openFile(filename);
        if(!file){
            return 0;
        }
        json data = json::parse(*file);
        data["masterPassword"] = crypto::hashPassword(password);
        std::ofstream out(filename);
        out << data.dump(4);
        
        return 1;
    }

    int logInMasterPassword(std::string filename, std::string password) {
        auto file = openFile(filename);
        if(!file){
            return 0;
        }           
        json data = json::parse(*file);

        if(data.contains("masterPassword")){
            std::string master = data["masterPassword"];
            if(crypto::hashPassword(password) == master) {
                return 1;
            }
        }
        return 0;
    }

    void savePassword(PasswordEntry entry, std::string filename, std::string masterPassword) {
        auto file = openFile(filename);
        if(!file){
            return;
        }
        
        json data = json::parse(*file);

        for(auto item : data){
            if (item["site"] == entry.site) {
                std::cout << "passmanager> This site already has password." << std::endl;
                return;
            }
            
        }

        auto encrypted = crypto::encrypt(entry.password, masterPassword);
        if (!encrypted) {
            std::cout << "passmanager> Encryption failed." << std::endl;
            return;
        }

        json entryJson = {
            {"site", entry.site},
            {"user", entry.username},
            {"password", *encrypted}
        };

        data.push_back(entryJson);
        std::ofstream out(filename);
        out << data.dump(4);
        std::cout << "passmanager> Password for site " << entry.site << " was succefully saved." << std::endl;
    }

    void showPassword(std::string filename, std::string site, std::string masterPassword) {
        auto file = openFile(filename);
        if(!file){
            return;
        }

        json data = json::parse(*file);

        for(auto item : data){
            if (item["site"] == site) {
                auto decrypted = crypto::decrypt(item["password"].get<std::string>(), masterPassword);
                if (decrypted) {
                    std::cout << "passmanager> " << *decrypted << std::endl;
                } else {
                    std::cout << "passmanager> Decryption failed." << std::endl;
                }
                return ;
            }
        }
        std::cout << "passmanager> You do not have any password for this site" << std::endl;
    }

    void showSites(std::string filename, std::string masterPassword) {
        auto file = openFile(filename);
        if(!file){
            return;
        }

        json data = json::parse(*file);
        
        if(data.size() < 1) {
            std::cout << "passmanager> You do not have any sites saved" << std::endl;
            return;
        }
        for(auto item : data){
            std::cout << "passmanager> " << item["site"].get<std::string>() << std::endl;
        }
        return ;
    }

    void removeSite(std::string filename, std::string site, std::string masterPassword) {
        auto file = openFile(filename);
        if(!file){
            return;
        }

        json data = json::parse(*file);

        auto it = std::find_if(data.begin(), data.end(), 
            [&site](const auto& item) { 
                return item["site"] == site; 
        });
        if (it != data.end()) {
            data.erase(it);
            std::ofstream out(filename);
            out << data.dump(4);
            std::cout << "passmanager> Site: " << site << " was deleted" << std::endl;
        } else {
            std::cout << "passmanager> Could not find site: " << site << std::endl;
        }
    }

    int getSiteDetails(std::string filename, std::string site, std::string masterPassword) {
        auto file = openFile(filename);
        if(!file){
            return 1;
        }

        json data = json::parse(*file);
        
        for(auto item : data){
            if (item["site"] == site) {
                std::cout << item["site"].get<std::string>() << std::endl;
                std::cout << item["user"].get<std::string>() << std::endl;
                auto decrypted = crypto::decrypt(item["password"].get<std::string>(), masterPassword);
                if (decrypted) {
                    std::cout << *decrypted << std::endl;
                } else {
                    std::cout << "[decryption failed]" << std::endl;
                }
                return 0;
            }
        }
        return 1;
    }

    void editSiteDetails(std::string filename, std::string site, std::string change, std::string changeItem, std::string masterPassword) {
        auto file = openFile(filename);
        if(!file){
            return;
        }

        json data = json::parse(*file);
        
        for(auto& item : data){      
            if (item["site"] == site) {
                if (change == "password") {
                    auto encrypted = crypto::encrypt(changeItem, masterPassword);
                    if (encrypted) {
                        item["password"] = *encrypted;
                    }
                } else {
                    item[change] = changeItem; 
                }
                std::ofstream out(filename);
                out << data.dump(4);
                return;
            }
        }
    }
    
} //namespace storage