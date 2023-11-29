#pragma once

#include "pch.h"

class KeyListener {
public:
    static std::map<int, bool> mapKeysState;
    static std::map<int, std::function<void()>> mapFunction;

    static void Update();

    static void Listen(int key, std::function<void()> fn);
};