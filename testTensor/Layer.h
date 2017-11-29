#pragma once

#include <iostream>
#include "Tensor.h"

using namespace std;

template<typename T> class Layer
{
private:
	int m_type;
	Tensor<T> m_layer;
	size_t m_size_store;
	Tensor<T>* p_parameter = nullptr;
	Tensor<bool> m_mask_relu;
	Tensor<size_t> m_index_poolmax;
	size_t m_size_parameter;
public:
	Layer() {

	}

	~Layer() {

	}

	Layer(int type) {
		m_type = type;
	}

	void setType(int type) {
		m_type = type;
	}

	int setLayer(size_t number, size_t dim_x, size_t dim_y, size_t dim_z) {
		if (m_type == TYPE_CONV) {
			m_layer = Tensor<T>(dim_x, dim_y, dim_z*number);
			m_size_store = number;
			return 0;
		}
		else if ((m_type == TYPE_SUM) || (m_type == TYPE_SIGMOD) || (m_type == TYPE_TANH)) {
			if ((dim_x != 1) || (dim_y != 1) || (number != 1)) {
				return -1;
			}
			m_layer = Tensor<T>(dim_x, dim_y, dim_z);
			m_size_store = 1;
			return 0;
		}
		else {
			return -1;
		}
	}

	void setParameter(size_t number, size_t dim_x, size_t dim_y, size_t dim_z) {
		if (m_type == TYPE_CONV) {
			if (p_parameter != nullptr) {
				delete[] p_parameter;
			}
			m_size_store = number;
			p_parameter = new Tensor<T>[m_size_store];
			for (size_t i = 0; i < m_size_store; i++) {
				p_parameter[i] = Tensor<T>(dim_x, dim_y, dim_z);
			}
			return 0;
		}
		else if (m_type == (m_type == TYPE_SUM) || (m_type == TYPE_SIGMOD) || (m_type == TYPE_TANH)) {
			if ((dim_y != 1) || (number != 1) || (dim_x != m_layer.getDimZ() + 1)) {
				return -1;
			}
			if (p_parameter != nullptr) {
				delete[] p_parameter;
			}
			
			m_size_store = 1;
			p_parameter = new Tensor<T>(dim_x, dim_y, dim_z);
			return 0;
		}
		else {
			return -1;
		}
	}

	int forward(Tensor<T> *output) {
		if (output == nullptr) {
			return -1;
		}
		size_t dim_x, dim_y, dim_z;
		m_layer.getSize(dim_x, dim_y, dim_z);
		size_t dim_output_x, dim_output_y, dim_output_z;
		output->getSize(dim_output_x, dim_output_y, dim_output_z);

		if (m_type == TYPE_CONV) {
			if (p_parameter == nullptr) {
				return -1;
			}
			size_t dim_parameter_x, dim_parameter_y, dim_parameter_z;
			p_parameter[0].getSize(dim_parameter_x, dim_parameter_y, dim_parameter_z);
			//
			size_t dim_t_output_x, dim_t_output_y, dim_t_output_y;
			dim_t_output_x = dim_x - dim_parameter_x + 1;
			dim_t_output_y = dim_y - dim_parameter_y + 1;
			dim_t_output_z = dim_y - dim_parameter_z + 1;
			if ((dim_t_output_x != dim_output_x) || (dim_t_output_y != dim_output_y) || (dim_t_output_z*m_size_parameter != dim_output_z)) {
				return -1;
			}
			//
			Tensor<T> t_output[m_size_parameter];
			for (size_t i = 0; i < m_size_parameter; i++) {
				t_output[i] = m_layer.conv(p_parameter[i]);
			}
			return collect(output, t_output, m_size_parameter);
		}
		else if ((m_type == TYPE_SUM) || (m_type == TYPE_SIGMOD) || (m_type == TYPE_TANH)) {
			if (p_parameter == nullptr) {
				return -1;
			}
			size_t dim_parameter_x, dim_parameter_y, dim_parameter_z;
			p_parameter[0].getSize(dim_parameter_x, dim_parameter_y, dim_parameter_z);

			if ((dim_output_z != dim_parameter_z) || (dim_output_x != 1) || (dim_ouput_y != 1)) {
				return -1;
			}
			*output = m_layer.fc(*p_parameter, m_type);
			return 0;
		}
		else if (m_type == TYPE_RELU) {
			if ((dim_output_z != dim_z) || (dim_output_y != dim_y) || (dim_output_x != dim_x)) {
				return -1;
			}
			*output = m_layer.relu(m_mask_relu);
			return 0;
		}
		return 0;
	}

private:
	int collect(Tensor<T> *output, Tensor<T> *input, size_t size) {
		if (input == nullptr) {
			return -1;
		}
		size_t dim_input_x, dim_input_y, dim_input_z;
		input[0].getSize(dim_input_x, dim_input_y, dim_input_z);
		if (output != nullptr) {
			size_t dim_output_x, dim_output_y, dim_output_z;
			output->getSize(dim_output_x, dim_output_y, dim_output_z);
			if ((dim_output_x != dim_input_x) || (dim_output_y != dim_input_y) || (dim_output_z != size*dim_input_z)) {
				return -1;
			}
			size_t step_data = input[0].getTotalSize();
			T* p_data_ouput = output->getData();
			for (size_t i = 0; i < size; i++) {
				memcpy(p_data_ouput, input[i].getData(), step_data * sizeof(T));
				p_data_ouput += step_data;
			}
			return 0;
		}
		else {
			output = new Tensor<T>(dim_input_x, dim_input_y, dim_input_z*size);
			size_t step_data = input[0].getTotalSize();
			T* p_data_ouput = output->getData();
			for (size_t i = 0; i < size; i++) {
				memcpy(p_data_ouput, input[i].getData(), step_data * sizeof(T));
				p_data_ouput += step_data;
			}
			return 0;
		}
	}

	int separate(Tensor<T> *output, Tensor<T> *input, size_t size) {
		if (input == nullptr) {
			return -1;
		}
		size_t dim_input_x, dim_input_y, dim_input_z;
		input->getSize(dim_input_x, dim_input_y, dim_input_z);
		if (output != nullptr) {
			size_t dim_output_x, dim_output_y, dim_output_z;
			output[0].getSize(dim_output_x, dim_output_y, dim_output_z);
			if ((dim_output_x != dim_input_x) || (dim_output_y != dim_input_y) || (dim_input_z != size*dim_output_z)) {
				return -1;
			}
			size_t step_data = output[0].getTotalSize();
			T* p_data_input = input->getData();
			for (size_t i = 0; i < size; i++) {
				memcpy(output[i].getData(), p_data_input, step_data * sizeof(T));
				p_data_input += step_data;
			}
			return 0;
		}
		else {
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
				output[i] = new Tensor<T>(dim_output_x, dim_output_y, dim_output_z);
				memcpy(output[i].getData(), p_data_input, step_data * sizeof(T));
				p_data_input += step_data;
			}
			return 0;
		}
	}
};

