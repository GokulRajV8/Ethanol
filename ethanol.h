/*
 * File name   : ethanol.h
 * Description : contains all the class definitions and method declarations for Ethanol neural networks
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#ifndef ETHANOL_H_
#define ETHANOL_H_

#include <vector>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

// Nodes are the building block of neural networks and carries out operations like neurons.
class Node {
private:
	// Input signals through various input connections
	// Range : 0 to 10
	vector<double> input_signal;
	// Weights assigned to input connections
	// Range -10 to 10
	vector<double> weight;
	// Bias of the particular node to decide the excitation point on the weighted input sum axis
	// Range -100 to 100
	double bias;
	// Output signal given by the excitation of the node through input signal
	// Range : 0 to 10
	double output_signal;

public:
	// Create a node with the given number of input connections
	Node(unsigned int input_size);
	// Get Set methods
	bool set_weight(vector<double>& weight_val);
	bool set_bias(double bias_val);
	vector<double> get_weight();
	double get_bias();
	vector<double> get_input_signal();
	double get_output_signal();

	// Trigger the node for the excitation through input signals
	// Returns true when successful
	bool trigger(vector<double>& input_signal_val);
};

// Layer consists of various nodes accepting the same input and giving various outputs
// which acts as the input for another layer or the neural network output.
// Layers organise the nodes for signal propagation through the network.
class Layer {
private:
	// Nodes making up the layer
	vector<Node> nodes;
	// Output signals from the nodes
	vector<double> output_signal;

public:
	// Create a layer with the given number of input signals and number of nodes
	Layer(unsigned int input_size, unsigned int nodes_count);
	// Get Set methods
	bool set_weight(vector<double>& weight_val, unsigned int node_id);
	bool set_bias(double bias_val, unsigned int node_id);
	vector<double> get_weight(unsigned int node_id);
	double get_bias(unsigned int node_id);
	vector<double> get_input_signal();
	vector<double> get_output_signal();

	// Trigger the layer for the excitation of nodes through input signals and produce output signals
	// Returns true when successful
	bool trigger(vector<double>& input_signal);
};

// Neural network act like simulated brain using neuron-like nodes working together to predict the output for the given input.
class Network {
private:
	// Layers making the neural network
	vector<Layer> layers;
	// File stream for reporting the network activities
	ofstream* report_stream;

	// Private method to verify the correctness of the input file used for neural network creation
	bool validate_file(vector<string>& file_data);

public:
	// Create a neural network with the details given in the input file and also setup a file stream for reporting
	Network(string file_name, ofstream& report_stream_val);
	// Get Set methods
	bool set_weight(vector<double>& weight_val, unsigned int layer_id, unsigned int node_id);
	bool set_bias(double bias_val, unsigned int layer_id, unsigned int node_id);
	vector<double> get_weight(unsigned int layer_id, unsigned int node_id);
	double get_bias(unsigned int layer_id, unsigned int node_id);
	// Returns the input signals from all the layers starting from input to the output layer
	vector<vector<double>> get_input_signal();
	vector<double> get_output_signal();

	// Trigger the neural network using input signals to get the output signals from the neural network
	// Returns true when successful
	bool trigger(vector<double>& input_signal);
	// Save the network configuration as a file for archiving and future network creations
	void save(string file_name);
};

// Trainer is used to tune the network to give correct answers using a set inputs and their corresponding outputs.
// It uses back-propagation from output layer to input layer to tune the weights and biases of the nodes.
class Trainer {
private:
	// Network that has to be trained
	Network* network;
	// Set of inputs and corresponding outputs used for training
	// Note : A row consists of input signals followed by the next row consisting of corresponding output signals
	// and hence contains even number of rows in the vector
	vector<vector<double>> dataset;
	// Learning rate used for the network training
	double epsilon;
	// File stream for reporting the trainer activities
	ofstream* report_stream;

	bool validate_file(vector<string>& file_data);

public:
	// Create a trainer with a network to be trained and also setup a file stream for reporting
	Trainer(Network& network_obj, ofstream& report_stream_val);
	// Load the data set from a file to use for the training of the network
	void load_dataset(string file_name);
	// Get Set methods
	void set_epsilon(double epsilon_val);
	double get_epsilon();
	// Trains the network using the inputs and outputs provided in the dataset
	// Returns true when successful
	bool train();
};

// Utility tools for the Ethanol neural network functionalities
class Tools {
public:
	// Converts the double value passed into a string that can be written in a file while saving the network info
	static string double_formatter(double val, bool is_bias);
	// Converts a vector containing double values into a formatted string for reporting purposes
	static string double_vector2string(vector<double>& vector_val);
};

#endif /* ETHANOL_H_ */