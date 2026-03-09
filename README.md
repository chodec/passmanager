# PassManager 🔐

A secure command-line password manager written in C++.

## Features

- **AES-256-CBC encryption** - All passwords are encrypted before storage
- **SHA-256 hashed master password** - Master password is hashed, never stored in plaintext
- **Secure input** - Passwords are hidden during input using `getpass()`
- **Full CRUD** - Add, list, show, edit, and remove passwords

## Installation

```bash
git clone https://github.com/chodec/passmanager.git
cd passmanager
mkdir build && cd build
cmake -G Ninja ..
ninja
```

## Usage

```bash
./passmanager
```

### Commands

- `add` - Add a new password
- `list` - List all saved sites
- `show` - Show password for a specific site
- `edit` - Edit existing password entry
- `remove` - Remove a password entry
- `q` - Quit

## Security

- Master password is hashed using SHA-256
- Individual passwords are encrypted using AES-256-CBC
- Encryption key is derived from master password using PBKDF2 (10000 iterations)
- Each password has unique salt and IV

## Tech Stack

- C++17
- OpenSSL (EVP API)
- nlohmann/json
- CMake + Ninja

## License

MIT
