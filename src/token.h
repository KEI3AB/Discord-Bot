#include <fstream>
#include <nlohmann/json.hpp>

std::string get_bot_token() {
    std::ifstream config_file("../config/config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("Could not open the config.json file");
    }

    nlohmann::json config;
    config_file >> config;

    return config["token"];
}