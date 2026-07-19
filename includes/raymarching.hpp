#pragma once

#include <cmath>
#include <string>
#include <array>
#include "math.hpp"

class rm_object {
private:
    std::array<int, 3> color = {};
public:
    virtual ~rm_object() = default;

    void set_color(int r, int g, int b) { color = std::to_array({r, g, b}); };
    std::string color_str();
    virtual double dist(const math::vec3d &p) const = 0;
};

class sphere : public rm_object {
public:
    math::vec3d c = {};
    double r = {};

    sphere() {};
    sphere(math::vec3d _c, double _r) : c(_c), r(_r) {};

    double dist(const math::vec3d &p) const override {
        return math::length(p - c) - r;
    }
};