#include "files.h"

Files::Files(){
	path = "";
	files.clear();
	dirs.clear();
}

void Files::setPath(std::string path) {
	this->path = path;
	files.clear();
	dirs.clear();
}

bool Files::isDirectory(std::filesystem::path entry){
	return std::filesystem::is_directory(entry);
}

bool Files::openDirectory() {
	std::string str = "";
	bool check = false;
	if (std::filesystem::exists(this->path) && std::filesystem::is_directory(this->path)) {
		check = true;
		for (const auto& entry : std::filesystem::directory_iterator(this->path)) {
			if (this->isDirectory(entry)) {
				dirs.push_back(entry.path());
				std::cout << "Folder " << entry.path().filename().string() << std::endl;
			}
			else {
				if (entry.path().extension() == ".json") {
					files.push_back(entry.path());
					std::cout << "File " << entry.path().filename().string() << std::endl;
				}
			}
		}
	}
	return check;
}

std::vector<std::filesystem::path> Files::getFiles(void){
	return files;
}

std::vector<std::filesystem::path> Files::getDirs(void) {
	return dirs;
}
