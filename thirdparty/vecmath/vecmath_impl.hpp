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
#ifndef SANDBOX_VECMATH_IMPL_H
#define SANDBOX_VECMATH_IMPL_H

#ifndef SANDBOX_VECMATH_IMPL_INCLUDE
#error "Do not include vecmath_impl.hpp directly; include vecmath.hpp instead."
#endif

#ifdef __CUDACC__
#define CUDA_DEVICE __host__ __device__
#define NDEBUG
#else
#define CUDA_DEVICE
#endif

#include <cassert>
#include <cmath>

namespace vecmath {

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>::Vector() { }

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>::Vector(Scalar x_, Scalar y_, Scalar z_) : x(x_), y(y_), z(z_), w(0.0) { }

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>::Vector(Scalar x_, Scalar y_, Scalar z_, Scalar w_) : x(x_), y(y_), z(z_), w(w_) { }

template <typename Scalar>
inline CUDA_DEVICE Scalar& Vector<Scalar>::operator[](int index) {
    assert(index >= 0 && index <= 3);
    return (&x)[index];
}

template <typename Scalar>
inline CUDA_DEVICE Scalar Vector<Scalar>::operator[](int index) const {
    assert(index >= 0 && index <= 3);
    return (&x)[index];
}

template <typename Scalar>
inline CUDA_DEVICE Scalar Vector<Scalar>::length() const {
    return std::sqrt(length_squared());
}

template <typename Scalar>
inline CUDA_DEVICE Scalar Vector<Scalar>::length_squared() const {
    return x * x + y * y + z * z;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> Vector<Scalar>::normalize() const {
    return *this / length();
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator+(const Vector<Scalar>& v1, const Vector<Scalar>& v2) {
    return Vector<Scalar>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>& operator+=(Vector<Scalar>& v1, const Vector<Scalar>& v2) {
    v1.x += v2.x; v1.y += v2.y; v1.z += v2.z;
    return v1;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator-(const Vector<Scalar>& v1, const Vector<Scalar>& v2) {
    return Vector<Scalar>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>& operator-=(Vector<Scalar>& v1, const Vector<Scalar>& v2) {
    v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z;
    return v1;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator-(const Vector<Scalar>& v) {
    return Vector<Scalar>(-v.x, -v.y, -v.z);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator*(const Vector<Scalar>& v, Scalar f) {
    return Vector<Scalar>(v.x * f, v.y * f, v.z * f);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator*(Scalar f, const Vector<Scalar>& v) {
    return v * f;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>& operator*=(Vector<Scalar>& v, Scalar f) {
    v.x *= f; v.y *= f; v.z *= f;
    return v;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator/(const Vector<Scalar>& v, Scalar f) {
    return Vector<Scalar>(v.x / f, v.y / f, v.z / f);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar>& operator/=(Vector<Scalar>& v, Scalar f) {
    v.x /= f; v.y /= f; v.z /= f;
    return v;
}

template <typename Scalar>
inline CUDA_DEVICE Scalar dot(const Vector<Scalar>& v1, const Vector<Scalar>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> cross(const Vector<Scalar>& v1, const Vector<Scalar>& v2) {
    return Vector<Scalar>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar>::Point() { }

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar>::Point(Scalar x_, Scalar y_, Scalar z_) : x(x_), y(y_), z(z_), w(1.0) { }

template <typename Scalar>
inline CUDA_DEVICE Scalar& Point<Scalar>::operator[](int index) {
    assert(index >= 0 && index <= 3);
    return (&x)[index];
}

template <typename Scalar>
inline CUDA_DEVICE Scalar Point<Scalar>::operator[](int index) const {
    assert(index >= 0 && index <= 3);
    return (&x)[index];
}

template <typename Scalar>
inline CUDA_DEVICE Scalar Point<Scalar>::distance(const Point<Scalar>& p) const {
    return (*this - p).length();
}

template <typename Scalar>
inline CUDA_DEVICE Scalar Point<Scalar>::distance_squared(const Point<Scalar>& p) const {
    return (*this - p).length_squared();
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar> operator+(const Point<Scalar>& p, const Vector<Scalar>& v) {
    return Point<Scalar>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar>& operator+=(Point<Scalar>& p, const Vector<Scalar>& v) {
    p.x += v.x; p.y += v.y; p.z += v.z;
    return p;
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar> operator-(const Point<Scalar>& p, const Vector<Scalar>& v) {
    return Point<Scalar>(p.x - v.x, p.y - v.y, p.z - v.z);
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar>& operator-=(Point<Scalar>& p, const Vector<Scalar>& v) {
    p.x -= v.x; p.y -= v.y; p.z -= v.z;
    return p;
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> operator-(const Point<Scalar>& p1, const Point<Scalar>& p2) {
    return Vector<Scalar>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

template <typename Scalar>
inline CUDA_DEVICE Ray<Scalar>::Ray(const Point<Scalar>& origin_, const Vector<Scalar>& direction_) : origin(origin_), direction(direction_) { }

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar> Ray<Scalar>::point_at(Scalar t) const {
    return origin + direction * t;
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::identity() {
    return Matrix<Scalar>(
        Vector<Scalar>(1.0, 0.0, 0.0, 0.0),
        Vector<Scalar>(0.0, 1.0, 0.0, 0.0),
        Vector<Scalar>(0.0, 0.0, 1.0, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::translate(const Vector<Scalar>& v) {
    return Matrix<Scalar>(
        Vector<Scalar>(1.0, 0.0, 0.0, v.x),
        Vector<Scalar>(0.0, 1.0, 0.0, v.y),
        Vector<Scalar>(0.0, 0.0, 1.0, v.z));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::rotate_x(Scalar angle) {
    Scalar c = std::cos(angle);
    Scalar s = std::sin(angle);

    return Matrix<Scalar>(
        Vector<Scalar>(1.0, 0.0, 0.0, 0.0),
        Vector<Scalar>(0.0,   c,  -s, 0.0),
        Vector<Scalar>(0.0,   s,   c, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::rotate_y(Scalar angle) {
    Scalar c = std::cos(angle);
    Scalar s = std::sin(angle);

    return Matrix<Scalar>(
        Vector<Scalar>(  c, 0.0,   s, 0.0),
        Vector<Scalar>(0.0, 1.0, 0.0, 0.0),
        Vector<Scalar>( -s, 0.0,   c, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::rotate_z(Scalar angle) {
    Scalar c = std::cos(angle);
    Scalar s = std::sin(angle);

    return Matrix<Scalar>(
        Vector<Scalar>(  c,  -s, 0.0, 0.0),
        Vector<Scalar>(  s,   c, 0.0, 0.0),
        Vector<Scalar>(0.0, 0.0, 1.0, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::rotate(const Vector<Scalar>& axis, Scalar angle) {
    Vector<Scalar> a = axis.normalize();

    Scalar c = std::cos(angle);
    Scalar s = std::sin(angle);
    Scalar cc = 1.0 - c;

    Scalar t1 = a.x * a.y * cc;
    Scalar t2 = a.x * a.z * cc;
    Scalar t3 = a.y * a.z * cc;

    Scalar u1 = a.x * s;
    Scalar u2 = a.y * s;
    Scalar u3 = a.z * s;

    return Matrix<Scalar>(
        Vector<Scalar>(a.x * a.x * cc + c, t1 - u3, t2 + u2, 0.0),
        Vector<Scalar>(t1 + u3, a.y * a.y * cc + c, t3 - u1, 0.0),
        Vector<Scalar>(t2 - u2, t3 + u1, a.z * a.z * cc + c, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::scale(Scalar f) {
    return Matrix<Scalar>(
        Vector<Scalar>(  f, 0.0, 0.0, 0.0),
        Vector<Scalar>(0.0,   f, 0.0, 0.0),
        Vector<Scalar>(0.0, 0.0,   f, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::scale(Scalar fx, Scalar fy, Scalar fz) {
    return Matrix<Scalar>(
        Vector<Scalar>( fx, 0.0, 0.0, 0.0),
        Vector<Scalar>(0.0,  fy, 0.0, 0.0),
        Vector<Scalar>(0.0, 0.0,  fz, 0.0));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar> Matrix<Scalar>::operator*(const Matrix<Scalar>& m) const {
    Vector<Scalar> r0_(
        r0.x * m.r0.x + r0.y * m.r1.x + r0.z * m.r2.x,
        r0.x * m.r0.y + r0.y * m.r1.y + r0.z * m.r2.y,
        r0.x * m.r0.z + r0.y * m.r1.z + r0.z * m.r2.z,
        r0.x * m.r0.w + r0.y * m.r1.w + r0.z * m.r2.w + r0.w);

    Vector<Scalar> r1_(
        r1.x * m.r0.x + r1.y * m.r1.x + r1.z * m.r2.x,
        r1.x * m.r0.y + r1.y * m.r1.y + r1.z * m.r2.y,
        r1.x * m.r0.z + r1.y * m.r1.z + r1.z * m.r2.z,
        r1.x * m.r0.w + r1.y * m.r1.w + r1.z * m.r2.w + r1.w);

    Vector<Scalar> r2_(
        r2.x * m.r0.x + r2.y * m.r1.x + r2.z * m.r2.x,
        r2.x * m.r0.y + r2.y * m.r1.y + r2.z * m.r2.y,
        r2.x * m.r0.z + r2.y * m.r1.z + r2.z * m.r2.z,
        r2.x * m.r0.w + r2.y * m.r1.w + r2.z * m.r2.w + r2.w);

    return Matrix<Scalar>(r0_, r1_, r2_);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> Matrix<Scalar>::transform(const Vector<Scalar>& v) const {
    return Vector<Scalar>(dot(r0, v), dot(r1, v), dot(r2, v));
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar> Matrix<Scalar>::transform(const Point<Scalar>& p) const {
    return Point<Scalar>(
        r0.x * p.x + r0.y * p.y + r0.z * p.z + r0.w,
        r1.x * p.x + r1.y * p.y + r1.z * p.z + r1.w,
        r2.x * p.x + r2.y * p.y + r2.z * p.z + r2.w);
}

template <typename Scalar>
inline CUDA_DEVICE Ray<Scalar> Matrix<Scalar>::transform(const Ray<Scalar>& r) const {
    return Ray<Scalar>(transform(r.origin), transform(r.direction));
}

template <typename Scalar>
inline CUDA_DEVICE Matrix<Scalar>::Matrix(const Vector<Scalar>& r0_, const Vector<Scalar>& r1_, const Vector<Scalar>& r2_) : r0(r0_), r1(r1_), r2(r2_) { }

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::identity() {
    Matrix<Scalar> id = Matrix<Scalar>::identity();
    return Transform<Scalar>(id, id);
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::translate(const Vector<Scalar>& v) {
    return Transform<Scalar>(Matrix<Scalar>::translate(v), Matrix<Scalar>::translate(-v));
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::rotate_x(Scalar angle) {
    return Transform<Scalar>(Matrix<Scalar>::rotate_x(angle), Matrix<Scalar>::rotate_x(-angle));
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::rotate_y(Scalar angle) {
    return Transform<Scalar>(Matrix<Scalar>::rotate_y(angle), Matrix<Scalar>::rotate_y(-angle));
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::rotate_z(Scalar angle) {
    return Transform<Scalar>(Matrix<Scalar>::rotate_z(angle), Matrix<Scalar>::rotate_z(-angle));
}
template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::rotate(const Vector<Scalar>& axis, Scalar angle) {
    return Transform<Scalar>(Matrix<Scalar>::rotate(axis, angle), Matrix<Scalar>::rotate(axis, -angle));
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::scale(Scalar f) {
    return Transform<Scalar>(Matrix<Scalar>::scale(f), Matrix<Scalar>::scale(1.0 / f));
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::scale(Scalar fx, Scalar fy, Scalar fz) {
    return Transform<Scalar>(Matrix<Scalar>::scale(fx, fy, fz), Matrix<Scalar>::scale(1.0 / fx, 1.0 / fy, 1.0 / fz));
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::operator*(const Transform<Scalar>& t) const {
    return Transform<Scalar>(mat * t.mat, t.inv * inv);
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar> Transform<Scalar>::inverse() const {
    return Transform<Scalar>(inv, mat);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> Transform<Scalar>::transform(const Vector<Scalar>& v) const {
    return mat.transform(v);
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar> Transform<Scalar>::transform(const Point<Scalar>& p) const {
    return mat.transform(p);
}

template <typename Scalar>
inline CUDA_DEVICE Ray<Scalar> Transform<Scalar>::transform(const Ray<Scalar>& r) const {
    return mat.transform(r);
}

template <typename Scalar>
inline CUDA_DEVICE Vector<Scalar> Transform<Scalar>::inverse_transform(const Vector<Scalar>& v) const {
    return inv.transform(v);
}

template <typename Scalar>
inline CUDA_DEVICE Point<Scalar> Transform<Scalar>::inverse_transform(const Point<Scalar>& p) const {
    return inv.transform(p);
}

template <typename Scalar>
inline CUDA_DEVICE Ray<Scalar> Transform<Scalar>::inverse_transform(const Ray<Scalar>& r) const {
    return inv.transform(r);
}

template <typename Scalar>
inline CUDA_DEVICE Transform<Scalar>::Transform(const Matrix<Scalar>& matrix, const Matrix<Scalar>& inverse) : mat(matrix), inv(inverse) { }

} // namespace vecmath

#endif // SANDBOX_VECMATH_IMPL_H
