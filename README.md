# Discord Bot Project

## Overview
This project is a Discord bot developed in C++ with various features and functionalities. It demonstrates the practical use of modern C++ features and libraries

## Features
- JSON configuration for easy customization.
- Scalable and extendable project structure.

## Technologies Used
- **C++ STL**
- **nlohmann/json**
- **DPP Library**
- **CMake**

## Dependencies
This project requires the following libraries:
- [DPP](https://github.com/brainboxdotcc/DPP.git) 
- [nlohmann/json](https://github.com/nlohmann/json.git)

You must install them in the `lib` directory (you can create it using the `mkdir lib` command).
Otherwise, the bot will not work.

## Configuration
The bot requires a `config/config.json` file for its configuration.
Configuration setup steps:
1. Create a `config` directory and `config.json` file:
```bash
mkdir config
cd config
touch config.json
```
2. Edit `config.json` file. Example:
```json
{
    "token" : "YOUR_BOT_TOKEN_HERE"
}
```

## How to Build Project
1. Clone  the repository:
```bash
git clone https://github.com/KEI3AB/Discord-Bot.git
cd Discord-Bot
```
2. Create a build directory and configure the project:
```bash
mkdir build
cd build
cmake ..
```
3. Build the project:
```bash
make
```

## Usage
Run the bot using the compiled executable:
```bash
./DiscordBotProject
```
## License
This project is open-source and available under the **MIT License**.