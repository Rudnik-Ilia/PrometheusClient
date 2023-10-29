#pragma once

#include <map>

template<typename T>
class OutScope
{
    public:
        OutScope(typename std::map<T, Counter<T>>::iterator& iterator): m_iterator(iterator)
        {
            std::cout << "Ctor OUTSCOPE" << std::endl;
        }

        ~OutScope()
        {
            --m_iterator;
            std::cout << "OUT ITER: " << m_iterator->first  <<  std::endl;
        }

    private:
        typename std::map<T, Counter<T>>::iterator& m_iterator{};
};