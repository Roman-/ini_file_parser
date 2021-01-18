/* ini_parser.h
 * Simple config file parser.
 */

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

// remove whitespace characters from left and right of the string
static inline std::string trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// removes single and double quotes from the beginning and end of the string, e.g. "abc" => abc
static std::string unQuote(std::string s) {
    while (s.size() > 1 && (s.front() == '\"' || s.front() == '\'') && s.front() == s.back())
        s = s.substr(1, s.size() - 2);
    return s;
}

class CfgParser {
public:
    CfgParser(const std::string& cfgFilePath, const std::vector<std::string>& mustHaveKeys = {}) {
        std::ifstream file(cfgFilePath.c_str());
        if (!file.is_open()) {
            std::cerr << "CfgParser: can\'t open config file: " << cfgFilePath << std::endl;
            return;
        }
        std::string line;
        int lineIndex = 0;
        while (std::getline(file, line)) {
            ++lineIndex;
            size_t ioHash = line.find('#');
            size_t ioEqual = line.find('=');
            // comment line
            if (ioHash != std::string::npos && (ioEqual == std::string::npos || ioEqual > ioHash))
                continue;
            // line without 'equal' sign
            if (ioEqual == std::string::npos)
                continue;
            std::string keyString = unQuote(trim(line.substr(0, ioEqual)));
            std::string valString = unQuote(trim(line.substr(ioEqual+1)));
            if (keyString.empty() || valString.empty())
                std::cerr << "Config file " << cfgFilePath << ", line #" << lineIndex << " is bad. Line:\n" << line << std::endl;
            else
                vals_[keyString] = valString; // latter key overwrites
        }
        // check must-have fileds
        for (const std::string& k: mustHaveKeys) {
            if (!hasKey(k)) {
                std::cerr << "Config file " << cfgFilePath << " does not have required key \"" << k << "\"" << std::endl;
                return; // leaving isValid = false
        }   }
        isValid_ = true;
    }

    bool isValid() const {
        return isValid_;
    }
    // returns true if config contains value
    bool hasKey(const std::string& key) const {
        return vals_.find(key) != vals_.cend();
    }
    // return string value by key. If key is not found, throws std::out_of_range
    std::string getString(const std::string& key) const {
        return vals_.at(key);
    }
    // return string value by key. If key is not found, returns provided default value
    std::string getString(const std::string& key, const std::string& defaultValue) const {
        auto v = vals_.find(key);
        return (v == vals_.cend()) ? defaultValue : v->second;
    }
    // return int value by key. May throw std::out_of_range and std::invalid_arg
    int getInt(const std::string& key) const {
        return stoi(vals_.at(key));
    }
    // return string value by key. If key is not found, returns provided default value
    // may throw std::invalid_arg
    int getInt(const std::string& key, int defaultValue) const {
        auto v = vals_.find(key);
        return (v == vals_.cend()) ? defaultValue : stoi(v->second);
    }
    // return float value by key. May throw std::out_of_range and std::invalid_arg
    float getFloat(const std::string& key) const {
        return stof(vals_.at(key));
    }
    // return float value by key. If key is not found, returns provided default value
    // may throw std::invalid_arg
    int getFloat(const std::string& key, float defaultValue) const {
        auto v = vals_.find(key);
        return (v == vals_.cend()) ? defaultValue : stof(v->second);
    }
    // return value by key as double. May throw std::out_of_range and std::invalid_arg
    double getDouble(const std::string& key) const {
        return stod(vals_.at(key));
    }
    // return double value by key. If key is not found, returns provided default value
    // may throw std::invalid_arg
    int getDouble(const std::string& key, double defaultValue) const {
        auto v = vals_.find(key);
        return (v == vals_.cend()) ? defaultValue : stod(v->second);
    }
private:
    // values from cfg file
    std::map<std::string, std::string> vals_;
    bool isValid_ = false;
};

#endif // CONFIGPARSER_H
