# Minishell  
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://opensource.org/licenses/GPL-3.0)  

Minishell is a project that recreates the basic functionalities of the Bash shell, offering a hands-on approach to understanding shell implementation and behavior.  

---


## 📚 Table of Contents  
- [Features](#-features)
- [Requirements](#-requirements)
- [Installation](#-installation)  
- [Building the Program](#️-build-the-program)  
- [Running the Program](#-running-the-program)  
- [Cleaning Up](#-cleaning-up)  
- [License](#license)  

---

## ✨ Features  

Minishell includes the following functionalities:  

### General Behavior
- Displays a prompt when waiting for user input.
- Maintains a working command history.
- Searches and executes the correct binaries using the `PATH` variable, or via relative/absolute paths.

### Quote Handling
- Single quotes (`'`) disable interpretation of special characters.
- Double quotes (`"`) allow interpretation of `$` (dollar sign) while disabling others.  

### Redirections  
- `<` redirects input.  
- `>` redirects output.  
- `<<` (heredoc) reads input until a delimiter is reached (not added to history).  
- `>>` appends output.  

### Pipelines
- Implements pipes (`|`) to connect commands in a sequence.  

### Environment Variables
- Expands variables prefixed with `$`.  
- Supports `$?` to retrieve the exit status of the last executed command.  

### Signal Handling  
- `ctrl-C`: Displays a new prompt on a new line.  
- `ctrl-D`: Exits the shell.  
- `ctrl-\`: Does nothing (interactive mode).  

### Built-in Commands
- **`echo`**: Includes the `-n` option.  
- **`cd`**: Accepts relative or absolute paths.  
- **`pwd`**: Prints the current directory.  
- **`export`**: Sets environment variables.  
- **`unset`**: Removes environment variables.  
- **`env`**: Displays environment variables.  
- **`exit`**: Exits the shell.  

---

## 📦 Requirements

Depending on your system, you may need to install the following dependencies
```bash
libreadline
```

Using APT:

```bash
sudo apt install libreadline-dev
```

---

## 🚀 Installation  

Clone the repository and navigate to the project directory:  
```bash
git clone https://github.com/joellehtonen/minishell.git  
cd minishell 
```

---

## 🛠️ Build the Program

### Use `make` command to create the program:
```bash
make
```

---

## 🎮 Running the Program

To run the program, execute the following command:
```bash
./minishell
```

---

## 🧹 Cleaning Up

To remove object files and libraries:
```bash
make fclean
```

---

## 📜 License

This project is licensed under the GNU GENERAL PUBLIC LICENSE - see the LICENSE.md file for details
