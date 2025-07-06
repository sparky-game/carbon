// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#include <carbon.h>

namespace pong {
  static constexpr auto c_MaxFPS             = 120;
  static constexpr auto c_ScreenWidthRatio   = 16;
  static constexpr auto c_ScreenHeightRatio  = 9;
  static constexpr auto c_ScreenRatioFactor  = 80;
  static constexpr auto c_ScreenWidth        = c_ScreenWidthRatio * c_ScreenRatioFactor;
  static constexpr auto c_ScreenHeight       = c_ScreenHeightRatio * c_ScreenRatioFactor;
  static constexpr auto c_BallSize           = 32;
  static constexpr auto c_BallSpeed          = 1000;
  static constexpr auto c_RacketPadding      = 100;
  static constexpr auto c_RacketLength       = 5;
  static constexpr auto c_RacketSpeed        = 1100;
  static constexpr auto c_MaxReflectionAngle = 75;
  static constexpr auto c_BackgroundColor    = 0x181818ff;
  static constexpr auto c_ForegroundColor    = 0xdcdcdcff;

  struct Entity {
    cbn::Vec2 position;
    cbn::Vec2 velocity;
    explicit Entity(const cbn::Vec2 &p, const cbn::Vec2 &v) : position{p}, velocity{v} {}
    virtual ~Entity(void) = default;
    virtual void Render(cbn::DrawCanvas &canvas) = 0;
  };

  struct Ball : Entity {
    Ball(const cbn::Vec2 &p, const cbn::Vec2 &v) : Entity{p, v} {}
    virtual void Render(cbn::DrawCanvas &canvas) override {
      canvas.DrawRect(CARBON_RECT_SQUARE_V(position, c_BallSize), c_ForegroundColor);
    }
  };

  struct Racket : Entity {
    Racket(const cbn::Vec2 &p, const cbn::Vec2 &v) : Entity{p, v} {}
    virtual void Render(cbn::DrawCanvas &canvas) override {
      canvas.DrawRect(CARBON_RECT_V(position, c_BallSize, c_RacketLength * c_BallSize), c_ForegroundColor);
    }
  };

  struct Game {
    Game(void) : m_Canvas{cbn::DrawCanvas::make(c_ScreenWidth, c_ScreenHeight)},
                 m_Ball{GetRandomBall()},
                 m_Racket_1{
                   CARBON_VEC2(c_RacketPadding, c_ScreenHeight/2 - (c_RacketLength * c_BallSize) / 2),
                   CARBON_VEC2_1(c_RacketSpeed)
                 },
                 m_Racket_2{
                   CARBON_VEC2(c_ScreenWidth - c_RacketPadding - c_BallSize, c_ScreenHeight/2 - (c_RacketLength * c_BallSize) / 2),
                   CARBON_VEC2_1(c_RacketSpeed)
                 },
                 m_Started{false}
    {
      cbn::win::Open(m_Canvas.width, m_Canvas.height, "Pong");
      cbn::win::SetMaxFPS(c_MaxFPS);
    }

    ~Game(void) {
      cbn::win::Close();
      m_Canvas.Free();
    }

    void Run(void) {
      cbn::win::ForFrame([&](const auto dt){
        Update(dt);
        Render();
      });
    }

  private:
    cbn::DrawCanvas m_Canvas;
    Ball m_Ball;
    Racket m_Racket_1;
    Racket m_Racket_2;
    bool m_Started;

    void Update(const f64 dt) {
      if (!m_Started && cbn::win::GetKeyDown(cbn::win::KeyCode::Space)) m_Started = true;
      if (m_Started) {
        m_Ball.position += m_Ball.velocity * dt;
        BallCollideWalls();
        BallCollideRackets();
      }
      MoveRackets(dt);
    }

    void Render(void) {
      m_Canvas.Fill(c_BackgroundColor);
      if (m_Started) m_Ball.Render(m_Canvas);
      m_Racket_1.Render(m_Canvas);
      m_Racket_2.Render(m_Canvas);
      cbn::win::Update(m_Canvas);
    }

