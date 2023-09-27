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
        std::unique_ptr<T> Build(Args&& ...args)
        {
            // m_metric = std::make_unique<T>(std::move(args)...);
            // return m_metric;
            return std::make_unique<T>(std::move(args)...);
        }

        template <typename U, typename... Args>
        static std::unique_ptr<Metric<U>> create(Args ...args) 
        {
            return std::make_unique<Metric<U>>(std::forward(args...));
        }

    private:
        std::string m_name;
        std::string m_help;
        std::vector<std::string> m_label_names;

        // std::unique_ptr<T> m_metric;
};

