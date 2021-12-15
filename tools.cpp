/*
 * File name   : tools.cpp
 * Description : contains all the definitions for methods declared in ethanol.h for class Tools
 * Author      : Gokul Raj V
 * Last update : version 0.1 - 18 Sep 2021
 */

#include "ethanol.h"

string Tools::double_formatter(double val, bool is_bias) {
	string output("");
	// appending the signage of the value
	if (val < 0.0)
		output.append("-");
	else
		output.append("+");
	val = fabs(val);
	// two digits will be displayed before the decimal point if the value is a bias
	// one digit will be displayed before the decimal point if the value is a weight
	if (is_bias) {
		// trimming only 6 digits after the decimal point
		long long int value = val * 1000000LL;
		long long int divider = 10000000LL;
		// appending the first two digits before the decimal point
		for (int i = 0; i < 2; ++i) {
			output.append(to_string(value / divider));
			value = value % divider;
			divider /= 10;
		}
		output.append(".");
		// appending the next six digits after the decimal point
		for (int i = 0; i < 6; ++i) {
			output.append(to_string(value / divider));
			value = value % divider;
			divider /= 10;
		}
	}
	else {
		// trimming only 7 digits after the decimal point
		long long int value = val * 10000000LL;
		long long int divider = 10000000LL;
		// appending the first digit before the decimal point
		output.append(to_string(value / divider) + ".");
		value = value % divider;
		divider /= 10;
		// appending the next seven digits after the decimal point
		for (int i = 0; i < 7; ++i) {
			output.append(to_string(value / divider));
			value = value % divider;
			divider /= 10;
		}
	}
	return output;
}

string Tools::double_vector2string(vector<double>& vector_val) {
	string output = string("[ ");
	for (vector<double>::iterator value = vector_val.begin(); value != vector_val.end(); ++value)
		output += to_string(*value) + " ";
	return output + "]";
}