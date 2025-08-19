// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

namespace pong {
  static constexpr auto c_Name              = "PONG";
  static constexpr auto c_ScreenWidthRatio  = 16;
  static constexpr auto c_ScreenHeightRatio = 9;
  static constexpr auto c_ScreenRatioFactor = 80;
  static constexpr auto c_ScreenWidth       = c_ScreenWidthRatio * c_ScreenRatioFactor;
  static constexpr auto c_ScreenHeight      = c_ScreenHeightRatio * c_ScreenRatioFactor;
  static constexpr auto c_BallSize          = 32;
  static constexpr auto c_RacketSize        = c_BallSize * 2;
  static constexpr auto c_PointsToWin       = 11;
  static constexpr auto c_PointsDiffToWin   = 2;

  namespace res {
    static cbn::SKAP s_AssetPack;
    static cbn::sprite_mgr::UID s_Sprite_YellowCard;
    static cbn::sprite_mgr::UID s_Sprite_RedCard;
    static cbn::audio::UID s_Sound_MenuMusic;
    static cbn::audio::UID s_Sound_MenuSelect;
    static cbn::audio::UID s_Sound_Serve_1;
    static cbn::audio::UID s_Sound_Serve_2;
    static cbn::audio::UID s_Sound_Wall;
    static cbn::audio::UID s_Sound_Racket;
    static cbn::audio::UID s_Sound_Goal;
    static cbn::audio::UID s_Sound_PointUp;
    static cbn::audio::UID s_Sound_Win;
    static cbn::audio::UID s_Sound_YellowCard;

