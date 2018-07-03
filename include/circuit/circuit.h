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
		Node* in;
		//! out(right) node
		Node* out;
		//! parent
		Component* parent;
		//! corresponding	quantum register
		const QuantumRegister* q_reg;

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

		Node(const std::string& name, Component* parent = nullptr)
			: name(name), in(nullptr), out(nullptr), parent(parent), q_reg(nullptr)
		{}

		/**
		 * copy constructor of Node
		 * @param obj Node object
		 */

		Node(const Node& obj)
			: name(obj.name), in(obj.in), out(obj.out), parent(obj.parent), q_reg(obj.q_reg)
		{}

		/**
		 * operator overloading (=)
		 * @param obj Node object
		 * @return the reference of Node itself
		 */

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
	 * operator overloading (==) of Node
	 * @param lhs left hand side
	 * @param rhs right hand side
	 * @return bool variable. if lhs==rhs -> true else false
	 */

	inline bool operator==(const Node& lhs, const Node& rhs){
		if(
				lhs.name   == rhs.name   &&
				lhs.in     == rhs.in     &&
				lhs.out    == rhs.out    &&
				lhs.parent == rhs.parent &&
				lhs.q_reg  == rhs.q_reg){
			return true;
		}
		else{
			return false;
		}
	}

	/**
	 * operator overloading (!=) of Node
	 * @param lhs left hand side
	 * @param rhs right hand side
	 * @return bool variable. if lhs!=rhs -> true else false
	 */

	inline bool operator!=(const Node& lhs, const Node& rhs){
		return !(lhs==rhs);
	}

	extern Node null_node;

	/**
	 * struct for representing Component
	 */

	class Component{
		public:

			/**
			 * enum for node IN and OUT
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
			Component* const parent;
			//! list of Nodes
			std::map<std::string, Node> nodes;
			/**
			 * constructor of Component
			 * @param name identifier name
			 * @param type component type
			 * @param parent the pointer to the parent
			 */
			Component(const std::string& name, ComponentType type, Component* parent = nullptr)
				:name(name), type(type), parent(parent){
				}

			/**
			 * destructor of Component
			 */
			virtual ~Component(){}

			/**
			 * get Node function
			 * @param node_name the node name
			 * @return corresponding node
			 */

			inline Node& getNode(const std::string& node_name){
				return this->nodes.at(node_name);
			}


			/**
			 * callback function which is called when detecting the node to be connected.
			 * This function can be overridden.
			 * @param node_name the node name to be connected
			 * @param added_node added node
			 * @param direction direction IN or OUT of the node
			 */

			virtual void OnConnect(const std::string& node_name, Node& added_node, ConnectDir direction){
				if(direction == ConnectDir::IN){
					//set "in" of the node
					this->getNode(node_name).in = &added_node;
				}
				else{
					//set "out" of the node
					this->getNode(node_name).out = &added_node;
				}
			}

			/**
			 * callback function which is called when catching the remove signal
			 * This function can be overridden.
			 * @param node_name the node name to be removed
			 * @param direction direction IN or OUT of the node
			 */

			virtual void OnRemove(const std::string& node_name, ConnectDir direction){
				if(direction == ConnectDir::IN){
					this->getNode(node_name).in = nullptr;
				}
				else{
					this->getNode(node_name).out = nullptr;
				}
			}

		public:

			/**
			 * connect the (IN|OUT) socket to the node <br/>
			 * <br/>
			 * (NodeToBeConnected) -- OUT IN -- (Node) -- OUT IN -- (NpdeToBeConnected) <br/>
			 * if the node is already connected, return false. else return true.
			 * this function may throw InvalidOperationException.
			 * @param node_name the name of the node 
			 * @param target target component to be connected
			 * @param target_node_name the name of the node to be connected
			 * @param direction direction IN or OUT of the node
			 * @return bool variable which shows if the function succeeds
			 */

			bool connectNode(const std::string& node_name, Component& target, const std::string& target_node_name, ConnectDir direction){
				if(direction == ConnectDir::IN){
					if(this->getNode(node_name).in != nullptr){
						// already connected
						return false;
					}

					//callback (ask the target if the connection can be accepted)
					target.OnConnect(target_node_name, this->getNode(node_name), ConnectDir::OUT);
					// if the callback succeeds...
					// set "in" of the node
					this->getNode(node_name).in = &(target.getNode(target_node_name));
					// set q_reg
					this->getNode(node_name).q_reg = target.getNode(target_node_name).q_reg;
				}
				else{
					if(this->getNode(node_name).out != nullptr){
						// already connected
						return false;
					}

					//callback (ask the target if the connection can be accepted)
					target.OnConnect(target_node_name, this->getNode(node_name), ConnectDir::IN);
					// if the callback succeeds...
					// set "out" of the node
					this->getNode(node_name).out = &(target.getNode(target_node_name));
					// set q_reg (not needed when connecting OUT node)
					//this->getNode(node_name).q_reg = target.getNode(target_node_name).q_reg;
				}

				return true;
			}

			/**
			 * remove the node <br/>
			 * if the node is not connected, return false. else return true.
			 * this function may throw InvalidOperationException.
			 * @param node_name the name of the node 
			 * @param direction direction IN or OUT of the node
			 * @return bool variable which shows if the function succeeds
			 */

			bool removeNode(const std::string& node_name, ConnectDir direction){
				if(direction == ConnectDir::IN){
					if(this->getNode(node_name).in == nullptr)
						return false;

					//callback
					const Node& target_node = this->getAdjNode(node_name, ConnectDir::IN);
					Component& target = *(target_node.parent);
					target.OnRemove(target_node.name, ConnectDir::OUT);

					this->getNode(node_name).in = nullptr;
				}
				else{
					if(this->getNode(node_name).out == nullptr)
						return false;

					//callback
					const Node& target_node = this->getAdjNode(node_name, ConnectDir::OUT);
					Component& target = *(target_node.parent);
					target.OnRemove(target_node.name, ConnectDir::IN);

					this->getNode(node_name).out = nullptr;
				}

				return true;
			}

			/**
			 * get Node function (const version)
			 * @param node_name the node name
			 * @return corresponding node
			 */

			inline const Node& getNode(const std::string& node_name) const{
				return this->nodes.at(node_name);
			}

			/**
			 * get the information of the adjacent node
			 * if the node does not exist, this function returns the reference of "null_node"
			 * @param node_name the name of the node 
			 * @param direction the direction (IN|OUT)
			 * @return the derived node
			 */

			const Node& getAdjNode(const std::string& node_name, ConnectDir direction) const{
				if(direction == ConnectDir::IN){
					if(this->getNode(node_name).in == nullptr){
						// return dummy node
						return null_node;
					}

					return *(this->getNode(node_name).in);
				}
				else{
					if(this->getNode(node_name).out == nullptr){
						//return dummy node
						return null_node;
					}

					return *(this->getNode(node_name).out);
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

			virtual const std::vector<CurrentOp> getCurrentOps() const{
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
					nodeinfos.push_back(std::make_pair(elem.first, elem.second.q_reg));
				}

				return ret;
			}

			/**
			 * get identifier
			 * @return identifier name
			 */

			const std::string getName() const noexcept{
				return this->name;
			}

			/**
			 * get component type
			 * @return component type
			 */

			ComponentType getType() const noexcept{
				return this->type;
			}

			/**
			 * get the pointer to its parents
			 * if the component has no parent, this function returns nullptr
			 * @return the pointer to its parents (may be nullptr)
			 */

			Component* getParent(){
				return this->parent;
			}

			/**
			 * get the pointer to its parents (const version)
			 * if the component has no parent, this function returns nullptr
			 * @return the pointer to its parents (may be nullptr)
			 */

			const Component* getParent() const{
				return this->parent;
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
			 * @param parent the pointer to its parent
			 */

			Measure(Component* parent = nullptr)
				: Component(default_str[MEASURE], ComponentType::MEASUREMENT, parent), measure("measure"){
					//add node "measure"
					this->nodes[measure] = Node(measure, this);
				}

			/**
			 * overidden function of OnConnect
			 * @param node_name the node name to be connected
			 * @param node node to be added
			 * @param direction direction IN or OUT of the node. if direction == OUT, this function throws InvalidOperationException.
			 */

			void OnConnect(const std::string& node_name, Node& node, ConnectDir direction) override{
				if(direction == ConnectDir::OUT){
					// cannot be connected
					throw InvalidOperationException("cannot connect Node after Measure component");
				}

				Component::OnConnect(node_name, node, direction);
			}

		public:

			/**
			 * generate shared_ptr of Measure
			 * @return shared_ptr of Measure
			 */

			inline static u_ptr<Measure> create(Component* parent = nullptr){
				struct impl: Measure{
					impl(Component* parent) : Measure(parent){}
				};
				return u_ptr<impl>(new impl(parent));
			}

	};

	/**
	 * class for Unitary operator with parameters 
	 * nodename : [any]
	 */

	class UnitaryOp : public Component{
		private:

			//! list of parameters
			const std::vector<double> params;

			/**
			 * constructor of UnitaryOp
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
			 * @param params a vector of double parameters
			 * @param node_strs a set of strings of node
			 * @param parent the pointer to its parent
			 */

			UnitaryOp(const std::string& gate_name, const std::vector<double>& params, const std::vector<std::string>& node_strs, Component* parent = nullptr)
				: Component(gate_name, ComponentType::UNITARY_OP_PARAM, parent), params(params){
					for(auto&& elem : node_strs){
						this->nodes[elem] = Node(elem, this);
					}
				}

			/**
			 * constructor of UnitaryOp (without params)
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
			 * @param node_strs a set of strings of node
			 * @param parent the pointer to its parent
			 */

			UnitaryOp(const std::string& gate_name, const std::vector<std::string>& node_strs, Component* parent = nullptr)
				: Component(gate_name, ComponentType::UNITARY_OP, parent){
					for(auto&& elem : node_strs){
						this->nodes[elem] = Node(elem, this);
					}
				}
		public:

			/**
			 * get the list of CurrentOp object (overridden function)
			 * @return the std::vector of CurrentOp object
			 */

			const std::vector<CurrentOp> getCurrentOps() const override{
				//create NodeInfo vector
				auto ret = 
					std::vector<CurrentOp>({{
							this->name,
							this->type,
							std::vector<NodeInfo>(),
							params,
							}});

				std::vector<NodeInfo>& nodeinfos = ret[0].p_info;

				for(auto&& elem : this->nodes){
					nodeinfos.push_back(std::make_pair(elem.first, elem.second.q_reg));
				}

				return ret;
			}

			/**
			 * generate shared_ptr of UnitaryOp 
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
			 * @param params a vector of double parameters
			 * @param node_strs a set of strings of node
			 * @return shared_ptr of UnitaryOp
			 */

			inline static u_ptr<UnitaryOp> create(const std::string& gate_name, const std::vector<double>& params, const std::vector<std::string>& node_strs, Component* parent = nullptr){
				struct impl: UnitaryOp{
					impl(const std::string& gate_name, const std::vector<double>& params, const std::vector<std::string>& node_strs, Component* parent) : UnitaryOp(gate_name, params, node_strs, parent){}
				};
				return u_ptr<impl>(new impl(gate_name, params, node_strs, parent));
			}

			/**
			 * generate shared_ptr of UnitaryOp (without params)
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
			 * @param node_strs a set of strings of node
			 * @return shared_ptr of UnitaryOp
			 */

			inline static u_ptr<UnitaryOp> create(const std::string& gate_name, const std::vector<std::string>& node_strs, Component* parent = nullptr){
				struct impl: UnitaryOp{
					impl(const std::string& gate_name, const std::vector<std::string>& node_strs, Component* parent) : UnitaryOp(gate_name, node_strs, parent){}
				};
				return u_ptr<impl>(new impl(gate_name, node_strs, parent));
			}
	};

	/**
	 * class for representing generalized control gate
	 * nodename : control1, control2, ... , control N
	 * subcomponents : target (UnitaryOp)
	 */

	//TODO: control gate ( : UnitaryOp)
	//TODO: measurement-if ( : Component)
	//TODO: const reference of this->nodes?

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


