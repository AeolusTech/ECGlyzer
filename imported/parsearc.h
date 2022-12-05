#ifndef PARSEARC_H
#define PARSEARC_H

#include <string>
#include <vector>

struct CHANNEL {
    std::string name;
    std::vector<short int> v;
};

std::vector<CHANNEL> ReadArcData(const std::string& filename);

int parsearc(const std::string& filename, const std::string& outputFilePath);

#endif  // PARSEARC_H
