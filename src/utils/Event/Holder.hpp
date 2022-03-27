#ifndef HOLDER_HPP
#define HOLDER_HPP

template<typename T, typename ...Args>
class FunctorEventHandler;

template<typename F>
class FunctorHolder
{
private:
    // TODO: refernce F &m_functor doesn't work but in my Callback in https://github.com/C4e10VeK/WindowWrapper/blob/master/include/Common/Callback.hpp work.
	F &m_functor;

	template<typename T, typename... Args> friend class FunctorEventHandler;
public:
	FunctorHolder(F &functor) : m_functor(functor) { }

	template<typename... Args>
	operator THandlerPtr<Args...>()
	{
		return std::make_shared<FunctorEventHandler<F, Args...>>(*this);
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