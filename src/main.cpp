#include <iostream>
#include "../include/circuit/circuit.h"

template<class T>
[[deprecated]] void f(T){}

using namespace qlib;

int main(void) {

	//quantum telepotation
	QuantumBit phi;
	QuantumBit ancilla1;
	QuantumBit ancilla2;

	auto q_phi = Op::Q(phi);
	auto q_ancilla1 = Op::Q(ancilla1);
	auto q_ancilla2 = Op::Q(ancilla2);

	auto H1 = Op::H();
	auto CNOT1 = Op::CNOT();

	auto CNOT2 = Op::CNOT();
	auto H2 = Op::H();

	auto MC1 = Op::MCtrl(Op::Z());
	auto MC2 = Op::MCtrl(Op::X());

	auto lastM = Op::M();

	//connect all nodes
	//TODO: modify interface
	
	q_ancilla1->connectNode(QubitStr::qubit(), *H1, UnitaryOpStr::singlenode());
	H1->connectNode(UnitaryOpStr::singlenode(), *CNOT1, ControlStr::control(0));
	q_ancilla2->connectNode(QubitStr::qubit(), CNOT1->getSubComponent(ControlStr::target()), UnitaryOpStr::singlenode());

	q_phi->connectNode(QubitStr::qubit(), *CNOT2, ControlStr::control(0));
	CNOT1->getSubComponent(ControlStr::target()).connectNode(UnitaryOpStr::singlenode(), MC2->getSubComponent(MeasCtrlStr::unitary()), UnitaryOpStr::singlenode());
	CNOT2->connectNode(ControlStr::control(0), *H2, UnitaryOpStr::singlenode());
	CNOT2->getSubComponent(ControlStr::target()).connectNode(UnitaryOpStr::singlenode(), MC2->getSubComponent(MeasCtrlStr::measure()), MeasureStr::measure());
	MC2->getSubComponent(MeasCtrlStr::unitary()).connectNode(UnitaryOpStr::singlenode(), MC1->getSubComponent(MeasCtrlStr::unitary()), UnitaryOpStr::singlenode());
	MC1->getSubComponent(MeasCtrlStr::unitary()).connectNode(UnitaryOpStr::singlenode(), *lastM, MeasureStr::measure());


	return 0;
}
     
