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
#include <exception>
#include <memory>

namespace qlib{

	//! abbrebiation of std::shared_ptr
	template<class T>
	using s_ptr = std::shared_ptr<T>;
	//! abbrebiation of std::unique_ptr
	template<class T>
	using u_ptr = std::unique_ptr<T>;

	//! default string of measurement
	const static std::string default_str_measure = "__default_measure_";

	/**
	 * print exception message
	 * @param exception_name exception name
	 * @param message additional exception message
	 * @return decorated string
	 */
	const std::string exception_format(const std::string& exception_name, const std::string& message){
		std::string retmsg = exception_name + " Exception: \n" + message;
		return retmsg;
	}

	/**
	 * An exception class for QuantumCircuit library
	 */

	class InvalidOperationException: public std::exception{
		private:
			//! exception message
			std::string message;

		public:

			/**
			 * constructor of InvalidOperationException
			 * @param msg additional exception message
			 */

			InvalidOperationException(const std::string& msg)
				: message(exception_format("InvalidOperation", msg)){
				}

			virtual const char* what() const noexcept{
				return message.c_str();
			}
	};


	/**
	 * abstract Component
	 */

	class Component{
		protected:
			//! an identifier
			std::string name;
			/**
			 * protected constructor of Component <br/>
			 * only friend class can call this constructor.
			 * @param name identifier name
			 */
			Component(const std::string& name)
				:name(name){
				}
	};

	/**
	 * quantum register
	 */

	struct QuantumRegister{
	};

	/**
	 * classical register
	 */

	struct ClassicalRegister{
	};

	/**
	 * class for Measurement operation
	 */

	class Measure: public Component{
		protected:

			//! pointer of QuantumRegister
			const QuantumRegister* p_qreg;
			//! pointer of ClassicalRegister
			const ClassicalRegister* p_creg;

			/**
			 * protected constructor of Measure
			 * @param qreg QuntumRegister to be measured
			 * @param creg corresponding ClassicalRegister
			 */

			Measure(const QuantumRegister& qreg, const ClassicalRegister& creg)
			: Component(default_str_measure), p_qreg(&qreg), p_creg(&creg){
			}
	};

	/**
	 * class for Unitary operator and Unitary container
	 */

	class AbstUnitary : public Component{
		protected:

	};

	/**
	 * class for Unitary operator
	 */

	class UnitaryOp: public Component{
		protected:
			//! array of pointer of QuantumRegister
			const std::vector<QuantumRegister*> p_qregs;

			/**
			 * protected constructor of UnitaryOp
			 * @param name identifier name
			 * @param 
			 */

			template<class... Args>
			UnitaryOp(const std::string& name, const Args&... args)
			: Component(name), p_qregs({(&args)...}){
			}

			template<class>

	};

	


	/**
	 * quantum circuit 
	 */

	class Circuit{
		private:
		public:
	};

}
