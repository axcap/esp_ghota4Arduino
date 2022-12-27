#ifndef StringUtils_H
#define StringUtils_H

#include <WString.h>
#include <sstream>

template <typename Out>
void split(String input, char delim, Out result) {
    std::string s(input.c_str());
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

#endif