#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>

#include "Event/IEventHandler.hpp"
#include "Event/HandlerPtr.hpp"
#include "Event/Holder.hpp"

template<typename T, typename ...Args>
class FunctorEventHandler : public IEventHandler<Args...>
{
private:
    FunctorHolder<T> m_functorHolder;
public:
    explicit FunctorEventHandler(T &functor) : m_functorHolder(functor) { }

    /**
     * @brief call subscribed functor/lanbda/function
     * 
     * @param args args used in event
     */
    void call(Args &&...args) override
    {
        m_functorHolder.call(std::forward<Args>(args)...);
    }
protected:
    bool isEqual(const IEventHandler<Args...> &o) const override
    {
        const auto *other = dynamic_cast<const FunctorEventHandler<T, Args...> *>(&o);
        return other != nullptr && other->m_functorHolder == m_functorHolder;
    }
};

template<class C, typename ...Args>
class MethodEventHandler : public IEventHandler<Args...>
{
private:
    using Method = void(C::*)(Args...);

    C &m_class;
    Method m_method;
public: 
    MethodEventHandler(C &c, Method m) : IEventHandler<Args...>(), m_class(c), m_method(m)
    {
        assert(m);
    }

    /**
     * @brief call method from class
     * 
     * @param args args used in event
     */
    void call(Args &&...args) override
    {
        (m_class.*m_method)(std::forward<Args>(args)...);
    }
protected:
    bool isEqual(const IEventHandler<Args...> &o) const override
    {
        const auto *other = dynamic_cast<const MethodEventHandler<C, Args...> *>(&o);
        return (other != nullptr && &other->m_class == &m_class && other->m_method == m_method);
    }
};

/**
 * @brief Interface used for subscribe and unsubscribe.
 * 
 * @tparam Args types used in event
 */
template<typename ...Args>
class IEvent
{
public:
    virtual ~IEvent() = default;

    /**
     * @brief subscribe lambda/function/method to event
     * 
     * @tparam T 
     * @param some use createMethodHandler(...) for subscribing  
     * @return IEvent& 
     */
    template<typename T>
    IEvent &operator+=(T &&some)
    {
        add(static_cast<HandlerPtr>(some));
        return *this;
    }

    /**
     * @brief unsubscribe lambda/function/method to event
     * 
     * @tparam T 
     * @param some use createMethodHandler(...) for unsubscribing  
     * @return IEvent& 
     */
    template<typename T>
    IEvent &operator-=(T &&some)
    {
        remove(static_cast<HandlerPtr>(some));
        return *this;
    }

protected:
    using HandlerPtr = THandlerPtr<Args...>;
    virtual void add(HandlerPtr &&handler) = 0;
    virtual void remove(HandlerPtr &&handler) = 0;
};

/**
 * @brief Event class used for subscribe/unsubscribe and call subscribed functions/lambdas/methods
 * 
 * @tparam Args types used in event
 */
template<typename ...Args>
class Event : public IEvent<Args...>
{
private:
    using HandlerPtr = THandlerPtr<Args...>;
    /**
     * @brief event handler list contains vector with handlers for call
     * 
     */
    class EventHandlerList
    {
    private:
        using HandlerIt = typename std::vector<HandlerPtr>::const_iterator;

        std::vector<HandlerPtr> m_handlers;
    public:
        EventHandlerList() : m_handlers() { }

        void add(HandlerPtr handler)
        {
            if (findHandler(handler) == m_handlers.end())
            {
                m_handlers.emplace_back(handler);
            }
        }

        void remove(HandlerPtr handler)
        {
            auto it = findHandler(handler);

            if (it != m_handlers.end())
            {
                m_handlers.erase(it);
            }
        }

        void call(Args &&...args)
        {
            for (auto &handle : m_handlers)
            {
                if (handle)
                    handle->call(std::forward<Args>(args)...);
            }
        }
    private:
        inline HandlerIt findHandler(HandlerPtr &handler) const
        {
            return std::find_if(m_handlers.cbegin(), m_handlers.cend(), 
                                    [&handler](const HandlerPtr &oneHandler)
                                    {
                                        return (*oneHandler == *handler);
                                    });
        }
    };

    EventHandlerList m_handlerList;
public:
    using IType = IEvent<Args...>;

    Event() : m_handlerList() { }

    /**
     * @brief call subscribed functions/lambdas/methods from operator as function or method.
     * 
     * @param args args used in Event
     */
    void operator()(Args ...args)
    {
        call(std::forward<Args>(args)...);
    }

    /**
     * @brief call subscribed functions/lambdas/methods
     * 
     * @param args args used in Event
     */
    void call(Args &&...args)
    {
        m_handlerList.call(std::forward<Args>(args)...);
    }
protected:
    void add(HandlerPtr &&handler) override
    {
        m_handlerList.add(handler);
    }

    void remove(HandlerPtr &&handler) override
    {
        m_handlerList.remove(handler);
    }

};

/**
 * @brief Create an Event Handler object
 * 
 * @tparam F 
 * @param functor 
 * @return FunctorHolder<F> 
 */
template<typename F>
FunctorHolder<F> createEventHandler(F &&functor)
{
    return FunctorHolder<F>(functor);
}

/**
 * @brief Create an Event Handler object
 * 
 * @tparam C 
 * @tparam Args 
 * @param object ref to a class or struct containing method
 * @param method ref for this method
 * @return std::shared_ptr<MethodEventHandler<C, Args...>> 
 */
template<class C, typename ...Args>
std::shared_ptr<MethodEventHandler<C, Args...>> createEventHandler(C &object, void(C::*method)(Args...))
{
    return std::make_shared<MethodEventHandler<C, Args...>>(object, method);
}

#endif