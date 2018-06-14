/**
 * @file default_parameters.cpp
 * @brief cpp file for instantiation of default parameters
 * @author Kohji Nishimura
 * @date 2018/06/12
 *
 */

#include "../include/default_parameters.h"
#include "../include/circuit/circuit.h"

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
		"__default_toffoli_",
		"__default_fredkin_",
	};

	//! dummy qubit
	const QuantumRegister null_qreg;
	//! dummy bit
	const ClassicalRegister null_creg;

	//! list of dagger_pairs (will be used by dagger() in UnitaryOp and UnitaryContainer)
	std::vector<std::pair<_ptr<AbstUnitary>, _ptr<AbstUnitary>>> Rule::dagger_pairs = {
		std::make_pair<_ptr<AbstUnitary>, _ptr<AbstUnitary>>(Op::X(null_qreg), Op::X(null_qreg)),
	};

} 
