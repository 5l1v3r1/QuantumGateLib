#include <iostream>
#include "../include/circuit/circuit.h"

template<class T>
[[deprecated]] void f(T){}

using namespace qlib;

//generate control x
_ptr<AbstUnitary> CX(const QuantumRegister& control, const QuantumRegister& target){
	return Op::Us("control_x", {Op::H(target), Op::CNOT(control, target), Op::H(target)});
}


int main(void) {

	Circuit circuit(3);

	// get quantum and classical register
	const std::vector<QuantumRegister>& qreg = circuit.getQuantumRegisters();
	const std::vector<ClassicalRegister>& creg = circuit.getClassicalRegisters();

	//print all registers addresses
	std::cout << "Quantum Registers: ";
	for(auto&& elem : qreg){
		std::cout << &elem << " ";
	}
	std::cout << std::endl;

	std::cout << "Classical Registers: ";
	for(auto&& elem : creg){
		std::cout << &elem << " ";
	}
	std::cout << std::endl;

	//generate Bell states
	auto BellGen = Op::Us("bell_gen", {Op::H(qreg[1]), Op::CNOT(qreg[1], qreg[2])});

	// construct quantum circuit (quantum teleportation protocol)
	circuit << Op::R(M_PI/8., qreg[0]) << BellGen << Op::CNOT(qreg[0], qreg[1]) << Op::H(qreg[0]) << CX(qreg[0], qreg[2]) << Op::CZ(qreg[0], qreg[2]);

	circuit << Op::Measurement(qreg[0], creg[0]) << Op::Measurement(qreg[1], creg[1]);

	//build command list
	auto list = circuit.buildCurrOpList();


	//print all commnands in list
	std::cout << "Command list: " << std::endl;
	for(auto&& elem : list){
		std::cout << elem.name << " " << elem.type << " q ";
		for(auto&& qreg : elem.p_qreg){
			std::cout << qreg << " ";
		}
		std::cout << " c ";
		for(auto&& creg : elem.p_creg){
			std::cout << creg << " ";
		}
		std::cout << " params ";
		for(auto&& param : elem.parameters){
			std::cout << param << " ";
		}
		std::cout << std::endl;
		
	}
	std::cout << std::endl;

	return 0;
}
     
