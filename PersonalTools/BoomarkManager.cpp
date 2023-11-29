#include "BoomarkManager.h"

std::string BoomarkManager::PATH_BOOMARKS = "C:\\Users\\danil\\Documents\\Video Boomarks\\boomarks";

time_t GetFileChangeTime(std::filesystem::directory_entry entry)
{
	struct stat t_stat;
	stat(entry.path().string().c_str(), &t_stat);
	time_t modTime = t_stat.st_ctime;

	return modTime;
}

void BoomarkManager::SaveBoomark()
{
	std::cout << "BoomarkManager::SaveBoomark" << std::endl;

	auto entries = FindEntriesInPath("C:\\Users\\danil\\Videos");

	if (entries.size() == 0)
	{
		std::cout << "No files found!" << std::endl;
		return;
	}

	auto lastFile = entries[0];
	time_t lastFileChangeTime = GetFileChangeTime(lastFile);

	for (auto entry : entries)
	{
		time_t changeTime = GetFileChangeTime(entry);

		if (changeTime > lastFileChangeTime)
		{
			lastFile = entry;
			lastFileChangeTime = changeTime;

			//std::cout << entry.path() << ", changeTime: " << changeTime << std::endl;
		}

	}

	std::cout << lastFile.path() << ", changeTime: " << lastFileChangeTime << std::endl;

	SaveBoomarkForVideo(lastFile);


}

void BoomarkManager::SaveBoomarkForVideo(std::filesystem::directory_entry videoEntry)
{
	std::cout << "BoomarkManager::SaveBoomarkForVideo " << videoEntry.path() << std::endl;

	//
	int totalBoomarks = GetNumOfBoomarks();
	int i = 0;
	int found = 0;
	int index = -1;
	
	while (found < totalBoomarks && index == -1)
	{
		if (std::filesystem::exists(PATH_BOOMARKS + "\\" + std::to_string(i) + ".json")) {
			found++;
		}
		else {
			index = i;
		}
		i++;
	}

	if (index == -1) index = totalBoomarks;

	//

	struct stat t_stat;
	stat(videoEntry.path().string().c_str(), &t_stat);
	time_t creationTime = t_stat.st_ctime;

	time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	time_t time = now - creationTime;
	std::cout << "Created at: " << creationTime << ", now: " << now << ", time: " << time << std::endl;

	//

	std::cout << "Saving boomark index " << index << std::endl;

	char boomarkPath[256];
	sprintf_s(boomarkPath, "%s\\%i.json", PATH_BOOMARKS.c_str(), index);

	//CreateBoomarkFile(boomarkPath, "Boomark", videoPath, (int)((float)time * 1000.0f));
	CreateBoomarkFile(boomarkPath, "Boomark", videoEntry.path().filename().string());
}


void BoomarkManager::CreateBoomarkFile(std::string savePath, std::string boomarkName, std::string videoName)
{
	std::cout << "BoomarkManager::CreateBoomarkFile " << savePath << std::endl;

	char data[512];
	//sprintf_s(data, "{\"name\": \"%s\", \"video\": \"%s\", \"time\": %d}", boomarkName.c_str(), videoPath.c_str(), 0);
	sprintf_s(data, "{\"name\": \"%s\", \"videoName\": \"%s\"}", boomarkName.c_str(), videoName.c_str());

	std::ofstream myfile;
	myfile.open(savePath);
	myfile << data;
	myfile.close();
}

int BoomarkManager::GetNumOfBoomarks()
{
	std::string path = PATH_BOOMARKS;

	int boomarks = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		boomarks++;
	}

	return boomarks;
}