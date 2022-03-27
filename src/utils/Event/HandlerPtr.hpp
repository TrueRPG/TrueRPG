#ifndef HANDLER_PTR_HPP
#define HANDLER_PTR_HPP

#include <memory>

template<typename ...Args>
class AbstractEventHandler;

template<class ...Types>
using THandlerPtr = std::shared_ptr<AbstractEventHandler<Types...>>;


#endif