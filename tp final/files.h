#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>


//Files: navegacion en un directorio
class Files
{
public:
	Files();

	//Setter del path
	void setPath(std::string path = "./blockchain");

	//Abre el directorio y guarda sus archivos y subdirectorios en arreglos dinamicos.
	bool openDirectory(void);

	//Chequea que sea un directorio
	bool isDirectory(std::filesystem::path);

	//Getters para paths de archivos y subdirectorios del path
	std::vector<std::filesystem::path> getFiles(void);
	std::vector<std::filesystem::path> getDirs(void);

private:
	std::string path;
	std::vector<std::filesystem::path> dirs;
	std::vector<std::filesystem::path> files;
};
