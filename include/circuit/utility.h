/**
 * @file utility.h
 * @brief utilities of circuit.h
 * @author Kohji Nishimura
 * @date 2018/06/22
 *
 */


#pragma once
#include <memory>
#include <exception>
#include <string>
#include <type_traits>
#include <limits>

namespace qlib{

	/** abbrebiation of std::unique_ptr
	 * @tparam T the type for unique_ptr
	 */
	template<class T>
		using u_ptr = std::unique_ptr<T>;

	/** abbrebiation of std::unique_ptr
	 * @tparam T the type for unique_ptr
	 */
	template<class T>
		using s_ptr = std::shared_ptr<T>;

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
	 * An exception class for QuantumCircuit library (InvalidOperation)
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
	 * An exception class for QuantumCircuit library (NullPointer)
	 */

	class NullPointerException: public std::exception{
		private:
			//! exception message
			std::string message;

		public:

			/**
			 * constructor of InvalidOperationException
			 * @param msg additional exception message
			 */

			NullPointerException(const std::string& msg)
				: message(exception_format("NullPointer", msg)){
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
	 * generating indexing string
	 * @param str string
	 * @param index the index of string
	 * @return str + index string
	 */

	inline const std::string genstr(const std::string& str, size_t index){
		return str + std::to_string(index);
	}

	/**
	 * unique_ptr factory
	 * @tparam Object a class derived from Component
	 */

	template<class Object>
	struct UniquePtrFactory{

		/**
		 * generate unique_ptr of the object
		 * @parma args arguments for instantiation of the object
		 * @return unique_ptr of the object
		 */
		template<class... Args>
		static u_ptr<Object> create(Args&&... args){
			return u_ptr<Object>(new Object(std::forward<Args>(args)...));
		}
	};

}
