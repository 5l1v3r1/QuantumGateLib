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

	using NodeInfo = std::pair<const std::string, const QuantumRegister*>;

	struct CurrentOp{
		//!component name
		std::string name;
		//!component type
		ComponentType type;
		//!component of node_name and the correspondingquantum registers
		std::vector<NodeInfo> p_info;
		//!component parameters
		std::vector<double> parameters;

		/**
		 * constructor of CurrentOp
		 * @param name component name
		 * @param type component type
		 * @param p_info component of node_name and the correspondingquantum registers
		 * @param parameters component parameters
		 */

		CurrentOp(const std::string& name, ComponentType type, const std::vector<NodeInfo>& p_info, const std::vector<double> parameters)
			: name(name), type(type), p_info(p_info), parameters(parameters)
		{}


	};

	/**************************************************/

	//!forward declaration
	class Component;

	/**
	 * struct for Node of Component
	 */

	struct Node{
		//! an identifier
		std::string name;
		//! in(left) node
		Node* in = nullptr;
		//! out(right) node
		Node* out = nullptr;
		//! parent
		Component* parent = nullptr;
		//! corresponding	quantum register
		const QuantumRegister* q_reg = nullptr;

		/**
		 * constructor of Node
		 */

		Node()
			: name(""), in(nullptr), out(nullptr), parent(nullptr), q_reg(nullptr)
		{}

		/**
		 * constructor of Node
		 * @param name the name of the node
		 * @param parent the parent of the node
		 */

		Node(const std::string& name, Component* parent)
			: name(name), in(nullptr), out(nullptr), parent(parent), q_reg(nullptr)
		{}

		/**
		 * copy constructor of Node
		 */

		Node(const Node& obj)
			: name(obj.name), in(obj.in), out(obj.out), parent(obj.parent), q_reg(obj.q_reg)
		{}

		Node& operator=(const Node& obj){
			this->name = obj.name;
			this->in = obj.in;
			this->out = obj.out;
			this->parent = obj.parent;
			this->q_reg = obj.q_reg;
			return *this;
		}
	};

	/**
	 * struct for representing Component
	 */

	class Component{
		public:

			/**
			 * enum for distinguishing IN and OUT
			 */

			enum ConnectDir{
				IN,
				OUT,
			};

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
			Component(const std::string& name, ComponentType type, Component* parent = nullptr)
				:name(name), type(type), parent(parent){
				}

			/**
			 * destructor of Component
			 */
			virtual ~Component(){}

			/**
			 * callback function which is called when detecting the node to be connected.
			 * This function can be overridden.
			 * @param node_name the node name to be connectedd
			 * @param node node to be added
			 * @param direction direction IN or OUT of the node
			 */

			virtual void OnConnected(const std::string& node_name, Node& node, ConnectDir direction){
				if(direction == ConnectDir::IN){
					//set "in" of the node
					this->nodes.at(node_name).in = &node;
				}
				else{
					//set "out" of the node
					this->nodes.at(node_name).out = &node;
				}
			}

		public:

			/**
			 * connect the (IN|OUT) socket to the node <br/>
			 * <br/>
			 * (NodeToBeConnected) -- OUT IN -- (Node) -- OUT IN -- (NpdeToBeConnected) <br/>
			 * @param node_name the name of the node 
			 * @param target target component to be connected
			 * @param target_node_name the name of the node to be connected
			 * @param direction direction IN or OUT of the node
			 */

			void connectNode(const std::string& node_name, Component& target, const std::string& target_node_name, ConnectDir direction){
				if(direction == ConnectDir::IN){
					//callback (ask the target if the connection can be accepted)
					target.OnConnected(target_node_name, this->nodes.at(node_name), ConnectDir::OUT);
					// if the callback succeeds...
					// set "in" of the node
					this->nodes.at(node_name).in = &(target.nodes.at(target_node_name));
					// set q_reg
					this->nodes.at(node_name).q_reg = target.nodes.at(target_node_name).q_reg;
				}
				else{
					//callback (ask the target if the connection can be accepted)
					target.OnConnected(target_node_name, this->nodes.at(node_name), ConnectDir::IN);
					// if the callback succeeds...
					// set "out" of the node
					this->nodes.at(node_name).out = &(target.nodes.at(target_node_name));
					// set q_reg (not needed when connecting OUT node)
					//this->nodes.at(node_name).q_reg = target.nodes.at(target_node_name).q_reg;
				}
			}

			/**
			 * remove the node
			 * @param node_name the name of the node 
			 * @param direction direction IN or OUT of the node
			 */

			void removeNode(const std::string& node_name, ConnectDir direction){
				if(direction == ConnectDir::IN){
					this->nodes.at(node_name).in = nullptr;
				}
				else{
					this->nodes.at(node_name).out = nullptr;
				}
			}

			/**
			 * get the information of the adjacent node
			 * @param node_name the name of the node 
			 * @param direction the direction (IN|OUT)
			 * @return the derived node
			 */

			const Node& getNode(const std::string& node_name, ConnectDir direction) const{
				if(direction == ConnectDir::IN){
					return *(this->nodes.at(node_name).in);
				}
				else{
					return *(this->nodes.at(node_name).out);
				}
			}

			/**
			 * get the subComponent(children) by name
			 * this function throws InvalidOperationException if not defined.
			 * @param component_name the name of subcomponent
			 * @return the derived component
			 */

			virtual Component& getSubComponent(const std::string& /*component_name*/){
				throw InvalidOperationException("this function is not defined.");
			}


			/**
			 * get the list of CurrentOp object
			 * @return the std::vector of CurrentOp object
			 */

			virtual const std::vector<CurrentOp> getCurrentOps() const noexcept{
				//create NodeInfo vector
				auto ret = 
					std::vector<CurrentOp>({{
							this->name,
							this->type,
							std::vector<NodeInfo>(),
							std::vector<double>(),
							}});

				std::vector<NodeInfo>& nodeinfos = ret[0].p_info;

				for(auto&& elem : this->nodes){
				}
			}

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

	};

	/**
	 * class for representing Measurement
	 * nodename: measure
	 */

	class Measure : public Component{
		private:

			/**
			 * nodename ("measure")
			 */

			const std::string measure;

			/**
			 * constructor of Measure with previous node
			 * @param prev_node previous node
			 */

			Measure()
				: Component(default_str[MEASURE], ComponentType::MEASUREMENT), measure("measure"){
					//add node "measure"
					this->nodes[measure] = Node(measure, this);
				}

			/**
			 * overidden function of OnConnected
			 * @param node_name the node name to be connected
			 * @param node node to be added
			 * @param direction direction IN or OUT of the node. if direction == OUT, this function throws InvalidOperationException.
			 */

			void OnConnected(const std::string& node_name, Node& node, ConnectDir direction) override{
				if(direction == ConnectDir::OUT){
					// cannot be connected
					throw InvalidOperationException("cannot connect Node after Measure component");
				}

				Component::OnConnected(node_name, node, direction);
			}

		public:

			/**
			 * generate shared_ptr of Measure
			 * @return shared_ptr of Measure
			 */

			inline static u_ptr<Measure> create(){
				struct impl: Measure{
					impl() : Measure(){}
				};
				return u_ptr<impl>(new impl());
			}

	};

	/**
	 * class for Unitary operator
	 * nodename : [any]
	 */

	class UnitaryOp : public Component{
		private:

			/**
			 * constructor of UnitaryOp
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, CNOT-gate...)
			 * @param nodes a set of strings of node
			 */

			template<class... Args, typename=typename std::enable_if<all_same<Args...>::value && std::is_same<typename first_type_remove_cvref<Args...>::type, std::string>::value, void>::type>
				UnitaryOp(const std::string& gate_name, const Args&... nodes)
				: Component(gate_name, ComponentType::UNITARY_OP){
					std::array<std::string, sizeof...(Args)> node_array= {nodes...};
					for(auto&& elem : node_array){
						this->nodes[elem] = Node(elem, this);
					}
				}
		public:

			/**
			 * generate shared_ptr of UnitaryOp 
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, CNOT-gate...)
			 * @param nodes a set of strings of node
			 * @return shared_ptr of UnitaryOp
			 */

			template<class... Args>
			inline static u_ptr<UnitaryOp> create(const std::string& gate_name, Args&&... nodes){
				struct impl: Measure{
					impl(const std::string& gate_name, Args&&... nodes) : UnitaryOp(gate_name, std::forward<Args...>(nodes...)){}
				};
				return u_ptr<impl>(new impl(gate_name, std::forward<Args...>(nodes...)));
			}

	};

	/**
	 * class for Unitary operator with parameters 
	 * nodename : [any]
	 */

	class UnitaryOpParam : public UnitaryOp{
		private:
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


