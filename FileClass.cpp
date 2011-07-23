#include <windows.h>
#include <fstream>
#include <string>
#include "FileClass.h"

bool FileClass::WriteLine(bool newfile, LPTSTR text) {
	std::ofstream myfile;
	if(newfile)
		myfile.open("data.dat", std::ios::trunc | std::ios::out);
	else
		myfile.open("data.dat", std::ios::out | std::ios::app);

	if(myfile.is_open()) {
		myfile << text;
		myfile.close();
		return true;
	}
	return false;
}

bool FileClass::LineExists(std::string path) {
	std::string line;
	std::ifstream myfile ("data.dat");

	if(myfile.is_open()) {
		while(!myfile.eof()) {
			getline(myfile, line);
			if(path == line) {
				myfile.close();
				return true;
			}
		}

		myfile.close();
		return false;
	}
	return false;
}