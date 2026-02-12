/*
  $$=========================$$
  ||       Math (Vec2)       ||
  $$=========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a 2D vector with two 32-bit floating-point (f32) values.
 */
CBNDEF_T(CBN_Vec2) {
  union {
    struct { f32 x, y; };
    f32 c[2];
  };
};
CBNDEF_AKA(cbn::math, Vec2, CBN_Vec2);

/**
 * @brief Creates a 2D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @return The newly created 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2(f32 x, f32 y);

/**
 * @brief Creates a 2D vector.
 * @param x The value to assign to both the X and Y fields.
 * @return The newly created 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_1(f32 x);

/**
 * @brief Creates a 2D vector (from polar coordinate system).
 * @param r The radial distance.
 * @param phi The polar angle in radians.
 * @return The newly created 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_p(f32 r, f32 phi);

/**
 * @brief Adds two 2D vectors together (element-wise).
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_add(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Subtracts one 2D vector from another one (element-wise).
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_sub(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Computes the Hadamard product (element-wise multiplication) between two 2D vectors.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_mult(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Computes the dot product between two 2D vectors.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @return The resultant scalar value.
 */
CBNDEF f32 carbon_math_vec2_dot(CBN_Vec2 u, CBN_Vec2 v);

/**
 * @brief Scales the 2D vector by the specified scalar value.
 * @param v The 2D vector.
 * @param s The scalar value.
 * @return The scaled 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_scale(CBN_Vec2 v, f32 s);

/**
 * @brief Returns the length of the 2D vector squared.
 * @param v The 2D vector.
 * @return The 2D vector's length squared.
 */
CBNDEF f32 carbon_math_vec2_len_squared(CBN_Vec2 v);

/**
 * @brief Returns the length of the 2D vector.
 * @param v The 2D vector.
 * @return The 2D vector's length.
 */
CBNDEF f32 carbon_math_vec2_len(CBN_Vec2 v);

/**
 * @brief Returns a 2D vector with the same direction as the specified one, but with a length of `1`.
 * @param v The 2D vector.
 * @return The normalized 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_norm(CBN_Vec2 v);

/**
 * @brief Restricts a 2D vector between a minimum and a maximum value.
 * @param v The 2D vector.
 * @param min The minimum value (as 2D vector).
 * @param max The maximum value (as 2D vector).
 * @return The restricted 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_clamp(CBN_Vec2 v, CBN_Vec2 min, CBN_Vec2 max);

/**
 * @brief Applies floor(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_floor(CBN_Vec2 v);

/**
 * @brief Applies abs(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_abs(CBN_Vec2 v);

/**
 * @brief Applies e^x function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_exp(CBN_Vec2 v);

/**
 * @brief Applies sin(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_sin(CBN_Vec2 v);

/**
 * @brief Applies cos(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_cos(CBN_Vec2 v);

/**
 * @brief Applies tan(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_tan(CBN_Vec2 v);

/**
 * @brief Applies tanh(x) function to the 2D vector (element-wise).
 * @param v The 2D vector.
 * @return The resultant 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_tanh(CBN_Vec2 v);

/**
 * @brief Performs a linear interpolation between two 2D vectors based on the given weighting.
 * @param u The first 2D vector.
 * @param v The second 2D vector.
 * @param t A value [0..1] indicating the weight of `v`.
 * @return The interpolated 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_lerp(CBN_Vec2 u, CBN_Vec2 v, f32 t);

/**
 * @brief Rotates the 2D vector by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @return The rotated 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_rotate(CBN_Vec2 v, f32 angle);

/**
 * @brief Rotates the 2D vector around a given pivot point by the specified rotation value (in degrees).
 * @param v The 2D vector.
 * @param angle The rotation to apply (in degrees).
 * @param pivot The pivot point around which to rotate the 2D vector.
 * @return The rotated 2D vector.
 */
CBNDEF CBN_Vec2 carbon_math_vec2_rotate_around_pivot(CBN_Vec2 v, f32 angle, CBN_Vec2 pivot);

/**
 * @brief Returns the string representation of the 2D vector using default formatting.
 * @param v The 2D vector.
 * @return The serialized 2D vector as `(X, Y)`.
 */
CBNDEF char *carbon_math_vec2_to_cstr(CBN_Vec2 v);

#ifdef __cplusplus
struct CBN_Vec3;  // Forward declaration
struct CBN_Vec4;  // Forward declaration

struct CBN_Vec2 : CBN_Vec2_t {
  constexpr CBN_Vec2(f32 _x, f32 _y) : CBN_Vec2_t{.c = {_x, _y}} {}
  constexpr CBN_Vec2(f32 _x = 0) : CBN_Vec2(_x, _x) {}

  CBN_Vec2 operator+(const CBN_Vec2 &v) const {
    return carbon_math_vec2_add(*this, v);
  }
  
