/**
 * @file circuit.h
 * @brief describing quantum circuits and quantum gates
 * @author Kohji Nishimura
 * @date 2018/06/03
 *
 */

#pragma once
#include <list>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <exception>
#include <memory>
#include <type_traits>
#include <inttypes.h>
#include "../default_parameters.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <tuple>
#include <functional>
#include <stack>
#include <iterator>

namespace qlib{

	/** abbrebiation of std::unique_ptr
	 * @tparam T the type for unique_ptr
	 */
	template<class T>
		using _ptr = std::unique_ptr<T>;

	/**
	 * print exception message
	 * @param exception_name exception name
	 * @param message additional exception message
	 * @return decorated string (exception message)
	 */
	inline const std::string exception_format(const std::string& exception_name, const std::string& message){
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
	 * class for type_traits (first_type (remove_cvref))
	 * @tparam T templeate parameter
	 * @tparam Ts templeate parameters
	 */

	template<typename T, typename... Ts>
		struct first_type_remove_cvref { using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;};

	/**
	 * check if two double variables are nealy the same
	 * @param a double value
	 * @param b double value
	 * @return check if a and b are nealy the same
	 */
	inline bool near_same(double a, double b){
		return std::abs(a-b) < std::numeric_limits<double>::epsilon();
	}

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
	 * dummy quantum register
	 */

	class DummyQuantumRegister{
		private:
			//! list of dummy quantum register
			std::vector<QuantumRegister> dummy_register;

			/**
			 * constructor of dummy quantum register
			 * @param pointer the value to be set to dummy_pointer
			 */

		public:
			DummyQuantumRegister(){}

			/**
			 * return object itself with dummy_pointer changed to index value
			 * @param index index value
			 * @return object itself with dummy_pointer changed to index value
			 */
			const QuantumRegister& operator[](size_t index){
				if(index >= dummy_register.size()){
					//resize dummy_register
					dummy_register.resize(index+1);
				}
				return dummy_register[index];
			}
	};

	/**
	 * dummy classical register
	 */

	struct DummyClassicalRegister{
		private:
			//! list of dummy classical register
			std::vector<ClassicalRegister> dummy_register;

			/**
			 * constructor of dummy classical register
			 * @param pointer the value to be set to dummy_pointer
			 */

		public:
			DummyClassicalRegister(){}

			/**
			 * return object itself with dummy_pointer changed to index value
			 * @param index index value
			 * @return object itself with dummy_pointer changed to index value
			 */
			const ClassicalRegister& operator[](size_t index){
				if(index >= dummy_register.size()){
					//resize dummy_register
					dummy_register.resize(index+1);
				}
				return dummy_register[index];
			}
	};

	//! dummy qubit
	extern DummyQuantumRegister null_qreg;
	//! dummy classical bit
	extern DummyClassicalRegister null_creg;

	/**
	 * struct for representing current operation in quantum circuit
	 */

	struct CurrentOp{
		//!component name
		std::string name;
		//!component type
		ComponentType type;
		//!component quantum registers
		std::vector<const QuantumRegister*> p_qreg;
		//!component classical registers
		std::vector<const ClassicalRegister*> p_creg;
		//!component parameters
		std::vector<double> parameters;
	};

	/**************************************************/

	//! forward declaration
	class Component;
	//! forward declaration
	class AbstUnitary;
	//! forward declaration
	struct CurrentOp;

	//TODO: implement Rules (constraints)
	//class Rule;


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

			/**
			 * destructor of Component
			 */
			virtual ~Component(){}

		public:
			/**
			 * get identifier
			 * @return identifier name
			 */

			const std::string getName() const noexcept{
				return name;
			}

			/**
			 * get component type
			 * @return component type
			 */

			ComponentType getType() const noexcept{
				return type;
			}

			/**
			 * get the list of CurrentOp object
			 * @return the std::vector of CurrentOp object
			 */

			virtual const std::vector<CurrentOp> getCurrOp() const noexcept = 0;

	};



	/**
	 * class for Measurement operation
	 */

