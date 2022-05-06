/**
* \file instruction.cpp
* \author Karol Desnos
* \license CeCILL-C
*/

#define _USE_MATH_DEFINES // To get M_PI
#include <math.h>

#include "instructions.h"

void fillInstructionSet(Instructions::Set& set) {
	auto minus = [](double a, double b) -> double { return a - b; };
	auto add = [](double a, double b) -> double { return a + b; };
	auto mult = [](double a, double b) -> double { return a * b; };
	auto div = [](double a, double b) -> double { return a / b; };
	auto max = [](double a, double b) -> double { return std::max(a, b); };
	auto ln = [](double a) -> double { return std::log(a); };
	auto exp = [](double a) -> double { return std::exp(a); };

	set.add(*(new Instructions::LambdaInstruction<double, double>(minus, "$0 = $1 - $2;")));
	set.add(*(new Instructions::LambdaInstruction<double, double>(add, "$0 = $1 + $2;")));
	set.add(*(new Instructions::LambdaInstruction<double, double>(mult, "$0 = $1 * $2;")));
	set.add(*(new Instructions::LambdaInstruction<double, double>(div, "$0 = $1 / $2;")));
	set.add(*(new Instructions::LambdaInstruction<double, double>(max, "$0 = (($1) < ($2)) ? ($2) : ($1);")));
	set.add(*(new Instructions::LambdaInstruction<double>(exp, "$0 = exp($1);")));
	set.add(*(new Instructions::LambdaInstruction<double>(ln, "$0 = log($1);")));
}

void deleteInstructions(Instructions::Set& set)
{
	for (unsigned int i = 0; i < set.getNbInstructions(); i++) {
		delete (&set.getInstruction(i));
	}
}
