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

	template<typename... Args>
	void call(Args &&...args)
	{
		m_functor(std::forward<Args>(args)...);
	}

	template<typename... Args>
	operator THandlerPtr<Args...>()
	{
		return std::make_shared<FunctorEventHandler<F, Args...>>(m_functor);
	}

    bool operator==(const FunctorHolder<F> &other) const
    {
        return (m_functor == other.m_functor);
    }
    
    bool operator!=(const FunctorHolder<F> &other) const
    {
        return !(*this == other);
    }
};

#endif