	class Measure: public Component{

		private:

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

			inline static _ptr<Measure> create(const QuantumRegister& qreg, const ClassicalRegister& creg){
				struct impl: Measure{
					impl(const QuantumRegister& qreg, const ClassicalRegister& creg)
						: Measure(qreg, creg){}
				};

				return std::make_shared<impl>(qreg, creg);
			}

			/**
			 * get quantum register
			 * @return QuantumRegister* variable
			 */

			const QuantumRegister* getQuantumRegister() const noexcept{
				return p_qreg;
			}

			/**
			 * get classical register
			 * @return QuantumRegister* variable
			 */

			const ClassicalRegister* getClassicalRegister() const noexcept{
				return p_creg;
			}

			/**
			 * get the list of CurrentOp object
			 * @return the std::vector of CurrentOp object
			 */

			virtual const std::vector<CurrentOp> getCurrOp() const noexcept override{
				return std::vector<CurrentOp>({{
						this->name,
						this->type,
						std::vector<const QuantumRegister*>({p_qreg}),
						std::vector<const ClassicalRegister*>({p_creg}),
						std::vector<double>(),
						}});
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

			/**
			 * destructor of AbstUnitary
			 */

			virtual ~AbstUnitary(){}

		public:

			/**
			 * get the list of parameters
			 * @return the list of parameters
			 */

			virtual const std::vector<double> getParams() const noexcept{
				return std::vector<double>();
			}

			/**
			 * get the list of registers
			 * @return the list of registers
			 */

			virtual const std::vector<const QuantumRegister*> getRegisters() const noexcept = 0;

			/**
			 * return the inverse operator (dagger operator) of this unitary
			 * @return the inverse operator (dagger operator) of this unitary
			 */

			//TODO: implement dagger operation
			//_ptr<AbstUnitary> dagger() const{
			//}
	};

	/**
	 * class for Unitary operator with parameters
	 * @tparam Nregister number of register 
	 * @tparam Nparam number of parameters
	 */

	template<size_t Nregister, size_t Nparam, typename = typename std::enable_if<Nregister != 0, void>::type>
		class UnitaryOp: public AbstUnitary{
			private:
				//! array of pointer of QuantumRegister
				const std::array<const QuantumRegister*, Nregister> p_qregs;
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
						std::vector<const QuantumRegister*> temp_vec(p_qregs.begin(), p_qregs.end());
						// check if temp_vec has duplicate elements
						std::sort(temp_vec.begin(), temp_vec.end());
						auto last = std::unique(temp_vec.begin(), temp_vec.end());
						if(last != temp_vec.end())
							throw InvalidOperationException("duplicate elements in QuantumRegister* list");
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
					inline static _ptr<UnitaryOp<Nregister, Nparam>> create(const std::string& name, const std::array<double, Nparam>& params, Args&&... args){
						struct impl : UnitaryOp<Nregister, Nparam>{
							impl(const std::string& name, const std::array<double, Nparam>& params, Args&&... args) : UnitaryOp<Nregister, Nparam>(name, params, std::forward<Args>(args)...){}
						};
						return std::make_shared<impl>(name, params, std::forward<Args>(args)...);
					}

				/**
				 * get the list of parameters
				 * @return the list of parameters
				 */

				virtual const std::vector<double> getParams() const noexcept override{
					return std::vector<double>(params.begin(), params.end());
				}

				/**
				 * get the list of registers
				 * @return the list of registers
				 */

				virtual const std::vector<const QuantumRegister*> getRegisters() const noexcept override{
					return std::vector<const QuantumRegister*>(p_qregs.begin(), p_qregs.end());
				}

				/**
				 * get the list of CurrentOp object
				 * @return the std::vector of CurrentOp object
				 */

				virtual const std::vector<CurrentOp> getCurrOp() const noexcept override{
					return std::vector<CurrentOp>({{
							this->name,
							this->type,
							std::vector<const QuantumRegister*>(p_qregs.begin(), p_qregs.end()),
							std::vector<const ClassicalRegister*>(),
							std::vector<double>(params.begin(), params.end()),
							}});
				}
		};

