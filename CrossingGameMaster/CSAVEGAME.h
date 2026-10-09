#pragma once

#include <string>
#include "SaveData.h"

class CSAVEGAME
{
public:
    static bool Save(const SaveData& data,
        const std::string& fileName);

    static bool Load(SaveData& data,
        const std::string& fileName);
};