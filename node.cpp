/*
 * File name   : node.cpp
 * Description : contains all the definitions for methods declared in ethanol.h for class Node
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#include "ethanol.h"

Node::Node(unsigned int input_size) {
	bias = 0.0;
	output_signal = 0.0;
	for(unsigned int i = 0; i < input_size; ++i) {
		weight.push_back(0.0);
		input_signal.push_back(0.0);
	}
}

bool Node::set_weight(vector<double> &weight_val) {
	if(weight_val.size() != weight.size())
		return false;
	weight.clear();
	weight = weight_val;
	return true;
}

bool Node::set_bias(double bias_val) {
	bias = bias_val;
	return true;
}

vector<double> Node::get_weight() {
	return weight;
}

double Node::get_bias() {
	return bias;
}

vector<double> Node::get_input_signal() {
	return input_signal;
}

double Node::get_output_signal() {
	return output_signal;
}

bool Node::trigger(vector<double> &input_signal_val) {
	if(input_signal_val.size() == input_signal.size()) {
		input_signal = input_signal_val;
		output_signal = 0.0;
		vector<double>::iterator input_pointer = input_signal.begin();
		vector<double>::iterator weight_pointer = weight.begin();
		// multiplying corresponding weights and signals and summing them
		while(input_pointer != input_signal.end()) {
			output_signal += (*input_pointer) * (*weight_pointer);
			++input_pointer;
			++weight_pointer;
		}
		// output signal is reduced to average to remove the summation effect of many signals
		output_signal /= input_signal.size();
		output_signal -= bias;
		// ReLU
		output_signal = (output_signal + fabs(output_signal)) / 2.0;
		// output signal is divided by 20 to remove the multiplication effect of weights and subtraction effect of bias
		output_signal /= 20.0;
		return true;
	}
	return false;
}
