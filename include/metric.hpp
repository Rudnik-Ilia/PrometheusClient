#pragma 

#include <memory>

#include "init.hpp"
#include "counter.hpp"

template<class T>
class Metric
{
    public:
        Metric(std::string name, std::string help, std::vector<std::string> label_names): 
        m_name(std::move(name)), 
        m_help(std::move(help)), 
        m_label_names(std::move(label_names))
        {}

        ~Metric()
        {}

        template <typename... Args>
        std::shared_ptr<T> Build(Args ...args)
        {
            m_metric = std::make_shared<T>(std::move(args)...);
            return m_metric;
        }

    private:
        std::string m_name;
        std::string m_help;
        std::vector<std::string> m_label_names;
        std::shared_ptr<T> m_metric;
};

