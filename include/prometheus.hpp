#pragma once

#include "group.hpp"
#include "holder.hpp"
#include "singleton.hpp"

template <template <typename> class Type, class T = int64_t>
class prometheus
{
public:
    static std::shared_ptr<Group<Type<T>>> GetGroup(
        std::string name,
        std::string help,
        std::vector<std::string> label_names = {})
    {
        auto group_ptr = std::make_shared<Group<Type<T>>>(
            std::move(name),
            std::move(help),
            std::move(label_names));

        Singleton<Holder>::GetInstance()->RegistrateMetric(group_ptr);

        return group_ptr;
    }

    static std::shared_ptr<Type<T>> GetMetric(
        std::string name,
        std::string help,
        std::vector<std::string> label_names = {},
        std::vector<std::string> label_values = {})
    {
        auto group_ptr = std::make_shared<Group<Type<T>>>(
            std::move(name),
            std::move(help),
            std::move(label_names));

        Singleton<Holder>::GetInstance()->RegistrateMetric(group_ptr);

        return group_ptr->Build(std::move(label_values));
    }
};