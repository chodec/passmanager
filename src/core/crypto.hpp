#pragma once

#include <string>
#include <optional>

namespace crypto {

    std::optional<std::string> encrypt(const std::string& plaintext, const std::string& password);
    
    std::optional<std::string> decrypt(const std::string& ciphertext, const std::string& password);

    std::string hashPassword(const std::string& password);

}
