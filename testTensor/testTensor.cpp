// testTensor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <windows.h>
#include <limits>
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
	d_input = Tensor<double>(dim_input_x, dim_input_y, dim_input_z);
	d_conv = Tensor<double>(dim_conv_x, dim_conv_y, dim_conv_z);
	d_input.clear();
	d_conv.clear();
	double *data_input, *data_conv, *data_d_output, *data_d_input, *data_d_conv;
	data_input = input.getData();
	data_conv = conv.getData();
	data_d_output = d_output.getData();
	data_d_input = d_input.getData();
	data_d_conv = d_conv.getData();
	for (size_t i = 0; i < dim_output_z; i++) {
		size_t index_input_z, index_conv_z;
		index_input_z = i / dim_conv_z;
		index_conv_z = i % dim_conv_z;
		for (size_t j = 0; j < dim_output_y; j++) {
			size_t index_d_output = d_output.getIndex(0, j, i);
			for (size_t k = 0; k < dim_output_x; k++) {
				double temp_value = data_d_output[index_d_output];
				index_d_output++;
				for (size_t h = 0; h < dim_conv_y; h++) {
					size_t index_input_data, index_conv_data;
					index_input_data = input.getIndex(k, h + j, index_input_z);
					index_conv_data = conv.getIndex(0, h, index_conv_z);
					for (size_t g = 0; g < dim_conv_x; g++) {
						data_d_input[index_input_data] += data_conv[index_conv_data] * temp_value;
						data_conv[index_conv_data] += data_d_input[index_input_data] * temp_value;
						index_input_data++;
						index_conv_data++;
					}
				}
			}
		}
	}
}

int PoolMax(Tensor<double> &input, size_t sizeWindow, Tensor<double> &output, Tensor<size_t> &d_input) {
	size_t dim_input_x, dim_input_y, dim_input_z;
	dim_input_x = input.getDimX();
	dim_input_y = input.getDimY();
	dim_input_z = input.getDimZ();
	if ((dim_input_x % sizeWindow == 0) && (dim_input_y % sizeWindow == 0)) {
		return -1;
	}
	size_t dim_output_x, dim_output_y, dim_output_z;
	dim_output_x = dim_input_x / sizeWindow;
	dim_output_y = dim_input_y / sizeWindow;
	dim_output_z = dim_input_z;
	output = Tensor<double>(dim_output_x, dim_output_y, dim_output_z);
	d_input = Tensor<size_t>(dim_output_x, dim_output_y, dim_output_z);
	size_t index_output = 0;
	size_t index_input;
	double *data_input, *data_output;
	size_t *data_d_input;
	data_input = input.getData();
	data_output = output.getData();
	data_d_input = d_input.getData();
	for (size_t i = 0; i < dim_output_z; i++) {
		for (size_t j = 0; j < dim_output_y; j++) {
			for (size_t k = 0; k < dim_output_x; k++) {
				double value_max = numeric_limits<double>::lowest();
				size_t index_max;
				for (size_t h = 0; h < sizeWindow; h++) {
					index_input = input.getIndex(k*sizeWindow, j*sizeWindow + h, i);
					for (size_t g = 0; g < sizeWindow; g++) {
						double temp = data_input[index_input];
						if (temp > value_max) {
							value_max = temp;
							index_max = index_input;
						}
						index_input++;
					}
				}
				data_output[index_output] = value_max;
				data_d_input[index_output] = index_max;
				index_output++;
			}
		}
	}
	return 0;
}


int main()
{
	while (1) {
		Sleep(1000);
	}
    return 0;
}

