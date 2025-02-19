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
#include "MySVM.h"
#include "MyFile.h"
#include "Layer.h"
#include "ConvLayer.h"
#include "FCLayer.h"

using namespace std;

#define LINER_REGRESSION		1
#define LOGISTIS_REGRESSION		2
/*
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

int poolMax(Tensor<double> &input, size_t sizeWindow, Tensor<double> &output, Tensor<size_t> &d_input) {
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

int trainingPoolMax(Tensor<double> &d_output, Tensor<size_t> &d_pos_input, Tensor<double> &d_input, size_t dim_x, size_t dim_y, size_t dim_z) {
	size_t dim_d_output_x, dim_d_output_y, dim_d_output_z;
	dim_d_output_x = d_output.getDimX();
	dim_d_output_y = d_output.getDimY();
	dim_d_output_z = d_output.getDimZ();
	if (dim_d_output_z != dim_z) {
		return -1;
	}
	size_t dim_d_pos_input_x, dim_d_pos_input_y, dim_d_pos_input_z;
	dim_d_pos_input_x = d_pos_input.getDimX();
	dim_d_pos_input_y = d_pos_input.getDimY();
	dim_d_pos_input_z = d_pos_input.getDimZ();
	if ((dim_d_output_z != dim_d_pos_input_z) || (dim_d_output_y != dim_d_pos_input_y) || (dim_d_output_x != dim_d_pos_input_x)) {
		return -2;
	}
	d_input = Tensor<double>(dim_x, dim_y, dim_z);
	d_input.clear();
	size_t index_d_output;
	for (size_t i = 0; i < dim_z; i++) {
		for (size_t j = 0; j < dim_d_output_y; j++) {
			index_d_output = d_output.getIndex(0, j, i);
			for (size_t k = 0; k < dim_d_output_x; k++) {
				d_input.setValueData(d_pos_input.getValueData(index_d_output), d_output.getValueData(index_d_output));
				index_d_output++;
			}
		}
	}
	return 0;
}

int relu(Tensor<double> &input, Tensor<double> &output, Tensor<bool> &mask_input) {
	size_t dim_input_x, dim_input_y, dim_input_z;
	dim_input_x = input.getDimX();
	dim_input_y = input.getDimY();
	dim_input_z = input.getDimZ();
	output = Tensor<double>(dim_input_x, dim_input_y, dim_input_z);
	mask_input = Tensor<bool>(dim_input_x, dim_input_y, dim_input_z);
	double *p_data_input, *p_data_output;
	bool *p_data_mask_input;
	p_data_input = input.getData();
	p_data_mask_input = mask_input.getData();
	p_data_output = output.getData();
	size_t limit = input.getTotalSize();
	for (size_t i = 0; i < limit; i++) {
		double temp = p_data_input[i];
		if (temp > 0) {
			p_data_output[i] = temp;
			p_data_mask_input[i] = true;
		}
		else {
			p_data_output[i] = 0.0;
			p_data_mask_input[i] = false;
		}
	}
	return 0;
}

int trainingRelu(Tensor<double> &d_output, Tensor<bool> &mask_input, Tensor<double> &d_input) {
	size_t dim_d_output_x, dim_d_output_y, dim_d_output_z;
	dim_d_output_x = d_output.getDimX();
	dim_d_output_y = d_output.getDimY();
	dim_d_output_z = d_output.getDimZ();
	size_t dim_mask_input_x, dim_mask_input_y, dim_mask_input_z;
	dim_mask_input_x = mask_input.getDimX();
	dim_mask_input_y = mask_input.getDimY();
	dim_mask_input_z = mask_input.getDimZ();
	if ((dim_d_output_x != dim_mask_input_x) || (dim_d_output_y != dim_mask_input_y) || (dim_d_output_z != dim_mask_input_z)) {
		return -1;
	}
	d_input = Tensor<double>(dim_d_output_x, dim_d_output_y, dim_d_output_z);
	double *p_data_d_input, *p_data_d_output;
	bool *p_data_mask_input;
	p_data_d_input = d_input.getData();
	p_data_mask_input = mask_input.getData();
	p_data_d_output = d_output.getData();
	size_t limit = d_output.getTotalSize();
	for (size_t i = 0; i < limit; i++) {
		bool temp = p_data_d_output[i];
		if (temp) {
			p_data_d_input[i] = p_data_d_output[i];
		}
		else {
			p_data_d_input[i] = 0.0;
		}
	}
	return 0;
}

int tranformTensorToSVM(Tensor<double> &input, MySVM<double> &output) {
	size_t limit = input.getTotalSize();
	double *p_data = input.getData();
	output = MySVM<double>(p_data, limit);
	return 0;
}

int tranformSVMToTensor(MySVM<double> &input, Tensor<double> &output, size_t dim_x, size_t dim_y, size_t dim_z) {
	size_t limit = input.getSize();
	if (dim_x*dim_y*dim_z != limit) {
		return -1;
	}
	output = Tensor<double>(input.getData(), dim_x, dim_y, dim_z);
	return 0;
}

int neural(Tensor<double> &input, Tensor<double> &parameter, Tensor<double> &output, int type) {
	size_t dim_input_x, dim_input_y, dim_input_z;
	dim_input_x = input.getDimX();
	dim_input_y = input.getDimY();
	dim_input_z = input.getDimZ();
	return 0;
}
*/
int main()
{
	char* file_data = nullptr;
	MyFile mFile = MyFile();
	size_t size_file;
	char *p_int = nullptr;
	double *p_double = nullptr;
	size_t limit;
	cout << "Load file image training ..." << endl;
	size_file = mFile.read("train-images.idx3-ubyte", file_data);
	cout << "Size file : " << size_file << endl;
	int number_image_training;
	p_int = (char*)(&number_image_training);
	p_int[0] = file_data[7];
	p_int[1] = file_data[6];
	p_int[2] = file_data[5];
	p_int[3] = file_data[4];
	cout << "Size sample : " << number_image_training << endl;
	int size_x, size_y;
	p_int = (char*)(&size_y);
	p_int[0] = file_data[11];
	p_int[1] = file_data[10];
	p_int[2] = file_data[9];
	p_int[3] = file_data[8];
	cout << "Size row : " << size_y << endl;
	p_int = (char*)(&size_x);
	p_int[0] = file_data[15];
	p_int[1] = file_data[14];
	p_int[2] = file_data[13];
	p_int[3] = file_data[12];
	cout << "Size col : " << size_x << endl;
	Tensor<double> store_data_input = Tensor<double>(size_x, size_y, number_image_training);
	p_double = store_data_input.getData();
	limit = store_data_input.getTotalSize();
	cout << "Total size data : " << limit << endl;
	for (size_t i = 0; i < limit; i++) {
		p_double[i] = (double)(file_data[i + 16]);
	}
	delete[] file_data;
	file_data = nullptr;
	cout << "Finish load file label training !" << endl;
	cout << "Load file label training ..." << endl;
	size_file = mFile.read("train-labels.idx1-ubyte", file_data);
	cout << "Size file : " << size_file << endl;
	int number_label_training;
	p_int = (char*)(&number_label_training);
	p_int[0] = file_data[7];
	p_int[1] = file_data[6];
	p_int[2] = file_data[5];
	p_int[3] = file_data[4];
	cout << "Size sample : " << number_label_training << endl;
	Tensor<double> store_data_ref = Tensor<double>(10, 1, number_label_training);
	for (int i = 0; i < number_label_training; i++) {
		store_data_ref.setValueData(file_data[i + 8], 1, i, 1.0);
	}
	limit = store_data_ref.getTotalSize();
	cout << "Total size data : " << limit << endl;
	delete[] file_data;
	file_data = nullptr;
	cout << "Finish load file label training !" << endl;

	ConvLayer<double> m_conv[3];
	m_conv[0] = ConvLayer<double>(3, 3, 1, 4, 2); // z = 3;
	m_conv[1] = ConvLayer<double>(4, 4, 4, 3, 2); // z = 9;
	m_conv[2] = ConvLayer<double>(3, 3, 3, 3, 3); // z = 27;
	FCLayer<double> m_fc[2];
	m_fc[0] = FCLayer<double>(27, 18, TYPE_SIGMOD);
	m_fc[1] = FCLayer<double>(18, 10, TYPE_SIGMOD);
	Tensor<double> m_data[6];

	m_data[0] = Tensor<double>(28, 28, 1);
	m_data[1] = Tensor<double>();
	m_data[2] = Tensor<double>();
	m_data[3] = Tensor<double>();
	m_data[4] = Tensor<double>();
	m_data[5] = Tensor<double>();

	cout << "Data[0] : [" << m_data[0].getDimX() << " x " << m_data[0].getDimY() << " x " << m_data[0].getDimZ() << " ]" << endl;
	m_conv[0].forward(m_data[0], m_data[1]);
	cout << "Data[1] : [" << m_data[1].getDimX() << " x " << m_data[1].getDimY() << " x " << m_data[1].getDimZ() << " ]" << endl;
	m_conv[1].forward(m_data[1], m_data[2]);
	cout << "Data[2] : [" << m_data[2].getDimX() << " x " << m_data[2].getDimY() << " x " << m_data[2].getDimZ() << " ]" << endl;
	m_conv[2].forward(m_data[2], m_data[3]);
	cout << "Data[3] : [" << m_data[3].getDimX() << " x " << m_data[3].getDimY() << " x " << m_data[3].getDimZ() << " ]" << endl;
	m_fc[0].forward(m_data[3], m_data[4]);
	cout << "Data[4] : [" << m_data[4].getDimX() << " x " << m_data[4].getDimY() << " x " << m_data[4].getDimZ() << " ]" << endl;
	m_fc[1].forward(m_data[4], m_data[5]);
	cout << "Data[5] : [" << m_data[5].getDimX() << " x " << m_data[5].getDimY() << " x " << m_data[5].getDimZ() << " ]" << endl;

	if (m_fc[1].backward(m_data[5], m_data[4]) != 0) {
		cout << "Error backward to 4" << endl;
	}
	if (m_fc[0].backward(m_data[4], m_data[3]) != 0) {
		cout << "Error backward to 3" << endl;
	}
	if (m_conv[2].backward(m_data[3], m_data[2]) != 0) {
		cout << "Error backward to 2" << endl;
	}
	if (m_conv[1].backward(m_data[2], m_data[1]) != 0) {
		cout << "Error backward to 1" << endl;
	}
	if (m_conv[0].backward(m_data[1], m_data[0]) != 0) {
		cout << "Error backward to 0" << endl;
	}
	
	while (1) {
		Sleep(1000);
	}
    return 0;
}

