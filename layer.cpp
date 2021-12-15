/*
 * File name   : layer.cpp
 * Description : contains all the definitions for methods declared in ethanol.h for class Layer
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#include "ethanol.h"

Layer::Layer(unsigned int input_size, unsigned int nodes_count) {
	for (unsigned int i = 0; i < nodes_count; ++i) {
		Node node(input_size);
		nodes.push_back(node);
		output_signal.push_back(0.0);
	}
}

bool Layer::set_weight(vector<double>& weight_val, unsigned int node_id) {
	if (node_id >= nodes.size())
		return false;
	return (nodes.begin() + node_id)->set_weight(weight_val);
}

bool Layer::set_bias(double bias_val, unsigned int node_id) {
	if (node_id >= nodes.size())
		return false;
	return (nodes.begin() + node_id)->set_bias(bias_val);
}

vector<double> Layer::get_weight(unsigned int node_id) {
	// if invalid node id, node 0 is selected
	if (node_id >= nodes.size())
		node_id = 0;
	return (nodes.begin() + node_id)->get_weight();
}

double Layer::get_bias(unsigned int node_id) {
	// if invalid node id, node 0 is selected
	if (node_id >= nodes.size())
		node_id = 0;
	return (nodes.begin() + node_id)->get_bias();
}

vector<double> Layer::get_input_signal() {
	return nodes.begin()->get_input_signal();
}

vector<double> Layer::get_output_signal() {
	return output_signal;
}

bool Layer::trigger(vector<double>& input_signal) {
	bool flag;
	for (vector<Node>::iterator node = nodes.begin(); node != nodes.end(); ++node) {
		// pointer is dereferenced instead of using pointer operator
		flag = (*node).trigger(input_signal);
		if (!flag)
			return false;
	}
	// clearing the output signal and repopulating it with new values
	output_signal.clear();
	for (vector<Node>::iterator node = nodes.begin(); node != nodes.end(); ++node) {
		output_signal.push_back(node->get_output_signal());
	}
	return true;
}