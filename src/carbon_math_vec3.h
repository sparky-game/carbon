/*
**  $$=========================$$
**  ||       Math (Vec3)       ||
**  $$=========================$$
*/

// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

/**
 * @brief Defines an inline 3D vector.
 * @param x The value to assign to the X field.
 * @param y The value to assign to the Y field.
 * @param z The value to assign to the Z field.
 */
#define CARBON_VEC3(x, y, z) (CBN_Vec3){{(f32)(x), (f32)(y), (f32)(z)}}

/**
 * @brief Defines an inline 3D vector.
 * @param x The value to assign to the X, Y and Z fields.
 */
#define CARBON_VEC3_1(x) CARBON_VEC3((x), (x), (x))

/**
 * @brief Defines an inline 3D vector.
 * @param v The ≥2D vector to assign to the X and Y fields.
 * @param z The value to assign to the Z field.
 */
#define CARBON_VEC3_2(v, z) CARBON_VEC3((v).x, (v).y, z)

/**
 * @brief Defines an inline 3D vector.
 * @param v The ≥3D vector to assign to the X, Y and Z fields.
 */
#define CARBON_VEC3_V(v) CARBON_VEC3((v).x, (v).y, (v).z)

/**
 * @brief Defines an inline 3D vector being the negated of the provided one.
 * @param v The ≥3D vector.
 */
#define CARBON_VEC3_N(v) CARBON_VEC3(-(v).x, -(v).y, -(v).z)

/**
 * @brief Defines an inline 3D vector whose 3 elements are equal to zero.
 */
#define CARBON_VEC3_ZERO CARBON_VEC3(0, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents -X direction.
 */
#define CARBON_VEC3_LEFT CARBON_VEC3(-1, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents +X direction.
 */
#define CARBON_VEC3_RIGHT CARBON_VEC3(1, 0, 0)

/**
 * @brief Defines an inline 3D vector which represents -Y direction.
 */
#define CARBON_VEC3_DOWN CARBON_VEC3(0, -1, 0)

/**
 * @brief Defines an inline 3D vector which represents +Y direction.
 */
#define CARBON_VEC3_UP CARBON_VEC3(0, 1, 0)

/**
 * @brief Defines an inline 3D vector which represents -Z direction.
 */
#define CARBON_VEC3_FORWARD CARBON_VEC3(0, 0, -1)

/**
 * @brief Defines an inline 3D vector which represents +Z direction.
 */
#define CARBON_VEC3_BACK CARBON_VEC3(0, 0, 1)

/**
 * @brief Defines an inline 3D vector whose 3 elements are equal to one.
 */
#define CARBON_VEC3_ONE CARBON_VEC3(1, 1, 1)

typedef union CBN_Vec4 CBN_Quat;  // Forward declaration

/**
 * @brief Represents a 3D vector with three 32-bit floating-point (f32) values.
 */
typedef union CBN_Vec3 {
  f32 items[3];
  struct {
    union { f32 x, r; };
    union { f32 y, g; };
    union { f32 z, b; };
  };
#ifdef __cplusplus
  /**
   * @see carbon_math_vec3_add
   */
  CBN_Vec3 operator+(const CBN_Vec3 &v) const;
  /**
   * @see carbon_math_vec3_sub
   */
  CBN_Vec3 operator-(const CBN_Vec3 &v) const;
  /**
   * @see carbon_math_vec3_dot
   */
  f32 operator*(const CBN_Vec3 &v) const;
  /**
   * @see carbon_math_vec3_cross
   */
  CBN_Vec3 Cross(const CBN_Vec3 &v) const;
  /**
   * @see carbon_math_vec3_to_cstr
   */
  const char *ToString(void) const;
  /**
   * @see carbon_math_vec3_rotate_x
   */
  CBN_Vec3 RotateX(f32 angle) const;
  /**
   * @see carbon_math_vec3_rotate_y
   */
  CBN_Vec3 RotateY(f32 angle) const;
  /**
   * @see carbon_math_vec3_rotate_z
   */
  CBN_Vec3 RotateZ(f32 angle) const;
  /**
   * @see carbon_math_vec3_rotate
   */
  CBN_Vec3 Rotate(CBN_Quat q) const;
  /**
   * @see carbon_math_vec3_project_2d
   */
  cbn::Opt<CBN_Vec2> Project2D(f32 near_z) const;
  // Swizzling Operations
  constexpr auto xx(void)  const;
  constexpr auto xy(void)  const;
  constexpr auto xz(void)  const;
  constexpr auto yx(void)  const;
  constexpr auto yy(void)  const;
  constexpr auto yz(void)  const;
  constexpr auto zx(void)  const;
  constexpr auto zy(void)  const;
  constexpr auto zz(void)  const;
  constexpr auto xxx(void) const;
  constexpr auto xxy(void) const;
  constexpr auto xxz(void) const;
  constexpr auto xyx(void) const;
  constexpr auto xyy(void) const;
  constexpr auto xyz(void) const;
  constexpr auto xzx(void) const;
  constexpr auto xzy(void) const;
  constexpr auto xzz(void) const;
  constexpr auto yxx(void) const;
  constexpr auto yxy(void) const;
  constexpr auto yxz(void) const;
  constexpr auto yyx(void) const;
  constexpr auto yyy(void) const;
  constexpr auto yyz(void) const;
  constexpr auto yzx(void) const;
  constexpr auto yzy(void) const;
  constexpr auto yzz(void) const;
  constexpr auto zxx(void) const;
  constexpr auto zxy(void) const;
  constexpr auto zxz(void) const;
  constexpr auto zyx(void) const;
  constexpr auto zyy(void) const;
  constexpr auto zyz(void) const;
  constexpr auto zzx(void) const;
  constexpr auto zzy(void) const;
  constexpr auto zzz(void) const;
#endif
} CBN_Vec3;

