#ifdef __cplusplus

struct CBN_Camera {
  using Type = CBN_Camera_Type;
  
  static cbn::Scope<CBN_Camera> New(const CBN_DrawCanvas *dc) {
    return {
      carbon_camera_create(dc),
      [](CBN_Camera *c){ carbon_camera_destroy(c); }
    };
  }
  
  void Reset(const CBN_DrawCanvas &dc) {
    carbon_camera_reset(this, &dc);
  }

  void SetType(Type t) {
    carbon_camera_set_type(this, t);
  }

  CBN_Vec3 GetPosition(void) const {
    return carbon_camera_get_position(this);
  }

  CBN_Quat GetRotation(void) const {
    return carbon_camera_get_rotation(this);
  }

  CBN_Mat4 GetView(void) const {
    return carbon_camera_get_view(this);
  }

  CBN_Mat4 GetProj(void) const {
    return carbon_camera_get_proj(this);
  }

  void MoveForward(f32 amount) {
    carbon_camera_move_forward(this, amount);
  }

  void MoveBackward(f32 amount) {
    carbon_camera_move_backward(this, amount);
  }

  void MoveLeft(f32 amount) {
    carbon_camera_move_left(this, amount);
  }
  
  void MoveRight(f32 amount) {
    carbon_camera_move_right(this, amount);
  }
  
  void MoveUp(f32 amount) {
    carbon_camera_move_up(this, amount);
  }
  
  void MoveDown(f32 amount) {
    carbon_camera_move_down(this, amount);
  }

  void Yaw(f32 amount) {
    carbon_camera_yaw(this, amount);
  }
  
  void Pitch(f32 amount) {
    carbon_camera_pitch(this, amount);
  }

  void OrthoZoom(f32 amount) {
    carbon_camera_ortho_zoom(this, amount);
  }
};

#endif
