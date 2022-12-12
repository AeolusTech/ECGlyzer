#ifndef PARSEARC_H
#define PARSEARC_H

#include <string>
#include <vector>

struct CHANNEL {
    char name[10];
    std::vector<short int> v;
};

std::vector<CHANNEL> ReadDataFromArc(const std::string& inputFilePath);
void SaveDataIntoCsv(std::vector<CHANNEL>&& channels, const std::string& outputFilePath);
int parsearc(const std::string& filename, const std::string& outputFilePath);

#endif  // PARSEARC_H