	/**
	 * class for Unitary operator without parameters
	 * @tparam Nregister number of register 
	 */

	template<size_t Nregister>
		class UnitaryOp<Nregister, 0> : public AbstUnitary{
			private:
				//! array of pointer of QuantumRegister
				const std::array<const QuantumRegister*, Nregister> p_qregs;
				//! number of registers
				const size_t num_register = Nregister;

				/**
				 * constructor of UnitaryOp <br/>
				 * if the function detects QuantumRegisters with the same pointer, this function throws InvalidOperationException.
				 * @param name identifier name
				 * @param args list of the pointers of QuantumRegister
				 * @tparam Args list of the types of args. Each type must be QuantumRegister
				 */

				template<class... Args, typename = typename std::enable_if<all_same<Args...>::value, void>::type>
					UnitaryOp(const std::string& name, const Args&... args) noexcept(false)
					: AbstUnitary(name, ComponentType::UNITARY_OP), p_qregs({&args...}){
						std::vector<const QuantumRegister*> temp_vec(p_qregs.begin(), p_qregs.end());
						// check if temp_vec has duplicate elements
						std::sort(temp_vec.begin(), temp_vec.end());
						auto last = std::unique(temp_vec.begin(), temp_vec.end());
						if(last != temp_vec.end())
							throw InvalidOperationException("duplicate elements in QuantumRegister* list");
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
					inline static _ptr<UnitaryOp<Nregister, 0>> create(const std::string& name, Args&&... args){
						struct impl : UnitaryOp<Nregister, 0>{
							impl(const std::string& name, Args&&... args) : UnitaryOp<Nregister, 0>(name, std::forward<Args>(args)...){}
						};
						return std::make_shared<impl>(name, std::forward<Args>(args)...);
					}

				/**
				 * get the list of registers
				 * @return the list of registers
				 */

				virtual const std::vector<const QuantumRegister*> getRegisters() const noexcept override{
					return std::vector<const QuantumRegister*>(p_qregs.begin(), p_qregs.end());
				}

				/**
				 * get the list of CurrentOp object
				 * @return the std::vector of CurrentOp object
				 */

				virtual const std::vector<CurrentOp> getCurrOp() const noexcept override{
					return std::vector<CurrentOp>({{
							this->name,
							this->type,
							std::vector<const QuantumRegister*>(p_qregs.begin(), p_qregs.end()),
							std::vector<const ClassicalRegister*>(),
							std::vector<double>(),
							}});
				}
		};

	/**
	 * class for Unitary container
	 */
	class UnitaryContainer: public AbstUnitary{
		private:
			//! list of unitaries this class holds
			const std::list<_ptr<AbstUnitary>> unitaries;

			/**
			 * constructor of UnitaryContainer
			 * @param name identifier name
			 * @param unitaries list of unitary operators
			 */

			UnitaryContainer(const std::string& name, const std::list<_ptr<AbstUnitary>>& unitaries)
				:AbstUnitary(name, ComponentType::UNITARY_CONTAINER), unitaries(unitaries){
				}

		public:

			/**
			 * generate shared_ptr of UnitaryContainer
			 * @param name identifier name
			 * @param unitaries list of unitary operators
			 * @return shared_ptr of UnitaryOp
			 */

			inline static _ptr<UnitaryContainer> create(const std::string& name, const std::list<_ptr<AbstUnitary>>& unitaries){
				struct impl : UnitaryContainer{
					impl(const std::string& name, const std::list<_ptr<AbstUnitary>>& unitaries) : UnitaryContainer(name, unitaries){}
				};
				return std::make_shared<impl>(name, unitaries);
			}

			/**
			 * get the list of registers <br/>
			 * returned list is sorted by std::sort algorithm.
			 * @return the list of registers
			 */

