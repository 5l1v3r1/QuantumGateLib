
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

	struct QuantumBit{
		//TODO: register num?
	};

	//! NodeInfo type
	using NodeInfo = std::pair<const std::string, const QuantumBit*>;

	/**
	 * struct for representing current operation in quantum circuit
	 */

	struct CurrentOp{
		//!component name
		std::string name;
		//!component of node_name and the correspondingquantum registers
		std::vector<NodeInfo> p_info;
		//!component parameters
		std::vector<double> parameters;

		/**
		 * constructor of CurrentOp
		 * @param name component name
		 * @param p_info component of node_name and the correspondingquantum registers
		 * @param parameters component parameters
		 */

		CurrentOp(const std::string& name, const std::vector<NodeInfo>& p_info, const std::vector<double> parameters)
			: name(name), p_info(p_info), parameters(parameters)
		{}


	};

	/**************************************************/

	//!forward declaration
	class Component;

	/**
	 * enum for node IN and OUT
	 */

	enum ConnectDir{
		IN,
		OUT,
	};

	/**
	 * inverse IN and OUT
	 * @param dir direction
	 * @return inverse direction
	 */

	inline ConnectDir inv(ConnectDir dir){
		return (dir==ConnectDir::IN)?ConnectDir::OUT:ConnectDir::IN;
	}

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
		const QuantumBit* q_reg;

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

		/**
		 * get the refernce of Node* from ConnectDir enum
		 * @enum dir direction
		 * @return the corresponding reference of Node*
		 */

		inline Node*& ptr(ConnectDir dir){
			if(dir == ConnectDir::IN){
				return in;
			}
			else{
				return out;
			}
		}

		/**
		 * get the refernce of Node* from ConnectDir enum (const version)
		 * @enum dir direction
		 * @return the corresponding reference of Node*
		 */

		inline Node* ptr(ConnectDir dir) const{
			if(dir == ConnectDir::IN){
				return in;
			}
			else{
				return out;
			}
		}

	};

	/**
	 * operator overloading (==) of Node
	 * @param lhs left hand side
	 * @param rhs right hand side
	 * @return bool variable. if lhs==rhs -> true else false
	 */

	inline bool operator==(const Node& lhs, const Node& rhs){
		return(
				lhs.name   == rhs.name   &&
				lhs.in     == rhs.in     &&
				lhs.out    == rhs.out    &&
				lhs.parent == rhs.parent &&
				lhs.q_reg  == rhs.q_reg
				);
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

	/**
	 * struct for representing Component
	 */

	class Component{
		protected:
			//! an identifier
			const std::string name;
			//! its parent
			Component* parent;
			//! list of Nodes
			std::map<std::string, Node> nodes;
			/**
			 * constructor of Component
			 * @param name identifier name
			 * @param parent the pointer to the parent
			 */
			Component(const std::string& name, Component* parent = nullptr)
				:name(name), parent(parent){
					// add Node to nodes
				}


			/**
			 * get Node function
			 * @param node_name the node name
			 * @return corresponding node
			 */

			inline Node& getNode(const std::string& node_name){
				return this->nodes.at(node_name);
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
			 * callback function which is called when detecting the node to be connected.
			 * This function can be overridden.
			 * @param node_name the node name to be connected
			 * @param added_node added node
			 * @param direction direction IN or OUT of the node
			 */

			virtual void OnConnect(const std::string& node_name, Node& added_node, ConnectDir direction){
				this->getNode(node_name).ptr(direction) = &added_node;
			}

			/**
			 * callback function which is called when catching the remove signal
			 * This function can be overridden.
			 * @param node_name the node name to be removed
			 * @param direction direction IN or OUT of the node
			 */

			virtual void OnRemove(const std::string& node_name, ConnectDir direction){
				this->getNode(node_name).ptr(direction) = nullptr;
			}

		public:

			/**
			 * destructor of Component
			 */
			virtual ~Component(){}


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

			bool connectNode(const std::string& node_name, Component& target, const std::string& target_node_name, ConnectDir direction = ConnectDir::OUT){
				if(this->getNode(node_name).ptr(direction) != nullptr){
					// already connected
					return false;
				}

				//callback (ask the target if the connection can be accepted)
				target.OnConnect(target_node_name, this->getNode(node_name), inv(direction));
				// if the callback succeeds...
				// set direction of the node
				this->getNode(node_name).ptr(direction) = &(target.getNode(target_node_name));
				// set q_reg if direction == IN
				if(direction == ConnectDir::IN)
					this->getNode(node_name).q_reg = target.getNode(target_node_name).q_reg;

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
				if(this->getNode(node_name).ptr(direction) == nullptr){
					//not connected
					return false;
				}

				//callback
				const Node& target_node = *(this->getAdjNode(node_name, direction));
				Component& target = *(target_node.parent);
				target.OnRemove(target_node.name, inv(direction));

				this->getNode(node_name).ptr(direction) = nullptr;

				return true;
			}


			/**
			 * get the pointer of the adjacent node
			 * @param node_name the name of the node 
			 * @param direction the direction (IN|OUT)
			 * @return the derived node
			 */

			inline const Node* getAdjNode(const std::string& node_name, ConnectDir direction) const{
				return this->getNode(node_name).ptr(direction);
			}

			/**
			 * get the subComponent(children) by name
			 * this function throws InvalidOperationException if not defined.
			 * @param component_name the name of subcomponent
			 * @return the corresponding component
			 */

			virtual Component& getSubComponent(const std::string& /*component_name*/) const{
				throw InvalidOperationException("the component has no children.");
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

			inline const std::string getName() const noexcept{
				return this->name;
			}

			/**
			 * get the pointer to its parents
			 * if the component has no parent, this function returns nullptr
			 * @return the pointer to its parents (may be nullptr)
			 */

			inline Component*& getParent(){
				return this->parent;
			}

			/**
			 * get the pointer to its parents (const version)
			 * if the component has no parent, this function returns nullptr
			 * @return the pointer to its parents (may be nullptr)
			 */

			inline Component* getParent() const{
				return this->parent;
			}

			/**
			 * get the const reference of the node map
			 */

			inline const std::map<std::string, Node>& getNodes() const{
				return this->nodes;
			}
	};

	/**
	 * class for representing qubit
	 * nodename: qubit
	 */

	class Qubit : public Component{
		private:
			//! friend class UniquePtrFactory, this class can be initialized only through UniquePtrFactory.
			friend struct UniquePtrFactory<Qubit>;

			//! quantum bit
			QuantumBit quantum_bit;

			/**
			 * constructor of Qubit
			 * @param quantum_bit an instance of QuantumBit
			 * @param parent the pointer to its parent
			 */

			Qubit(const QuantumBit& quantum_bit, Component* parent = nullptr)
				: Component(default_str(QUBIT), parent), quantum_bit(quantum_bit){
					//add node "qubit"
					this->nodes[QubitStr::qubit()] = Node(QubitStr::qubit(), this);
				}

			/**
			 * overidden function of OnConnect
			 * @param node_name the node name to be connected
			 * @param node node to be added
			 * @param direction direction IN or OUT of the node. if direction == OUT, this function throws InvalidOperationException.
			 */

			void OnConnect(const std::string& node_name, Node& node, ConnectDir direction) override{
				if(direction == ConnectDir::IN){
					// cannot be connected
					throw InvalidOperationException("cannot connect Node before Qubit component");
				}

				Component::OnConnect(node_name, node, direction);
			}

		public:
			//TODO: getCurrentOps
	};

	/**
	 * class for representing Measurement
	 * nodename: measure
	 */

	class Measure : public Component{
		private:
			//! friend class UniquePtrFactory, this class can be initialized only through UniquePtrFactory.
			friend struct UniquePtrFactory<Measure>;

			/**
			 * constructor of Measure with previous node
			 * @param parent the pointer to its parent
			 */

			Measure(Component* parent = nullptr)
				: Component(default_str(MEASURE), parent){
					//add node "measure"
					this->nodes[MeasureStr::measure()] = Node(MeasureStr::measure(), this);
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
	};

	/**
	 * class for Unitary operator with parameters 
	 * nodename : [any]
	 */

	class UnitaryOp : public Component{
		protected:
			//! friend class UniquePtrFactory, this class can be initialized only through UniquePtrFactory.
			friend struct UniquePtrFactory<UnitaryOp>;

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
				: Component(gate_name, parent), params(params){
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
				: Component(gate_name, parent){
					for(auto&& elem : node_strs){
						this->nodes[elem] = Node(elem, this);
					}
				}

			/**
			 * constructor of UnitaryOp (without params and node_strs)
			 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
			 * @param parent the pointer to its parent
			 */

			UnitaryOp(const std::string& gate_name, Component* parent = nullptr)
				: Component(gate_name, parent){
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
							std::vector<NodeInfo>(),
							params,
							}});

				std::vector<NodeInfo>& nodeinfos = ret[0].p_info;

				for(auto&& elem : this->nodes){
					nodeinfos.push_back(std::make_pair(elem.first, elem.second.q_reg));
				}

				return ret;
			}
	};

	/**
	 * class for representing generalized control gate
	 * nodename : control0, control1, ... , control N-1 (N = ctrl_num)
	 * subcomponents : target (UnitaryOp)
	 */

	class ControlGate : public UnitaryOp{
		private:
			//! friend class UniquePtrFactory, this class can be initialized only through UniquePtrFactory.
			friend struct UniquePtrFactory<ControlGate>;

			//! target unitary operator
			u_ptr<UnitaryOp> target_unitary;

			/**
			 * constructor of ControlGate
			 * @param ctrl_num the number of control nodes
			 * @param target_unitary target unitary gate (rvalue reference)
			 * @param parent the pointer to the parent
			 */

			ControlGate(size_t ctrl_num, u_ptr<UnitaryOp>&& target_unitary, Component* parent = nullptr)
				: UnitaryOp(default_str(CONTROL), parent), target_unitary(std::move(target_unitary)){
					for(size_t i=0; i<ctrl_num; i++){
						this->nodes[ControlStr::control(i)] = Node(ControlStr::control(i), this);
					}
					//set parent of target_unitary
					this->target_unitary->getParent() = this;
				}

		public:

			/**
			 * get the subComponent(children) by name
			 * this function throws InvalidOperationException if not defined.
			 * @param component_name the name of subcomponent
			 * @return the derived component
			 */

			Component& getSubComponent(const std::string& component_name) const override{
				if(component_name == ControlStr::target()){
					return *target_unitary;
				}
				else{
					//otherwise, throw exception
					throw InvalidOperationException("the subcomponent which does not exist requested.");
				}
			}

			//TODO: getCurrentOps
	};

	/**
	 * abstract class of component which has subcomponents
	 * nodename : [none]
	 * subcomponents : [any]
	 */

	class AbstComponentContainer : public Component{
		protected:
			//! subcomponents
			std::map<std::string, u_ptr<Component>> subcomponents;

			/**
			 * constructor of AbstComponentContainer
			 * @param name identifier name
			 * @param parent the pointer to the parent
			 */

			AbstComponentContainer(const std::string& name, Component* parent = nullptr)
				: Component(name, parent){
					// add u_ptr<Component> to subcomponents
				}

			/**
			 * get subcomponent
			 * @param component_name the component name
			 * @return corresponding component
			 */

			inline Component& getComponent(const std::string& component_name){
				return *(this->subcomponents.at(component_name));
			}

			/**
			 * get subcomponent (const version)
			 * @param component_name the component name
			 * @return corresponding component
			 */

			inline const Component& getComponent(const std::string& component_name) const{
				return *(this->subcomponents.at(component_name));
			}

		public:

			/**
			 * get the subComponent(children) by name
			 * this function throws InvalidOperationException if not defined.
			 * @param component_name the name of subcomponent
			 * @return the derived component
			 */

			Component& getSubComponent(const std::string& component_name) const override{
				return *(this->subcomponents.at(component_name));
			}

			//TODO: getCurrentOps
	};

	/**
	 * class for representing measurement-if operation 
	 * nodename : [none]
	 * subcomponents : measure (Measure), unitary (Unitary)
	 */

	class MeasureCtrl : public AbstComponentContainer{
		private:
			//! friend class UniquePtrFactory, this class can be initialized only through UniquePtrFactory.
			friend struct UniquePtrFactory<MeasureCtrl>;

			/**
			 * constructor of MeasureCtrl
			 * @param measure unique_ptr of measurement operator
			 * @param unitary unique_ptr of the unitary operator
			 * @param parent the pointer to the parent
			 */

			MeasureCtrl(u_ptr<Measure>&& measure, u_ptr<UnitaryOp>&& unitary, Component* parent = nullptr)
				: AbstComponentContainer(default_str(MEASURE_CONTROL), parent){
					//set measure and target component
					this->subcomponents[MeasCtrlStr::measure()] = std::move(measure);
					this->subcomponents[MeasCtrlStr::unitary()] = std::move(unitary);
					// set parents
					this->getComponent(MeasCtrlStr::measure()).getParent() = this;
					this->getComponent(MeasCtrlStr::unitary()).getParent() = this;
				}
	};

	/**
	 * class for representing a container of the components
	 * nodename : [none]
	 * subcomponents : [any]
	 */

	//class ComponentContainer : public AbstComponentContainer{
	//	private:
	//		//! friend class UniquePtrFactory, this class can be initialized only through UniquePtrFactory.
	//		friend struct UniquePtrFactory<ComponentContainer>;

	//		//TODO: how to make the constructor of the ComponentContainer?

	//	public:
	//};


	/**
	 * components factory
	 */

	struct Op{

		/**
		 * qubit factory
		 * @param quantum_bit QuantumBit instance
		 * @param parent the parent of the component 
		 * @return unique_ptr of Qubit class
		 */

		inline static u_ptr<Qubit> Q(const QuantumBit& quantum_bit, Component* parent = nullptr){
			return UniquePtrFactory<Qubit>::create(quantum_bit, parent);
		}

		/**
		 * measurement operator factory
		 * @param parent the parent of the component 
		 * @return unique_ptr of Measure class
		 */

		inline static u_ptr<Measure> M(Component* parent = nullptr){
			return UniquePtrFactory<Measure>::create(parent);
		}

		/**
		 * unitary operator factory
		 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
		 * @param params a vector of double parameters
		 * @param node_strs a set of strings of node
		 * @param parent the parent of the component 
		 * @return unique_ptr of UnitaryOp class
		 */

		inline static u_ptr<UnitaryOp> U(const std::string& gate_name, const std::vector<double>& params, const std::vector<std::string>& node_strs, Component* parent = nullptr){
			return UniquePtrFactory<UnitaryOp>::create(gate_name, params, node_strs, parent);
		}

		/**
		 * unitary operator factory (without params)
		 * @param gate_name the gate_name of the UnitaryOp (X-gate, R-gate...)
		 * @param node_strs a set of strings of node
		 * @param parent the parent of the component 
		 * @return unique_ptr of UnitaryOp class
		 */

		inline static u_ptr<UnitaryOp> U(const std::string& gate_name, const std::vector<std::string>& node_strs, Component* parent = nullptr){
			return UniquePtrFactory<UnitaryOp>::create(gate_name, node_strs, parent);
		}

		/**
		 * generalized control gate factory 
		 * @param ctrl_num the number of control nodes
		 * @param target_unitary target unitary gate (rvalue reference)
		 * @param parent the pointer to the parent
		 * @return unique_ptr of ControlGate class
		 */

		inline static u_ptr<ControlGate> Ctrl(size_t ctrl_num, u_ptr<UnitaryOp>&& target, Component* parent = nullptr){
			return UniquePtrFactory<ControlGate>::create(ctrl_num, std::move(target), parent);
		}

		/**
		 * measurement-control component factory
		 * @param unitary unique_ptr of the unitary operator
		 * @param parent the pointer to the parent
		 * @return unique_ptr of MeasureCtrl class
		 */

		inline static u_ptr<MeasureCtrl> MCtrl(u_ptr<UnitaryOp>&& unitary, Component* parent = nullptr){
			return UniquePtrFactory<MeasureCtrl>::create(Op::M(), std::move(unitary), parent);
		}

		/**
		 * generate Id gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of Id gate
		 */

		inline static u_ptr<UnitaryOp> Id(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::Id), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate X gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of X gate
		 */

		inline static u_ptr<UnitaryOp> X(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::X), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate Y gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of Y gate
		 */

		inline static u_ptr<UnitaryOp> Y(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::Y), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate Z gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of Z gate
		 */

		inline static u_ptr<UnitaryOp> Z(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::Z), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate H (hadamard) gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of H gate
		 */

		inline static u_ptr<UnitaryOp> H(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::H), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate S (phase) gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of S gate
		 */

		inline static u_ptr<UnitaryOp> S(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::S), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}
		/**
		 * generate T (pi/8) gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of T gate
		 */

		inline static u_ptr<UnitaryOp> T(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::T), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate CNOT gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of CNOT gate
		 */

		inline static u_ptr<ControlGate> CNOT(Component* parent = nullptr){
			return Op::Ctrl(1, Op::X(), parent);
		}

		/**
		 * generate SWAP gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of SWAP gate
		 */

		inline static u_ptr<UnitaryOp> SWAP(Component* parent = nullptr){
			return Op::U(default_str(DefaultString::SWAP), std::vector<std::string>({SwapStr::swap0(), SwapStr::swap1()}), parent);
		}

		/**
		 * generate CZ gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of CZ gate
		 */

		inline static u_ptr<ControlGate> CZ(Component* parent = nullptr){
			return Op::Ctrl(1, Op::Z(), parent);
		}

		/**
		 * generate CS gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of CS gate
		 */

		inline static u_ptr<ControlGate> CS(Component* parent = nullptr){
			return Op::Ctrl(1, Op::S(), parent);
		}

		/**
		 * generate TOFFOLI (CCNOT) gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of TOFFOLI gate
		 */

		inline static u_ptr<ControlGate> TOFFOLI(Component* parent = nullptr){
			return Op::Ctrl(2, Op::X(), parent);
		}

		/**
		 * generate FREDKIN (CSWAP) gate
		 * @param parent the pointer to the parent
		 * @return unique_ptr of FREDKIN gate
		 */

		inline static u_ptr<ControlGate> FREDKIN(Component* parent = nullptr){
			return Op::Ctrl(1, Op::SWAP(), parent);
		}

		/**
		 * generate R (phase-shift) gate with parameter
		 * @param phi phase parameter
		 * @param parent the pointer to the parent
		 * @return unique_ptr of R gate
		 */

		inline static u_ptr<UnitaryOp> R(double phi, Component* parent = nullptr){
			return Op::U(default_str(DefaultString::R), std::vector<double>({phi}), std::vector<std::string>({UnitaryOpStr::singlenode()}), parent);
		}

		/**
		 * generate CR (controlled phase-shift) gate with parameter
		 * @param phi phase parameter
		 * @param parent the pointer to the parent
		 * @return unique_ptr of CR gate
		 */

		inline static u_ptr<ControlGate> CR(double phi, Component* parent = nullptr){
			return Op::Ctrl(1, Op::R(phi), parent);
		}
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
