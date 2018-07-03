/**
 * @file default_parameters.cpp
 * @brief cpp file for instantiation of default parameters
 * @author Kohji Nishimura
 * @date 2018/06/12
 *
 */

#include "../include/default_parameters.h"
#include "../include/circuit/circuit.h"
#include <cmath>

namespace qlib {
	//! default string of measurement
	const std::array<std::string, DefaultString::NUMSTR> default_str= {
		"__default_measure_",
		"__default_id_",
		"__default_x_",
		"__default_y_",
		"__default_z_",
		"__default_h_",
		"__default_s_",
		"__default_t_",
		"__default_r_",
		"__default_cnot_",
		"__default_swap_",
		"__default_cz_",
		"__default_cs_",
		"__default_cr_",
		"__default_toffoli_",
		"__default_fredkin_",
	};

	//! dummy qubit
	DummyQuantumRegister null_qreg;

	//! dummy node
	Node null_node("dummy");

	//! list of dagger pairs and function handlers (will be used by dagger() function in UnitaryOp and UnitaryContainer)
	//TODO: implement Rules
	/*
		std::vector<std::tuple<_ptr<AbstUnitary>, _ptr<AbstUnitary>, ParameterFunction>> Rule::dagger_pairs = {
	//UNITARY_OP -> UNITARY_OP
	std::make_tuple(Op::X(null_qreg[0]),Op::X(null_qreg[0]), nullptr), //X.dagger() == X
	std::make_tuple(Op::Y(null_qreg[0]),Op::Y(null_qreg[0]), nullptr),
	std::make_tuple(Op::Z(null_qreg[0]),Op::Z(null_qreg[0]), nullptr),
	std::make_tuple(Op::H(null_qreg[0]),Op::H(null_qreg[0]), nullptr),
	std::make_tuple(Op::S(null_qreg[0]),Op::R(-M_PI/2., null_qreg[0]), nullptr),
	std::make_tuple(Op::T(null_qreg[0]),Op::R(-M_PI/4., null_qreg[0]), nullptr),
	std::make_tuple(Op::CNOT(null_qreg[0], null_qreg[1]),Op::CNOT(null_qreg[0], null_qreg[1]), nullptr),
	std::make_tuple(Op::SWAP(null_qreg[0], null_qreg[1]),Op::SWAP(null_qreg[0], null_qreg[1]), nullptr),
	std::make_tuple(Op::CZ(null_qreg[0], null_qreg[1]),Op::CZ(null_qreg[0], null_qreg[1]), nullptr),
	std::make_tuple(Op::CS(null_qreg[0], null_qreg[1]),Op::CR(-M_PI/2., null_qreg[0], null_qreg[1]), nullptr),
	std::make_tuple(Op::TOFFOLI(null_qreg[0], null_qreg[1], null_qreg[2]), Op::TOFFOLI(null_qreg[0], null_qreg[1], null_qreg[2]), nullptr),
	std::make_tuple(Op::FREDKIN(null_qreg[0], null_qreg[1], null_qreg[2]), Op::FREDKIN(null_qreg[0], null_qreg[1], null_qreg[2]), nullptr),
	//UNITARY_OP_PARAM -> UNITARY_OP_PARAM
	std::make_tuple(Op::R(0, null_qreg[0]), Op::R(0, null_qreg[0]), [](std::vector<double> x){return std::vector<double>{-x[0]};}),//R(x).dagger() == R(-x)
	std::make_tuple(Op::CR(0, null_qreg[0], null_qreg[1]), Op::CR(0, null_qreg[0], null_qreg[1]), [](std::vector<double> x){return std::vector<double>{-x[0]};}),

	};
	*/
} 
