
#include "semver.h"
#include <sstream>
#include <vector>
#include <Arduino.h>
#include "string_utils.h"

using namespace std;

semver_t from_string(string version){
    auto numbers = split_string(version, '.');
    auto major = std::atoi(numbers.at(0).c_str());
    auto minor = std::atoi(numbers.at(1).c_str());
    int patch;
    char *prerelease_ptr = nullptr;

    auto split_at = numbers.at(2).find('-');
    if(split_at != std::string::npos){
        patch = std::atoi(numbers.at(2).substr(0, split_at).c_str());
        auto prerelease = numbers.at(2).substr(split_at + 1);
        prerelease_ptr = (char*)malloc(prerelease.length());
        prerelease.copy(prerelease_ptr, prerelease.length());
    }else{
        patch = std::atoi(numbers.at(2).c_str());
    }

    semver_t _ver = semver_t{
        major, minor, patch,
        0, prerelease_ptr
    };

    return _ver;
}

string render_to_string(semver_t* sem){
    char *str = (char*)calloc(255, sizeof(char));
    Serial.printf("Empty: %s\n", str);
    semver_render(sem, (char*)str);
    Serial.printf("Rendered: %s\n", str);
    auto value = string((char*)str);
    Serial.printf("value: %s\n", value.c_str());
    free(str);
    Serial.printf("After free1: %s\n", str);
    Serial.printf("After free2: %s\n", value.c_str());
    return value.c_str();
}