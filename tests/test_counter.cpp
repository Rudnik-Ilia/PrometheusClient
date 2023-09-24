#include "../include/counter.hpp"
#include "../include/anxilary_functions.hpp"

using vectorStr = std::vector<std::string>;

int main()
{
    auto c1 = std::make_shared<Counter<double>>(vectorStr{"get", "post"});

    LOG(c1->GetValue());
    c1->Inc();
    LOG(c1->GetValue());
    c1->Inc(5.7);
    LOG(c1->GetValue());
    

    return 0;
}