// Swizzling Operations
#define CARBON_VEC_xz(v)  CARBON_VEC2((v).x, (v).z)
#define CARBON_VEC_yz(v)  CARBON_VEC2((v).y, (v).z)
#define CARBON_VEC_zx(v)  CARBON_VEC2((v).z, (v).x)
#define CARBON_VEC_zy(v)  CARBON_VEC2((v).z, (v).y)
#define CARBON_VEC_zz(v)  CARBON_VEC2((v).z, (v).z)
#define CARBON_VEC_xxz(v) CARBON_VEC3((v).x, (v).x, (v).z)
#define CARBON_VEC_xyz(v) CARBON_VEC3((v).x, (v).y, (v).z)
#define CARBON_VEC_xzx(v) CARBON_VEC3((v).x, (v).z, (v).x)
#define CARBON_VEC_xzy(v) CARBON_VEC3((v).x, (v).z, (v).y)
#define CARBON_VEC_xzz(v) CARBON_VEC3((v).x, (v).z, (v).z)
#define CARBON_VEC_yxz(v) CARBON_VEC3((v).y, (v).x, (v).z)
#define CARBON_VEC_yyz(v) CARBON_VEC3((v).y, (v).y, (v).z)
#define CARBON_VEC_yzx(v) CARBON_VEC3((v).y, (v).z, (v).x)
#define CARBON_VEC_yzy(v) CARBON_VEC3((v).y, (v).z, (v).y)
#define CARBON_VEC_yzz(v) CARBON_VEC3((v).y, (v).z, (v).z)
#define CARBON_VEC_zxx(v) CARBON_VEC3((v).z, (v).x, (v).x)
#define CARBON_VEC_zxy(v) CARBON_VEC3((v).z, (v).x, (v).y)
#define CARBON_VEC_zxz(v) CARBON_VEC3((v).z, (v).x, (v).z)
#define CARBON_VEC_zyx(v) CARBON_VEC3((v).z, (v).y, (v).x)
#define CARBON_VEC_zyy(v) CARBON_VEC3((v).z, (v).y, (v).y)
#define CARBON_VEC_zyz(v) CARBON_VEC3((v).z, (v).y, (v).z)
#define CARBON_VEC_zzx(v) CARBON_VEC3((v).z, (v).z, (v).x)
#define CARBON_VEC_zzy(v) CARBON_VEC3((v).z, (v).z, (v).y)
#define CARBON_VEC_zzz(v) CARBON_VEC3((v).z, (v).z, (v).z)

/**
 * @brief Adds two 3D vectors together (element-wise).
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_add(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Subtracts one 3D vector from another one (element-wise).
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_sub(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Computes the Hadamard product (element-wise multiplication) between two 3D vectors.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_mult(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Computes the dot product between two 3D vectors.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant scalar value.
 */
CARBON_API f32 carbon_math_vec3_dot(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Computes the cross product between two 3D vectors.
 * @param u The first 3D vector.
 * @param v The second 3D vector.
 * @return The resultant 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_cross(CBN_Vec3 u, CBN_Vec3 v);

/**
 * @brief Scales the 3D vector by the specified scalar value.
 * @param v The 3D vector.
 * @param s The scalar value.
 * @return The scaled 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_scale(CBN_Vec3 v, f32 s);

/**
 * @brief Returns the length of the 3D vector squared.
 * @param v The 3D vector.
 * @return The 3D vector's length squared.
 */
CARBON_API f32 carbon_math_vec3_len_squared(CBN_Vec3 v);

/**
 * @brief Returns the length of the 3D vector.
 * @param v The 3D vector.
 * @return The 3D vector's length.
 */
CARBON_API f32 carbon_math_vec3_len(CBN_Vec3 v);

/**
 * @brief Returns a 3D vector with the same direction as the specified one, but with a length of `1`.
 * @param v The 3D vector.
 * @return The normalized 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_norm(CBN_Vec3 v);

/**
 * @brief Returns the string representation of the 3D vector using default formatting.
 * @param v The 3D vector.
 * @return The serialized 3D vector as `(X, Y, Z)`.
 */
CARBON_API char *carbon_math_vec3_to_cstr(CBN_Vec3 v);

/**
 * @brief Rotates the 3D vector around the X-axis by the specified angle (in degrees).
 * @param v The 3D vector.
 * @param angle X-axis rotation angle (in degrees).
 * @return The rotated 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_rotate_x(CBN_Vec3 v, f32 angle);

/**
 * @brief Rotates the 3D vector around the Y-axis by the specified angle (in degrees).
 * @param v The 3D vector.
 * @param angle Y-axis rotation angle (in degrees).
 * @return The rotated 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_rotate_y(CBN_Vec3 v, f32 angle);

/**
 * @brief Rotates the 3D vector around the Z-axis by the specified angle (in degrees).
 * @param v The 3D vector.
 * @param angle Z-axis rotation angle (in degrees).
 * @return The rotated 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_rotate_z(CBN_Vec3 v, f32 angle);

/**
 * @brief Rotates the 3D vector by the provided quaternion.
 * @param v The 3D vector.
 * @param q The rotation quaternion.
 * @return The rotated 3D vector.
 */
CARBON_API CBN_Vec3 carbon_math_vec3_rotate(CBN_Vec3 v, CBN_Quat q);

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
CARBON_API bool carbon_math_vec3_project_2d(CBN_Vec3 v, f32 near_z, CBN_Vec2 *out_v);

// Local Variables:
// mode: c++
// End:
