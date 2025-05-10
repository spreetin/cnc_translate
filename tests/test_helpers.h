#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <variant>

template<typename K, typename V>
std::string serializeMap(const std::map<K, V> &map){
    std::stringstream ss;
    for (auto & i : map){
        ss << "Key: \"" << i.first << "\", Value: \"" << i.second << "\"" << std::endl;
    }
    return ss.str();
}

template<typename T>
std::string serializeVector(const std::vector<T> &vector){
    return "";
}

template<>
std::string serializeVector<std::string>(const std::vector<std::string> &vector){
    std::stringstream ss;
    for (auto & line : vector){
        ss << line << std::endl;
    }
    return ss.str();
}

template<>
std::string serializeVector<int>(const std::vector<int> &vector){
    std::stringstream ss;
    for (auto & line : vector){
        ss << std::to_string(line) << std::endl;
    }
    return ss.str();
}

std::string parseNodeValue(std::variant<int,double,std::string> &variant){
    switch (variant.index()){
    case 0:
        return "I: \"" + std::to_string(std::get<int>(variant)) + "\"";
    case 1:
        return "F: \"" + std::to_string(std::get<double>(variant)) + "\"";
    case 2:
        return "S: \"" + std::get<std::string>(variant) + "\"";
    }
    return "";
}

#endif // TEST_HELPERS_H
