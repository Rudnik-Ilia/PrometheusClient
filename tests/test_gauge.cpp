#include "../include/gauge.hpp"
#include "anxilary_functions.hpp"


int main()
{
    Gauge g1({"get", "post"});
    LOG(g1.GetValue());


    return 0;
}