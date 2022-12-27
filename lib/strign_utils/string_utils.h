#ifndef StringUtils_H
#define StringUtils_H

#include <WString.h>
#include <sstream>
#include <vector>

using namespace std;

// template <typename Out>
// void split(const char* input, char delim, Out result) {
//     std::string s(input);
//     std::istringstream iss(s);
//     std::string item;
//     while (std::getline(iss, item, delim)) {
//         *result++ = item;
//     }
// }

vector<string> split_string (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

#endif