			virtual const std::vector<const QuantumRegister*> getRegisters() const noexcept override{
				// concat all registers unitaries hold
				std::vector<const QuantumRegister*> ret_vec;
				for(auto&& elem : unitaries){
					const std::vector<const QuantumRegister*>& vec = elem->getRegisters();
					ret_vec.insert(ret_vec.end(), vec.begin(), vec.end());
				}
				//eliminate duplicate elements
				std::sort(ret_vec.begin(), ret_vec.end());
				auto last = std::unique(ret_vec.begin(), ret_vec.end());
				ret_vec.erase(last, ret_vec.end());
				return ret_vec;
			}

			/**
			 * get the list of Unitary gates
			 */

			const std::list<_ptr<AbstUnitary>> getUnitaries() const noexcept{
				return unitaries;
			}

			/**
			 * get the list of CurrentOp object
			 * @return the std::vector of CurrentOp object
			 */

			virtual const std::vector<CurrentOp> getCurrOp() const noexcept override{
				//concat all elements
				std::vector<CurrentOp> ret_vec;
				for(auto&& elem : unitaries){
					const std::vector<CurrentOp>& vec = elem->getCurrOp();
					ret_vec.insert(ret_vec.end(), vec.begin(), vec.end());
				}
				return ret_vec;
			}
	};


	/**
	 * components factory
	 */

	class Op{

		public:

			/**
			 * getter for measurement operator
			 * @param qreg QuantumRegister class
			 * @param creg ClassicalRegister class
			 * @return shared_ptr of Measure class
			 */

			inline static _ptr<Measure> Measurement(const QuantumRegister& qreg, const ClassicalRegister& creg){
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
				inline static _ptr<UnitaryOp<Nregister, 0>> U(const std::string& name, const Args&... args){
					return UnitaryOp<Nregister, 0>::create(name, args...);
				}

			/**
			 * generate Id gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of Id operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> Id(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::Id], qreg);
			}

			/**
			 * generate X gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of X operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> X(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::X], qreg);
			}

			/**
			 * generate Y gate
			 * @param registers arguments of QuantumRegister
			 * @return shared_ptr of Y operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> Y(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::Y], qreg);
			}

			/**
			 * generate Z gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of Z operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> Z(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::Z], qreg);
			}

			/**
			 * generate H (hadamard) gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of H operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> H(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::H], qreg);
			}

			/**
			 * generate S (phase) gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of S operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> S(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::S], qreg);
			}

			/**
			 * generate T (pi/8) gate
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of T operator
			 */

			inline static _ptr<UnitaryOp<1, 0>> T(const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::T], qreg);
			}

			/**
			 * generate CNOT gate
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CNOT operator
			 */

			inline static _ptr<UnitaryOp<2, 0>> CNOT(const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CNOT], control, target);
			}

			/**
			 * generate SWAP gate
			 * @param qreg1 qubit 1
			 * @param qreg2 qubit 2
			 * @return shared_ptr of SWAP operator
			 */

			inline static _ptr<UnitaryOp<2, 0>> SWAP(const QuantumRegister& qreg1, const QuantumRegister& qreg2){
				return Op::U(default_str[DefaultString::SWAP], qreg1, qreg2);
			}

			/**
			 * generate CZ gate
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CZ operator
			 */

