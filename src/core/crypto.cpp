#include "crypto.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <cstring>

namespace crypto {

    std::string bytesToHex(const unsigned char* bytes, size_t len) {
        std::string hex;
        char buf[3];
        for (size_t i = 0; i < len; i++) {
            snprintf(buf, sizeof(buf), "%02x", bytes[i]);
            hex += buf;
        }
        return hex;
    }

    std::string hashPassword(const std::string& password) {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hashLen;

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, password.c_str(), password.length());
        EVP_DigestFinal_ex(ctx, hash, &hashLen);
        EVP_MD_CTX_free(ctx);

        return bytesToHex(hash, hashLen);
    }

    std::vector<unsigned char> hexToBytes(const std::string& hex) {
        std::vector<unsigned char> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            unsigned int byte;
            sscanf(hex.substr(i, 2).c_str(), "%02x", &byte);
            bytes.push_back(static_cast<unsigned char>(byte));
        }
        return bytes;
    }

    std::optional<std::string> encrypt(const std::string& plaintext, const std::string& password) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return std::nullopt;

        unsigned char key[32];
        unsigned char iv[16];
        unsigned char salt[8];

        if (!RAND_bytes(salt, sizeof(salt))) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        if (!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt,
                           (const unsigned char*)password.c_str(), password.length(),
                           10000, key, iv)) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int outLen1 = 0, outLen2 = 0;

        if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &outLen1,
                              (const unsigned char*)plaintext.c_str(), plaintext.size())) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + outLen1, &outLen2)) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        EVP_CIPHER_CTX_free(ctx);

        ciphertext.resize(outLen1 + outLen2);

        std::string result;
        result += bytesToHex(salt, 8);
        result += ":";
        result += bytesToHex(iv, 16);
        result += ":";
        result += bytesToHex(ciphertext.data(), ciphertext.size());

        return result;
    }

    std::optional<std::string> decrypt(const std::string& ciphertext, const std::string& password) {
        std::vector<std::string> parts;
        std::string part;
        for (char c : ciphertext) {
            if (c == ':') {
                parts.push_back(part);
                part.clear();
            } else {
                part += c;
            }
        }
        parts.push_back(part);

        if (parts.size() != 3) return std::nullopt;

        std::vector<unsigned char> salt = hexToBytes(parts[0]);
        std::vector<unsigned char> iv = hexToBytes(parts[1]);
        std::vector<unsigned char> encrypted = hexToBytes(parts[2]);

        unsigned char key[32];

        if (!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt.data(),
                           (const unsigned char*)password.c_str(), password.length(),
                           10000, key, iv.data())) {
            return std::nullopt;
        }

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return std::nullopt;

        if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        std::vector<unsigned char> plaintext(encrypted.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int outLen1 = 0, outLen2 = 0;

        if (!EVP_DecryptUpdate(ctx, plaintext.data(), &outLen1, encrypted.data(), encrypted.size())) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + outLen1, &outLen2)) {
            EVP_CIPHER_CTX_free(ctx);
            return std::nullopt;
        }

        EVP_CIPHER_CTX_free(ctx);
        plaintext.resize(outLen1 + outLen2);

        return std::string(plaintext.begin(), plaintext.end());
    }

}