    void BallCollideWalls(void) {
      if (m_Ball.position.x < 0 || m_Ball.position.x + c_BallSize >= m_Canvas.width) {
        m_Started = false;
        m_Ball = GetRandomBall();
      }
      if (m_Ball.position.y < 0 || m_Ball.position.y + c_BallSize >= m_Canvas.height) {
        m_Ball.velocity.y *= -1;
      }
      cbn::math::Clamp(m_Ball.position.y, 0, m_Canvas.height - c_BallSize);
    }

    void BallCollideRackets(void) {
      auto r1   = CARBON_RECT_V(m_Racket_1.position, c_BallSize, c_RacketLength * c_BallSize);
      auto r2   = CARBON_RECT_V(m_Racket_2.position, c_BallSize, c_RacketLength * c_BallSize);
      auto ball = CARBON_RECT_SQUARE_V(m_Ball.position, c_BallSize);
      f64 ball_center_y = m_Ball.position.y + c_BallSize/2;
      if (r1.Overlaps(ball)) {
        f64 racket_center_y = m_Racket_1.position.y + (c_RacketLength * c_BallSize) / 2;
        f64 relative_y = (ball_center_y - racket_center_y) / ((c_RacketLength * c_BallSize) / 2);
        cbn::math::Clamp(relative_y, -1, 1);
        f64 angle = relative_y * (c_MaxReflectionAngle * cbn::math::pi / 180);
        auto speed = m_Ball.velocity.Length();
        m_Ball.velocity.x = cbn::math::Abs(speed * cbn::math::Cos(angle));
        m_Ball.velocity.y = speed * cbn::math::Sin(angle);
      }
      if (r2.Overlaps(ball)) {
        f64 racket_center_y = m_Racket_2.position.y + (c_RacketLength * c_BallSize) / 2;
        f64 relative_y = (ball_center_y - racket_center_y) / ((c_RacketLength * c_BallSize) / 2);
        cbn::math::Clamp(relative_y, -1, 1);
        f64 angle = relative_y * (c_MaxReflectionAngle * cbn::math::pi / 180);
        auto speed = m_Ball.velocity.Length();
        m_Ball.velocity.x = -cbn::math::Abs(speed * cbn::math::Cos(angle));
        m_Ball.velocity.y = speed * cbn::math::Sin(angle);
      }
    }

    void MoveRackets(const f64 dt) {
      if (cbn::win::GetKeyDown(cbn::win::KeyCode::W)) {
        m_Racket_1.position.y -= m_Racket_1.velocity.y * dt;
      }
      if (cbn::win::GetKeyDown(cbn::win::KeyCode::UpArrow)) {
        m_Racket_2.position.y -= m_Racket_2.velocity.y * dt;
      }
      if (cbn::win::GetKeyDown(cbn::win::KeyCode::S)) {
        m_Racket_1.position.y += m_Racket_1.velocity.y * dt;
      }
      if (cbn::win::GetKeyDown(cbn::win::KeyCode::DownArrow)) {
        m_Racket_2.position.y += m_Racket_2.velocity.y * dt;
      }
      cbn::math::Clamp(m_Racket_1.position.y, 0, m_Canvas.height - (c_RacketLength * c_BallSize));
      cbn::math::Clamp(m_Racket_2.position.y, 0, m_Canvas.height - (c_RacketLength * c_BallSize));
    }

    static Ball GetRandomBall(void) {
      using namespace cbn::math::const_literals;
      auto angle = cbn::math::Rand(0, 2_pi);
      return {
        CARBON_VEC2(c_ScreenWidth/2 - c_BallSize/2, c_ScreenHeight/2 - c_BallSize/2),
        CARBON_VEC2(c_BallSpeed * cbn::math::Cos(angle), c_BallSpeed * cbn::math::Sin(angle))
      };
    }
  };
}

int main(void) {
  pong::Game game;
  game.Run();
}
