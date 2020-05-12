#pragma once

#include "dist.hpp"
#include "mfl/abstract_font_face.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace mfl
{
    struct font_parameters
    {
        points size{0};
        dist_t x_height = 0;
        dist_t capital_m_width = 0;
        math_constants math_info;
    };

    class font_library
    {
    public:
        explicit font_library(const points font_size, font_face_creator create_face);

        [[nodiscard]] const abstract_font_face& get_face(const font_family family, const points size) const;
        [[nodiscard]] const font_parameters& normal_parameters() const { return normal_parameters_; }
        [[nodiscard]] const font_parameters& small_parameters() const { return small_parameters_; }
        [[nodiscard]] const font_parameters& tiny_parameters() const { return tiny_parameters_; }

    private:
        mutable std::vector<std::pair<font_family, std::unique_ptr<abstract_font_face>>> cached_faces_;
        font_face_creator create_face_;
        font_parameters normal_parameters_;
        font_parameters small_parameters_;
        font_parameters tiny_parameters_;
    };
}