			inline static _ptr<UnitaryOp<2, 0>> CZ(const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CZ], control, target);
			}

			/**
			 * generate CS gate
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CS operator
			 */

			inline static _ptr<UnitaryOp<2, 0>> CS(const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CS], control, target);
			}

			/**
			 * generate TOFFOLI (CCNOT) gate
			 * @param control1 control qubit 1
			 * @param control2 control qubit 2
			 * @param target target qubit
			 * @return shared_ptr of TOFFOLI operator
			 */

			inline static _ptr<UnitaryOp<3, 0>> TOFFOLI(const QuantumRegister& control1, const QuantumRegister& control2, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::TOFFOLI], control1, control2, target);
			}

			/**
			 * generate FREDKIN (CSWAP)  gate
			 * @param control control qubit
			 * @param target1 target qubit 1
			 * @param target2 target qubit 2
			 * @return shared_ptr of FREDKIN operator
			 */

			inline static _ptr<UnitaryOp<3, 0>> FREDKIN(const QuantumRegister& control, const QuantumRegister& target1, const QuantumRegister& target2){
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
				inline static _ptr<UnitaryOp<sizeof...(Args), Nparam>> U(const std::string& name, const std::array<double, Nparam>& params, const Args&... args){
					return UnitaryOp<sizeof...(Args), Nparam>::create(name, params, args...);
				}

			/**
			 * generate R (phase-shift) gate with parameter
			 * @param phi phase parameter
			 * @param qreg arguments of QuantumRegister
			 * @return shared_ptr of R operator
			 */

			inline static _ptr<UnitaryOp<1, 1>> R(double phi, const QuantumRegister& qreg){
				return Op::U(default_str[DefaultString::R], std::array<double,1>{phi}, qreg);
			}

			/**
			 * generate CR (controlled phase-shift) gate
			 * @param phi phase parameter
			 * @param control control qubit
			 * @param target target qubit
			 * @return shared_ptr of CR operator
			 */

			inline static _ptr<UnitaryOp<2, 1>> CR(double phi, const QuantumRegister& control, const QuantumRegister& target){
				return Op::U(default_str[DefaultString::CR], std::array<double,1>{phi},  control, target);
			}

			/**
			 * getter for UnitaryContainer operator
			 * @param name identifier name
			 * @param unitaries list of AbstUnitary operator
			 * @return shared_ptr of UnitaryContainer class
			 */

			inline static _ptr<UnitaryContainer> Us(const std::string& name, const std::list<_ptr<AbstUnitary>>& unitaries){
				return UnitaryContainer::create(name, unitaries);
			}


	};



	/**
	 * quantum circuit 
	 */

	class Circuit{
		private:
			//! list of components placed in quantum circuit
			std::list<_ptr<Component>> components;
			//! quantum register
			std::vector<QuantumRegister> qreg;
			//! classical register
			std::vector<ClassicalRegister> creg;
		public:
			/**
			 * constructor of Circuit
			 * @param num_register number of (quantum and classical) register
			 */
			Circuit(size_t num_register)
				:qreg(num_register), creg(num_register){
				}

			/**
			 * constructor of Circuit
			 * @param num_qreg number of quantum register
			 * @param num_creg number of classical register
			 */
			Circuit(size_t num_qreg, size_t num_creg)
				:qreg(num_qreg), creg(num_creg){
				}

			/**
			 * return the reference of quantum register
			 * @return the reference of quantum register
			 */

			inline const std::vector<QuantumRegister>& getQuantumRegisters(){
				return this->qreg;
			}

			/**
			 * return the reference of classical register
			 * @return the reference of classical register
			 */

			inline const std::vector<ClassicalRegister>& getClassicalRegisters(){
				return this->creg;
			}

			/**
			 * add components to quantum circuit
			 * @param component shard_ptr of Component
			 * @return the reference of Circuit itself
			 */

			inline Circuit& addComponent(_ptr<Component> component){
				components.push_back(component);
				return *this;
			}

			/**
			 * build current operation list registered in container of Circuit
			 * @return std::vector of CurrentOp struct
			 */
			inline const std::vector<CurrentOp> buildCurrOpList(){
				std::vector<CurrentOp> ret_vec;
				for(auto&& elem : components){
					const std::vector<CurrentOp>& vec = elem->getCurrOp();
					ret_vec.insert(ret_vec.end(), vec.begin(), vec.end());
				}

				return ret_vec;
			}


			//TODO: implement circuit optimization
			//TODO: implement expand function (expand all UnitaryContainer)
			//TODO: other components? (for? if? (quantum for?)(quantum if?))
	};


}

/**
 * add component to quantum circuit (with operator<<)
 * @param component shard_ptr of Component
 * @return the reference of Circuit itself
 */

inline qlib::Circuit& operator<<(qlib::Circuit& circuit, qlib::_ptr<qlib::Component> component){
	return circuit.addComponent(component);
}


