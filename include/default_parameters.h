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

	//! default string of measurement
	extern const std::array<std::string, DefaultString::NUMSTR> default_str;

	/**
	 * default node strings for Measure class
	 */

	struct MeasureStr{
		/** nodename : measure
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
		/** nodename : singlenode
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

		/** nodename : target
		 * @return string
		 */
		inline const static std::string target(){
			return "target";
		}

		/** nodename : controlx
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
}
