#pragma 

#include <memory>

#include "init.hpp"
#include "counter.hpp"

template<class T>
class Metric
{
    public:
        Metric(std::string&& name, std::string&& help, std::vector<std::string>&& label_names = {}): 
        m_name(std::move(name)), 
        m_help(std::move(help)), 
        m_label_names(std::move(label_names))
        {}

        ~Metric(){}

        template <typename... Args>
        std::shared_ptr<T> Build(Args&& ...args)
        {
            auto metric = std::make_shared<T>(std::move(std::vector{args})...);
            m_storage.push_back(metric);
            return metric;
        }

        void Show()
        {
            for(auto& iter : m_storage)
            {
                std::cout << m_name << ' ' << m_label_names[0] << '=' << iter->GetLabels()[0] << " { " << iter->GetValue() << " }"<< std::endl;
            }
        }

    private:
        std::string m_name;
        std::string m_help;
        std::vector<std::string> m_label_names;
        std::vector<std::shared_ptr<T>> m_storage;
};

        // Return a smart pointer on Counter, Gauge, Histogram
        // template <typename... Args>
        // std::unique_ptr<T> Build(Args&& ...args)
        // {
        //     auto metric = std::make_unique<T>(std::move(args)...);
        //     // m_storage.push_back(metric);
        //     return metric;
        // }
        // template <typename U, typename... Args>
        // static std::unique_ptr<Metric<U>> create(Args ...args) 
        // {
        //     return std::make_unique<Metric<U>>(std::forward(args...));
        // }