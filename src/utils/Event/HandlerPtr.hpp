#ifndef HANDLER_PTR_HPP
#define HANDLER_PTR_HPP

#include <memory>

template<typename ...Args>
class IEventHandler;

template<class ...Types>
using THandlerPtr = std::shared_ptr<IEventHandler<Types...>>;


#endif