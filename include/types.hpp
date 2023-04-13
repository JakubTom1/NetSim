#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <cstdlib>
#include <functional>

using ElementID = int;

using Time = int;

using TimeOffset = int;

using  ProbabilityGenerator = std::function<double()>;

#endif