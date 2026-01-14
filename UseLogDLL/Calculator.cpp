#include "pch.h"
#include "Calculator.h"
#include <stdexcept>

Calculator::Calculator()
{
}

Calculator::~Calculator()
{
}

int Calculator::Add(int a, int b)
{
	LOG_INFO(" ‰»Î£∫" + std::to_string(a) + "," + std::to_string(b));
	return a + b;
}

int Calculator::Subtract(int a, int b)
{
	return a - b;
}

int Calculator::Multiply(int a, int b)
{
	return a * b;
}

double Calculator::Divide(double a, double b)
{
	if (b == 0.0)
	{
		throw std::runtime_error("Divide by zero");
	}

	return a / b;
}
