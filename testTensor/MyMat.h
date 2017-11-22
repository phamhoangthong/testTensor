#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

template<typename T> class MyMat
{
private:
	T *p_data = nullptr;
	size_t mCol, mRow;
	size_t mStep;
	size_t mSize;
public:
	MyMat() {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mCol = 0;
		mRow = 0;
		mStep = 0;
		mSize = 0;
	}

	MyMat(size_t row, size_t col) {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mCol = col;
		mRow = row;
		mStep = col;
		mSize = col*row;
		p_data = new T[mSize];
	}

	MyMat(T *data,size_t row, size_t col) {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mCol = col;
		mRow = row;
		mStep = col;
		mSize = col*row;
		p_data = new T[mSize];
		memcpy(p_data, data, mSize * sizeof(T));
	}

	MyMat<T> operator=(MyMat<T> &obj) {
		return MyMat<T>(obj.getData(), obj.getRow(), obj.getCol());
	}

	MyMat<T> operator+(MyMat<T> &obj) {
		if ((mCol == obj.getCol()) && (mRow == obj.getRow())) {
			MyMat<T> mReturn(mRow, mCol);
			T* p_data_obj = obj.getData();
			T* p_data_return = mReturn.getData();
			for (size_t i = 0; i < mSize; i++) {
				p_data_return[i] = p_data_obj[i] + p_data[i];
			}
			return mReturn;
		}
		else {
			return NULL;
		}
	}

	MyMat<T> operator+(T &obj) {
		MyMat<T> mReturn(mRow, mCol);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < mSize; i++) {
			p_data_return[i] = obj + p_data[i];
		}
		return mReturn;
	}

	MyMat<T> operator-(MyMat<T> &obj) {
		if ((mCol == obj.getCol()) && (mRow == obj.getRow())) {
			MyMat<T> mReturn(mRow, mCol);
			T* p_data_obj = obj.getData();
			T* p_data_return = mReturn.getData();
			for (size_t i = 0; i < mSize; i++) {
				p_data_return[i] = p_data[i] - p_data_obj[i];
			}
			return mReturn;
		}
		else {
			return MyMat<T>();
		}
	}

	MyMat<T> operator-(T &obj) {
		MyMat<T> mReturn(mRow, mCol);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < mSize; i++) {
			p_data_return[i] = p_data[i] - obj;
		}
		return mReturn;
	}

	MyMat<T> operator*(MyMat<T> &obj) {
		if (mCol == obj.getRow()) {
			size_t tCol = obj.getcol();
			MyMat<T> mReturn(mRow, tCol);
			T* p_data_obj = obj.getData();
			T* p_data_return = mReturn.getData();
			size_t index_obj, index_return, index;
			for (size_t i = 0; i < mRow; i++) {
				for (size_t j = 0; j < tCol; j++) {
					T tValue = 0;
					index = i*mStep;
					index_return = i*tCol + j;
					index_obj = j;
					for (size_t k = 0; k < mCol; k++) {
						tValue += p_data[index] * p_data_obj[index_obj];
						index++;
						index_obj += tCol;
					}
					p_data_return[index_return] = tValue;
				}
			}
			return mReturn;
		}
		else {
			return MyMat<T>();
		}
	}

	MyMat<T> operator*(T &obj) {
		MyMat<T> mReturn(mRow, mCol);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < mSize; i++) {
			p_data_return[i] = obj*p_data[i];
		}
		return mReturn;
	}

	MyMat<T> operator/(T &obj) {
		MyMat<T> mReturn(mRow, mCol);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < mSize; i++) {
			p_data_return[i] = p_data[i]/obj;
		}
		return mReturn;
	}

	MyMat<T> tranpose() {
		MyMat<T> mReturn(mCol, mRow);
		T* p_data_return = mReturn.getData();
		size_t index_return, index;
		size_t tStep = mRow;
		for (size_t i = 0; i < mRow; i++) {
			index = i*step;
			index_return = i;
			for (size_t j = 0; j < mCol; j++) {
				p_data_return[index_return] = p_data[index];
				i++;
				index_return += tStep;
			}
		}
		return mReturn;
	}

	T* getData() {
		return p_data;
	}

	size_t getCol() {
		return mCol;
	}

	size_t getRow() {
		return mRow;
	}

	virtual ~MyMat() {
		if (p_data != nullptr) {
			delete[] p_data;
		}
		mCol = 0;
		mRow = 0;
		mStep = 0;
		mSize = 0;
	}
};

