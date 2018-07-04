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
		//! swap gate
		SWAP,
		//! generalized control gate
		CONTROL,
		//! the number of default strings
		NUMSTR,
	};

	//! default string of measurement
	extern const std::array<std::string, DefaultString::NUMSTR> default_str;

	/**
	 * default node strings for Measure class
	 */

	struct MeasureStr{
		//! nodename : measure
		const static std::string measure;
	};

	/**
	 * default node strings for UnitaryOp class with single node
	 */

	struct UnitaryOpStr{
		//! nodename : s 
		const static std::string singlenode;
	};

	/**
	 * default node string for swap unitary gate
	 */

	struct SwapStr{
		//nodename : swap0, swap1
		const static std::string swap;
	};
}
