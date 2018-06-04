/**
 * @file circuit.h
 * @brief describing quantum circuits and quantum gates
 * @author Kohji Nishimura
 * @date 2018/06/03
 *
 */

#pragma once
#include <vector>
#include <string>

namespace qlib{

	/**
	 * quantum register
	 */

	struct qReg{
	};

	/**
	 * classical register
	 */

	struct cReg{
	};

	/**
	 * Abstract components
	 */

	class Component{
		private:
			//! identifier name
			std::string name;
	};

	/**
	 * A class for representing quantum unitary gates.<br/>
	 * Each unitary gate is identified by its name string.
	 */

	class Unitary : public Component{
		private:
			//! gate array
			std::vector<qReg> q_registers;

		public:
			/**
			 * return its inverse operator
			 * @return inverse unitary operator
			 */

			virtual void dagger() const = 0;
	};

	/**
	 * A class for representing measurement process.
	 */

	class Measurement : public Component{
		private:
			//! gate
			qReg q_register;
	};

	/**
	 * A class for representing quantum circuits
	 */

	class Circuit{
	};
}
