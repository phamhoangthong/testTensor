#pragma once

#include <iostream>
#include <cstdlib> 
#include "Tensor.h"

template<typename T> class ConvLayer {
private:
	Tensor<T>* p_parameter = nullptr;
	Tensor<T> m_conv;
	Tensor<T> m_relu;
	Tensor<bool> m_mask_relu;
	Tensor<size_t> m_index_poolmax;
	size_t m_size_parameter;
	size_t dim_window_poolmax;
	T m_rate_learning;
public:
	ConvLayer() {

	}

	~ConvLayer() {
		if (p_parameter != nullptr) {
			delete[] p_parameter;
		}
		p_parameter = nullptr;
	}

	ConvLayer(size_t dim_parameter_x, size_t dim_parameter_y, size_t dim_parameter_z, size_t size_parameter, size_t size_window) {
		init(dim_parameter_x, dim_parameter_y, dim_parameter_z, size_parameter, size_window);
	}

	void init(size_t dim_parameter_x, size_t dim_parameter_y, size_t dim_parameter_z, size_t size_parameter, size_t size_window) {
		if (p_parameter != nullptr) {
			delete[] p_parameter;
		}
		m_size_parameter = size_parameter;
		p_parameter = new Tensor<T>[m_size_parameter];
		for (size_t i = 0; i < m_size_parameter; i++) {
			p_parameter[i] = Tensor<T>(dim_parameter_x, dim_parameter_y, dim_parameter_z);
			size_t m_size = p_parameter[i].getTotalSize();
			T * p_data_temp = p_parameter[i].getData();
			for (size_t j = 0; j < m_size; j++) {
				int temp = rand()% m_size;
				p_data_temp[j] = ((T)temp) / ((T)m_size);
			}
		}
		dim_window_poolmax = size_window;
	}

	void setRateLearing(T rate_learning) {
		m_rate_learning = rate_learning;
	}

	int forward(Tensor<T> &input, Tensor<T> &output) {
		size_t dim_input_x, dim_input_y, dim_input_z;
		input.getSize(dim_input_x, dim_input_y, dim_input_z);
		if (p_parameter == nullptr) {
			return -2;
		}
		size_t dim_parameter_x, dim_parameter_y, dim_parameter_z;
		p_parameter[0].getSize(dim_parameter_x, dim_parameter_y, dim_parameter_z);
		if ((dim_parameter_z > dim_input_z) || (dim_parameter_x > dim_input_x) || (dim_parameter_y > dim_input_y)) {
			return -3;
		}
		// update new ver
		Tensor<T> *p_t_conv = new Tensor<T>[m_size_parameter];
		for (size_t i = 0; i < m_size_parameter; i++) {
			p_t_conv[i] = input.conv(p_parameter[i]);
		}
		Tensor<T> *p_tensor = &m_conv;
		if (collect(p_tensor, p_t_conv, m_size_parameter) != 0) {
			return -4;
		}
		delete[] p_t_conv;
		//
		m_relu = m_conv.relu(m_mask_relu);
		output = m_relu.poolMax(m_index_poolmax, dim_window_poolmax);
		if (output.getDimX() == 0) {
			return -5;
		}
		return 0;
	}

	int backward(Tensor<T> &input, Tensor<T> &output) {
		size_t dim_input_x, dim_input_y, dim_input_z;
		input.getSize(dim_input_x, dim_input_y, dim_input_z);
		if (m_relu.updatePoolMax(input, m_index_poolmax, dim_window_poolmax) != 0) {
			return -2;
		}
		if (m_conv.updateRelu(m_relu, m_mask_relu) != 0) {
			return -3;
		}
		Tensor<T> *p_t_conv;
		if (separate(p_t_conv, &m_conv, m_size_parameter) != 0) {
			return -4;
		}
		size_t dim_output_x, dim_output_y, dim_output_z;
		output.getSize(dim_output_x, dim_output_y, dim_output_z);
		size_t dim_temp_x, dim_temp_y, dim_temp_z;
		p_t_conv[0].getSize(dim_temp_x, dim_temp_y, dim_temp_z);
		if (p_parameter == nullptr) {
			return -5;
		}
		size_t dim_parameter_x, dim_parameter_y, dim_parameter_z;
		p_parameter[0].getSize(dim_parameter_x, dim_parameter_y, dim_parameter_z);
		if ((dim_output_x - dim_parameter_x + 1 != dim_temp_x) || (dim_output_y - dim_parameter_y + 1 != dim_temp_y) || (dim_output_z - dim_parameter_z + 1 != dim_temp_z)) {
			return -6;
		}
		Tensor<T> temp_d_output(dim_output_x, dim_output_y, dim_output_z);
		temp_d_output.clear();
		for (size_t i = 0; i < m_size_parameter; i++) {
			Tensor<T> t_output = output;
			if (t_output.updateConv(p_t_conv[i], p_parameter[i], m_rate_learning) != 0) {
				return -7;
			}
			temp_d_output += t_output;
		}
		output = temp_d_output;
		return 0;
	}
private:
	int collect(Tensor<T> *&output, Tensor<T> *input, size_t size) {
		if (input == nullptr) {
			return -1;
		}
		size_t dim_input_x, dim_input_y, dim_input_z;
		input[0].getSize(dim_input_x, dim_input_y, dim_input_z);
		output = new Tensor<T>(dim_input_x, dim_input_y, dim_input_z*size);
		size_t step_data = input[0].getTotalSize();
		T* p_data_ouput = output->getData();
		for (size_t i = 0; i < size; i++) {
			memcpy(p_data_ouput, input[i].getData(), step_data * sizeof(T));
			p_data_ouput += step_data;
		}
		return 0;
	}

	int separate(Tensor<T> *&output, Tensor<T> *input, size_t size) {
		if (input == nullptr) {
			return -1;
		}
		size_t dim_input_x, dim_input_y, dim_input_z;
		input->getSize(dim_input_x, dim_input_y, dim_input_z);
		output = new Tensor<T>[size];
		size_t dim_output_x, dim_output_y, dim_output_z;
		dim_output_x = dim_input_x;
		dim_output_y = dim_input_y;
		dim_output_z = dim_input_z / size;
		if (dim_input_z%size != 0) {
			return -1;
		}
		size_t step_data = dim_output_x*dim_output_y*dim_output_z;
		T* p_data_input = input->getData();
		for (size_t i = 0; i < size; i++) {
			output[i] = Tensor<T>(dim_output_x, dim_output_y, dim_output_z);
			memcpy(output[i].getData(), p_data_input, step_data * sizeof(T));
			p_data_input += step_data;
		}
		return 0;
	}
};