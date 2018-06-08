#include <iostream>
#include "../include/circuit/circuit.h"

template<class T>
[[deprecated]] void f(T){}

int main(void) {
	using namespace qlib;
	std::array<QuantumRegister, 3> qreg;
	std::array<ClassicalRegister, 3> creg;
	auto meas = Op::Measurement(qreg[1], creg[1]);
	f(meas);
	auto H = Op::U("__default_Hadamard_", qreg[1], qreg[2], qreg[0]);
	f(H);
	const std::array<double,1> t = {3.65};
	auto R = Op::U("__default_Rotate_", t, qreg[1], qreg[2]);
	f(R);
	auto U_module = Op::Us("__default_Module");
	f(U_module);
	auto U_module2 = Op::Us("__default_Module", {H, R, U_module});
	f(U_module2);
	std::cout << "Hello world" << std::endl;
	return 0;
}
     
