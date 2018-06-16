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
	//! dummy bit
	DummyClassicalRegister null_creg;

	//! list of dagger pairs (will be used by dagger() function in UnitaryOp and UnitaryContainer)
	std::vector<std::pair<_ptr<AbstUnitary>, _ptr<AbstUnitary>>> Rule::dagger_pairs = {
		std::make_pair(Op::X(null_qreg[0]),Op::X(null_qreg[0])),
		std::make_pair(Op::Y(null_qreg[0]),Op::Y(null_qreg[0])),
		std::make_pair(Op::Z(null_qreg[0]),Op::Z(null_qreg[0])),
		std::make_pair(Op::H(null_qreg[0]),Op::H(null_qreg[0])),
		std::make_pair(Op::S(null_qreg[0]),Op::R(-M_PI/2., null_qreg[0])),
		std::make_pair(Op::T(null_qreg[0]),Op::R(-M_PI/4., null_qreg[0])),
		std::make_pair(Op::CNOT(null_qreg[0], null_qreg[1]),Op::CNOT(null_qreg[0], null_qreg[1])),
		std::make_pair(Op::SWAP(null_qreg[0], null_qreg[1]),Op::SWAP(null_qreg[0], null_qreg[1])),
		std::make_pair(Op::CZ(null_qreg[0], null_qreg[1]),Op::CZ(null_qreg[0], null_qreg[1])),
		std::make_pair(Op::CS(null_qreg[0], null_qreg[1]),Op::CR(-M_PI/2., null_qreg[0], null_qreg[1])),
		std::make_pair(Op::TOFFOLI(null_qreg[0], null_qreg[1], null_qreg[2]), Op::TOFFOLI(null_qreg[0], null_qreg[1], null_qreg[2])),
		std::make_pair(Op::FREDKIN(null_qreg[0], null_qreg[1], null_qreg[2]), Op::FREDKIN(null_qreg[0], null_qreg[1], null_qreg[2])),
	};

} 
