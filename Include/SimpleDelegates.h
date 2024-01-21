#pragma once
#include <functional>
#include <map>
#include <cassert>
#include <typeindex>
#include <string>

//Simple delegates is a small library created and maintained by Franciszek Luczak.
//It was created for small personal projects created in C++.
//IT IS NOT RECOMMENDED TO USE IN ANY PROJECTS OF HIGH IMPORTANCE OR VALUE
//User is free to use the Simple Delegates library to any extent and up to their will. (Yet with extra causion)

//The design and overall flow is designed to replicate Action delegates in C#:https://learn.microsoft.com/en-us/dotnet/api/system.action-1?view=net-7.0
//The best recommended use can be found in the library's documentation.

//uncomment to remove asserts
//#define NDEBUG
namespace fluczak
{
	namespace sdel
	{
		template <typename>
		class Delegate;

		template <typename ReturnType, typename... Args>
		class Delegate<ReturnType(Args...)>
		{
		public:
			/*
			 * Binds a member  function \param a_method  of type:
			 * \tparam T of an instance: \param t
			 */
			template<typename T>
			void bind(T* t, ReturnType(T::* a_method)(Args...))
			{
				auto tempFunction = [=](Args ... as) { (t->*a_method)(as...); };
				auto hash_value = HashMemberFunction(t, a_method);
				m_functions.insert(std::pair<std::size_t, std::function<ReturnType(Args...)>>(hash_value, tempFunction));
			}

			/*
			 * Binds a global  function \param a_method  of type:
			 * \tparam T
			*/
			void bind(ReturnType(*a_method)(Args...))
			{
				auto tempFunction = [=](Args ... as) { (*a_method)(as...); };
				auto hash_value = std::hash<ReturnType(*)(Args...)>()(a_method);
				m_functions.insert(std::pair<std::size_t, std::function<ReturnType(Args...)>>(hash_value, tempFunction));
			}

			/*
			 * \Unbinds member function \param a_method from instance \param t  of type \tparam T
			 */
			template<typename T>
			void unbind(T* t, ReturnType(T::* a_method)(Args...))
			{
				auto found = m_functions.find(HashMemberFunction(t, a_method));
				assert(found != m_functions.end());
				m_functions.erase(found);
			}

			/*
			 * \Unbinds global function \param a_method
			 */
			void unbind(ReturnType(*a_method)(Args...))
			{
				auto hash_value = std::hash<ReturnType(*)(Args...)>()(a_method);
				auto found = m_functions.find(hash_value);
				assert(found != m_functions.end());
				m_functions.erase(found);
			}

			/*
			 * Invoke all bound functions. Can call with arguments
			 * of previously defined types.IMPORTANT: It is not recommended
			 * to bind or call anything with type other than void- it is possible,
			 * but the return value will be discarded...
			 */
			void operator() (Args... args)
			{
				auto duplicate_functions = m_functions;
				if (m_functions.size() == 0)return;
				for (auto& func : duplicate_functions)
				{
					func.second(args...);
				}
			}

			/*
			 * \this removes all bound functions at once.
			 */
			void clear()
			{
				m_functions.clear();
			}

		private:
			std::unordered_map<std::size_t, std::function<ReturnType(Args...)>> m_functions = {};


			template<typename T, typename R>
			std::size_t HashMemberFunction(T* instance, R(T::* func)(Args...))
			{
				std::hash<T*> hashInstance;
				std::hash<std::string> hashString;

				char buf[sizeof(func)];
				std::memcpy(&buf, &func, sizeof(func));
				const std::string temp(buf, sizeof(func));

				const std::size_t instance_hash = hashInstance(instance);
				const std::size_t char_hash = hashString(temp);

				// Combine hashes to create a unique hash for the pair
				return char_hash ^ (char_hash + 0x9e3779b9 + (instance_hash << 6) + (instance_hash >> 2));
			}
		};

	}
}
