#pragma once
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cerrno>

using namespace std;

class MyFile
{
public:
	MyFile();
	~MyFile();

	size_t read(string file, char* &data);
};

