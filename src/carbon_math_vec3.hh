/*
  $$=========================$$
  ||       Math (Vec3)       ||
  $$=========================$$
*/
// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

/**
 * @brief Represents a 3D vector with three 32-bit floating-point (f32) values.
 */
CBNDEF_T(CBN_Vec3) {
  union {
    struct { f32 x, y, z; };
    struct { CBN_Vec2 xy; f32 __pad1; };
    struct { f32 __pad2; CBN_Vec2 yz; };
    f32 c[3];
  };
};
CBNDEF_AKA(cbn::math, Vec3, CBN_Vec3);

// Swizzling
#define CARBON_VEC_xz(v) carbon_math_vec2((v).x, (v).z)

// Forward declarations
#ifdef __cplusplus
typedef struct CBN_Vec4 CBN_Quat;
#else
typedef struct CBN_Vec4_t CBN_Quat;
#endif

/**
 * @brief Creates a 3D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @param z The value to assign to the Z field.
 * @return The newly created 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3(f32 x, f32 y, f32 z);

/**
 * @brief Creates a 3D vector.
 * @param x The value to assign to the X, Y and Z fields.
 * @return The newly created 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_1(f32 x);

/**
 * @brief Creates a 3D vector.
 * @param v The 2D vector to assign to the X and Y fields.
 * @param z The value to assign to the Z field.
 * @return The newly created 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_2(CBN_Vec2 v, f32 z);

/**
 * @brief Adds two 3D vectors together (element-wise).
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Subtracts one 3D vector from another one (element-wise).
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Computes the Hadamard product (element-wise multiplication) between two 3D vectors.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_mult(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Computes the dot product between two 3D vectors.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant scalar value.
 */
CBNDEF f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Computes the cross product between two 3D vectors.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Scales the 3D vector by the specified scalar value.
 * @param v The 3D vector.
 * @param s The scalar value.
 * @return The scaled 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_scale(CBN_Vec3 v, f32 s);

/**
 * @brief Returns a 3D vector being the negated of the provided one.
 * @param v The 3D vector.
 * @return The negated 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_neg(CBN_Vec3 v);

/**
 * @brief Returns the length of the 3D vector squared.
 * @param v The 3D vector.
 * @return The 3D vector's length squared.
 */
CBNDEF f32 carbon_math_vec3_len_squared(CBN_Vec3 v);

/**
 * @brief Returns the length of the 3D vector.
 * @param v The 3D vector.
 * @return The 3D vector's length.
 */
CBNDEF f32 carbon_math_vec3_len(CBN_Vec3 v);

/**
 * @brief Returns a 3D vector with the same direction as the specified one, but with a length of `1`.
 * @param v The 3D vector.
 * @return The normalized 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_norm(CBN_Vec3 v);

/**
 * @brief Returns the string representation of the 3D vector using default formatting.
 * @param v The 3D vector.
 * @return The serialized 3D vector as `(X, Y, Z)`.
 */
CBNDEF char *carbon_math_vec3_to_cstr(CBN_Vec3 v);

/**
 * @brief Performs a linear interpolation between two 3D vectors based on the given weighting.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @param t A value [0..1] indicating the weight of `v`.
 * @return The interpolated 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_lerp(CBN_Vec3 u, CBN_Vec3 v, f32 t);

/**
 * @brief Rotates the 3D vector around the X-axis by the specified angle (in degrees).
 * @param v The 3D vector.
 * @param angle X-axis rotation angle (in degrees).
 * @return The rotated 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_rotate_x(CBN_Vec3 v, f32 angle);

/**
 * @brief Rotates the 3D vector around the Y-axis by the specified angle (in degrees).
 * @param v The 3D vector.
 * @param angle Y-axis rotation angle (in degrees).
 * @return The rotated 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_rotate_y(CBN_Vec3 v, f32 angle);

/**
 * @brief Rotates the 3D vector around the Z-axis by the specified angle (in degrees).
 * @param v The 3D vector.
 * @param angle Z-axis rotation angle (in degrees).
 * @return The rotated 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_rotate_z(CBN_Vec3 v, f32 angle);

/**
 * @brief Rotates the 3D vector by the provided quaternion.
 * @param v The 3D vector.
 * @param q The rotation quaternion.
 * @return The rotated 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_rotate(CBN_Vec3 v, CBN_Quat q);

/**
 * @brief Projects a 3D point/vector into 2D Normalized Device Coordinates (NDC).
 *
 * It performs the perspective divide algorithm, mapping (x, y, z) in clip space
 * to (x/z, y/z) in NDC space. The resulting coordinates are in the [-1..1] range
 * for objects inside the canonical view frustum. Z is not preserved; only the
 * 2D projection is returned. The result is valid only for points in front of the camera.
 *
 * @param v The 3D vector.
 * @param near_z The near-plane distance (Z-axis).
 * @param out_v The projected vector in 2D space as NDC coordinates (output argument pointer).
 * @return A boolean value indicating whether it projected the 3D vector successfully or not.
 */
CBNDEF bool carbon_math_vec3_project_2d(CBN_Vec3 v, f32 near_z, CBN_Vec2 *out_v);

/**
 * @brief Computes the reflection direction for an incident 3D vector.
 * @param i The incident 3D vector.
 * @param n The normal 3D vector.
 * @return The reflection direction 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_reflect(CBN_Vec3 i, CBN_Vec3 n);

/**
 * @brief Computes the refraction direction for an incident 3D vector.
 * @param i The incident 3D vector.
 * @param n The normal 3D vector.
 * @param idx The refractive index of the medium.
 * @return The refraction direction 3D vector.
 */
