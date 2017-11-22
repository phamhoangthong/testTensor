#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

template<typename T> class MySVM
{
private:
	T *p_data = nullptr;
	size_t mSize = 0;
public:
	MySVM() {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mSize = 0;
	}

	MySVM(size_t size) {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mSize = size;
		p_data = new T[mSize];
	}

	MySVM(T *data, size_t size) {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mSize = size;
		p_data = new T[mSize];
		memcpy(p_data, data, mSize * sizeof(T));
	}

	MySVM<T> operator=(MySVM<T> &obj) {
		return MySVM<T> mReturn(obj.getData(), obj.getSize());
	}

	size_t getSize() {
		return mSize;
	}

	T* getData() {
		return p_data;
	}

	size_t getSizeItem() {
		return sizeof(T);
	}

	virtual ~MySVM() {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		p_data = nullptr;
		mSize = 0;
	}
};

