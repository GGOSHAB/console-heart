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

class plane : public rm_object {
public:
    math::vec3d n = {};
    double d = {};

    plane() {};
    plane(math::vec3d _n, double _d) : n(_n.norm()), d(_d) {};

    double dist(const math::vec3d &p) const override {
        return p * n + d;
    }
};

class heart : public rm_object {
public:
    math::vec3d c = {};
    double r = {};

    heart() {};
    heart(math::vec3d _c, double _r) : c(_c), r(_r) {};

    double dist(const math::vec3d &_p) const override {
        auto p = _p;
        p -= c;
        p /= r;
        
        double x = p[0], y = p[1], z = p[2];
        
        double rad = 15.0;
        z *= 2 - y / 15.0;
        y = 4 + 1.2 * y - std::abs(x) * std::sqrt(std::max(0.0, (20.0 - std::abs(x)) / 15.0));
        return r * (math::length(math::vec3d{x, y, z}) - rad);
    }
};

class letter_I : public rm_object {
public:
    math::vec3d c = {};
    double r = 0;

    letter_I() {};
    letter_I(math::vec3d _c, double _r) : c(_c), r(_r) {};

    double dist(const math::vec3d &_p) const override {
        auto p = _p;
        p -= c;
        p /= r;

        p[1] = std::max(0.0, std::abs(p[1]) - 1.5);
        return r * (p.length() - 0.3);
    }
};

class letter_U : public rm_object {
public:
    math::vec3d c = {};
    double r = 0;

    letter_U() {};
    letter_U(math::vec3d _c, double _r) : c(_c), r(_r) {};

    double dist(const math::vec3d &_p) const override {
        const double l = 2.0, d = 1.0, sr = 0.3;
        
        auto p = _p;
        p -= (c - math::vec3d{0.0, r * (l - d) / 2.0, 0.0});
        p /= r;
        p[0] = std::abs(p[0]);

        if (p[1] > l) {
            return r * (math::length(p - math::vec3d{d, l, 0.0}) - sr);
        } else if (p[1] > 0.0) {
            double d_line = std::abs(p[0] - d);
            return r * (std::hypot(d_line, p[2]) - sr);
        } else {
            double d_circle = std::hypot(p[0], p[1]) - d;
            return r * (std::hypot(d_circle, p[2]) - sr);
        }
    }
};