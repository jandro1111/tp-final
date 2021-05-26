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
	if (std::filesystem::exists(this->path)) {
		//Si se trata de un solo archivo, lo guarda
		if (std::filesystem::is_regular_file(this->path)) {
			std::filesystem::path p = this->path;
			if (p.extension() == ".json") {
				files.push_back(p);
			}
		}
		//Si contiene varios archivos o subdirectorios, los busca, chequea y guarda. 
		else {
			for (const auto& entry : std::filesystem::directory_iterator(this->path)) {
				if (this->isDirectory(entry)) {
					dirs.push_back(entry.path());
				}
				else {
					if (std::filesystem::is_regular_file(entry)) {
						if (entry.path().extension() == ".json") {
							files.push_back(entry.path());
						}
					}
				}
			}
		}
	}
	return (dirs.size() != 0 || files.size() != 0);
}

std::vector<std::filesystem::path> Files::getFiles(void){
	return files;
}

std::vector<std::filesystem::path> Files::getDirs(void) {
	return dirs;
}
