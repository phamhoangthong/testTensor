#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <windows.h>

template<typename T> class Tensor
{
private:
	T* m_data;
	size_t dim_x, dim_y, dim_z;
	size_t step_y, step_z;
	size_t size;
public:
	Tensor();
	Tensor(size_t dim_x, size_t dim_y, size_t dim_z);
	virtual ~Tensor();

	Tensor<T> operator=(Tensor<T> &obj);

	void setSize(size_t dim_x, size_t dim_y, size_t dim_z);
	void resize(size_t dim_x, size_t dim_y, size_t dim_z);

	size_t getDimX();
	size_t getDimY();
	size_t getDimZ();
	size_t getTotalSize();
	size_t getIndex(size_t x, size_t y, size_t z);

	void getSize(size_t &dim_x, size_t &dim_y, size_t &dim_z);
	T* getData();
	T getValueData(size_t x, size_t y, size_t z);
	T getValueData(size_t index);
	T* getDataXY(size_t z);


	void setValueData(size_t x, size_t y, size_t z, T data);
	void setValueData(size_t index, T data);
	void setData(T* data);
	void setDataXY(size_t z, T* data);
	void clear();
};

template<typename T> Tensor<T>::Tensor() {
	this->dim_x = 0;
	this->dim_y = 0;
	this->dim_z = 0;
	m_data = nullptr;
	size = 0;
}

template<typename T> Tensor<T>::Tensor(size_t dim_x, size_t dim_y, size_t dim_z) {
	this->dim_x = dim_x;
	this->dim_y = dim_y;
	this->dim_z = dim_z;
	m_data = new T[this->dim_x*this->dim_y*this->dim_z];
	step_y = dim_x;
	step_z = dim_x * dim_y;
	size = dim_x * dim_y * dim_z;
}

template<typename T> Tensor<T>::~Tensor() {
	if (m_data != nullptr) {
		delete[] m_data;
		m_data = nullptr;
		size = 0;
	}
}

template<typename T> Tensor<T> Tensor<T>::operator=(Tensor<T> &obj) {
	Tensor<T> m_return(obj.getDimX(), obj.getDimY(), obj.getDimZ());
	memcpy(m_return.getData(), obj.getData(), obj.getTotalSize() * sizeof(T));
	return m_return;
}

template<typename T> void Tensor<T>::setSize(size_t dim_x, size_t dim_y, size_t dim_z) {
	resize(dim_x, dim_y, dim_z)
}

template<typename T> void Tensor<T>::resize(size_t dim_x, size_t dim_y, size_t dim_z) {
	if (m_data != nullptr) {
		delete[] m_data;
	}
	this->dim_x = dim_x;
	this->dim_y = dim_y;
	this->dim_z = dim_z;
	m_data = new T[this->dim_x*this->dim_y*this->dim_z];
	step_y = dim_x;
	step_z = dim_x * dim_y;
	size = dim_x * dim_y * dim_z;
}

template<typename T> size_t Tensor<T>::getDimX() {
	return dim_x;
}

template<typename T> size_t Tensor<T>::getDimY() {
	return dim_y;
}

template<typename T> size_t Tensor<T>::getDimZ() {
	return dim_z;
}

template<typename T> size_t Tensor<T>::getTotalSize() {
	return size;
}

template<typename T> void Tensor<T>::getSize(size_t &dim_x, size_t &dim_y, size_t &dim_z) {
	dim_x = this->dim_x;
	dim_y = this->dim_y;
	dim_z = this->dim_z;
}

template<typename T> size_t Tensor<T>::getIndex(size_t x, size_t y, size_t z) {
	return z*step_z + y*step_y + x;
}

template<typename T> T* Tensor<T>::getData() {
	return m_data;
}

template<typename T> T Tensor<T>::getValueData(size_t x, size_t y, size_t z) {
	if ((x < dim_x) && (y < dim_y) && (z < dim_z)) {
		size_t index = getIndex(x, y, z);
		return m_data[index];
	}
	else {
		return 0;
	}

}

template<typename T> T Tensor<T>::getValueData(size_t index) {
	if (index < size) {
		return m_data[index];
	}
	else {
		return 0;
	}

}

template<typename T> void Tensor<T>::setValueData(size_t x, size_t y, size_t z, T data) {
	if ((x < dim_x) && (y < dim_y) && (z < dim_z)) {
		size_t index = getIndex(x, y, z);
		m_data[index] = data;
	}
}

template<typename T> void Tensor<T>::setValueData(size_t index, T data) {
	if (index < size) {
		m_data[index] = data;
	}
}

template<typename T> void Tensor<T>::setData(T* data) {
	memcpy(m_data, data, size * sizeof(T));
}

template<typename T> void Tensor<T>::setDataXY(size_t z, T* data) {
	if (z < dim_z) {
		size_t index = z*step_z;
		memcpy(m_data + index, data, step_z * sizeof(T));
	}
}

template<typename T> T* Tensor<T>::getDataXY(size_t z) {
	if (z < dim_z) {
		return (m_data + z*step_z);
	}
	else {
		return nullptr;
	}
}

template<typename T> void Tensor<T>::clear() {
	size_t index = 0;
	memset(m_data, 0, size * sizeof(T));
}