  CBN_Vec2 operator+(f32 s) const {
    return CBN_Vec2(x + s, y + s);
  }
  
  friend CBN_Vec2 operator+(f32 s, const CBN_Vec2 &v) {
    return v + s;
  }
  
  CBN_Vec2 &operator+=(const CBN_Vec2 &v) {
    *this = *this + v;
    return *this;
  }
  
  CBN_Vec2 &operator+=(f32 s) {
    *this = *this + s;
    return *this;
  }
  
  CBN_Vec2 operator-(const CBN_Vec2 &v) const {
    return carbon_math_vec2_sub(*this, v);
  }
  
  CBN_Vec2 operator-(f32 s) const {
    return CBN_Vec2(x - s, y - s);
  }
  
  friend CBN_Vec2 operator-(f32 s, const CBN_Vec2 &v) {
    return -v + s;
  }

  CBN_Vec2 operator-(void) const {
    return *this * -1;
  }
  
  void operator-=(const CBN_Vec2 &v) {
    *this = *this - v;
  }

  CBN_Vec2 operator*(const CBN_Vec2 &v) const {
    return carbon_math_vec2_mult(*this, v);
  }
  
  CBN_Vec2 operator*(f32 s) const {
    return carbon_math_vec2_scale(*this, s);
  }
  
  friend CBN_Vec2 operator*(f32 s, const CBN_Vec2 &v) {
    return v * s;
  }
  
  void operator*=(f32 s) {
    *this = *this * s;
  }

  CBN_Vec2 operator/(const CBN_Vec2 &v) const;

  CBN_Vec2 operator/(f32 s) const {
    return carbon_math_vec2_scale(*this, 1/s);
  }
  
  void operator/=(f32 s) {
    *this = *this / s;
  }
  
  f32 Dot(const CBN_Vec2 &v) const {
    return carbon_math_vec2_dot(*this, v);
  }
  
  static f32 Dot(const CBN_Vec2 &u, const CBN_Vec2 &v) {
    return u.Dot(v);
  }
  
  f32 Length(void) const {
    return carbon_math_vec2_len(*this);
  }

  f32 LengthSquared(void) const {
    return carbon_math_vec2_len_squared(*this);
  }

  CBN_Vec2 Normalize(void) const {
    return carbon_math_vec2_norm(*this);
  }

  void Clamp(const CBN_Vec2 &min, const CBN_Vec2 &max) {
    *this = carbon_math_vec2_clamp(*this, min, max);
  }

  CBN_Vec2 Floor(void) const {
    return carbon_math_vec2_floor(*this);
  }

  CBN_Vec2 Abs(void) const {
    return carbon_math_vec2_abs(*this);
  }

  CBN_Vec2 Exp(void) const {
    return carbon_math_vec2_exp(*this);
  }

  CBN_Vec2 Sin(void) const {
    return carbon_math_vec2_sin(*this);
  }

  CBN_Vec2 Cos(void) const {
    return carbon_math_vec2_cos(*this);
  }

  CBN_Vec2 Tan(void) const {
    return carbon_math_vec2_tan(*this);
  }

  CBN_Vec2 Tanh(void) const {
    return carbon_math_vec2_tanh(*this);
  }

  CBN_Vec2 Lerp(const CBN_Vec2 &v, f32 t) const {
    return carbon_math_vec2_lerp(*this, v, t);
  }

  CBN_Vec2 Rotate(f32 angle) const {
    return carbon_math_vec2_rotate(*this, angle);
  }

  CBN_Vec2 RotatePivot(f32 angle, const CBN_Vec2 &pivot) const {
    return carbon_math_vec2_rotate_around_pivot(*this, angle, pivot);
  }

  const char *ToString(void) const {
    return carbon_math_vec2_to_cstr(*this);
  }

  CBN_Vec2 xx(void)   const { return CBN_Vec2(x);          }
  CBN_Vec2 xy(void)   const { return CBN_Vec2(x, y);       }
  CBN_Vec2 yx(void)   const { return CBN_Vec2(y, x);       }
  CBN_Vec2 yy(void)   const { return CBN_Vec2(y);          }
  //CBN_Vec3 xxx(void)  const { return CBN_Vec3(x);          }
  //CBN_Vec3 xxy(void)  const { return CBN_Vec3(x, x, y);    }
  //CBN_Vec3 xyx(void)  const { return CBN_Vec3(x, y, x);    }
  //CBN_Vec3 xyy(void)  const { return CBN_Vec3(x, y, y);    }
  //CBN_Vec3 yxx(void)  const { return CBN_Vec3(y, x, x);    }
  //CBN_Vec3 yxy(void)  const { return CBN_Vec3(y, x, y);    }
  //CBN_Vec3 yyx(void)  const { return CBN_Vec3(y, y, x);    }
  //CBN_Vec3 yyy(void)  const { return CBN_Vec3(y);          }
  CBN_Vec4 xyyx(void) const;
};
#endif
