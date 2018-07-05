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
	/**
	 * enum for representing flags of specific string
	 */

	enum DefaultString{
		//! qubit
		QUBIT,
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
		//! measurement-control gate
		MEASURE_CONTROL,
		//! the number of default strings
		NUMSTR,
	};

	/** 
	 * default string
	 * @param str DefaultString enum
	 * @return corresponding string
	 */
	inline const std::string default_str(DefaultString str){
		switch(str){
			case DefaultString::QUBIT:
				return "__default_qubit_";
			case DefaultString::MEASURE:
				return "__default_measure_";
			case DefaultString::Id:
				return "__default_id_";
			case DefaultString::X:
				return "__default_x_";
			case DefaultString::Y:
				return "__default_y_";
			case DefaultString::Z:
				return "__default_z_";
			case DefaultString::H:
				return "__default_h_";
			case DefaultString::S:
				return "__default_s_";
			case DefaultString::T:
				return "__default_t_";
			case DefaultString::R:
				return "__default_r_";
			case DefaultString::SWAP:
				return "__default_swap_";
			case DefaultString::CONTROL:
				return "__default_ctrl_";
			case DefaultString::MEASURE_CONTROL:
				return "__default_measure_ctrl_";
			default:
				return "";
		}
	}

	
	 // TODO: enum for Component type?


	/**
	 * default node string for Qubit class
	 */

	struct QubitStr{
		/** 
		 * nodename : qubit
		 * @return string
		 */
		inline const static std::string qubit(){
			return "qubit";
		}
	};

	/**
	 * default node strings for Measure class
	 */

	struct MeasureStr{
		/** 
		 * nodename : measure
		 * @return string
		 */
		inline const static std::string measure(){
			return "measure";
		}
	};

	/**
	 * default node strings for UnitaryOp class with single node
	 */

	struct UnitaryOpStr{
		/** 
		 * nodename : singlenode
		 * @return string
		 */
		inline const static std::string singlenode(){
			return "singlenode";
		}
	};

	/**
	 * default node string for swap unitary gate
	 */

	struct SwapStr{

		/** 
		 * prefix : swap
		 * @return string
		 */
		inline const static std::string swap(){
			return "swap";
		}

		/** 
		 * nodename : swap0
		 * @return string
		 */
		inline const static std::string swap0(){
			return "swap0";
		}

		/** 
		 * nodename : swap1
		 * @return string
		 */
		inline const static std::string swap1(){
			return "swap1";
		}
	};

	/**
	 * default node string for control gate
	 */

	struct ControlStr{
		/** 
		 * prefix : control
		 * @return string
		 */
		inline const static std::string control(){
			return "control";
		}

		/** 
		 * subcomponent : target
		 * @return string
		 */
		inline const static std::string target(){
			return "target";
		}

		/** 
		 * nodename : controlx
		 * @return string
		 */
		inline const static std::string control(size_t x){
			return "control"+std::to_string(x);
		}
	};

	/**
	 * default node string for measure-control gate
	 */

	struct MeasCtrlStr{
		/**
		 * nodename : measure
		 * @return string
		 */
		inline const static std::string measure(){
			return "measure";
		}

		/**
		 * nodename : unitary
		 * @return string
		 */
		inline const static std::string unitary(){
			return "unitary";
		}
	};

	/**
	 * default node string for container?
	 */

}
