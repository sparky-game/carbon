// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

namespace pong {
  using namespace cbn::math::literals;

  static constexpr auto c_Name                 = "PONG";
  static constexpr auto c_MaxFPS               = 120;
  static constexpr auto c_ScreenWidthRatio     = 16;
  static constexpr auto c_ScreenHeightRatio    = 9;
  static constexpr auto c_ScreenRatioFactor    = 80;
  static constexpr auto c_ScreenWidth          = c_ScreenWidthRatio * c_ScreenRatioFactor;
  static constexpr auto c_ScreenHeight         = c_ScreenHeightRatio * c_ScreenRatioFactor;
  static constexpr auto c_BallSize             = 32;
  static constexpr auto c_BallSpeed            = 1000;
  static constexpr auto c_BallSpeedAdjustment  = 400;
  static constexpr auto c_BallSpeedMin         = c_BallSpeed * 0.9;
  static constexpr auto c_BallSpeedMax         = c_BallSpeed * 2;
  static constexpr auto c_RacketPadding        = 100;
  static constexpr auto c_RacketLength         = 5;
  static constexpr auto c_RacketHeight         = c_RacketLength * c_BallSize;
  static constexpr auto c_RacketSpeed          = 1100;
  static constexpr auto c_MaxReflectionAngle   = 75_deg;
  static constexpr auto c_PointsToWin          = 11;
  static constexpr auto c_PointsFontSize       = 8;
  static constexpr auto c_PointsFontPadding    = CARBON_VEC2(100, 50);
  static constexpr auto c_StartMsgText         = "Press SPACE to serve";
  static constexpr auto c_StartMsgFontSize     = 5;
  static constexpr auto c_StartMsgFontColor    = 0xffdd33ff;
  static constexpr auto c_StartMsgBoxPadding   = CARBON_VEC2(20, 15);
  static constexpr auto c_HUDDebugTextPosition = CARBON_VEC2_1(10);
  static constexpr auto c_HUDDebugFontSize     = 2;
  static constexpr auto c_HUDDebugFontColor    = 0x737373ff;
  static constexpr auto c_NetThickness         = 4;
  static constexpr auto c_NetColor             = c_HUDDebugFontColor;
  static constexpr auto c_BackgroundColor      = 0x181818ff;
  static constexpr auto c_ForegroundColor      = 0xdcdcdcff;

