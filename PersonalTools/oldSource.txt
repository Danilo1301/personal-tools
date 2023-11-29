#include <iostream>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <functional>
#include <map>
#include <comdef.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
 
#pragma comment(lib, "winmm.lib")

const bool SHOW_CONSOLE = false;

void SetKeyState(int key, bool bState)
{
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);

    if ((bState && !(keyState[key] & 1)) ||
        (!bState && (keyState[key] & 1)))
    {
        keybd_event(key,
            0x45,
            KEYEVENTF_EXTENDEDKEY | 0,
            0);

        keybd_event(key,
            0x45,
            KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
            0);
    }
}

bool IsGTARunning()
{
    const char* str = "GTA:SA:MP";
    size_t size = strlen(str) + 1;
    wchar_t* name = new wchar_t[size];

    size_t outSize;
    mbstowcs_s(&outSize, name, size, str, size - 1);

    HWND hwnd;
    hwnd = FindWindow(NULL, name);
    if (hwnd != 0) {
        return true;
    }
    else {
        return false;
    }
}

class KeyListener {
public:
    static std::map<int, bool> mapKeysState;
    static std::map<int, std::function<void()>> mapFunction;

    static void update()
    {
        for (std::map<int, bool>::iterator it = mapKeysState.begin(); it != mapKeysState.end(); ++it)
        {
            int key = it->first;
            bool prevState = it->second;

            bool newState = (GetKeyState(key) & 0x8000);

            if (prevState != newState)
            {
                if (prevState == true && newState == false) { mapFunction.at(key)(); }

                it->second = newState;
            }
        }
    }

    static void listen(int key, std::function<void()> fn)
    {
        mapKeysState.insert(std::pair<int, bool>(key, false));
        mapFunction.insert(std::pair<int, std::function<void()>>(key, fn));

    }
};

std::map<int, bool> KeyListener::mapKeysState;
std::map<int, std::function<void()>> KeyListener::mapFunction;


std::string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

int GetBoomarks()
{
    std::string path = GetCurrentDirectory() + "\\boomarks";

    int boomarks = 0;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        boomarks++;
    }

    return boomarks;

    /*
    std::string path(GetCurrentDirectory());
    char newPath[256];
    sprintf_s(newPath, "%s\\boomarks\\total", path.c_str());

    std::string temp;
    std::ifstream infile;

    infile.open(newPath);

    int boomarks = 0;

    if (std::getline(infile, temp))
    {
        boomarks = std::stoi(temp);
    }
    else
    {
        std::cout << "Failed to read file 'boomarks'!" << std::endl;
    }

    infile.close();

    return boomarks;
    */
}

/*
void SetBoomarks(int boomarks)
{
    std::string path(GetCurrentDirectory());
    char newPath[256];
    sprintf_s(newPath, "%s\\boomarks\\total", path.c_str());

    std::ofstream myfile;
    myfile.open(newPath);
    myfile << boomarks;
    myfile.close();
}
*/

void CreateBoomarkFile(int boomark, std::string fileName)
{
    std::string path(GetCurrentDirectory());
    char newPath[256];
    sprintf_s(newPath, "%s\\boomarks\\%i", path.c_str(), boomark);

    std::ofstream myfile;
    myfile.open(newPath);
    myfile << fileName;
    myfile.close();
}

std::vector<std::filesystem::directory_entry> FindEntriesInPath(std::string path) {
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

void ProcessBoomark()
{
    if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) return;
    if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000)) return;
    
    //if (!(GetKeyState(VK_CONTROL) & 0x8000)) return;

    //if (!IsGTARunning()) { return; }

    auto entries = FindEntriesInPath("C:\\Users\\danil\\Videos");

    if (entries.size() == 0) return;

    auto lastFTime = std::filesystem::last_write_time(entries[0].path());
    auto lastFile = entries[0];

    for (auto entry : entries)
    {
        auto ftime = std::filesystem::last_write_time(entry.path());

        if (ftime > lastFTime) {
            lastFile = entry;
            lastFTime = ftime;
        }


        //std::cout << "File: " << entry.path() << std::endl;
    }

    //

    int totalBoomarks = GetBoomarks();
    int i = 0;
    int found = 0;
    int index = -1;
    std::string path = GetCurrentDirectory() + "\\boomarks";
    while (found < totalBoomarks && index == -1)
    {
        if (std::filesystem::exists(path + "\\" + std::to_string(i))) {
            found++;
        }
        else {
            index = i;
        }
        i++;
    }

    if (index == -1) index = totalBoomarks;

    //

    std::cout << "Saving boomark " << index << " for file: " << lastFile.path().filename() << std::endl;

    CreateBoomarkFile(index, lastFile.path().filename().string());

    PlaySound(TEXT("success.wav"), NULL, SND_FILENAME | SND_ASYNC);
 
}

void ProcessMuteAudio()
{
    SetKeyState(VK_VOLUME_MUTE, true);
    SetKeyState(VK_VOLUME_MUTE, false);
}

void ProcessAudioFix()
{
    SetKeyState(VK_VOLUME_UP, true);
    SetKeyState(VK_VOLUME_UP, false);
    SetKeyState(VK_VOLUME_DOWN, true);
    SetKeyState(VK_VOLUME_DOWN, false);
}

void ProcessKeyboardMouseLED()
{
    SetKeyState(VK_SCROLL, (GetKeyState(VK_LBUTTON) & 0x8000));

    //various bugs
}

int main()
{
    ::ShowWindow(::GetConsoleWindow(), SHOW_CONSOLE ? SW_SHOW : SW_HIDE);

    //KeyListener::listen(VK_F9, ProcessMuteAudio);
    //KeyListener::listen(VK_F10, ProcessAudioFix);
    KeyListener::listen(VK_F11, ProcessBoomark);
    KeyListener::listen(66, ProcessBoomark); //B

    while (true)
    {
        KeyListener::update();

        //ProcessKeyboardMouseLED();

        Sleep(1);
    }
}