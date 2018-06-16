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
	auto R = Op::R(3.14, qreg[2]);
	f(R);
	auto R1 = Op::R(3.14, null_qreg[10]);
	f(R1);
	auto R2 = Op::U("rth", null_qreg[0], null_qreg[1]);
	f(R2);
	auto R3 = Op::U("rth", qreg[1], qreg[1], qreg[1], qreg[1], qreg[1], qreg[1], qreg[1]);
	f(R3);
	auto R4 = Op::U("rth", std::array<double,3>{1,1,1}, qreg[1], qreg[1], qreg[1]);
	f(R4);
	auto F2 = Op::FREDKIN(qreg[2], qreg[1], qreg[0]);
	f(F2);
	auto F3 = Op::FREDKIN(null_qreg[0], null_qreg[1], null_qreg[2]);
	f(F3);
	auto U_module = Op::Us("__default_Module",{Op::CZ(qreg[0], qreg[2])});
	f(U_module);
	auto U_module2 = Op::Us("__default_Module", {H, R, U_module});
	f(U_module2);
	
	return 0;
}
     
