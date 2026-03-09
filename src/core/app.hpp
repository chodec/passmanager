#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <optional>

namespace app {

// TODO: Declare run function
// Takes: const Command reference
// Returns: int (exit code)

// TODO: Declare handler functions for each command:
// handleAdd, handleList, handleGet, handleRemove, handleEdit
// All take const Command reference, return int
int validateInput(std::string input);

int handleAdd(std::string filename, std::string masterPassword);

int handleGet(std::string filename, std::string masterPassword);

int handleList(std::string filename, std::string masterPassword);

int handleRemove(std::string filename, std::string masterPassword);

int handleEdit(std::string filename, std::string masterPassword);

std::optional<std::string> getMastePassword(std::string filename);

int setMasterPassword(std::string filename);

std::string logIn(std::string filename);

} // namespace app
