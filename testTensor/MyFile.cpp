#include "stdafx.h"
#include "MyFile.h"


MyFile::MyFile()
{
}


MyFile::~MyFile()
{
}


size_t MyFile::read(string file, char* &data) {
	ifstream inStream(file.c_str(), ios::in | ios::binary);
	if (inStream) {
		size_t size_file;
		inStream.seekg(0, ios::end);
		size_file = inStream.tellg();
		inStream.seekg(0, ios::beg);
		if (data != nullptr) {
			delete[] data;
		}
		data = new char[size_file];
		inStream.read(data, size_file);
		inStream.close();
		return size_file;
	}
	else {
		return 0;
	}
}