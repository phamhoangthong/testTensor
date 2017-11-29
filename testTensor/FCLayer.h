#pragma once

#include <iostream>
#include <cstdlib> 
#include "Tensor.h"

template<typename T> class FCLayer {
private:
	Tensor<T> m_parameter;
	T m_rate_learning;
	int m_type;
public:
	FCLayer() {

	}

	~FCLayer() {

	}

	FCLayer(size_t dim_input, size_t dim_out, int type) {
		init(dim_input, dim_out, type);
	}

	void init(size_t dim_input, size_t dim_out, int type) {
		m_parameter = Tensor<T>(dim_input + 1, 1, dim_out);
		m_type = type;
	}

	int forward(Tensor<T> &input, Tensor<T> &output) {
		if ((m_type != TYPE_SUM)|| (m_type != TYPE_SIGMOD) || (m_type != TYPE_TANH)) {
			return -1;
		}
		output = input.fc(m_parameter, m_type);
		if (output.getDimX() == 0) {
			return -1;
		}
		return 0;
	}

	int backward(Tensor<T> &input, Tensor<T> &output) {
		if ((m_type != TYPE_SUM) || (m_type != TYPE_SIGMOD) || (m_type != TYPE_TANH)) {
			return -1;
		}
		if(output.updateFc(input, m_parameter, m_type) !=0 ) {
			return -1;
		}
		return 0;
	}
};