#include "framework/svg_comparator.hpp"

namespace mfl
{
    bool svg_comparator::contentsAreEquivalent(const std::string received_path, const std::string approved_path) const
    {
        std::ifstream astream(approved_path.c_str(), std::ios::binary | std::ifstream::in);
        std::ifstream rstream(received_path.c_str(), std::ios::binary | std::ifstream::in);
        if (!astream.good() or !rstream.good()) return false;

        std::string aline;
        std::string rline;
        for (; std::getline(astream, aline);)
        {
            if (!std::getline(rstream, rline)) return false;

            const auto ignore_line = (aline.starts_with("<g id=\"surface") && rline.starts_with("<g id=\"surface"));
            if (!ignore_line && (aline != rline)) return false;
        }

        return !static_cast<bool>(std::getline(rstream, rline));
    }
}
