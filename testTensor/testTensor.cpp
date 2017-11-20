// testTensor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <windows.h>
#include "Tensor.h"

using namespace std;

void matrixConvolution(double *input, double *conv, double *output, size_t dim_x, size_t dim_y, size_t dim_conv_x, size_t dim_conv_y, size_t dim_output_x, size_t dim_output_y) {
	for (int i = 0; i < dim_output_y; i++) {
		size_t index_output = i*dim_output_x;
		for (int j = 0; j < dim_output_x; j++) {
			size_t index_input = 0;
			size_t index_conv = 0;
			double t_value = 0.0;
			for (int k = 0; k < dim_conv_y; k++) {
				index_conv = k*dim_conv_x;
				index_input = (i + k) *dim_x + j;
				for (int h = 0; h < dim_conv_x; h++) {
					t_value += input[index_input] * conv[index_conv];
					index_input++;
					index_conv++;
				}
			}
			output[index_output] = t_value;
			index_output++;
		}
	}
}

Tensor<double> convolution(Tensor<double> &a, Tensor<double> &b) {
	size_t dim_input_x, dim_input_y, dim_input_z, dim_conv_x, dim_conv_y, dim_conv_z, dim_output_x, dim_output_y, dim_output_z;
	dim_input_x = a.getDimX();
	dim_input_y = a.getDimY();
	dim_input_z = a.getDimZ();
	dim_conv_x = b.getDimX();
	dim_conv_y = b.getDimY();
	dim_conv_z = b.getDimZ();
	dim_output_x = dim_input_x - dim_conv_x + 1;
	dim_output_y = dim_input_y - dim_conv_y + 1;
	dim_output_z = dim_input_z * dim_conv_z;

	Tensor<double> data_return = Tensor<double>(dim_output_x, dim_output_y, dim_output_z);
	for (int i = 0; i < dim_input_z; i++) {
		for (int j = 0; j < dim_conv_z; j++) {
			matrixConvolution(a.getDataXY(i), b.getDataXY(j), data_return.getDataXY(i*dim_conv_z + j), dim_input_x, dim_input_y, dim_conv_x, dim_conv_y, dim_output_x, dim_output_y);
		}
	}
	return data_return;
}

void convolutionForTraining(Tensor<double> &input, Tensor<double> &output, Tensor<double> &conv, Tensor<double> &d_conv, Tensor<double> &d_input) {
	size_t dim_input_x, dim_input_y, dim_input_z, dim_conv_x, dim_conv_y, dim_conv_z, dim_output_x, dim_output_y, dim_output_z;
	dim_input_x = input.getDimX();
	dim_input_y = input.getDimY();
	dim_input_z = input.getDimZ();
	dim_conv_x = conv.getDimX();
	dim_conv_y = conv.getDimY();
	dim_conv_z = conv.getDimZ();
	dim_output_x = dim_input_x - dim_conv_x + 1;
	dim_output_y = dim_input_y - dim_conv_y + 1;
	dim_output_z = dim_input_z * dim_conv_z;

	output = Tensor<double>(dim_output_x, dim_output_y, dim_output_z);
	d_conv = Tensor<double>(dim_conv_x, dim_conv_y, dim_conv_z);
	d_input = Tensor<double>(dim_input_x, dim_input_y, dim_input_z);
}

void trainingConvolution(Tensor<double> &input, Tensor<double> &conv, Tensor<double> &d_output, Tensor<double> &d_input, Tensor<double> &d_conv) {
	size_t dim_input_x, dim_input_y, dim_input_z;
	size_t dim_conv_x, dim_conv_y, dim_conv_z;
	size_t dim_output_x, dim_output_y, dim_output_z;
	dim_input_x = input.getDimX();
	dim_input_y = input.getDimY();
	dim_input_z = input.getDimZ();
	dim_conv_x = conv.getDimX();
	dim_conv_y = conv.getDimY();
	dim_conv_z = conv.getDimZ();
	dim_output_x = dim_input_x - dim_conv_x + 1;
	dim_output_y = dim_input_y - dim_conv_y + 1;
	dim_output_z = dim_input_z * dim_conv_z;
	if ((dim_output_x != d_output.getDimX()) || (dim_output_y != d_output.getDimY()) || (dim_output_z != d_output.getDimZ())) {
		return;
	}
	for (size_t i = 0; i < dim_output_z; i++) {
		size_t index_input, index_conv;
		index_input = i / dim_conv_z;
		index_conv = i % dim_conv_z;
		for (size_t j = 0; j < dim_output_y; j++) {
			for (size_t k = 0; k < dim_output_x; k++) {
				// <conv>
				for (size_t h = 0; h < dim_input_y; h++) {
					for (size_t g = 0; g < dim_input_x) {

					}
				}
			}
		}
	}

}

int main()
{
	/*Tensor<double> m_tensor = Tensor<double>(2, 2, 2);

	cout << "--- Begin test ---" << endl;
	cout << " - Test write read value" << endl;
	m_tensor.setValueData(1, 1, 1, 0.2);
	m_tensor.setValueData(0, 0, 0, 0.1);
	double temp = m_tensor.getValueData(1, 1, 1);
	if (temp != 0.2) {
		cout << " - Fail test" << endl;
		return 0;
	}
	temp = m_tensor.getValueData(0, 0, 0);
	if (temp != 0.1) {
		cout << " - Fail test" << endl;
		return 0;
	}*/
	while (1) {
		Sleep(1000);
	}
    return 0;
}

