#ifndef HOLDER_HPP
#define HOLDER_HPP

#include "HandlerPtr.hpp"

template<typename T, typename ...Args>
class FunctorEventHandler;

template<typename F>
class FunctorHolder
{
private:
	F m_functor;

	template<typename T, typename... Args> friend class FunctorEventHandler;
public:
	FunctorHolder(F &functor) : m_functor(functor) { }

	/**
	 * @brief call sunscribed functor/lanbda/function
	 * 
	 * @param args args use in event
	 */
	template<typename... Args>
	void call(Args &&...args)
	{
		m_functor(std::forward<Args>(args)...);
	}

	/**
	 * @brief overloaded cast operator for create handler
	 * 
	 * @tparam Args 
	 * @return THandlerPtr<Args...> 
	 */
	template<typename... Args>
	operator THandlerPtr<Args...>()
	{
		return std::make_shared<FunctorEventHandler<F, Args...>>(m_functor);
	}

	/**
	 * @brief overloade operator for compare holders
	 * 
	 * @param other 
	 * @return true - if equal
	 * @return false - if not equal
	 */
	bool operator==(const FunctorHolder<F> &other) const
	{
		return (m_functor == other.m_functor);
	}
	
	/**
	 * @brief overloade operator for compare holders
	 * 
	 * @param other 
	 * @return true - if not equal
	 * @return false - if equal
	 */
	bool operator!=(const FunctorHolder<F> &other) const
	{
		return !(*this == other);
	}
};

#endif