    static void Init(void) {
      CBN_ASSERT(cbn::fs::cd(cbn::fs::GetBinDir()));
      if (auto i = cbn::SKAP::make("pong.skap")) s_AssetPack = *i;
      else CARBON_UNREACHABLE;
      cbn::sprite_mgr::Init();
      if (auto i = s_AssetPack.LoadSprite("./yellow-card.png")) s_Sprite_YellowCard = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadSprite("./red-card.png")) s_Sprite_RedCard = *i;
      else CARBON_UNREACHABLE;
      cbn::audio::Init();
      if (auto i = s_AssetPack.LoadAudioStream("./menu-music.ogg")) s_Sound_MenuMusic = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./menu-select.ogg")) s_Sound_MenuSelect = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./serve-1.ogg")) s_Sound_Serve_1 = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./serve-2.ogg")) s_Sound_Serve_2 = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./wall.ogg")) s_Sound_Wall = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./racket.ogg")) s_Sound_Racket = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./goal.ogg")) s_Sound_Goal = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./point-up.ogg")) s_Sound_PointUp = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./win.ogg")) s_Sound_Win = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./yellow-card.ogg")) s_Sound_YellowCard = *i;
      else CARBON_UNREACHABLE;
      CBN_INFO("Initialized resource manager successfully");
    }

    static void Shutdown(void) {
      cbn::audio::Shutdown();
      cbn::sprite_mgr::Shutdown();
      s_AssetPack.Free();
      CBN_INFO("Shutdowned resource manager successfully");
    }
  }

  struct Color {
    enum Value : u32 {
      Black = 0x181818ff,
      Grey = 0x737373ff,
      White = 0xdcdcdcff,
      Blue = 0x469fd2ff,
      Cream = 0xfffdd0ff
    };
    explicit constexpr operator bool(void) const = delete;
    constexpr operator Value(void) const { return m_Value; }
    constexpr bool operator==(Color c) const { return m_Value == c.m_Value; }
    constexpr bool operator!=(Color c) const { return m_Value != c.m_Value; }
  private:
    Value m_Value;
  };

  template <cbn::meta::Numeric T>
  struct Stat final {
    void Set(const T value) {
      m_Value = value;
      UpdateString();
    }
    auto GetInt(void) const { return m_Value; }
    auto GetStr(void) const { return m_String.c_str(); }
    auto operator+(const Stat &s) const { return m_Value + s.GetInt(); }
    auto operator-(const Stat &s) const { return m_Value - s.GetInt(); }
    auto &operator++(void) {
      Set(m_Value + 1);
      return *this;
    }
    auto &operator--(void) {
      Set(m_Value - 1);
      return *this;
    }
    auto operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }
    auto operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }
    auto operator<=>(const Stat &s) const { return m_Value <=> s.GetInt(); }
    auto operator<=>(const T value) const { return m_Value <=> value; }
  private:
    T m_Value {0};
    std::string m_String {"0"};
    void UpdateString(void) { m_String = std::to_string(m_Value); }
  };

  struct Drawable {
    virtual ~Drawable(void) = default;
    virtual void Render(cbn::DrawCanvas &canvas) const = 0;
  };

  struct Net final : Drawable {
    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      static constexpr auto color = Color::Blue;
      canvas.DrawRect(m_Net, color);
    }

  private:
    static constexpr auto c_Thickness = 4;
    const cbn::Rect m_Net {CARBON_RECT(c_ScreenWidth/2 - c_Thickness + 1, 0, c_Thickness, c_ScreenHeight)};
  };

  struct Entity : Drawable {
    cbn::Vec2 position;
    cbn::Vec2 velocity;
    explicit Entity(const cbn::Vec2 &p = CARBON_VEC2_ZERO, const cbn::Vec2 &v = CARBON_VEC2_ZERO) : position{p}, velocity{v} {}
    virtual void Update([[maybe_unused]] const f64 dt) {}
  };

  struct Ball final : Entity {
    explicit Ball(void) : Entity{CARBON_VEC2(c_ScreenWidth/2 - c_BallSize/2, c_ScreenHeight/2 - c_BallSize/2)}
    {
      using namespace cbn::math::literals;
      auto angle = cbn::math::Mod((cbn::math::Rand(0, 120_deg) + 300_deg), 2_pi);
      if (cbn::math::Rand() < 0.5) angle += pi;
      velocity = CARBON_VEC2(speed_initial * cbn::math::Cos(angle), speed_initial * cbn::math::Sin(angle));
    }

    virtual void Update(const f64 dt) final override {
      position += velocity * dt;
    }

    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      canvas.DrawRect(CARBON_RECT_SQUARE_V(position, c_BallSize), Color::White);
    }

    static constexpr auto speed_min {1500};
    static constexpr auto speed_initial {speed_min / 2};
    static constexpr auto speed_max {speed_min * 3};
    static constexpr auto speed_delta {1000};
    u8 wall_hits {0};
  };

  struct Racket final : Entity {
    explicit Racket(const cbn::Vec2 &p) : Entity{p, CARBON_VEC2(0, c_Speed)} {}

    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      canvas.DrawRect(CARBON_RECT_SQUARE_V(position, c_RacketSize), Color::Cream);
    }

  private:
    static constexpr auto c_Speed {1100};
  };

  enum struct PlayerSide { Left, Right };

  template <PlayerSide S>
  struct Player final : Drawable {
    Racket racket;
    Stat<u8> points;
    u32 score {0};
    u8 yellow_cards {0};

    explicit Player(void) : racket{CARBON_VEC2(0, c_ScreenHeight/2 - c_RacketSize/2)}
    {
      static constexpr auto padding = 100;
      if constexpr (S == PlayerSide::Left) racket.position.x = padding;
      else racket.position.x = c_ScreenWidth - padding - c_BallSize;
    }

    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      racket.Render(canvas);
      Render_Points(canvas);
      if (yellow_cards) Render_Cards(canvas);
    }

    void Reset(void) {
      points.Set(0);
      score = 0;
      yellow_cards = 0;
    }

  private:
    void Render_Points(cbn::DrawCanvas &canvas) const {
      const auto text = points.GetStr();
      static constexpr auto text_size = 8;
      static constexpr auto text_padding = CARBON_VEC2(100, 50);
      static constexpr auto text_color = Color::White;
      const auto text_width = canvas.TextWidth(text, text_size);
      auto text_pos = CARBON_VEC2(c_ScreenWidth/2, text_padding.y);
      if constexpr (S == PlayerSide::Left) text_pos.x -= text_padding.x + text_width;
      else text_pos.x += text_padding.x;
      canvas.DrawText(text, text_pos, text_size, text_color);
    }

    void Render_Cards(cbn::DrawCanvas &canvas) const {
      static constexpr auto padding = 10;
      static const auto * const sp = cbn::sprite_mgr::Lookup(res::s_Sprite_YellowCard);
      {
        static const auto sp_pos = CARBON_VEC2(padding, c_ScreenHeight - padding - sp->height);
        canvas.DrawSprite(sp, sp_pos);
      }
      {
        const auto text = cbn::str::fmt("x%hhu", yellow_cards);
        static constexpr auto text_size = 2;
        static constexpr auto text_color = Color::White;
        static const auto text_height = canvas.TextHeight(text_size);
        static const auto text_pos = CARBON_VEC2(2*padding + sp->width, c_ScreenHeight - padding - text_height);
        canvas.DrawText(text, text_pos, text_size, text_color);
      }
    }
  };

  namespace scene {
    struct Scene {
      using OnSwitch = cbn::Func<void()>;
      virtual ~Scene(void) = default;
      virtual void OnEnter(void) {}
      virtual void OnExit(void) {}
      virtual void Update(const f64 dt) = 0;
      virtual void Render(void) = 0;
    };

    struct MainMenu final : Scene {
      explicit MainMenu(cbn::DrawCanvas &canvas, OnSwitch to_match)
        : m_Canvas{canvas},
          m_SwitchToMatch{std::move(to_match)}
      {}

      virtual void OnEnter(void) final override {
        cbn::audio::Play(res::s_Sound_MenuMusic);
      }

      virtual void OnExit(void) final override {
        cbn::audio::Stop(res::s_Sound_MenuMusic);
      }

      virtual void Update(const f64 dt) final override {
        if (!m_OptionsMenu) Update_MainMenu();
        else Update_OptionsMenu(dt);
      }

      virtual void Render(void) final override {
        m_Canvas.Fill(Color::Black);
        if (!m_OptionsMenu) Render_MainMenu();
        else Render_OptionsMenu();
        Render_Copyright();
      }

    private:
      cbn::DrawCanvas &m_Canvas;
      OnSwitch m_SwitchToMatch;
      bool m_OptionsMenu {false};

      void Update_MainMenu(void) {
        if (cbn::win::GetKeyDown(cbn::win::KeyCode::One)) {
          cbn::audio::Play(res::s_Sound_MenuSelect);
          m_SwitchToMatch();
        }
        if (cbn::win::GetKeyDown(cbn::win::KeyCode::Two)) {
          cbn::audio::Play(res::s_Sound_MenuSelect);
          m_OptionsMenu = true;
        }
      }

      void Update_OptionsMenuAdjustVolume(const f64 dt) {
        f32 volume = cbn::audio::GetVolume();
        if (cbn::win::GetKey(cbn::win::KeyCode::LeftArrow) ||
            cbn::win::GetKey(cbn::win::KeyCode::DownArrow)) {
          volume -= 0.25 * dt;
          cbn::audio::SetVolume(cbn::math::ToClamped(volume, 0, 1));
        }
        else if (cbn::win::GetKey(cbn::win::KeyCode::RightArrow) ||
                 cbn::win::GetKey(cbn::win::KeyCode::UpArrow)) {
          volume += 0.25 * dt;
          cbn::audio::SetVolume(cbn::math::ToClamped(volume, 0, 1));
        }
      }

      void Update_OptionsMenu(const f64 dt) {
        if (cbn::win::GetKeyDown(cbn::win::KeyCode::Escape)) {
          cbn::audio::Play(res::s_Sound_MenuSelect);
          m_OptionsMenu = false;
        }
        Update_OptionsMenuAdjustVolume(dt);
      }

      void Render_MainMenu(void) {
        Render_MainTitle();
        Render_StartButton();
        Render_OptionsButton();
      }

      void Render_OptionsMenu(void) {
        Render_OptionsTitle();
        Render_OptionsGoBack();
        Render_OptionsVolume();
      }

      void Render_MainTitle(void) {
        static constexpr auto text = c_Name;
        static constexpr auto text_size = 14;
        static constexpr auto text_color = Color::White;
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, 50);
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }

      void Render_StartButton(void) {
        static constexpr auto text = "<1> Start";
        static constexpr auto text_size = 4;
        static constexpr auto text_color = Color::Black;
        static constexpr auto text_padding = CARBON_VEC2(20, 15);
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_height = m_Canvas.TextHeight(text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, 300);
        m_Canvas.DrawBox(CARBON_RECT(text_pos.x - text_padding.x,
                                     text_pos.y - text_padding.y - 2*text_size,
                                     text_pos.x + text_width + text_padding.x,
                                     text_pos.y + text_height + text_padding.y));
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }

      void Render_OptionsButton(void) {
        static constexpr auto text = "<2> Options";
        static constexpr auto text_size = 4;
        static constexpr auto text_color = Color::Black;
        static constexpr auto text_padding = CARBON_VEC2(20, 15);
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_height = m_Canvas.TextHeight(text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, 415);
        m_Canvas.DrawBox(CARBON_RECT(text_pos.x - text_padding.x,
                                     text_pos.y - text_padding.y - 2*text_size,
                                     text_pos.x + text_width + text_padding.x,
                                     text_pos.y + text_height + text_padding.y));
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }

      void Render_OptionsTitle(void) {
        static constexpr auto text = "Options";
        static constexpr auto text_size = 8;
        static constexpr auto text_color = Color::White;
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, 50);
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }

      void Render_OptionsGoBack(void) {
        static constexpr auto text = "<ESC> Go Back";
        static constexpr auto text_size = 2;
        static constexpr auto text_color = Color::White;
        static constexpr auto text_pos = CARBON_VEC2(25, 50);
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }

      void Render_OptionsVolume(void) {
        const auto text = cbn::str::fmt("Volume: %.2f", cbn::audio::GetVolume());
        static constexpr auto text_size = 3;
        static constexpr auto text_color = Color::White;
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_height = m_Canvas.TextHeight(text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, c_ScreenHeight/2 - text_height/2);
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }

      void Render_Copyright(void) {
        static constexpr auto text = "Copyright (C) Wasym A. Alonso. All Rights Reserved.";
        static constexpr auto text_size = 2;
        static constexpr auto text_color = Color::Grey;
        static constexpr auto text_padding = 10;
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_height = m_Canvas.TextHeight(text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth - text_padding - text_width, c_ScreenHeight - text_padding - text_height);
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }
    };

    struct Match final : Scene {
      explicit Match(cbn::DrawCanvas &canvas, OnSwitch to_menu)
        : m_Canvas{canvas},
          m_SwitchToMenu{std::move(to_menu)}
      {}

      virtual void OnEnter(void) final override {
        m_P1.Reset(), m_P2.Reset();
      }

      virtual void Update(const f64 dt) final override {
        if (!m_Playing) {
          Update_CheckVictory();
          Update_WaitForServe();
        }
        else {
          m_Ball.Update(dt);
          Update_BallEntersGoalSlot();
          Update_BallCollideWalls();
          Update_BallCollideRackets();
        }
        Update_MoveRackets(dt);
      }

      virtual void Render(void) final override {
        m_Canvas.Fill(Color::Black);
        m_Net.Render(m_Canvas);
        m_P1.Render(m_Canvas), m_P2.Render(m_Canvas);
        Render_GoalSlots();
        Render_HUDDebug();
        if (m_Playing) m_Ball.Render(m_Canvas);
        Render_VictoryMsg();
      }

    private:
      cbn::DrawCanvas &m_Canvas;
      OnSwitch m_SwitchToMenu;
      Net m_Net;
      Ball m_Ball;
      Player<PlayerSide::Left> m_P1;
      Player<PlayerSide::Right> m_P2;
      cbn::Chrono m_StallingTimer {};
      cbn::Chrono m_VictoryTimer {};
      bool m_Playing {false};
      bool m_AI {true};

      void Update_CheckVictory(void) {
        using namespace cbn::time::literals;
        const bool is_win = (m_P1.points >= c_PointsToWin || m_P2.points >= c_PointsToWin) && cbn::math::Abs(m_P1.points - m_P2.points) >= c_PointsDiffToWin;
        if (!is_win) return;
        if (!m_VictoryTimer.IsRunning()) {
          m_VictoryTimer.Restart();
          cbn::audio::Play(res::s_Sound_Win);
        }
        else m_VictoryTimer.Update();
        if (m_VictoryTimer.elapsed >= 5_s) {
          m_VictoryTimer.Stop();
          m_SwitchToMenu();
        }
      }

      void Update_WaitForServe(void) {
        using namespace cbn::time::literals;
        if (cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
          m_StallingTimer.Stop();
          Update_ServeBall();
        }
        else {
          if (!m_StallingTimer.IsRunning()) m_StallingTimer.Restart();
          else m_StallingTimer.Update();
          if (m_StallingTimer.elapsed >= 10_s) {
            ++m_P1.yellow_cards;
            m_StallingTimer.Stop();
            cbn::audio::Play(res::s_Sound_YellowCard);
          }
        }
      }

      void Update_ServeBall(void) {
        m_Playing = true;
        const auto &sound = cbn::math::Rand() < 0.15 ? res::s_Sound_Serve_2 : res::s_Sound_Serve_1;
        cbn::audio::ShiftPitch(sound);
        cbn::audio::Play(sound);
      }

      void Update_PlayerScoredGoal(void) {
        m_Playing = false;
        m_Ball = Ball{};
        cbn::audio::ShiftPitch(res::s_Sound_Goal);
        cbn::audio::Play(res::s_Sound_Goal);
        cbn::audio::Play(res::s_Sound_PointUp);
      }

      void Update_BallEntersGoalSlot(void) {
        static const auto slot_top = m_Canvas.height / 3;
        static const auto slot_bottom = m_Canvas.height * (2.f/3.f);
        if (m_Ball.position.x < 0 && m_Ball.position.y >= slot_top && m_Ball.position.y + c_BallSize < slot_bottom) {
          Update_PlayerScoredGoal();
          ++m_P2.points;
        }
        if (m_Ball.position.x + c_BallSize >= m_Canvas.width && m_Ball.position.y >= slot_top && m_Ball.position.y + c_BallSize < slot_bottom) {
          Update_PlayerScoredGoal();
          ++m_P1.points;
        }
      }

      void Update_BallCollideWalls(void) {
        static const auto slot_top = m_Canvas.height / 3;
        static const auto slot_bottom = m_Canvas.height * (2.f/3.f);
        if (m_Ball.position.y < 0 || m_Ball.position.y + c_BallSize >= m_Canvas.height) {
          m_Ball.velocity.y *= -1;
          ++m_Ball.wall_hits;
          cbn::math::Clamp(m_Ball.position.y, 0, m_Canvas.height - c_BallSize);
          cbn::audio::ShiftPitch(res::s_Sound_Wall);
          cbn::audio::Play(res::s_Sound_Wall);
        }
        if ((m_Ball.position.x < 0 && (m_Ball.position.y < slot_top || m_Ball.position.y >= slot_bottom)) ||
            (m_Ball.position.x + c_BallSize >= m_Canvas.width && (m_Ball.position.y < slot_top || m_Ball.position.y >= slot_bottom))) {
          m_Ball.velocity.x *= -1;
          cbn::math::Clamp(m_Ball.position.x, 0, m_Canvas.width - c_BallSize);
          cbn::audio::ShiftPitch(res::s_Sound_Wall);
          cbn::audio::Play(res::s_Sound_Wall);
        }
      }

      void Update_BallCollideRackets(void) {
        if (Update_BallCollideRacket(m_P1)) return;
        else (Update_BallCollideRacket(m_P2));
      }

      template <PlayerSide S>
      bool Update_BallCollideRacket(Player<S> &p) {
        using namespace cbn::math::literals;
        static constexpr auto dir = S == PlayerSide::Left ? 1 : -1;
        const auto racket = CARBON_RECT_SQUARE_V(p.racket.position, c_RacketSize);
        const auto ball = CARBON_RECT_SQUARE_V(m_Ball.position, c_BallSize);
        if (!racket.Overlaps(ball)) return false;
        const auto ball_center_y = m_Ball.position.y + c_BallSize/2;
        const auto racket_center_y = p.racket.position.y + c_RacketSize/2;
        const auto relative_y = cbn::math::ToClamped((ball_center_y - racket_center_y) / (c_RacketSize/2), -1, 1);
        const auto angle = (relative_y * 45_deg) + cbn::math::Rand(-5_deg, 5_deg);
        const auto d_speed = m_Ball.speed_delta * (1 - 2 * cbn::math::Abs(relative_y));
        const auto speed = cbn::math::ToClamped(m_Ball.velocity.Length() + d_speed, m_Ball.speed_min, m_Ball.speed_max);
        m_Ball.velocity = CARBON_VEC2(dir * cbn::math::Abs(speed * cbn::math::Cos(angle)), speed * cbn::math::Sin(angle));
        m_Ball.wall_hits = 0;
        p.score += 10;
        cbn::audio::ShiftPitch(res::s_Sound_Racket);
        cbn::audio::Play(res::s_Sound_Racket);
        return true;
      }

      void Update_MoveRackets(const f64 dt) {
        if (cbn::win::GetKeyDown(cbn::win::KeyCode::Tab)) m_AI = !m_AI;
        Update_MoveRacket(m_P1.racket, cbn::win::KeyCode::W, cbn::win::KeyCode::S, dt);
        if (!m_AI) Update_MoveRacket(m_P2.racket, cbn::win::KeyCode::UpArrow, cbn::win::KeyCode::DownArrow, dt);
        else Update_MoveRacketWithAI(m_P2.racket, dt);
      }

      void Update_MoveRacket(Racket &r, const cbn::win::KeyCode up, const cbn::win::KeyCode down, const f64 dt) {
        if (cbn::win::GetKey(up))   r.position.y -= r.velocity.y * dt;
        if (cbn::win::GetKey(down)) r.position.y += r.velocity.y * dt;
        cbn::math::Clamp(r.position.y, 0, m_Canvas.height - c_RacketSize);
      }

      void Update_MoveRacketWithAI(Racket &r, const f64 dt) {
        static constexpr auto deadzone = 30;
        static auto target = c_ScreenHeight/2 - c_RacketSize/2;
        const auto t = m_Ball.position.y + c_BallSize/2 - c_RacketSize/2;
        const auto difficulty = cbn::math::Rand() < 0.5 ? 30 : 80;
        cbn::math::Lerp(target, t, difficulty * dt);
        if (r.position.y + deadzone < target)      r.position += r.velocity * dt;
        else if (r.position.y - deadzone > target) r.position -= r.velocity * dt;
        cbn::math::Clamp(r.position.y, 0, m_Canvas.height - c_RacketSize);
      }

      void Render_GoalSlots(void) const {
        static constexpr auto color = Color::Blue;
        static constexpr auto thickness = 8;
        m_Canvas.DrawRect(CARBON_RECT(0, m_Canvas.height/3, thickness, m_Canvas.height/3), color);
        m_Canvas.DrawRect(CARBON_RECT(m_Canvas.width - thickness, m_Canvas.height/3, thickness, m_Canvas.height/3), color);
      }

      void Render_HUDDebug(void) const {
        static constexpr auto text_size = 2;
        static constexpr auto text_color = Color::Grey;
        static constexpr auto text_pos = CARBON_VEC2_1(10);
        static const auto text_height = m_Canvas.TextHeight(text_size);
        static const std::string carbon_ver = cbn::str::fmt(CARBON_NAME " %s", cbn::Version(0, 0, 0));
        static const std::string skap_ver = cbn::str::fmt("AssetPack %llu", res::s_AssetPack.header.build_ver);
        const char *text[] = {
          carbon_ver.c_str(),
          skap_ver.c_str(),
          cbn::str::fmt("%u fps", cbn::win::GetFPS()),
          cbn::str::fmt("AI status: %s", m_AI ? "enabled" : "disabled"),
          cbn::str::fmt("Score: %06u", m_P1.score)
        };
        for (usz i = 0; i < CARBON_ARRAY_LEN(text); ++i) {
          m_Canvas.DrawText(text[i], CARBON_VEC2(text_pos.x, text_pos.y + i*text_height), text_size, text_color);
        }
      }

      void Render_VictoryMsg(void) const {
        if (!m_VictoryTimer.IsRunning()) return;
        static const auto text = m_P1.points > m_P2.points ? "PL1 WINS" : m_AI ? "CPU WINS" : "PL2 WINS";
        static constexpr auto text_size = 4;
        static constexpr auto text_color = Color::Black;
        static constexpr auto text_padding = CARBON_VEC2(20, 15);
        static const auto text_width = m_Canvas.TextWidth(text, text_size);
        static const auto text_height = m_Canvas.TextHeight(text_size);
        static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, c_ScreenHeight/2);
        m_Canvas.DrawBox(CARBON_RECT(text_pos.x - text_padding.x,
                                     text_pos.y - text_padding.y - 2*text_size,
                                     text_pos.x + text_width + text_padding.x,
                                     text_pos.y + text_height + text_padding.y));
        m_Canvas.DrawText(text, text_pos, text_size, text_color);
      }
    };
  }

  struct WindowSpec final {
    usz width, height;
    std::string name;
    u32 max_fps;
  };

  struct Window final {
    explicit Window(WindowSpec spec) : m_Canvas{cbn::DrawCanvas::make(spec.width, spec.height)}
    {
      res::Init();
      cbn::win::Open(spec.width, spec.height, spec.name.c_str());
      cbn::win::SetMaxFPS(spec.max_fps);
      if (auto i = res::s_AssetPack.Lookup<cbn::Image>("./icon.png")) cbn::win::SetIcon(*i);
      else CARBON_UNREACHABLE;
    }

    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;

    ~Window(void) {
      cbn::win::Close();
      res::Shutdown();
      m_Canvas.Free();
    }

    cbn::DrawCanvas &operator*(void) { return m_Canvas; }

    template <typename T>
    void Loop(T &&callback) {
      cbn::win::ForFrame([&](const auto dt){
        callback(dt);
        cbn::win::Update(m_Canvas);
      });
    }

  private:
    cbn::DrawCanvas m_Canvas {};
  };

  struct Game final {
    explicit Game(void)
      : m_Window{{
          .width = c_ScreenWidth,
          .height = c_ScreenHeight,
          .name = c_Name,
          .max_fps = 0  // Set to unlimited for debug purposes
        }},
        m_Scene_1{*m_Window, [&](){
          m_Scene->OnExit();
          m_Scene = &m_Scene_2;
          m_Scene->OnEnter();
        }},
        m_Scene_2{*m_Window, [&](){
          m_Scene->OnExit();
          m_Scene = &m_Scene_1;
          m_Scene->OnEnter();
        }}
    {
      m_Scene->OnEnter();
    }

    Game(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(const Game &) = delete;
    Game &operator=(Game &&) = delete;

    void Run(void) {
      m_Window.Loop([this](const auto dt){
        m_Scene->Update(dt);
        m_Scene->Render();
      });
    }

  private:
    Window m_Window;
    scene::Scene *m_Scene {&m_Scene_1};
    scene::MainMenu m_Scene_1;
    scene::Match m_Scene_2;
  };
}

// TODO: If after serving the ball, the player doesn't respond, the point doesn't count,
//       and it has to be repeated. If this happens 3 times in a row, the point counts and
//       the opponent gets a yellow card.
// TODO: 4 yellow cards ==> red card, which is a direct expulsion and it will lose the game.
// TODO: If after hitting the ball, it bounces off the walls (collides) 7 times or more,
//       the game stops (needs to be served again), and the player who did it gets a blue card.
// TODO: 3 blue cards ==> yellow card, and +1 point to the opponent.
// TODO: Add abilities that can be used during a game:
//         - Slow down ball for 2s or so.
//         - Dash with the racket up and down.
//         - Smash the ball forward (the racket goes up to the net to smash the ball and returns).
//           If ends up in goal, +500 in score.

int main(void) {
  pong::Game game;
  game.Run();
}
