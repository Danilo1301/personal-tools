#include "KeyListener.h"

std::map<int, bool> KeyListener::mapKeysState;
std::map<int, std::function<void()>> KeyListener::mapFunction;

void KeyListener::Update()
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

void KeyListener::Listen(int key, std::function<void()> fn)
{
    mapKeysState.insert(std::pair<int, bool>(key, false));
    mapFunction.insert(std::pair<int, std::function<void()>>(key, fn));
}