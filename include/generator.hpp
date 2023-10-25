// #pragma once

// #include "init.hpp"

// class Generator
// {
//     public:
//         explicit Generator(size_t limit) : m_limit(limit){}

//         class Iterator
//         {
//             public:
//                 explicit Iterator(size_t val): m_val(val) {}

//                 bool operator!=(const Iterator& other) const 
//                 {
//                     return m_val <= other.m_val;
//                 }

//                 int operator*() const 
//                 {
//                     return m_val;
//                 }

//                 void operator++() 
//                 {
//                     ++m_val;
//                 }

//             private:
//                 size_t m_val;
//         };

//         Iterator begin() 
//         {
//             return Iterator(1);
//         }

//         Iterator end() 
//         {
//             return Iterator(m_limit);
//         }

//     private:
//         size_t m_limit;
// };