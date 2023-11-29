#pragma once

#include <windows.h>
#include <iostream>

#include <vector>
#include <map>
#include <functional>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib") 

static std::vector<std::filesystem::directory_entry> FindEntriesInPath(std::string path) {
    std::vector<std::filesystem::directory_entry> entries;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {

        if (std::filesystem::is_directory(entry.path())) {
            for (auto entry : FindEntriesInPath(entry.path().string()))
                entries.push_back(entry);

            continue;
        }

        entries.push_back(entry);
    }

    return entries;
}
