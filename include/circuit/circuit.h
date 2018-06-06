/**
 * @file circuit.h
 * @brief describing quantum circuits and quantum gates
 * @author Kohji Nishimura
 * @date 2018/06/03
 *
 */

#pragma once
#include <vector>
#include <array>
#include <string>
#include <exception>
#include <memory>
#include <type_traits>

namespace qlib{

	/** abbrebiation of std::shared_ptr
	 * @tparam T the type for shared_ptr
	 */
	template<class T>
		using s_ptr = std::shared_ptr<T>;

	/** abbrebiation of std::unique_ptr
	 * @tparam T the type for unique_ptr
	 */
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
		//TODO: register num?
	};

	/**
	 * classical register
	 */

	struct ClassicalRegister{
		//TODO: register num?
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
			/**
			 * protected constructor of AbstUnitary
			 * @param name identifier name
			 */
			AbstUnitary(const std::string& name)
				: Component(name){
				}
	};

	/**
	 * class for type_traits (all_same)
	 */

	template<typename... T>
		struct all_same : std::false_type { };

	/**
	 * class for type_traits (all_same)
	 */

	template<>
		struct all_same<> : std::true_type { };

	/**
	 * class for type_traits (all_same)
	 */

	template<typename T>
		struct all_same<T> : std::true_type { };

	/**
	 * class for type_traits (all_same)
	 */

	template<typename T, typename... Ts>
		struct all_same<T, T, Ts...> : all_same<T, Ts...> { };

	/**
	 * class for Unitary operator
	 * @tparam Ngate number of gate
	 */

	template<size_t Ngate>
		class UnitaryOp: public AbstUnitary{
			protected:
				//! array of pointer of QuantumRegister
				const std::array<QuantumRegister*, Ngate> p_qregs;
				//! number of gates
				const size_t num_gate = Ngate;

				/**
				 * protected constructor of UnitaryOp
				 * @param name identifier name
				 * @param args list of the points of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 */

				template<class... Args, typename = typename std::enable_if<all_same<Args...>::value, void>::type>
					UnitaryOp(const std::string& name, const Args&... args)
					: AbstUnitary(name), p_qregs({(&args)...}){
					}
		};

	/**
	 * class for Unitary operator with parameters
	 * @tparam Ngate number of gate
	 * @tparam Nparam number of parameters
	 * @tparam T the type of parameter (assumed to be double)
	 */

	template<size_t Ngate, size_t Nparam, typename T>
		class UnitaryOpParam: public UnitaryOp<Ngate>{
			protected:
				//! list of parameters
				const std::array<T, Nparam> params;

				/**
				 * protected constructor of UnitaryOpParam
				 * @param name identifier name
				 * @param args list of the points of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 */

				template<class... Args, typename = typename std::enable_if<all_same<Args...>::value, void>::type>
				UnitaryOpParam(const std::string& name, const std::array<T, Nparam>& params, const Args&... args)
				: UnitaryOp<Ngate>(name, args...), params(params){
				}
		};


		/**
		 * class for Unitary container
		 */
		class UnitaryContainer: public AbstUnitary{
			protected:
				//! list of unitaries this class holds
				std::vector<s_ptr<AbstUnitary>> unitaries;

				/**
				 * protected constructor of UnitaryContainer
				 * @param name identifier name
				 */

				UnitaryContainer(const std::string& name)
					:AbstUnitary(name){
					}

				/**
				 * protected constructor of UnitaryContainer
				 * @param name identifier name
				 * @param unitaries list of unitary operators
				 */

				UnitaryContainer(const std::string& name, const std::vector<s_ptr<AbstUnitary>> unitaries)
					:AbstUnitary(name), unitaries(unitaries){
					}
		};




	/**
	 * quantum circuit 
	 */

	class Circuit{
		private:
		public:
	};

}
