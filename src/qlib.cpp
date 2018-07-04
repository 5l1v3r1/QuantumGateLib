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

//TODO: remove cpp file

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
		"__default_swap_",
		"__default_ctrl_",
	};

	//! dummy node
	Node null_node("dummy");

	//! nodename : measure
	const std::string MeasureStr::measure = "measure";
	//! nodename : singlenode 
	const std::string UnitaryOpStr::singlenode = "singlenode";


} 
