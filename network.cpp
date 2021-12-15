/*
 * File name   : network.cpp
 * Description : contains all the definitions for methods declared in ethanol.h for class Network
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#include "ethanol.h"

bool Network::validate_file(vector<string>& file_data) {
	// checking the first line
	if (file_data[0].compare("NETWORK DETAILS") != 0) {
		*report_stream << "Error\nTitle missing";
		return false;
	}
	// checking the second line
	if (file_data[1].size() != 0) {
		*report_stream << "Error\nLine 2 is not empty";
		return false;
	}
	// since / is valid in the following lines, it is made invalid on the third line to prevent the creation of empty input layer
	if (file_data[2].compare("/") == 0) {
		*report_stream << "Error\nLine 3 is invalid";
		return false;
	}
	unsigned int line_number = 2;
	while (line_number != file_data.size() - 1) {
		// empty space is added in the end of the line so that the last value string is no different from other values
		string line(file_data[line_number] + " ");
		// layer creation character /
		if (line.compare("/ ") == 0) {
			++line_number;
			continue;
		}
		unsigned int line_length = line.length();
		// checking whether the line all the values fully and have more than one value
		if (line_length % 11 != 0 || line_length <= 11) {
			*report_stream << "Error\nLine " + to_string(line_number + 1) + " has invalid length";
			return false;
		}
		// verifying bias
		// temporary word_str is created since substr returns a temporary object and word will become a dangling pointer
		string word_str = line.substr(0, 11);
		const char* word = word_str.c_str();
		// verifying the correctness and the position of symbols
		if (((word[0] != '+') && (word[0] != '-')) || (word[3] != '.') || (word[10] != ' ')) {
			*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position 1 symbol error";
			return false;
		}
		// verifying that all other characters are digits for the value
		if (!isdigit(word[1]) || !isdigit(word[2]) || !isdigit(word[4]) || !isdigit(word[5]) || !isdigit(word[6]) || !isdigit(word[7]) || !isdigit(word[8]) || !isdigit(word[9])) {
			*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position 1 digit error";
			return false;
		}
		// verifying weights like done for bias
		for (unsigned int word_cursor = 11; word_cursor < line_length; word_cursor += 11) {
			word_str = line.substr(word_cursor, 11);
			word = word_str.c_str();
			if (((word[0] != '+') && (word[0] != '-')) || (word[2] != '.') || (word[10] != ' ')) {
				*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position " + to_string(word_cursor + 1) + " symbol error";
				return false;
			}
			if (!isdigit(word[1]) || !isdigit(word[3]) || !isdigit(word[4]) || !isdigit(word[5]) || !isdigit(word[6]) || !isdigit(word[7]) || !isdigit(word[8]) || !isdigit(word[9])) {
				*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position " + to_string(word_cursor + 1) + " digit error";
				return false;
			}
		}
		++line_number;
	}
	// making sure that the last layer is created
	if (file_data[line_number].compare("/") != 0) {
		*report_stream << "Error\nFile not terminated properly";
		return false;
	}
	*report_stream << "Success";
	return true;
}

Network::Network(string file_name, ofstream& report_stream_val) {
	// assigning report stream for report generation
	report_stream = &report_stream_val;
	ifstream ifile;
	vector<string> file_data;
	vector<vector<vector<double>>> network_info;
	// opening the file for reading
	ifile.open(file_name, ios::in);
	string line;
	// reading the file and saving the lines
	while (getline(ifile, line))
		file_data.push_back(line);
	ifile.close();
	*report_stream << "\n--- REPORTING STARTED FOR THE ETHANOL NEURAL NETWORK ---\n";
	// validating the file data
	*report_stream << "\nFILE VALIDATION    : ";
	if (!validate_file(file_data))
		exit(0);
	// converting file data into network info
	vector<vector<double>> layer_info;
	for (vector<string>::iterator file_line = file_data.begin() + 2; file_line != file_data.end(); ++file_line) {
		// save the layer info into network info once / is encountered and clear the layer info
		if (file_line->compare("/") == 0) {
			network_info.push_back(layer_info);
			layer_info.clear();
			continue;
		}
		// getting the bias and weights word by word from the line and saving as node info
		vector<double> node_info;
		for (unsigned int i2 = 0; i2 < file_line->length(); i2 += 11) {
			node_info.push_back(stod(file_line->substr(i2, 10)));
		}
		// save the node info into the layer info
		layer_info.push_back(node_info);
	}
	// validating the network info
	*report_stream << "\nNETWORK VALIDATION : ";
	// number of inputs taken by the other nodes in the first layer should be equal to that of the first node
	unsigned int input_count = network_info[0][0].size() - 1;
	for (unsigned int node_id = 1; node_id < network_info[0].size(); ++node_id) {
		if (network_info[0][node_id].size() - 1 != input_count) {
			*report_stream << "Error\nAt Layer 1 node " + to_string(node_id + 1);
			exit(0);
		}
	}
	// number of inputs taken by a node in the remaining layers should be equal to the number of nodes in the previous layer
	for (unsigned int layer_id = 1; layer_id < network_info.size(); ++layer_id) {
		for (unsigned int node_id = 0; node_id < network_info[layer_id].size(); ++node_id) {
			if (network_info[layer_id][node_id].size() - 1 != network_info[layer_id - 1].size()) {
				*report_stream << "Error\nAt Layer " + to_string(layer_id + 1) + " node " + to_string(node_id + 1);
				exit(0);
			}
		}
	}
	*report_stream << "Success";
	*report_stream << "\n";
	// creating layers
	for (vector<vector<vector<double>>>::iterator i = network_info.begin(); i != network_info.end(); ++i) {
		Layer layer = Layer(i->begin()->size() - 1, i->size());
		layers.push_back(layer);
	}
	// assigning weights and biases to the nodes
	for (unsigned int layer_id = 0; layer_id < network_info.size(); ++layer_id) {
		for (unsigned int node_id = 0; node_id < (network_info.begin() + layer_id)->size(); ++node_id) {
			vector<double> weight;
			vector<vector<double>>::iterator node_info = (network_info.begin() + layer_id)->begin() + node_id;
			// saving the bias value
			double bias = *(node_info->begin());
			// saving the weight values
			for (vector<double>::iterator weight_val = node_info->begin() + 1; weight_val != node_info->end(); ++weight_val)
				weight.push_back(*weight_val);
			// setting the bias and weights for the node
			set_bias(bias, layer_id, node_id);
			set_weight(weight, layer_id, node_id);
		}
	}
}

bool Network::set_weight(vector<double>& weight_val, unsigned int layer_id, unsigned int node_id) {
	if (layer_id >= layers.size()) {
		return false;
	}
	bool flag = (layers.begin() + layer_id)->set_weight(weight_val, node_id);
	if (flag) {
		*report_stream << "\n-- ASSIGN --";
		*report_stream << "\nLAYER  : " + to_string(layer_id);
		*report_stream << "\nNODE   : " + to_string(node_id);
		*report_stream << "\nWEIGHT : " + Tools::double_vector2string(weight_val);
		*report_stream << "\n";
		return true;
	}
	return false;
}

bool Network::set_bias(double bias_val, unsigned int layer_id, unsigned int node_id) {
	if (layer_id >= layers.size())
		return false;
	bool flag = (layers.begin() + layer_id)->set_bias(bias_val, node_id);
	if (flag) {
		*report_stream << "\n-- ASSIGN --";
		*report_stream << "\nLAYER  : " + to_string(layer_id);
		*report_stream << "\nNODE   : " + to_string(node_id);
		*report_stream << "\nBIAS   : " + to_string(bias_val);
		*report_stream << "\n";
		return true;
	}
	return false;
}

vector<double> Network::get_weight(unsigned int layer_id, unsigned int node_id) {
	// if invalid layer id, layer 0 is selected
	if (layer_id >= layers.size())
		layer_id = 0;
	return (layers.begin() + layer_id)->get_weight(node_id);
}

double Network::get_bias(unsigned int layer_id, unsigned int node_id) {
	// if invalid layer id, layer 0 is selected
	if (layer_id >= layers.size())
		layer_id = 0;
	return (layers.begin() + layer_id)->get_bias(node_id);
}

vector<vector<double>> Network::get_input_signal() {
	vector<vector<double>> input_signal;
	for (vector<Layer>::iterator i = layers.begin(); i != layers.end(); ++i)
		input_signal.push_back(i->get_input_signal());
	return input_signal;
}

vector<double> Network::get_output_signal() {
	return (layers.end() - 1)->get_output_signal();
}

bool Network::trigger(vector<double>& input_signal) {
	bool flag;
	unsigned int layer_id = 0;
	// saving the input signal into output signal and triggering the layers one by one using the latest output signal
	vector<double> output_signal = input_signal;
	*report_stream << "\n-- NETWORK TRIGGER --";
	for (vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		flag = layer->trigger(output_signal);
		if (!flag)
			return false;
		*report_stream << "\nLAYER  : " + to_string(layer_id) + "\nINPUT  : " + Tools::double_vector2string(output_signal);
		output_signal = layer->get_output_signal();
		*report_stream << "\nOUTPUT : " + Tools::double_vector2string(output_signal) + "\n";
		++layer_id;
	}
	return true;
}

void Network::save(string file_name) {
	ofstream ofile;
	// opening the file for writing and truncating the old data
	ofile.open(file_name, ios::out | ios::trunc);
	// writing first two lines
	ofile << "NETWORK DETAILS\n";
	int layer_size;
	vector<double> weight;
	for (vector<Layer>::iterator layer = layers.begin(); layer != layers.end(); ++layer) {
		// new line for new layer
		ofile << "\n";
		layer_size = layer->get_output_signal().size();
		for (int node_id = 0; node_id < layer_size; ++node_id) {
			// writing the bias value first followed by a space
			ofile << Tools::double_formatter(layer->get_bias(node_id), true) << " ";
			weight = layer->get_weight(node_id);
			for (vector<double>::iterator weight_val = weight.begin(); weight_val != weight.end(); ++weight_val) {
				// writing the weight values and followed by a new line if it is the last weight or followed by a space
				if (weight_val == weight.end() - 1)
					ofile << Tools::double_formatter(*weight_val, false) << "\n";
				else
					ofile << Tools::double_formatter(*weight_val, false) << " ";
			}
			// clearing the weight values for next node
			weight.clear();
		}
		// writing the layer creation line after each layer
		ofile << "/";
	}
	ofile.close();
	*report_stream << "\n-- NETWORK SAVE --\nFILE NAME : " + file_name;
}