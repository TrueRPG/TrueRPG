#ifndef ABSTRACT_EVENT_HANDLER_HPP
#define ABSTRACT_EVENT_HANDLER_HPP

template<typename ...Args>
class AbstractEventHandler
{
public:
	AbstractEventHandler() = default;
	virtual ~AbstractEventHandler() = default;
	virtual void call(Args &&...args) = 0;

	/**
	 * @brief overloaded operator for comprasion between AbstractEventHandler
	 * 
	 * @param o 
	 * @return true - if equal
	 * @return false - if not equal
	 */
	bool operator==(const AbstractEventHandler<Args...> &o) const
	{
		return isEqual(o);
	}

	/**
	 * @brief overloaded operator for comprasion between AbstractEventHandler
	 * 
	 * @param o 
	 * @return true - if not equal
	 * @return false - if equal
	 */
	bool operator!=(const AbstractEventHandler<Args...> &o) const
	{
		return !isEqual(o);
	}
protected:
	virtual bool isEqual(const AbstractEventHandler<Args...> &o) const = 0;
};


#endif