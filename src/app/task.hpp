#pragma once

#include <string>
#include <vector>

namespace app {

struct Task {
    std::string operation; // "add", "subtract", "multiply", "divide", "power", "factorial"
    std::vector<int> operands;
};

} // namespace app