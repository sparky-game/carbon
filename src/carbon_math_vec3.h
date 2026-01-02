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
#define CARBON_VEC3(x, y, z) (CBN_Vec3){{.c = {(f32)(x), (f32)(y), (f32)(z)}}}

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

// Swizzling Operations
#define CARBON_VEC_xz(v)  CARBON_VEC2((v).x, (v).z)
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

/**
 * @brief Represents a 3D vector with three 32-bit floating-point (f32) values.
 */
struct CBN_Vec3_t {
  union {
    struct { f32 x, y, z; };
    struct { CBN_Vec2 xy; f32 __pad1; };
    struct { f32 __pad2; CBN_Vec2 yz; };
    f32 c[3];
  };
};

// Forward declarations
#ifdef __cplusplus
typedef struct CBN_Vec4 CBN_Quat;
#else
typedef struct CBN_Vec4_t CBN_Quat;
#endif


#ifdef __cplusplus
struct CBN_Vec3 : CBN_Vec3_t {
  /**
   * @see carbon_math_vec3_add
   */
  CBN_Vec3 operator+(const CBN_Vec3 &v) const;
  CBN_Vec3 operator+(f32 s) const;
  friend CBN_Vec3 operator+(f32 s, const CBN_Vec3 &v);
  CBN_Vec3 &operator+=(const CBN_Vec3 &v);
  CBN_Vec3 &operator+=(f32 s);
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
   * @see carbon_math_vec3_lerp
   */
  CBN_Vec3 Lerp(const CBN_Vec3 &v, f32 t) const;
  /**
   * @see carbon_math_quat_from_euler
   */
  CBN_Quat ToQuat(void) const;
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
  CBN_Vec2 xx(void)  const;
  CBN_Vec2 xz(void)  const;
  CBN_Vec2 yx(void)  const;
  CBN_Vec2 yy(void)  const;
  CBN_Vec2 zx(void)  const;
  CBN_Vec2 zy(void)  const;
  CBN_Vec2 zz(void)  const;
  CBN_Vec3 xxx(void) const;
  CBN_Vec3 xxy(void) const;
  CBN_Vec3 xxz(void) const;
  CBN_Vec3 xyx(void) const;
  CBN_Vec3 xyy(void) const;
  CBN_Vec3 xyz(void) const;
  CBN_Vec3 xzx(void) const;
  CBN_Vec3 xzy(void) const;
  CBN_Vec3 xzz(void) const;
  CBN_Vec3 yxx(void) const;
  CBN_Vec3 yxy(void) const;
  CBN_Vec3 yxz(void) const;
  CBN_Vec3 yyx(void) const;
  CBN_Vec3 yyy(void) const;
  CBN_Vec3 yyz(void) const;
  CBN_Vec3 yzx(void) const;
  CBN_Vec3 yzy(void) const;
  CBN_Vec3 yzz(void) const;
  CBN_Vec3 zxx(void) const;
  CBN_Vec3 zxy(void) const;
  CBN_Vec3 zxz(void) const;
  CBN_Vec3 zyx(void) const;
  CBN_Vec3 zyy(void) const;
  CBN_Vec3 zyz(void) const;
  CBN_Vec3 zzx(void) const;
  CBN_Vec3 zzy(void) const;
  CBN_Vec3 zzz(void) const;
};
#else
typedef struct CBN_Vec3_t CBN_Vec3;
#endif
CBNDEF_T(cbn::math, Vec3, CBN_Vec3);

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

// Local Variables:
// mode: c++
// End:
