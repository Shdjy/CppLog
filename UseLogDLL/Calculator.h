#pragma once

#ifdef USELOGDLL_EXPORTS
#define CALC_API __declspec(dllexport)
#else
#define CALC_API __declspec(dllimport)
#endif

class CALC_API Calculator
{
public:
	Calculator();
	~Calculator();

	int Add(int a, int b);
	int Subtract(int a, int b);
	int Multiply(int a, int b);
	double Divide(double a, double b); // 除法，除数为0抛异常

private:
	// 内部辅助函数可以放这里
};
