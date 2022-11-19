#ifndef RPG_RESULT_H
#define RPG_RESULT_H

#include "Types.h"
#include "Logger.h"

template <class T = void>
struct Error
{
    std::error_code code;
    T Error;
};

template<>
struct Error<void>
{
    std::error_code code;
};

template <typename RetT, class E = Error<>>
class Result
{
private:
    union
    {
        RetT m_value;
        E m_error;
    };

    bool m_isInit;
public:

    inline Result() : m_isInit(false) { }
    inline explicit Result(const RetT &v) : m_value(v), m_isInit(true) { }
    inline explicit Result(RetT &&v) : m_value(std::move(v)), m_isInit(true) { }
	template<typename ...Args>
	inline explicit Result(Args &&...args) : m_value(std::forward<Args>(args)...), m_isInit(true) { }
    inline explicit Result(E e) : m_error(e), m_isInit(false) { }

    inline Result(Result &&res) noexcept : m_isInit(res.m_isInit)
    {
        if (m_isInit)
        {
            m_value = std::move(res.m_value);
            return;
        }

        m_error = std::move(res.m_error);
    }

    inline Result(const Result &res) noexcept : m_isInit(res.m_isInit)
    {
        if (m_isInit)
        {
            m_value = res.m_value;
            return;
        }

        m_error = res.m_error;
    }

    inline ~Result() { destroy(); }

    inline RetT value() const noexcept { return m_value; }
    inline E error() const noexcept { return m_error; }
    inline bool hasValue() const noexcept { return m_isInit; }

    inline RetT except()
    {
        if (m_isInit) return m_value;

        logger::error(m_error.code.message());
        throw;
    }

    inline explicit operator bool() const noexcept { return m_isInit; }
    inline explicit operator RetT() const noexcept { return m_value; }

    inline Result &operator=(const Result &res) noexcept
    {
        m_isInit = res.m_isInit;
        if (m_isInit)
        {
            new (&m_value) RetT{res.m_value};
            return *this;
        }

        m_error = res.m_error;
        return *this;
    }
    inline Result &operator=(Result &&res) noexcept
    {
        m_isInit = res.m_isInit;
        if (m_isInit)
        {
            new (&m_value) RetT{std::move(res.m_value)};
            return *this;
        }

        m_error = std::move(res.m_error);
        return *this;
    }

    inline Result &operator=(RetT &&value) noexcept
    {
        destroy();
        m_isInit = true;
        new (&m_value) RetT(std::move(value));
        return *this;
    }

    inline Result &operator=(const RetT &value) noexcept
    {
        destroy();
        m_isInit = true;
        new (&m_value) RetT(value);
        return *this;
    }

    inline Result &operator=(E &&error) noexcept
    {
        destroy();
        m_isInit = false;
        m_error = std::move(error);
        return *this;
    }

    inline Result &operator=(const E &error) noexcept
    {
        destroy();
        m_isInit = false;
        m_error = error;
        return *this;
    }

private:
    inline void destroy()
    {
        if (m_isInit)
            m_value.~RetT();
    }
};

#endif // RPG_RESULT_H
