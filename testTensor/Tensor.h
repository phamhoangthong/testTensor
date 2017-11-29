#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <windows.h>

#define		TYPE_SUM		1
#define		TYPE_SIGMOD		2
#define		TYPE_TANH		3
#define		TYPE_CONV		4
#define		TYPE_RELU		5
#define		TYPE_POOLMAX	6
#define		TYPE_FC			7
template<typename T> class Tensor
{
private:
	T* m_data = nullptr;
	size_t dim_x, dim_y, dim_z;
	size_t step_y, step_z;
	size_t size;
	int type;
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
	
	~Tensor() {
		if (m_data != nullptr) {
			delete[] m_data;
			m_data = nullptr;
			size = 0;
		}
	}

	Tensor<T> operator=(Tensor<T> &obj) {
		Tensor<T> m_return;
		m_return.setSize(obj.getDimX(), obj.getDimY(), obj.getDimZ());
		memcpy(m_return.getData(), obj.getData(), obj.getTotalSize() * sizeof(T));
		return m_return;
	}

	void setSize(size_t dim_x, size_t dim_y, size_t dim_z) {
		resize(dim_x, dim_y, dim_z);
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

	size_t getStepY() {
		return step_y;
	}

	size_t getStepZ() {
		return step_z;
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

	int operator+= (Tensor<T> &obj) {
		if ((dim_x != obj.getDimX()) || (dim_y != obj.getDimY()) || (dim_z != obj.getDimZ())) {
			return -1;
		}
		T* p_data_obj = obj.getData();
		for (size_t i = 0; i < size; i++) {
			m_data[i] += p_data_obj[i];
		}
		return 0;
	}

	int operator-= (Tensor<T> &obj) {
		if ((dim_x != obj.getDimX()) || (dim_y != obj.getDimY()) || (dim_z != obj.getDimZ())) {
			return -1;
		}
		T* p_data_obj = obj.getData();
		for (size_t i = 0; i < size; i++) {
			m_data[i] -= p_data_obj[i];
		}
		return 0;
	}

	int operator*= (T &obj) {
		Tensor<T> mReturn = Tensor<T>(dim_x, dim_y, dim_z);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < size; i++) {
			m_data[i] *= obj;
		}
		return 0;
	}

	int operator/= (T &obj) {
		Tensor<T> mReturn = Tensor<T>(dim_x, dim_y, dim_z);
		T* p_data_return = mReturn.getData();
		for (size_t i = 0; i < size; i++) {
			m_data[i] /= obj;
		}
		return 0;
	}
	// new
	Tensor<T> conv(Tensor<T> &obj) {
		size_t dim_conv_x, dim_conv_y, dim_conv_z, dim_output_x, dim_output_y, dim_output_z;
		dim_conv_x = obj.getDimX();
		dim_conv_y = obj.getDimY();
		dim_conv_z = obj.getDimZ();
		if (dim_conv_z > dim_z) {
			return Tensor<T>();
		}
		dim_output_x = dim_x - dim_conv_x + 1;
		dim_output_y = dim_y - dim_conv_y + 1;
		dim_output_z = dim_z - dim_conv_z + 1;
		Tensor<T> output(dim_output_x, dim_output_y, dim_output_z);
		T *p_data_obj = obj.getData();
		T *p_data_output = output.getData();
		size_t index_output = 0;
		for (size_t index_output_z = 0; index_output_z < dim_output_z; index_output_z++) {
			for (size_t index_output_y = 0; index_output_y < dim_output_y; index_output_y++) {
				for (size_t index_output_x = 0; index_output_x < dim_output_x; index_output_x++) {
					T tValue = 0;
					size_t index_conv = 0;
					size_t index;
					for (size_t index_obj_z = 0; index_obj_z < dim_conv_z; index_obj_z++) {
						for (size_t index_obj_y = 0; index_obj_y < dim_conv_y; index_obj_y++) {
							index = index_output_x + (index_output_y + index_obj_y)*step_y + (index_output_z + index_obj_z)*step_z;
							for (size_t index_obj_x = 0; index_obj_x < dim_conv_x; index_obj_x++) {
								tValue += p_data_output[index]*p_data_obj[index_conv];
								index_conv++;
								index++;
							}
						}
					}
					p_data_obj[index_output] = tValue;
					index_output++;
				}
			}
		}
		return output;
	}
	// old
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
		mask_input = Tensor<bool>(dim_x, dim_y, dim_z);
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
							double temp = m_data[index];
							if (temp > value_max) {
								value_max = temp;
								index_max = index;
							}
							index++;
						}
					}
					data_output[index_output] = value_max;
					data_d_pos[index_output] = index_max;
					index_output++;
				}
			}
		}
		return output;
	}

	Tensor<T> fc(Tensor<T> &obj, int type) {
		if ((dim_x != 1) || (dim_y != 1)) {
			return Tensor<T>();
		}
		size_t dim_obj_x, dim_obj_y, dim_obj_z;
		obj.getSize(dim_obj_x, dim_obj_y, dim_obj_z);
		if ((dim_obj_y != 1) || (dim_obj_x != dim_z + 1)) {
			return Tensor<T>();
		}
		Tensor<T> mReturn(1, 1, dim_obj_z);
		size_t index_obj = 0;
		T* p_data_obj = obj.getData();
		T* p_data_return = mReturn.getData();
		for (size_t index_return = 0; index_return < dim_obj_z; index_return++) {
			T t_value = 0;
			for (size_t index = 0; index < dim_z; index++) {
				t_value += m_data[index] * p_data_obj[index_obj];
				index_obj++;
			}
			t_value += p_data_obj[index_obj];
			index_obj++;
			if (type == TYPE_SUM) {
				p_data_return[index_return] = t_value;
			}
			else if (type == TYPE_SIGMOD) {
				p_data_return[index_return] = (T)(1 / (1 + exp((double)(-t_value))));
			}
			else if (type == TYPE_TANH) {
				T temp_1 = (T)(exp((double)(t_value)));
				T temp_2 = (T)(exp((double)(-t_value)));
				p_data_return[index_return] = (temp_1 - temp_2) / (temp_1 + temp_2);
			}
		}
		return mReturn;
	}

	int updateRelu(Tensor<T> &obj, Tensor<bool> &mask) {
		size_t dim_obj_x, dim_obj_y, dim_obj_z;
		obj.getSize(dim_obj_x, dim_obj_y, dim_obj_z);
		size_t dim_mask_x, dim_mask_y, dim_mask_z;
		mask.getSize(dim_mask_x, dim_mask_y, dim_mask_z);
		if ((dim_obj_x != dim_mask_x) || (dim_obj_y != dim_mask_y) || (dim_obj_z != dim_mask_z)) {
			return -1;
		}
		if ((dim_obj_x != dim_x) || (dim_obj_y != dim_y) || (dim_obj_z != dim_z)) {
			return -1;
		}
		size_t limit = size;
		T *p_data_obj = obj.getData();
		bool *p_mask_obj = mask.getData();
		for (size_t i = 0; i < limit; i++) {
			if (p_mask_obj[i]) {
				m_data[i] = p_data_obj[i];//update rate learning
			}
			else {
				m_data[i] = 0;
			}
		}
		return 0;
	}

	int updatePoolMax(Tensor<T> &obj, Tensor<size_t> &pos, size_t sizeWindow) {
		size_t dim_obj_x, dim_obj_y, dim_obj_z;
		obj.getSize(dim_obj_x, dim_obj_y, dim_obj_z);
		size_t dim_pos_x, dim_pos_y, dim_pos_z;
		pos.getSize(dim_pos_x, dim_pos_y, dim_pos_z);
		if ((dim_obj_x != dim_pos_x) || (dim_obj_y != dim_pos_y) || (dim_obj_z != dim_pos_z)) {
			return -1;
		}
		if ((dim_x != dim_obj_x*sizeWindow) || (dim_y != dim_obj_y*sizeWindow) || (dim_z != dim_obj_z*sizeWindow)) {
			return -1;
		}
		size_t limit = obj.getTotalSize();
		T *p_data_obj = obj.getData();
		size_t *p_data_pos = pos.getData();
		clear();
		for (size_t i = 0; i < limit; i++) {
			m_data[p_data_pos[i]] = p_data_obj[i]; //update rate learning
		}
		return 0;
	}

	int updateConv(Tensor<T> &obj, Tensor<T> &parameter, T &rate_learning) {
		size_t dim_obj_x, dim_obj_y, dim_obj_z;
		obj.getSize(dim_obj_x, dim_obj_y, dim_obj_z);
		size_t dim_parameter_x, dim_parameter_y, dim_parameter_z;
		parameter.getSize(dim_parameter_x, dim_parameter_y, dim_parameter_z);
		if (((dim_x - dim_parameter_x + 1) != dim_obj_x) || ((dim_y - dim_parameter_y + 1) != dim_obj_y) || ((dim_z - dim_parameter_z + 1) != dim_obj_z)) {
			return -1;
		}
		Tensor<T> d_parameter(dim_parameter_x, dim_parameter_y, dim_parameter_z);
		Tensor<T> d(dim_x, dim_y, dim_z);
		size_t step_obj_y = obj.getStepY();
		size_t step_obj_z = obj.getStepZ();
		size_t step_parameter_y = parameter.getStepY();
		size_t step_parameter_z = parameter.getStepZ();
		T *p_data_obj = obj.getData();
		T *p_data_parameter = parameter.getData();
		T *p_data_d_parameter = d_parameter.getData();
		T *p_data_d = d.getData();
		size_t index_obj = 0;
		size_t index = 0;
		size_t index_parameter = 0;
		d.clear();
		d_parameter.clear();
		for (size_t index_obj_z = 0; index_obj_z < dim_obj_z; index_obj_z++) {
			for (size_t index_obj_y = 0; index_obj_y < dim_obj_y; index_obj_y++) {
				for (size_t index_obj_x = 0; index_obj_x < dim_obj_y; index_obj_x++) {
					index_parameter = 0;
					for (size_t index_parameter_z = 0; index_parameter_z < dim_parameter_z; index_parameter_z++) {
						for (size_t index_parameter_y = 0; index_parameter_y < dim_parameter_y; index_parameter_y++) {
							index = index_obj_x + (index_obj_y + index_parameter_y)*step_y + (index_obj_z + index_parameter_z)*step_z;
							for (size_t index_parameter_x = 0; index_parameter_x < dim_parameter_x; index_parameter_x++) {
								p_data_d[index] += p_data_parameter[index_parameter] * p_data_obj[index_obj];
								p_data_d_parameter[index_parameter] += m_data[index] * p_data_obj[index_obj]; // update rate learning
								index_parameter++;
								index++;
							}
						}
					}
					index_obj++;
				}
			}
		}
		*this = d;
		parameter += d_parameter*rate_learning;
		return 0;
	}

	int updateFc(Tensor<T> &obj, Tensor<T> &parameter, int type) {
		if ((dim_x != 1) || (dim_y != 1)) {
			return -1;
		}
		size_t dim_obj_x, dim_obj_y, dim_obj_z;
		obj.getSize(dim_obj_x, dim_obj_y, dim_obj_z);
		if ((dim_obj_x != 1) || (dim_obj_y != 1)) {
			return -1;
		}
		size_t dim_parameter_x, dim_parameter_y, dim_parameter_z;
		parameter.getSize(dim_parameter_x, dim_parameter_y, dim_parameter_z);
		if (dim_parameter_y != 1) {
			return -1;
		}
		if (dim_obj_z != dim_parameter_z) {
			return -1;
		}
		if (dim_parameter_x != dim_z + 1) {
			return -1;
		}
		Tensor<T> d(dim_x, dim_y, dim_z);
		Tensor<T> d_parameter(dim_parameter_x, dim_parameter_y, dim_parameter_z);
		d.clear();
		d_parameter.clear();
		T *p_data_parameter = parameter.getData();
		T *p_data_obj = obj.getData();
		T *p_d = d.getData();
		T *p_d_parameter = d_parameter.getData();
		size_t index_parameter = 0;
		for (size_t index_parameter_z = 0; index_parameter_z < dim_parameter_z; index_parameter_z++) {
			T t_value_obj = p_data_obj[index_parameter_z];
			if (type == TYPE_SUM) {
			}
			else if (type == TYPE_SIGMOD) {
				t_value_obj = t_value_obj*(1 - t_value_obj);
			}
			else if (type == TYPE_TANH) {
				t_value_obj = 1 - t_value_obj*t_value_obj;
			}

			for (size_t index_parameter_x = 0; index_parameter_x < dim_parameter_x - 1; index_parameter_x++) {
				p_d[index_parameter_x] += t_value_obj*p_data_obj[index_parameter];
				p_d_parameter[index_parameter] += t_value_obj*m_data[index_parameter_x];
				index_parameter++;
			}
			p_d_parameter[index_parameter] += t_value_obj;
			index_parameter++;
		}
		*this = d;
		parameter += d_parameter;
		return 0;
	}
};