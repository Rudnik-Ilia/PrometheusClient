#pragma 

#include "init.hpp"

template<class T>
class Metric
{
    public:
        Metric()
        {

        }
        ~Metric()
        {

        }
    private:
        std::string m_name;
        std::string m_help;
        std::vector<std::string> m_label_names;
};

