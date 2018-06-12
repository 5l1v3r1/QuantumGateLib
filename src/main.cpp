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
	auto H = Op::H(qreg[0]);
	f(H);
	auto Id = Op::Id(qreg[2]);
	f(Id);
	const std::array<double,1> t = {3.65};
	auto R = Op::U("__default_Rotate_", t, qreg[2], qreg[0]);
	f(R);
	auto R2 = Op::R(3.14, qreg[1]);
	f(R2);
	auto U_module = Op::Us("__default_Module");
	f(U_module);
	auto U_module2 = Op::Us("__default_Module", {H, R, U_module});
	f(U_module2);
	return 0;
}
     
