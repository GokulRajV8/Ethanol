/*
 * File name   : trainer.cpp
 * Description : contains all the definitions for methods declared in ethanol.h for class Trainer
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#include "ethanol.h"

bool Trainer::validate_file(vector<string>& file_data) {
	// checking the first line
	if (file_data[0].compare("TRAINING DATA") != 0) {
		*report_stream << "Error\nTitle missing";
		return false;
	}
	// checking the second line
	if (file_data[1].size() != 0) {
		*report_stream << "Error\nLine 2 is not empty";
		return false;
	}
	unsigned int inputs = (network->get_input_signal())[0].size();
	unsigned int outputs = network->get_output_signal().size();
	unsigned int line_number = 2;
	for (vector<string>::iterator file_line = file_data.begin() + 2; file_line != file_data.end(); ++file_line) {
		// empty space is added in the end of the line so that the last value string is no different from other values
		string line(*file_line + " ");
		// checking whether the line has the length to exactly fit the input and output values
		if (line.size() != (10 * (inputs + outputs) + 2)) {
			*report_stream << "Error\nLine " + to_string(line_number + 1) + " has invalid length";
			return false;
		}
		// verifying inputs
		// loop works only number of inputs times
		unsigned int word_cursor = 0;
		while (word_cursor != 10 * inputs) {
			string word_str = line.substr(word_cursor, 10);
			const char* word = word_str.c_str();
			// get out of the loop if input-output separator is encountered
			if (word[0] == '/')
				break;
			// verifying the correctness and the position of symbols
			if ((word[1] != '.') || (word[9] != ' ')) {
				*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position " + to_string(word_cursor + 1) + " symbol error";
				return false;
			}
			// verifying that all other characters are digits for the value
			if (!isdigit(word[0]) || !isdigit(word[2]) || !isdigit(word[3]) || !isdigit(word[4]) || !isdigit(word[5]) || !isdigit(word[6]) || !isdigit(word[7]) || !isdigit(word[8])) {
				*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position " + to_string(word_cursor + 1) + " digit error";
				return false;
			}
			word_cursor += 10;
		}
		// verifying the correctness of number of inputs and making sure that the input-output separator is present correctly
		if (word_cursor != 10 * inputs) {
			*report_stream << "Error\nLine " + to_string(line_number + 1) + " has invalid number of inputs";
			return false;
		}
		if (line.substr(word_cursor, 2).compare("/ ") != 0) {
			*report_stream << "Error\nLine " + to_string(line_number + 1) + " has invalid separator";
			return false;
		}
		word_cursor += 2;
		// verifying outputs
		// loop works only number of outputs times
		while (word_cursor != (10 * (inputs + outputs) + 2)) {
			string word_str = line.substr(word_cursor, 10);
			const char* word = word_str.c_str();
			// verifying the correctness and the position of symbols
			if ((word[1] != '.') || (word[9] != ' ')) {
				*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position " + to_string(word_cursor + 1) + " symbol error";
				return false;
			}
			// verifying that all other characters are digits for the value
			if (!isdigit(word[0]) || !isdigit(word[2]) || !isdigit(word[3]) || !isdigit(word[4]) || !isdigit(word[5]) || !isdigit(word[6]) || !isdigit(word[7]) || !isdigit(word[8])) {
				*report_stream << "Error\nIn line " + to_string(line_number + 1) + " and position " + to_string(word_cursor + 1) + " digit error";
				return false;
			}
			word_cursor += 10;
		}
		++line_number;
	}
	*report_stream << "Success";
	return true;
}

Trainer::Trainer(Network& network_obj, ofstream& report_stream_val) {
	network = &network_obj;
	epsilon = 0.0;
	report_stream = &report_stream_val;
	*report_stream << "\n--- REPORTING STARTED FOR THE ETHANOL NEURAL NETWORK TRAINER ---\n";
}

void Trainer::load_dataset(string file_name) {
	ifstream ifile;
	vector<string> file_data;
	// opening the file for reading
	ifile.open(file_name, ios::in);
	string line;
	// reading the file and saving the lines
	while (getline(ifile, line))
		file_data.push_back(line);
	ifile.close();
	// validating the file data
	*report_stream << "\nFILE VALIDATION : ";
	if (!validate_file(file_data))
		exit(0);
	*report_stream << "\n";
	// converting file data into training data
	unsigned int inputs = (network->get_input_signal())[0].size();
	unsigned int outputs = network->get_output_signal().size();
	// clearing the previous data set
	dataset.clear();
	vector<double> data;
	for (vector<string>::iterator file_line = file_data.begin() + 2; file_line != file_data.end(); ++file_line) {
		unsigned int i = 0;
		// saving all the input values in the dataset first
		for (; i < 10 * inputs; i += 10)
			data.push_back(stod(file_line->substr(i, 9)));
		dataset.push_back(data);
		// clearing the input values
		data.clear();
		i += 2;
		// saving all the output values in the dataset later
		for (; i < 10 * (inputs + outputs); i += 10)
			data.push_back(stod(file_line->substr(i, 9)));
		dataset.push_back(data);
		// clearing the output values
		data.clear();
	}
}

void Trainer::set_epsilon(double epsilon_val) {
	epsilon = epsilon_val;
	*report_stream << "\n-- ASSIGN --\nLEARNING RATE : " + to_string(epsilon) + "\n";
}

double Trainer::get_epsilon() {
	return epsilon;
}

bool Trainer::train() {
	// epoch stands for the single input-output combination in the dataset
	unsigned int epoch = 1;
	bool flag;
	*report_stream << "\n-- TRAINING --\n";
	for (vector<vector<double>>::iterator input = dataset.begin(); input != dataset.end(); input += 2) {
		vector<double> output_expected = *(input + 1);
		*report_stream << "\nEPOCH             : " + to_string(epoch) + "\n";
		// network triggered
		flag = network->trigger(*input);
		if (!flag)
			return flag;
		// save all the input signals of the network and save the output signal as well
		vector<vector<double>> network_input = network->get_input_signal();
		vector<double> output_signal = network->get_output_signal();
		// calculating the mean square error in the output
		double error = 0.0;
		for (unsigned int output_id = 0; output_id != output_signal.size(); ++output_id)
			error += (output_expected[output_id] - output_signal[output_id]) * (output_expected[output_id] - output_signal[output_id]);
		error /= output_signal.size();
		*report_stream << "\nMEAN SQUARE ERROR : " + to_string(error) + "\nLEARNING RATE     : " + to_string(epsilon) + "\n";
		// back propagating from the last layer to the first layer
		for (int layer_id = network_input.size() - 1; layer_id != -1; --layer_id) {
			vector<double> layer_input = network_input[layer_id];
			vector<double> new_input = layer_input;
			for (unsigned int node_id = 0; node_id != output_signal.size(); ++node_id) {
				// creating new weight, input and bias
				double temp_weight = 0.0, temp_input = 0.0, temp_bias = 0.0;
				vector<double> node_weight = network->get_weight(layer_id, node_id);
				vector<double> new_weight = node_weight;
				// computing the gradient for reduction of square error of the node output
				// 1/20 is not included in the below multiplication to reduce the input value of epsilon
				double gradient = -2.0 * (output_signal[node_id] - output_expected[node_id]) * ((output_signal[node_id] == 0.0) ? 0.0 : 1.0) * (1.0 / node_weight.size());
				// iterating through the inputs of the node to calculate new weight and input values
				for (unsigned int input_id = 0; input_id < layer_input.size(); ++input_id) {
					temp_weight = new_weight[input_id] + epsilon * gradient * layer_input[input_id];
					temp_input = new_input[input_id] + epsilon * gradient * node_weight[input_id];
					// making sure the weight do not cross bounds
					if (temp_weight > 9.9999999)
						new_weight[input_id] = 9.9999999;
					else if (temp_weight < -9.9999999)
						new_weight[input_id] = -9.9999999;
					else
						new_weight[input_id] = temp_weight;
					// making sure the input do not cross bounds
					if (temp_input > 9.9999999)
						new_input[input_id] = 9.9999999;
					else if (temp_input < 0.0)
						new_input[input_id] = 0.0;
					else
						new_input[input_id] = temp_input;
				}
				// setting the new weight values to the node
				network->set_weight(new_weight, layer_id, node_id);
				// calculating new bias value and assigning it to the node after making sure it is within bounds
				temp_bias = network->get_bias(layer_id, node_id) + epsilon * gradient * -1.0 * node_weight.size();
				if (temp_bias > 99.999999)
					network->set_bias(99.999999, layer_id, node_id);
				else if (temp_bias < -99.999999)
					network->set_bias(-99.999999, layer_id, node_id);
				else
					network->set_bias(temp_bias, layer_id, node_id);
			}
			// updating the expected and actual output signal values to move to the previous layer
			output_expected = new_input;
			output_signal = layer_input;
		}
		++epoch;
	}
	return true;
}