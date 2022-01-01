/*
 * File name   : tester.cpp
 * Description : used to test the Ethanol Neural Network functionalities
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#include "ethanol.h"
#include <iostream>

using namespace std;

//testing network
void test1() {
	ofstream report_stream("test\\report.txt", ios::out | ios::trunc);
	Network network("test\\test1.txt", report_stream);
	vector<double> input_val;
	vector<double> output_val;

	// getting input signals
	cout << "Welcome to Ethanol Neural Network ...\n";
	for (unsigned int i = 0; i < network.get_input_signal().begin()->size(); ++i) {
		double temp;
		cout << "Enter the " << i << " input : ";
		cin >> temp;
		input_val.push_back(temp);
	}

	// network processing
	bool flag = network.trigger(input_val);
	output_val = network.get_output_signal();
	if (flag) {
		cout << "\nOutput signal is :";
		for (auto i = output_val.begin(); i != output_val.end(); ++i)
			cout << " " << *i;
	}
	else
		cout << "\nProcessing failed ...";
	network.save("test\\result.txt");
	report_stream.close();
}

//testing trainer
void test2() {
	ofstream report_stream;
	report_stream.open("test\\report.txt", ios::out | ios::trunc);
	Network network("test\\test1.txt", report_stream);
	Trainer trainer(network, report_stream);
	trainer.load_dataset("test\\test2.txt");
	trainer.set_epsilon(1.0);
	bool flag = trainer.train();
	if (flag) {
		printf("Network training successful");
		network.save("test\\result.txt");
	}
	else
		printf("Network crashed");
	report_stream.close();
}

int main() {
	test1();
	cout << "\n\n";
	// test2();
	return 0;
}