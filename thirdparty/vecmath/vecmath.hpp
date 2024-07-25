/*
 * Copyright 2011 Jesper de Jong
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SANDBOX_VECMATH_H
#define SANDBOX_VECMATH_H

namespace vecmath {

template <typename Scalar> struct Vector;
template <typename Scalar> struct Point;
template <typename Scalar> struct Ray;
template <typename Scalar> class Matrix;
template <typename Scalar> class Transform;

// Vector template
template <typename Scalar>
struct Vector {
    Scalar x, y, z, w;

    Vector();
    Vector(Scalar x_, Scalar y_, Scalar z_);

    Scalar& operator[](int index);
    Scalar operator[](int index) const;

    Scalar length() const;
    Scalar length_squared() const;
    Vector<Scalar> normalize() const;

private:
    // Constructor used by Matrix
    Vector(Scalar x_, Scalar y_, Scalar z_, Scalar w_);

    friend class Matrix<Scalar>;
};

template <typename Scalar> Vector<Scalar> operator+(const Vector<Scalar>& v1, const Vector<Scalar>& v2);
template <typename Scalar> Vector<Scalar>& operator+=(Vector<Scalar>& v1, const Vector<Scalar>& v2);

template <typename Scalar> Vector<Scalar> operator-(const Vector<Scalar>& v1, const Vector<Scalar>& v2);
template <typename Scalar> Vector<Scalar>& operator-=(Vector<Scalar>& v1, const Vector<Scalar>& v2);

template <typename Scalar> Vector<Scalar> operator-(const Vector<Scalar>& v);

template <typename Scalar> Vector<Scalar> operator*(const Vector<Scalar>& v, Scalar f);
template <typename Scalar> Vector<Scalar> operator*(Scalar f, const Vector<Scalar>& v);
template <typename Scalar> Vector<Scalar>& operator*=(Vector<Scalar>& v, Scalar f);

template <typename Scalar> Vector<Scalar> operator/(const Vector<Scalar>& v, Scalar f);
template <typename Scalar> Vector<Scalar>& operator/=(Vector<Scalar>& v, Scalar f);

template <typename Scalar> Scalar dot(const Vector<Scalar>& v1, const Vector<Scalar>& v2);
template <typename Scalar> Vector<Scalar> cross(const Vector<Scalar>& v1, const Vector<Scalar>& v2);

// Point template
template <typename Scalar>
struct Point {
    Scalar x, y, z, w;

    Point();
    Point(Scalar x_, Scalar y_, Scalar z_);

    Scalar& operator[](int index);
    Scalar operator[](int index) const;

    Scalar distance(const Point<Scalar>& p) const;
    Scalar distance_squared(const Point<Scalar>& p) const;
};

template <typename Scalar> Point<Scalar> operator+(const Point<Scalar>& p, const Vector<Scalar>& v);
template <typename Scalar> Point<Scalar>& operator+=(Point<Scalar>& p, const Vector<Scalar>& v);

template <typename Scalar> Point<Scalar> operator-(const Point<Scalar>& p, const Vector<Scalar>& v);
template <typename Scalar> Point<Scalar>& operator-=(Point<Scalar>& p, const Vector<Scalar>& v);

template <typename Scalar> Vector<Scalar> operator-(const Point<Scalar>& p1, const Point<Scalar>& p2);

// Ray template
template <typename Scalar>
struct Ray {
    Point<Scalar> origin;
    Vector<Scalar> direction;

    Ray(const Point<Scalar>& origin_, const Vector<Scalar>& direction_);

    Point<Scalar> point_at(Scalar t) const;
};

// Matrix template
template <typename Scalar>
class Matrix {
public:
    static Matrix<Scalar> identity();

    static Matrix<Scalar> translate(const Vector<Scalar>& v);

    static Matrix<Scalar> rotate_x(Scalar angle);
    static Matrix<Scalar> rotate_y(Scalar angle);
    static Matrix<Scalar> rotate_z(Scalar angle);
    static Matrix<Scalar> rotate(const Vector<Scalar>& axis, Scalar angle);

    static Matrix<Scalar> scale(Scalar f);
    static Matrix<Scalar> scale(Scalar fx, Scalar fy, Scalar fz);

    Matrix<Scalar> operator*(const Matrix<Scalar>& m) const;

    Vector<Scalar> transform(const Vector<Scalar>& v) const;
    Point<Scalar> transform(const Point<Scalar>& p) const;
    Ray<Scalar> transform(const Ray<Scalar>& r) const;

private:
    Matrix(const Vector<Scalar>& r0_, const Vector<Scalar>& r1_, const Vector<Scalar>& r2_);

    Vector<Scalar> r0, r1, r2;
};

// Transform template
template <typename Scalar>
class Transform {
public:
    static Transform<Scalar> identity();

    static Transform<Scalar> translate(const Vector<Scalar>& v);

    static Transform<Scalar> rotate_x(Scalar angle);
    static Transform<Scalar> rotate_y(Scalar angle);
    static Transform<Scalar> rotate_z(Scalar angle);
    static Transform<Scalar> rotate(const Vector<Scalar>& axis, Scalar angle);

    static Transform<Scalar> scale(Scalar f);
    static Transform<Scalar> scale(Scalar fx, Scalar fy, Scalar fz);

    Transform<Scalar> operator*(const Transform<Scalar>& t) const;

    Transform<Scalar> inverse() const;

    Vector<Scalar> transform(const Vector<Scalar>& v) const;
    Point<Scalar> transform(const Point<Scalar>& p) const;
    Ray<Scalar> transform(const Ray<Scalar>& r) const;

    Vector<Scalar> inverse_transform(const Vector<Scalar>& v) const;
    Point<Scalar> inverse_transform(const Point<Scalar>& p) const;
    Ray<Scalar> inverse_transform(const Ray<Scalar>& r) const;

private:
    Transform(const Matrix<Scalar>& matrix, const Matrix<Scalar>& inverse);

    Matrix<Scalar> mat, inv;
};

} // namespace vecmath

#define SANDBOX_VECMATH_IMPL_INCLUDE
#include "vecmath_impl.hpp"
#undef SANDBOX_VECMATH_IMPL_INCLUDE

// Include SSE-optimized implementation unless disabled or compiling for CUDA
#if !defined(SANDBOX_NO_SSE) && !defined(__CUDACC__)
#include "vecmath_sse.hpp"
#endif

#endif // SANDBOX_VECMATH_H