CBNDEF CBN_Vec3 carbon_math_vec3_refract(CBN_Vec3 i, CBN_Vec3 n, f32 idx);

#ifdef __cplusplus
struct CBN_Vec3 : CBN_Vec3_t {
  constexpr CBN_Vec3(f32 _x, f32 _y, f32 _z) : CBN_Vec3_t{.c = {_x, _y, _z}} {}
  constexpr CBN_Vec3(f32 _x = 0) : CBN_Vec3(_x, _x, _x) {}

  CBN_Vec3 operator+(const CBN_Vec3 &v) const {
    return carbon_math_vec3_add(*this, v);
  }
  
  CBN_Vec3 operator+(f32 s) const {
    return CBN_Vec3(x + s, y + s, z + s);
  }
  
  friend CBN_Vec3 operator+(f32 s, const CBN_Vec3 &v) {
    return v + s;
  }
  
  CBN_Vec3 &operator+=(const CBN_Vec3 &v) {
    *this = *this + v;
    return *this;
  }
  
  CBN_Vec3 &operator+=(f32 s) {
    *this = *this + s;
    return *this;
  }

  CBN_Vec3 operator-(const CBN_Vec3 &v) const {
    return carbon_math_vec3_sub(*this, v);
  }

  f32 operator*(const CBN_Vec3 &v) const {
    return carbon_math_vec3_dot(*this, v);
  }

  CBN_Vec3 Cross(const CBN_Vec3 &v) const {
    return carbon_math_vec3_cross(*this, v);
  }

  CBN_Vec3 Lerp(const CBN_Vec3 &v, f32 t) const {
    return carbon_math_vec3_lerp(*this, v, t);
  }

  CBN_Quat ToQuat(void) const;
  //{
  //  return carbon_math_quat_from_euler(*this);
  //}

  const char *ToString(void) const {
    return carbon_math_vec3_to_cstr(*this);
  }

  CBN_Vec3 RotateX(f32 angle) const {
    return carbon_math_vec3_rotate_x(*this, angle);
  }

  CBN_Vec3 RotateY(f32 angle) const {
    return carbon_math_vec3_rotate_y(*this, angle);
  }

  CBN_Vec3 RotateZ(f32 angle) const {
    return carbon_math_vec3_rotate_z(*this, angle);
  }

  CBN_Vec3 Rotate(CBN_Quat q) const;
  //{
  //  return carbon_math_vec3_rotate(*this, q);
  //}

  cbn::Opt<CBN_Vec2> Project2D(f32 near_z) const {
    CBN_Vec2 v;
    if (!carbon_math_vec3_project_2d(*this, near_z, &v)) return {};
    return v;
  }

  CBN_Vec2 xx(void)  const { return CBN_Vec2(x);       }
  CBN_Vec2 xz(void)  const { return CBN_Vec2(x, z);    }
  CBN_Vec2 yx(void)  const { return CBN_Vec2(y, x);    }
  CBN_Vec2 yy(void)  const { return CBN_Vec2(y);       }
  CBN_Vec2 zx(void)  const { return CBN_Vec2(z, x);    }
  CBN_Vec2 zy(void)  const { return CBN_Vec2(z, y);    }
  CBN_Vec2 zz(void)  const { return CBN_Vec2(z);       }
  CBN_Vec3 xxx(void) const { return CBN_Vec3(x);       }
  CBN_Vec3 xxy(void) const { return CBN_Vec3(x, x, y); }
  CBN_Vec3 xxz(void) const { return CBN_Vec3(x, x, z); }
  CBN_Vec3 xyx(void) const { return CBN_Vec3(x, y, x); }
  CBN_Vec3 xyy(void) const { return CBN_Vec3(x, y, y); }
  CBN_Vec3 xyz(void) const { return CBN_Vec3(x, y, z); }
  CBN_Vec3 xzx(void) const { return CBN_Vec3(x, z, x); }
  CBN_Vec3 xzy(void) const { return CBN_Vec3(x, z, y); }
  CBN_Vec3 xzz(void) const { return CBN_Vec3(x, z, z); }
  CBN_Vec3 yxx(void) const { return CBN_Vec3(y, x, x); }
  CBN_Vec3 yxy(void) const { return CBN_Vec3(y, x, y); }
  CBN_Vec3 yxz(void) const { return CBN_Vec3(y, x, z); }
  CBN_Vec3 yyx(void) const { return CBN_Vec3(y, y, x); }
  CBN_Vec3 yyy(void) const { return CBN_Vec3(y);       }
  CBN_Vec3 yyz(void) const { return CBN_Vec3(y, y, z); }
  CBN_Vec3 yzx(void) const { return CBN_Vec3(y, z, x); }
  CBN_Vec3 yzy(void) const { return CBN_Vec3(y, z, y); }
  CBN_Vec3 yzz(void) const { return CBN_Vec3(y, z, z); }
  CBN_Vec3 zxx(void) const { return CBN_Vec3(z, x, x); }
  CBN_Vec3 zxy(void) const { return CBN_Vec3(z, x, y); }
  CBN_Vec3 zxz(void) const { return CBN_Vec3(z, x, z); }
  CBN_Vec3 zyx(void) const { return CBN_Vec3(z, y, x); }
  CBN_Vec3 zyy(void) const { return CBN_Vec3(z, y, y); }
  CBN_Vec3 zyz(void) const { return CBN_Vec3(z, y, z); }
  CBN_Vec3 zzx(void) const { return CBN_Vec3(z, z, x); }
  CBN_Vec3 zzy(void) const { return CBN_Vec3(z, z, y); }
  CBN_Vec3 zzz(void) const { return CBN_Vec3(z);       }
};
#endif
