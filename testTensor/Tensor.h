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
	Tensor() {
		this->dim_x = 0;
		this->dim_y = 0;
		this->dim_z = 0;
		m_data = nullptr;
		size = 0;
	}

	Tensor(size_t dim_x, size_t dim_y, size_t dim_z) {
		this->dim_x = dim_x;
		this->dim_y = dim_y;
		this->dim_z = dim_z;
		m_data = new T[this->dim_x*this->dim_y*this->dim_z];
		step_y = dim_x;
		step_z = dim_x * dim_y;
		size = dim_x * dim_y * dim_z;
	}

	Tensor(T* data, size_t dim_x, size_t dim_y, size_t dim_z) {
		this->dim_x = dim_x;
		this->dim_y = dim_y;
		this->dim_z = dim_z;
		m_data = new T[this->dim_x*this->dim_y*this->dim_z];
		step_y = dim_x;
		step_z = dim_x * dim_y;
		size = dim_x * dim_y * dim_z;
		memcpy(m_data, data, size * sizeof(T));
	}
	
	virtual ~Tensor() {
		if (m_data != nullptr) {
			delete[] m_data;
			m_data = nullptr;
			size = 0;
		}
	}

	Tensor<T> operator=(Tensor<T> &obj) {
		Tensor<T> m_return(obj.getDimX(), obj.getDimY(), obj.getDimZ());
		memcpy(m_return.getData(), obj.getData(), obj.getTotalSize() * sizeof(T));
		return m_return;
	}

	void setSize(size_t dim_x, size_t dim_y, size_t dim_z) {
		resize(dim_x, dim_y, dim_z)
	}

	void resize(size_t dim_x, size_t dim_y, size_t dim_z) {
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

	size_t getDimX() {
		return dim_x;
	}

	size_t getDimY() {
		return dim_y;
	}

	size_t getDimZ() {
		return dim_z;
	}

	size_t getTotalSize() {
		return size;
	}

	void getSize(size_t &dim_x, size_t &dim_y, size_t &dim_z) {
		dim_x = this->dim_x;
		dim_y = this->dim_y;
		dim_z = this->dim_z;
	}

	size_t getIndex(size_t x, size_t y, size_t z) {
		return z*step_z + y*step_y + x;
	}
	
	T* getData() {
		return m_data;
	}

	T getValueData(size_t x, size_t y, size_t z) {
		if ((x < dim_x) && (y < dim_y) && (z < dim_z)) {
			size_t index = getIndex(x, y, z);
			return m_data[index];
		}
		else {
			return 0;
		}
	}

	T getValueData(size_t index) {
		if (index < size) {
			return m_data[index];
		}
		else {
			return 0;
		}
	}

	T* getDataXY(size_t z) {
		if (z < dim_z) {
			return (m_data + z*step_z);
		}
		else {
			return nullptr;
		}
	}

	void setValueData(size_t x, size_t y, size_t z, T data) {
		if ((x < dim_x) && (y < dim_y) && (z < dim_z)) {
			size_t index = getIndex(x, y, z);
			m_data[index] = data;
		}
	}

	void setValueData(size_t index, T data) {
		if (index < size) {
			m_data[index] = data;
		}
	}

	void setData(T* data) {
		memcpy(m_data, data, size * sizeof(T));
	}

	void setDataXY(size_t z, T* data) {
		if (z < dim_z) {
			size_t index = z*step_z;
			memcpy(m_data + index, data, step_z * sizeof(T));
		}
	}

	void clear() {
		size_t index = 0;
		memset(m_data, 0, size * sizeof(T));
	}

	Tensor<T> operator+ (Tensor<T> &obj) {
		if ((dim_x != obj.getDimX()) || (dim_y != obj.getDimY()) || (dim_z != obj.getDimZ())) {
			return Tensor<T>();
		}
		Tensor<T> mReturn = Tensor<T>(dim_x, dim_y, dim_z);
		T* p_data_return = mReturn.getData();
		T* p_data_obj = obj.getData();
		for (size_t i = 0; i < size; i++) {
			p_data_return[i] = m_data[i] + p_data_obj[i];
		}
		return mReturn;
	}

	Tensor<T> operator- (Tensor<T> &obj) {
		if ((dim_x != obj.getDimX()) || (dim_y != obj.getDimY()) || (dim_z != obj.getDimZ())) {
			return Tensor<T>();
		}
		Tensor<T> mReturn = Tensor<T>(dim_x, dim_y, dim_z);
		T* p_data_return = mReturn.getData();
		T* p_data_obj = obj.getData();
		for (size_t i = 0; i < size; i++) {
			p_data_return[i] = m_data[i] - p_data_obj[i];
		}
		return mReturn;
	}

	Tensor<T> operator* (T &obj) {
		Tensor<T> mReturn = Tensor<T>(dim_x, dim_y, dim_z);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < size; i++) {
			p_data_return[i] = obj*m_data[i];
		}
		return mReturn;
	}

	Tensor<T> operator/ (T &obj) {
		Tensor<T> mReturn = Tensor<T>(dim_x, dim_y, dim_z);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < size; i++) {
			p_data_return[i] = m_data[i]/obj;
		}
		return mReturn;
	}

	Tensor<T> convolution(Tensor<T> &obj) {
		size_t dim_conv_x, dim_conv_y, dim_conv_z, dim_output_x, dim_output_y, dim_output_z;
		dim_conv_x = obj.getDimX();
		dim_conv_y = obj.getDimY();
		dim_conv_z = obj.getDimZ();
		dim_output_x = dim_x - dim_conv_x + 1;
		dim_output_y = dim_y - dim_conv_y + 1;
		dim_output_z = dim_z * dim_conv_z;
		size_t step_conv_y, step_conv_z, step_output_y, step_output_z;
		step_conv_y = dim_conv_x;
		step_conv_z = dim_conv_x*dim_conv_y;
		step_output_y = dim_output_x;
		step_output_z = dim_output_x*dim_output_y;
		Tensor<T> mReturn = Tensor<T>(dim_output_x, dim_output_y, dim_output_z);
		T* p_data_return = mReturn.getData();
		T* p_data_conv = obj.getData();
		size_t index_output = 0;
		for (size_t i = 0; i < dim_z; i++) {
			for (size_t j = 0; j < dim_conv_z; j++) {
				//size_t index_output = (i*dim_conv_z + j)*step_output_z;
				for (size_t k = 0; k < dim_output_y; k++) {
					for (size_t h = 0; h < dim_output_x; h++) {
						size_t index = 0;
						size_t index_conv = j*step_conv_z;
						T tValue = 0;
						for (size_t g = 0; g < dim_conv_y; g++) {
							index = i*step_z + (g + k)*step_y;
							for (size_t l = 0; l < dim_conv_x; l++) {
								tValue += m_data[index] * p_data_conv[index_conv];
								index_conv++;
								index++;
							}
						}
						p_data_return[index] = tValue;
						index_output++;
					}
				}
			}
		}
		return data_return;
	}

	Tensor<T> relu(Tensor<bool> &mask_input) {
		Tensor<T> output = Tensor<T>(dim_x, dim_y, dim_z);
		mask_input = Tensor<bool>(dim_input_x, dim_input_y, dim_input_z);
		double *p_data_output;
		bool *p_data_mask_input;
		p_data_mask_input = mask_input.getData();
		p_data_output = output.getData();
		size_t limit = size;
		for (size_t i = 0; i < limit; i++) {
			double temp = m_data[i];
			if (temp > 0) {
				p_data_output[i] = temp;
				p_data_mask_input[i] = true;
			}
			else {
				p_data_output[i] = 0;
				p_data_mask_input[i] = false;
			}
		}
		return output;
	}

	Tensor<T> poolMax(Tensor<size_t> &pos, size_t sizeWindow) {
		if ((dim_x % sizeWindow == 0) && (dim_y % sizeWindow == 0)) {
			return Tensor<T>();
		}

		size_t dim_output_x, dim_output_y, dim_output_z;
		dim_output_x = dim_x / sizeWindow;
		dim_output_y = dim_y / sizeWindow;
		dim_output_z = dim_z;
		Tensor<T> output = Tensor<double>(dim_output_x, dim_output_y, dim_output_z);
		pos = Tensor<size_t>(dim_output_x, dim_output_y, dim_output_z);
		size_t index_output = 0;
		size_t index_pos = 0;
		size_t index = 0;
		double *data_output;
		size_t *data_d_pos;
		data_output = output.getData();
		data_d_pos = pos.getData();
		for (size_t i = 0; i < dim_output_z; i++) {
			for (size_t j = 0; j < dim_output_y; j++) {
				for (size_t k = 0; k < dim_output_x; k++) {
					double value_max = numeric_limits<double>::lowest();
					size_t index_max;
					for (size_t h = 0; h < sizeWindow; h++) {
						index = k*sizeWindow + (j*sizeWindow + h)*step_y + i*step_z;
						for (size_t g = 0; g < sizeWindow; g++) {
							double temp = data_input[index_input];
							if (temp > value_max) {
								value_max = temp;
								index_max = index_input;
							}
							index++;
						}
					}
					data_output[index_output] = value_max;
					data_d_input[index_output] = index_max;
					index_output++;
				}
			}
		}
		return output;
	}

};