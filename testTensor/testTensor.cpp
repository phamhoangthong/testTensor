// testTensor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

template<typename T> class MTensor {
private:
	T* m_data;
	size_t dim_x, dim_y, dim_z;
	size_t step_y, step_z;
public:
	MTensor();
	MTensor(size_t dim_x, size_t dim_y, size_t dim_z);
	virtual ~MTensor();

	MTensor<T> operator=(const MTensor<T> &obj);

	void setSize(size_t dim_x, size_t dim_y, size_t dim_z);
	void resize(size_t dim_x, size_t dim_y, size_t dim_z);
	
	size_t getDimX();
	size_t getDimY();
	size_t getDimZ();
	size_t getTotalSize();

	void getSize(size_t &dim_x, size_t &dim_y, size_t &dim_z);
	T* getData();
	T getValueData(size_t x, size_t y, size_t z);
	T getValueData(size_t index);

	void setValueData(size_t x, size_t y, size_t z, T data);
	void setValueData(size_t index, T data);
	void setData(T* data);
	void setDataXY(size_t z, T* data) {

	}
};

template<typename T> MTensor<T>::MTensor() {
	this->dim_x = 0;
	this->dim_y = 0;
	this->dim_z = 0;
	m_data = nullptr;
}

template<typename T> MTensor<T>::MTensor(size_t dim_x, size_t dim_y, size_t dim_z) {
	this->dim_x = dim_x;
	this->dim_y = dim_y;
	this->dim_z = dim_z;
	m_data = new T[this->dim_x*this->dim_y*this->dim_z];
	step_y = dim_x;
	step_z = dim_x * dim_y;
}

template<typename T> MTensor<T>::~MTensor() {
	if (m_data != nullptr) {
		delete[] m_data;
		m_data = nullptr;
	}
}

template<typename T> MTensor<T> MTensor<T>::operator=(const MTensor<T> &obj) {
	MTensor<T> m_return;
	m_return.setSize(obj.getDimX(), obj.getDimY, obj.getDimZ);
	memcpy(m_return.getData, obj.getData, obj.getTotalSize() * sizeof(T));
	return m_return;
}

template<typename T> void MTensor<T>::setSize(size_t dim_x, size_t dim_y, size_t dim_z) {
	resize(dim_x, dim_y, dim_z)
}

template<typename T> void MTensor<T>::resize(size_t dim_x, size_t dim_y, size_t dim_z) {
	if (m_data != nullptr) {
		delete[] m_data;
	}
	this->dim_x = dim_x;
	this->dim_y = dim_y;
	this->dim_z = dim_z;
	m_data = new T[this->dim_x*this->dim_y*this->dim_z];
	step_y = dim_x;
	step_z = dim_x * dim_y;
}

template<typename T> size_t MTensor<T>::getDimX() {
	return dim_x;
}

template<typename T> size_t MTensor<T>::getDimY() {
	return dim_y;
}

template<typename T> size_t MTensor<T>::getDimZ() {
	return dim_z;
}

template<typename T> size_t MTensor<T>::getTotalSize() {
	return dim_x*dim_y*dim_z;
}

template<typename T> void MTensor<T>::getSize(size_t &dim_x, size_t &dim_y, size_t &dim_z) {
	dim_x = this->dim_x;
	dim_y = this->dim_y;
	dim_z = this->dim_z;
}

template<typename T> T* MTensor<T>::getData() {
	return m_data;
}

template<typename T> T MTensor<T>::getValueData(size_t x, size_t y, size_t z) {
	size_t index = z*step_z + y*step_y + x;
	return m_data[index];
}

template<typename T> T MTensor<T>::getValueData(size_t index) {
	return m_data[index];
}

template<typename T> void MTensor<T>::setValueData(size_t x, size_t y, size_t z, T data) {
	size_t index = z*step_z + y*step_y + x;
	m_data[index] = data;
}

template<typename T> void MTensor<T>::setValueData(size_t index, T data) {
	m_data[index] = data;
}

template<typename T> void MTensor<T>::setData(T* data) {
	memcpy(m_data, data, getTotalSize() * sizeof(T));
}

int main()
{
	MTensor<double> m_tensor = MTensor<double>(10, 10, 10);
    return 0;
}

