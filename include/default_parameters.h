/**
 * @file default_parameters.h
 * @brief default parameters
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
		//! controlled-R
		CR,
		//! toffoli gate
		TOFFOLI,
		//! fredkin gate (controlled-SWAP)
		FREDKIN,
		//! the number of default strings
		NUMSTR,
	};

	//! default string of measurement
	extern const std::array<std::string, DefaultString::NUMSTR> default_str;

	/**
	 * enum for representing component types
	 */
	enum ComponentType{
		//! undefined
		UNDEFINED,
		//! measurement
		MEASUREMENT,
		//! single unitary operator
		UNITARY_OP,
		//! single unitary operator with parameters
		UNITARY_OP_PARAM,
		//! unitary container
		UNITARY_CONTAINER,
	};
}
