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
#include "../default_parameters.h"

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

	/**
	 * print exception message
	 * @param exception_name exception name
	 * @param message additional exception message
	 * @return decorated string (exception message)
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

	/***************  Utility structs  ***************/

	/**
	 * class for type_traits (all_same)
	 * @tparam T templeate parameter
	 */

	template<typename... T>
		struct all_same : std::false_type { };

	/**
	 * class for type_traits (all_same)
	 * @tparam T templeate parameter
	 */

	template<>
		struct all_same<> : std::true_type { };

	/**
	 * class for type_traits (all_same)
	 * @tparam T templeate parameter
	 */

	template<typename T>
		struct all_same<T> : std::true_type { };

	/**
	 * class for type_traits (all_same)
	 * @tparam T templeate parameter
	 * @tparam Ts templeate parameters
	 */

	template<typename T, typename... Ts>
		struct all_same<T, T, Ts...> : all_same<T, Ts...> { };

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


	/**************************************************/


	/**
	 * abstract Component placed in quantum circuit
	 */

	class Component{
		protected:
			//! an identifier
			const std::string name;
			//! component type
			const ComponentType type;
			/**
			 * protected constructor of Component
			 * @param name identifier name
			 * @param type component type
			 */
			Component(const std::string& name, const ComponentType type)
				:name(name), type(type){
				}

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
			 * constructor of Measure
			 * @param qreg QuntumRegister to be measured
			 * @param creg corresponding ClassicalRegister
			 */

			Measure(const QuantumRegister& qreg, const ClassicalRegister& creg)
				: Component(default_str[MEASURE], ComponentType::MEASUREMENT), p_qreg(&qreg), p_creg(&creg){
				}

		public:

			/**
			 * generate shared_ptr of Measure
			 * @param qreg QuntumRegister to be measured
			 * @param creg corresponding ClassicalRegister
			 * @return shared_ptr of Measure
			 */

			inline static s_ptr<Measure> create(const QuantumRegister& qreg, const ClassicalRegister& creg){
				struct impl: Measure{
					impl(const QuantumRegister& qreg, const ClassicalRegister& creg)
						: Measure(qreg, creg){}
				};

				return std::make_shared<impl>(qreg, creg);
			}

	};

	/**
	 * class for Unitary operator and Unitary container
	 */

	class AbstUnitary : public Component{
		protected:

			/**
			 * constructor of AbstUnitary
			 * @param name identifier name
			 * @param type component type
			 */
			AbstUnitary(const std::string& name, const ComponentType type)
				: Component(name, type){
				}

	};

	/**
	 * class for Unitary operator with parameters
	 * @tparam Nregister number of register 
	 * @tparam Nparam number of parameters
	 */

	template<size_t Nregister, size_t Nparam, typename = typename std::enable_if<Nregister != 0, void>::type>
		class UnitaryOp: public AbstUnitary{
			protected:
				//! array of pointer of QuantumRegister
				const std::array<const QuantumRegister*, Nregister> p_qregs;
				//! number of registers
				const size_t num_register = Nregister;
				//! list of parameters
				const std::array<double, Nparam> params;

				/**
				 * constructor of UnitaryOp
				 * @param name identifier name
				 * @param params double array of parameters
				 * @param args list of the pointers of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 */

				template<class... Args, typename = typename std::enable_if<all_same<Args...>::value, void>::type>
					UnitaryOp(const std::string& name, const std::array<double, Nparam>& params, const Args&... args)
					: AbstUnitary(name, ComponentType::UNITARY_OP_PARAM), p_qregs({&args...}), params(params){
					}

			public:

				/**
				 * generate shared_ptr of UnitaryOp
				 * @param name identifier name
				 * @param params double array of parameters
				 * @param args list of the points of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 * @return shared_ptr of UnitaryOp
				 */

				template<class... Args>
					inline static s_ptr<UnitaryOp<Nregister, Nparam>> create(const std::string& name, const std::array<double, Nparam>& params, const Args&... args){
						struct impl : UnitaryOp<Nregister, Nparam>{
							impl(const std::string& name, const std::array<double, Nparam>& params, const Args&... args) : UnitaryOp<Nregister, Nparam>(name, params, args...){}
						};
						return std::make_shared<impl>(name, params, args...);
					}

		};

	/**
	 * class for Unitary operator without parameters
	 * @tparam Nregister number of register 
	 */

	template<size_t Nregister>
		class UnitaryOp<Nregister, 0> : public AbstUnitary{
			protected:
				//! array of pointer of QuantumRegister
				const std::array<const QuantumRegister*, Nregister> p_qregs;
				//! number of registers
				const size_t num_register = Nregister;

				/**
				 * constructor of UnitaryOp
				 * @param name identifier name
				 * @param args list of the pointers of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 */

				template<class... Args, typename = typename std::enable_if<all_same<Args...>::value, void>::type>
					UnitaryOp(const std::string& name, const Args&... args)
					: AbstUnitary(name, ComponentType::UNITARY_OP), p_qregs({&args...}){
					}

			public:

				/**
				 * generate shared_ptr of UnitaryOp
				 * @param name identifier name
				 * @param args list of the points of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 * @return shared_ptr of UnitaryOp
				 */

				template<class... Args>
					inline static s_ptr<UnitaryOp<Nregister, 0>> create(const std::string& name, const Args&... args){
						struct impl : UnitaryOp<Nregister, 0>{
							impl(const std::string& name, const Args&... args) : UnitaryOp<Nregister, 0>(name, args...){}
						};
						return std::make_shared<impl>(name, args...);
					}

		};

	/**
	 * class for Unitary container
	 */
	class UnitaryContainer: public AbstUnitary{
		protected:
			//! list of unitaries this class holds
			const std::vector<s_ptr<AbstUnitary>> unitaries;

			/**
			 * constructor of UnitaryContainer
			 * @param name identifier name
			 * @param unitaries list of unitary operators
			 */

			UnitaryContainer(const std::string& name, const std::vector<s_ptr<AbstUnitary>>& unitaries)
				:AbstUnitary(name, ComponentType::UNITARY_CONTAINER), unitaries(unitaries){
				}

		public:

			/**
			 * generate shared_ptr of UnitaryContainer
			 * @param name identifier name
			 * @param unitaries list of unitary operators
			 * @return shared_ptr of UnitaryOp
			 */

			inline static s_ptr<UnitaryContainer> create(const std::string& name, const std::vector<s_ptr<AbstUnitary>>& unitaries){
				struct impl : UnitaryContainer{
					impl(const std::string& name, const std::vector<s_ptr<AbstUnitary>>& unitaries) : UnitaryContainer(name, unitaries){}
				};
				return std::make_shared<impl>(name, unitaries);
			}
	};


	/**
	 * components factory
	 */

	//TODO: w/ flyweight pattern?
	//TODO: how to apply rules for each unitary gates? (eg. X^2=I, CNOT*inv(CNOT)*CNOT=SWAP)

	class Op{

		public:

			/**
			 * getter for measurement operator
			 * @param qreg QuantumRegister class
			 * @param creg ClassicalRegister class
			 * @return shared_ptr of Measure class
			 */

			inline static s_ptr<Measure> Measurement(const QuantumRegister& qreg, const ClassicalRegister& creg){
				return Measure::create(qreg, creg);
			}

			/**
			 * getter for UnitaryOp operator without parameters
			 * @param name identifier name
			 * @param args arguments of QuantumRegister
			 * @tparam Args list of the types of arguments. Each type must be QuantumRegister
			 * @tparam Nregister the number of arguments
			 * @return shared_ptr of UnitaryOp class
			 */

			template<class... Args, size_t Nregister = sizeof...(Args), typename = typename std::enable_if<all_same<Args...>::value, void>::type>
				inline static s_ptr<UnitaryOp<Nregister, 0>> U(const std::string& name, const Args&... args){
					return UnitaryOp<Nregister, 0>::create(name, args...);
				}

			/**
			 * generate Id gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of Id operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> Id(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::Id], qreg);
			}

			/**
			 * generate X gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of X operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> X(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::X], qreg);
			}

			/**
			 * generate Y gate
			 * @param registers arguments of QuantumRegister
			 * @return shared_ptr of Y operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> Y(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::Y], qreg);
			}

			/**
			 * generate Z gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of Z operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> Z(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::Z], qreg);
			}

			/**
			 * generate H gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of H operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> H(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::H], qreg);
			}

			/**
			 * generate S gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of S operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> S(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::S], qreg);
			}

			/**
			 * generate T gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of T operator
			 */

			inline static s_ptr<UnitaryOp<1, 0>> T(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::T], qreg);
			}

			/**
			 * generate CNOT gate
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CNOT operator
			 */

			inline static s_ptr<UnitaryOp<2, 0>> CNOT(const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CNOT], control, target);
			}

			/**
			 * generate SWAP gate
			 * @param qreg1 qubit 1
			 * @param qreg2 qubit 2
			 * @return shared_ptr of SWAP operator
			 */

			inline static s_ptr<UnitaryOp<2, 0>> SWAP(const QuantumRegister& qreg1, const QuantumRegister& qreg2){
				return Op::U(default_str[DefaultString::SWAP], qreg1, qreg2);
			}

			/**
			 * generate CZ gate
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CZ operator
			 */

			inline static s_ptr<UnitaryOp<2, 0>> CZ(const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CZ], control, target);
			}

			/**
			 * generate CS gate
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CS operator
			 */

			inline static s_ptr<UnitaryOp<2, 0>> CS(const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CS], control, target);
			}

			/**
			 * generate TOFFOLI gate
			 * @param control1 control qubit 1
			 * @param control2 control qubit 2
			 * @param target target qubit
			 * @return shared_ptr of TOFFOLI operator
			 */

			inline static s_ptr<UnitaryOp<3, 0>> TOFFOLI(const QuantumRegister& control1, const QuantumRegister& control2, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::TOFFOLI], control1, control2, target);
			}

			/**
			 * generate FREDKIN gate
			 * @param control control qubit
			 * @param target1 target qubit 1
			 * @param target2 target qubit 2
			 * @return shared_ptr of FREDKIN operator
			 */

			inline static s_ptr<UnitaryOp<3, 0>> FREDKIN(const QuantumRegister& control, const QuantumRegister& target1, const QuantumRegister& target2){
				return Op::U(default_str[DefaultString::FREDKIN], control, target1, target2);
			}


			/**
			 * getter for UnitaryOp operator with parameters
			 * @param name identifier name
			 * @param params list of parameters
			 * @param args arguments of QuantumRegister*
			 * @tparam Args list of the types of arguments. Each type must be QuantumRegister
			 * @tparam Nparam the number of parameters
			 * @tparam T the type of parameters
			 * @return shared_ptr of UnitaryOp class
			 */

			template<class... Args, size_t Nparam, typename = typename std::enable_if<Nparam != 0 && all_same<Args...>::value, void>::type>
				inline static s_ptr<UnitaryOp<sizeof...(Args), Nparam>> U(const std::string& name, const std::array<double, Nparam>& params, const Args&... args){
					return UnitaryOp<sizeof...(Args), Nparam>::create(name, params, args...);
				}

			/**
			 * generate R (phase-shift) gate with parameter
			 * @param phi phase parameter
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of R operator
			 */

			inline static s_ptr<UnitaryOp<1, 1>> R(double phi, const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::R], std::array<double,1>{phi}, qreg);
			}

			/**
			 * getter for UnitaryContainer operator
			 * @param name identifier name
			 * @param unitaries list of AbstUnitary operator
			 * @return shared_ptr of UnitaryContainer class
			 */

			inline static s_ptr<UnitaryContainer> Us(const std::string& name, const std::vector<s_ptr<AbstUnitary>>& unitaries){
				return UnitaryContainer::create(name, unitaries);
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
