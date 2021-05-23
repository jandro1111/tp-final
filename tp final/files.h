#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>

class Files
{
public:
	Files();
	void setPath(std::string path = "./blockchain");
	bool openDirectory(void);
	bool isDirectory(std::filesystem::path);
	std::vector<std::filesystem::path> getFiles(void);
	std::vector<std::filesystem::path> getDirs(void);

private:
	std::string path;
	std::vector<std::filesystem::path> dirs;
	std::vector<std::filesystem::path> files;
};
