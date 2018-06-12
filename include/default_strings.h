/**
 * @file default_strings.h
 * @brief default strings which represent quantum components
 * @author Kohji Nishimura
 * @date 2018/06/12
 *
 */


#pragma once
#include <string>
#include <array>

namespace qlib{
	/*
	 * enum for representing flags of specific string
	 */

	enum DefaultString{
		//! measurement
		MEASURE,
		//! pauli id
		Id,
		//! pauli x
		X,
		//! pauli y
		Y,
		//! pauli z
		Z,
		//! hadamard
		H,
		//! phase gate
		S,
		//! pi/8 gate
		T,
		//! rotate (phase-shift) gate
		R,
		//! controlled-NOT
		CNOT,
		//! swap gate
		SWAP,
		//! controlled-Z
		CZ,
		//! controlled-S
		CS,
		//! toffoli gate
		TOFFOLI,
		//! fredkin gate (controlled-SWAP)
		FREDKIN,
		//! the number of default strings
		NUMSTR,
	};

	//! default string of measurement
	const static std::array<std::string, DefaultString::NUMSTR> default_str= {
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
}
