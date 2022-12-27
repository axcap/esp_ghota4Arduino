#include <Arduino.h>
#include "SemverClass.h"

#include "string_utils.h"

SemverClass::SemverClass(void)
{
    _ver = semver_t{0,0,0};
}

SemverClass::SemverClass( const char* version )
{
  assert(version);
  if (semver_parse_version(version, &_ver)) {
      Serial.printf( "Invalid semver string '%s' passed to constructor. Defaulting to 0\n", version );
      _ver = semver_t{0,0,0};
  }
}

SemverClass SemverClass::fromString(String version)
{
    version = version.substring(1);
    std::vector<std::string> elems;
    split(version, '.', std::back_inserter(elems));  

    return SemverClass(std::stoi(elems.at(0)), std::stoi(elems.at(1)), std::stoi(elems.at(2)));
}

SemverClass::SemverClass( int major, int minor, int patch )
{
    _ver = semver_t{major, minor, patch};
}

semver_t* SemverClass::ver()
{
    return &_ver;
}