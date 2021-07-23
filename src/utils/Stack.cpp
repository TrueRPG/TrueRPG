#include "Stack.h"

Stack::Stack(const int range, int stack_end) : m_range(range)
{
    m_stack = new int[m_range];
}

Stack::~Stack()
{
    delete[] m_stack;
}

void Stack::push(int element)
{
    m_stack[m_stackEnd] = element;
    ++m_stackEnd;
}

int Stack::pop()
{
    int temp;
    temp = m_stack[m_stackEnd - 1];
    m_stack[m_stackEnd - 1] = 0;
    --m_stackEnd;
    return temp;
}

int Stack::getElement()
{
    return m_stack[m_stackEnd - 1];
}

int Stack::popSearch(int key)
{
    int temp = 0;
	int * pTempArray = new int[m_stackEnd];

    for (int n = 0; n < m_stackEnd; ++n)
    {
		pTempArray[n] = m_stack[n];
    }
    for (int i = 0; i < m_stackEnd; ++i)
    {
        if (m_stack[i] == key)
        {
            continue;
        }
        m_stack[temp] = pTempArray[i];
        ++temp;
    }
    m_stack[m_stackEnd - 1] = 0;
    --m_stackEnd;

	delete[] pTempArray;
    return pTempArray[temp - 1];
}
