#pragma once

#include "pch.h"

class BoomarkManager {
public:
	static std::string PATH_BOOMARKS;

	static void SaveBoomark();
	static void SaveBoomarkForVideo(std::filesystem::directory_entry videoEntry);

	/*
	Dont need to worry about videoPath and time, it will get auto processed in the videoManager app
	*/
	static void CreateBoomarkFile(std::string savePath, std::string boomarkName, std::string videoPath);
	//static void CreateBoomarkFile(std::string path, std::string name, std::string videoPath, int time);

	static int GetNumOfBoomarks();
};