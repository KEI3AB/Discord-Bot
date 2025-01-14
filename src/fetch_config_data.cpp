#include "fetch_config_data.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

std::string fetch_config_data(const std::string& key) {
    std::ifstream config_file("../config/config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("Could not open the config.json file");
    }

    nlohmann::json config;
    config_file >> config;

    if (!config.contains(key)) {
        throw std::runtime_error("Key '" + key + "' not found in the config.json file");
    }

    config_file.close();
    
    return config[key];
}