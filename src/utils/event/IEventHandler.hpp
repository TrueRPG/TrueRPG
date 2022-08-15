#ifndef ABSTRACT_EVENT_HANDLER_HPP
#define ABSTRACT_EVENT_HANDLER_HPP

template<typename ...Args>
class IEventHandler
{
public:
    IEventHandler() = default;
    virtual ~IEventHandler() = default;
    virtual void call(Args &&...args) = 0;

    /**
     * @brief overloaded operator for comparison between AbstractEventHandlers
     * 
     * @param o 
     * @return true - if equal
     * @return false - if not equal
     */
    bool operator==(const IEventHandler<Args...> &o) const
    {
        return isEqual(o);
    }

    /**
     * @brief overloaded operator for comparison between AbstractEventHandlers
     * 
     * @param o 
     * @return true - if not equal
     * @return false - if equal
     */
    bool operator!=(const IEventHandler<Args...> &o) const
    {
        return !isEqual(o);
    }
protected:
    virtual bool isEqual(const IEventHandler<Args...> &o) const = 0;
};


#endif