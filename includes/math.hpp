#pragma once

#include <array>
#include <iostream>

#include <cmath>
#include <algorithm>

namespace math {
    template<typename T, size_t N> class vector;

    template<typename T, size_t N> T operator*(const vector<T, N>&, const vector<T, N>&);
    template<typename T, size_t N> std::ostream& operator<<(std::ostream&, const vector<T, N>&);

    template<typename T, size_t N>
    class vector {
        private:

        std::array<T, N> data = {};

        public:
        
        vector();
        vector(const std::array<T, N>&);
        template<typename U>
        vector(std::initializer_list<U>);

        T& operator[](int);
        const T& operator[](int) const;

        vector& operator+=(const vector&);

        vector& operator-=(const vector&);

        template<typename U>
        vector& operator*=(const U&);
        template<typename U>
        vector& operator/=(const U&);

        friend T operator* <>(const vector&, const vector&);
        friend std::ostream& operator<< <>(std::ostream&, const vector&);

        vector& norm();
        T length() const;
    };

    using vec2f = vector<float, 2>;
    using vec2d = vector<double, 2>;
    
    using vec3f = vector<float, 3>;
    using vec3d = vector<double, 3>;
}

namespace math {
    template<typename T, size_t N> 
    vector<T, N>::vector() {};

    template<typename T, size_t N> 
    vector<T, N>::vector(const std::array<T, N> &_data) : data(_data) {};

    template<typename T, size_t N>
    template<typename U>
    vector<T, N>::vector(std::initializer_list<U> list) {
        std::copy_n(list.begin(), std::min(list.size(), N), data.begin());
    }

    template<typename T, size_t N>
    T& vector<T, N>::operator[](int i) { return data[i]; }

    template<typename T, size_t N>
    const T& vector<T, N>::operator[](int i) const { return data[i]; }

    template<typename T, size_t N> 
    vector<T, N>& vector<T, N>::operator+=(const vector<T, N> &b) {
        for (int i = 0; i < N; ++i) {
            data[i] += b.data[i];
        }
        return *this;
    }

    template<typename T, size_t N> 
    vector<T, N>& vector<T, N>::operator-=(const vector<T, N> &b) {
        for (int i = 0; i < N; ++i) {
            data[i] -= b.data[i];
        }
        return *this;
    }

    template<typename T, size_t N> 
    vector<T, N> operator+(vector<T, N> a, const vector<T, N> &b) {
        return (a += b);
    }

    template<typename T, size_t N> 
    vector<T, N> operator-(vector<T, N> a, const vector<T, N> &b) {
        return (a -= b);
    }

    template<typename T, size_t N>
    template<typename U>
    vector<T, N>& vector<T, N>::operator*=(const U& x) {
        for (int i = 0; i < N; ++i) {
            data[i] *= x;
        }
        return *this;
    }

    template<typename T, size_t N>
    template<typename U>
    vector<T, N>& vector<T, N>::operator/=(const U& x) {
        for (int i = 0; i < N; ++i) {
            data[i] /= x;
        }
        return *this;
    }

    template<typename T, size_t N, typename U>
    vector<T, N> operator*(vector<T, N> a, const U &b) { return (a *= b); }
    
    template<typename T, size_t N, typename U>
    vector<T, N> operator*(const U &b, vector<T, N> a) { return (a *= b); }

    template<typename T, size_t N> 
    T operator*(const vector<T, N> &a, const vector<T, N> &b) {
        T res = 0;
        for (int i = 0; i < N; ++i) {
            res += a.data[i] * b.data[i];
        }
        return res;
    }

    template<typename T, size_t N> 
    std::ostream& operator<<(std::ostream &out, const vector<T, N> &a) {
        out << "(";
        for (int i = 0; i < N; ++i) {
            out << a.data[i] << (i + 1 == N ? ")" : ", ");
        }
        return out;
    }
    
    template<typename T, size_t N> 
    inline T vector<T, N>::length() const {
        return std::sqrt(*this * *this);
    }
    template<typename T, size_t N> 
    inline T length(const vector<T, N> &v) {
        return v.length();
    }

    template<typename T, size_t N> 
    inline vector<T, N>& vector<T, N>::norm() {
        return (*this) /= length();
    }
    template<typename T, size_t N> 
    inline vector<T, N> norm(vector<T, N> v) {
        return v.norm();
    }

    template<typename T>
    inline T clamp(const T &x, const T &a, const T &b) {
        return std::min(std::max(x, a), b);
    }
    
    template<typename T>
    T mix(T a, T b, T h) {
        return a * (1 - h) + b * h;
    }

    template<typename T>
    inline T smooth_min(const T &a, const T &b, const T &k) {
        T h = clamp(0.5 + 0.5 * (b - a), 0.0, 1.0);
        return mix(b, a, h) - k * h * (1.0 - h);
    }

    template<typename T>
    inline T smooth_min(const T &k, const std::initializer_list<T> &list) {
        auto it = list.begin();
        T res = *it;
        for (++it; it != list.end(); ++it) {
            res = smooth_min(res, *it, k);
        }

        return res;
    }   
}
