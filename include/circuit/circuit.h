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
#include <inttypes.h>
#include <iostream>
#include <cmath>
#include <tuple>
#include <functional>
#include <iterator>
#include <map>

#include "../default_parameters.h"
#include "utility.h"

namespace qlib{

	/**
	 * quantum register
	 */

	struct QuantumRegister{
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

	//! dummy qubit
	extern DummyQuantumRegister null_qreg;

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
		//!component parameters
		std::vector<double> parameters;
	};

	/**************************************************/

	//!forward declaration
	class Component;

	/**
	 * struct for Node of Component
	 */

	struct Node{
		//! an identifier
		const std::string name;
		//! in(left) node
		Node* in;
		//! out(right) node
		Node* out;
		//! parent
		Component* parent;
	};

	class Component{
		protected:
			//! an identifier
			const std::string name;
			//! component type
			const ComponentType type;
			//! its parent
			Component* parent;
			//! list of Nodes
			std::map<std::string, Node> nodes;
			/**
			 * constructor of Component
			 * @param name identifier name
			 * @param type component type
			 */
			Component(const std::string& name, const ComponentType type, const Component* parent = nullptr)
				:name(name), type(type), parent(parent){
				}

			/**
			 * get the list of CurrentOp object
			 * @return the std::vector of CurrentOp object
			 */

			virtual const std::vector<CurrentOp> getCurrOp() const noexcept = 0;
	};


	/**
	 * quantum circuit 
	 */

	class Circuit{
	};

}

/**
 * add component to quantum circuit (with operator<<)
 * @param component shard_ptr of Component
 * @return the reference of Circuit itself
 */

//inline qlib::Circuit& operator<<(qlib::Circuit& circuit, qlib::_ptr<qlib::Component> component){
//	return circuit.addComponent(component);
//}


