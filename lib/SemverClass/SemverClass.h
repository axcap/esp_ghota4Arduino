#ifndef SemverClass_H
#define SemverClass_H

#include <Arduino.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

extern "C"
{
#include "semver.h"
}

struct SemverClass
{
public:
    SemverClass(void);
    SemverClass(const char *version);
    SemverClass(int major, int minor = 0, int patch = 0);
    ~SemverClass() { semver_free(&_ver); }
    semver_t *ver();

    static SemverClass fromString(String version);

    bool operator> (SemverClass const &obj) {
        return semver_gt(_ver, obj._ver);
    }

private:
    semver_t _ver = semver_t();
};
#endif