  namespace res {
    static cbn::SKAP s_AssetPack;
    static cbn::sprite_mgr::UID s_Sprite_YellowCard;
    static cbn::sprite_mgr::UID s_Sprite_RedCard;
    static cbn::audio::UID s_Sound_Music;
    static cbn::audio::UID s_Sound_Serve;
    static cbn::audio::UID s_Sound_P1;
    static cbn::audio::UID s_Sound_P2;
    static cbn::audio::UID s_Sound_Goal;
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
      if (auto i = s_AssetPack.LoadAudioStream("./music.ogg")) s_Sound_Music = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./serve.ogg")) s_Sound_Serve = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./p1.ogg")) s_Sound_P1 = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./p2.ogg")) s_Sound_P2 = *i;
      else CARBON_UNREACHABLE;
      if (auto i = s_AssetPack.LoadAudio("./goal.ogg")) s_Sound_Goal = *i;
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

  template <cbn::meta::Numeric T>
  struct Stat final {
    void Set(const T value) {
      m_Value = value;
      UpdateString();
    }

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

    auto GetInt(void) const {
      return m_Value;
    }

    auto GetStr(void) const {
      return m_String.c_str();
    }

    bool operator==(const Stat &s) const {
      return m_Value == s.GetInt();
    }

    bool operator==(const T value) const {
      return m_Value == value;
    }

  private:
    T m_Value {0};
    std::string m_String {"0"};

    void UpdateString(void) {
      m_String = std::to_string(m_Value);
    }
  };

  struct Drawable {
    virtual ~Drawable(void) = default;
    virtual void Render(cbn::DrawCanvas &canvas) const = 0;
  };

  struct Entity : Drawable {
    cbn::Vec2 position;
    cbn::Vec2 velocity;
    explicit Entity(const cbn::Vec2 &p = CARBON_VEC2_ZERO, const cbn::Vec2 &v = CARBON_VEC2_ZERO) : position{p}, velocity{v} {}
  };

  struct Ball final : Entity {
    explicit Ball(void) : Entity{CARBON_VEC2(c_ScreenWidth/2 - c_BallSize/2, c_ScreenHeight/2 - c_BallSize/2)}
    {
      auto angle = cbn::math::Mod((cbn::math::Rand(0, 120_deg) + 300_deg), 2_pi);
      if (cbn::math::Rand() <= 0.5) angle += pi;
      velocity = CARBON_VEC2(c_BallSpeed * cbn::math::Cos(angle), c_BallSpeed * cbn::math::Sin(angle));
    }

    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      canvas.DrawRect(CARBON_RECT_SQUARE_V(position, c_BallSize), c_ForegroundColor);
    }
  };

  struct Racket final : Entity {
    explicit Racket(const cbn::Vec2 &p) : Entity{p, CARBON_VEC2(0, c_RacketSpeed)} {}

    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      canvas.DrawRect(CARBON_RECT_V(position, c_BallSize, c_RacketHeight), c_ForegroundColor);
    }
  };

  enum struct PlayerSide { Left, Right };

  template <PlayerSide S>
  struct Player final : Drawable {
    Racket racket;
    Stat<u8> points;
    u32 score {0};
    u8 yellow_cards {0};

    explicit Player(void) : racket{CARBON_VEC2(0, c_ScreenHeight/2 - c_RacketHeight/2)}
    {
      if constexpr (S == PlayerSide::Left) racket.position.x = c_RacketPadding;
      else racket.position.x = c_ScreenWidth - c_RacketPadding - c_BallSize;
    }

    virtual void Render(cbn::DrawCanvas &canvas) const final override {
      racket.Render(canvas);
      RenderPoints(canvas);
      if (yellow_cards) RenderCards(canvas);
    }

    void Reset(void) {
      points.Set(0);
      score = 0;
      yellow_cards = 0;
    }

  private:
    void RenderPoints(cbn::DrawCanvas &canvas) const {
      auto pos = CARBON_VEC2(c_ScreenWidth/2, c_PointsFontPadding.y);
      if constexpr (S == PlayerSide::Left) pos.x -= c_PointsFontPadding.x + canvas.TextWidth(points.GetStr(), c_PointsFontSize);
      else pos.x += c_PointsFontPadding.x;
      canvas.DrawText(points.GetStr(), pos, c_PointsFontSize, c_ForegroundColor);
    }

    void RenderCards(cbn::DrawCanvas &canvas) const {
      static constexpr auto padding = 10;
      static const auto * const sp = cbn::sprite_mgr::Lookup(res::s_Sprite_YellowCard);
      static const auto sp_pos = CARBON_VEC2(padding, c_ScreenHeight - padding - sp->height);
      static const auto text_height = canvas.TextHeight(c_HUDDebugFontSize);
      static const auto text_pos = CARBON_VEC2(2*padding + sp->width, c_ScreenHeight - padding - text_height);
      canvas.DrawSprite(sp, sp_pos);
      canvas.DrawText(cbn::str::fmt("x%hhu", yellow_cards),
                      text_pos,
                      c_HUDDebugFontSize,
                      c_ForegroundColor);
    }
  };

  struct Game final {
    explicit Game(void) : m_Canvas{cbn::DrawCanvas::make(c_ScreenWidth, c_ScreenHeight)},
                          m_Net{CARBON_RECT(c_ScreenWidth/2 - c_NetThickness + 1, 0, c_NetThickness, c_ScreenHeight)}
    {
      res::Init();
      cbn::win::Open(m_Canvas.width, m_Canvas.height, c_Name);
      cbn::win::SetMaxFPS(c_MaxFPS);
      if (auto i = res::s_AssetPack.Lookup<cbn::Image>("./icon.png")) cbn::win::SetIcon(*i);
      cbn::audio::Play(res::s_Sound_Music);
    }

    Game(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(const Game &) = delete;
    Game &operator=(Game &&) = delete;

    ~Game(void) {
      cbn::win::Close();
      res::Shutdown();
      m_Canvas.Free();
    }

    void Run(void) {
      cbn::win::ForFrame([&](const auto dt){
        Update(dt);
        Render();
        cbn::win::Update(m_Canvas);
      });
    }

  private:
    cbn::DrawCanvas m_Canvas {};
    const cbn::Rect m_Net {};
    Ball m_Ball;
    Player<PlayerSide::Left> m_P1;
    Player<PlayerSide::Right> m_P2;
    cbn::Chrono m_StallingTimer {};
    bool m_Playing {false};
    bool m_StartScreen {true};
    bool m_PlayingMusic {false};
    bool m_AI {false};

    void Update(const f64 dt) {
      if (!m_Playing) {
        Update_PlayerWinner();
        Update_ServeBall();
      }
      else {
        m_Ball.position += m_Ball.velocity * dt;
        Update_BallCollideWalls();
        Update_BallCollideRackets();
      }
      Update_MoveRackets(dt);
    }

    void Update_PlayerWinner(void) {
      if (m_P1.points == m_P2.points || (m_P1.points != c_PointsToWin && m_P2.points != c_PointsToWin)) return;
      m_StartScreen = true;
      if (!m_PlayingMusic) {
        cbn::audio::Play(res::s_Sound_Win);
        cbn::audio::Play(res::s_Sound_Music);
      }
      m_PlayingMusic = true;
    }

    void Update_ServeBall(void) {
      using namespace cbn::time::literals;
      if (cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) {
        if (m_StartScreen) m_P1.Reset(), m_P2.Reset();
        else m_StallingTimer.Stop();
        m_Playing = true;
        m_StartScreen = false;
        m_PlayingMusic = false;
        cbn::audio::Stop(res::s_Sound_Music);
        cbn::audio::Play(res::s_Sound_Serve);
      }
      else {
        if (m_StartScreen || m_Playing) return;
        if (!m_StallingTimer.IsRunning()) m_StallingTimer.Restart();
        else m_StallingTimer.Update();
        if (m_StallingTimer.elapsed >= 10_s) {
          ++m_P1.yellow_cards;
          m_StallingTimer.Stop();
          cbn::audio::Play(res::s_Sound_YellowCard);
        }
      }
    }

    void Update_PlayerScoredGoal(void) {
      m_Playing = false;
      m_Ball = Ball{};
      if (m_P1.points != c_PointsToWin && m_P2.points != c_PointsToWin) cbn::audio::Play(res::s_Sound_Goal);
    }

    void Update_BallCollideWalls(void) {
      if (m_Ball.position.x < 0) {
        Update_PlayerScoredGoal();
        ++m_P2.points;
      }
      if (m_Ball.position.x + c_BallSize >= m_Canvas.width) {
        Update_PlayerScoredGoal();
        ++m_P1.points;
      }
      if (m_Ball.position.y < 0 || m_Ball.position.y + c_BallSize >= m_Canvas.height) m_Ball.velocity.y *= -1;
      cbn::math::Clamp(m_Ball.position.y, 0, m_Canvas.height - c_BallSize);
    }

    void Update_BallCollideRackets(void) {
      if (Update_ResolveBallCollisionWithRacket(m_P1.racket, 1, res::s_Sound_P1)) {
        m_P1.score += 10;
        return;
      }
      if (Update_ResolveBallCollisionWithRacket(m_P2.racket, -1, res::s_Sound_P2)) m_P2.score += 10;
    }

    bool Update_ResolveBallCollisionWithRacket(const Racket &r, i8 dir, const cbn::audio::UID &sound) {
      auto ball = CARBON_RECT_SQUARE_V(m_Ball.position, c_BallSize);
      if (!CARBON_RECT_V(r.position, c_BallSize, c_RacketHeight).Overlaps(ball)) return false;
      const f64 ball_center_y = m_Ball.position.y + c_BallSize/2;
      const f64 racket_center_y = r.position.y + c_RacketHeight/2;
      f64 relative_y = (ball_center_y - racket_center_y) / (c_RacketHeight/2);
      cbn::math::Clamp(relative_y, -1, 1);
      const f64 angle = relative_y * (c_MaxReflectionAngle);
      const f64 d_speed = c_BallSpeedAdjustment * (1 - 2 * cbn::math::Abs(relative_y));
      auto speed = m_Ball.velocity.Length() + d_speed;
      cbn::math::Clamp(speed, c_BallSpeedMin, c_BallSpeedMax);
      m_Ball.velocity.x = dir * cbn::math::Abs(speed * cbn::math::Cos(angle));
      m_Ball.velocity.y = speed * cbn::math::Sin(angle);
      cbn::audio::Play(sound);
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
      cbn::math::Clamp(r.position.y, 0, m_Canvas.height - c_RacketHeight);
    }

    void Update_MoveRacketWithAI(Racket &r, const f64 dt) {
      static constexpr auto error_chance = 0.65;
      static constexpr auto max_error = 80.0;
      static constexpr auto deadzone = 30;
      static constexpr auto smoothing_factor = 30;
      static constexpr auto return_center_speed_factor = 0.25;
      static constexpr auto center_y = c_ScreenHeight/2 - c_RacketHeight/2;
      static f64 smooth_target = center_y;
      f64 target = center_y;
      f64 speed = r.velocity.y;
      if (m_Ball.velocity.x < 0) speed *= return_center_speed_factor;
      else {
        const f64 t = m_Ball.position.y + c_BallSize/2 - c_RacketHeight/2;
        target = cbn::math::Rand() < error_chance ? t + cbn::math::Rand(-max_error, max_error) : t;
      }
      cbn::math::Lerp(smooth_target, target, smoothing_factor * dt);
      if (r.position.y + deadzone < smooth_target)      r.position.y += speed * dt;
      else if (r.position.y - deadzone > smooth_target) r.position.y -= speed * dt;
      cbn::math::Clamp(r.position.y, 0, m_Canvas.height - c_RacketHeight);
    }

    void Render(void) {
      m_Canvas.Fill(c_BackgroundColor);
      m_Canvas.DrawRect(m_Net, c_NetColor);
      m_P1.Render(m_Canvas), m_P2.Render(m_Canvas);
      Render_HUDDebug();
      Render_StartScreen();
      if (m_Playing) m_Ball.Render(m_Canvas);
    }

    void Render_HUDDebug(void) {
      static const auto text_height = m_Canvas.TextHeight(c_HUDDebugFontSize);
      static const std::string carbon_ver = cbn::str::fmt(CARBON_NAME " %s", cbn::Version(0, 0, 0));
      static const std::string skap_ver = cbn::str::fmt("AssetPack %llu", res::s_AssetPack.header.build_ver);
      const char *lines[] = {
        carbon_ver.c_str(),
        skap_ver.c_str(),
        cbn::str::fmt("%u fps", cbn::win::GetFPS()),
        cbn::str::fmt("AI status: %s", m_AI ? "enabled" : "disabled"),
        cbn::str::fmt("Score: %06u", m_P1.score)
      };
      for (usz i = 0; i < CARBON_ARRAY_LEN(lines); ++i) {
        m_Canvas.DrawText(lines[i],
                          CARBON_VEC2(c_HUDDebugTextPosition.x, c_HUDDebugTextPosition.y + i*text_height),
                          c_HUDDebugFontSize,
                          c_HUDDebugFontColor);
      }
    }

    void Render_StartScreen(void) {
      if (!m_StartScreen) return;
      static const auto text_width = m_Canvas.TextWidth(c_StartMsgText, c_StartMsgFontSize);
      static const auto text_height = m_Canvas.TextHeight(c_StartMsgFontSize);
      static const auto text_pos = CARBON_VEC2(c_ScreenWidth/2 - text_width/2, c_ScreenHeight/2 + 100);
      m_Canvas.DrawBox(CARBON_RECT(text_pos.x - c_StartMsgBoxPadding.x,
                                   text_pos.y - c_StartMsgBoxPadding.y - 2*c_StartMsgFontSize,
                                   text_pos.x + text_width + c_StartMsgBoxPadding.x,
                                   text_pos.y + text_height + c_StartMsgBoxPadding.y));
      m_Canvas.DrawText(c_StartMsgText,
                        text_pos,
                        c_StartMsgFontSize,
                        c_StartMsgFontColor);
    }
  };
}

// TODO: If after serving the ball, the opponent doesn't respond, the point doesn't count,
//       and it has to be repeated. If this happens 3 times in a row, the point counts and
//       the opponent gets a yellow card.
// TODO: If a player has 3 yellow cards and commits a fault, it will get a red card, which is a
//       direct expulsion and it will lose the game.
// TODO: Add abilities that can be used during a game:
//         - Slow down ball for 2s or so.
//         - Dash with the racket up and down.
//         - Smash the ball forward (the racket goes up to the net to smash the ball and returns).
//           If ends up in goal, +500 in score.

int main(void) {
  pong::Game game;
  game.Run();
}
