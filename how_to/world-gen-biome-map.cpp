#include <carbon.h>

#include <array>

static constexpr u32 c_Regions[] = {
  0x1580d1ff,  // #1580d1 (Ocean [0])
  0xe3d99fff,  // #e3d99f (Warm [1])
  0xa4cf72ff,  // #a4cf72 (Lush [2])
  0x2d662bff,  // #2d662b (Cold [3])
  0xd1f0eeff,  // #d1f0ee (Ice [4])
  0x156dd1ff   // #156dd1 (DeepOcean [5])
};

static constexpr i64 convolute(i64 x, i64 a) {
  return (x * (x * 6364136223846793005 + 1442695040888963407)) + a;
}

struct Layer {
  explicit Layer(i64 base_seed) : m_BaseSeed{base_seed}
  {
    InitBaseSeed(base_seed);
  }

  explicit Layer(i64 base_seed, Layer *parent) : m_BaseSeed{base_seed}, m_Parent{parent}
  {
    InitBaseSeed(base_seed);
  }

  void InitWorldGenSeed(i64 seed) {
    m_WorldGenSeed = seed;
    if (m_Parent) m_Parent->InitWorldGenSeed(seed);
    m_WorldGenSeed = convolute(m_WorldGenSeed, m_BaseSeed);
    m_WorldGenSeed = convolute(m_WorldGenSeed, m_BaseSeed);
    m_WorldGenSeed = convolute(m_WorldGenSeed, m_BaseSeed);
  }

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) = 0;

  virtual cbn::DrawCanvas Process(usz size) {
    if (m_Parent) m_Parent->Process(size);
    return ProcessThisLayer(size);
  }

protected:
  i64 m_BaseSeed {0};
  i64 m_WorldGenSeed {0};
  i64 m_ChunkSeed {0};
  Layer *m_Parent {nullptr};

  void InitBaseSeed(i64 base_seed) {
    m_BaseSeed = convolute(m_BaseSeed, base_seed);
    m_BaseSeed = convolute(m_BaseSeed, base_seed);
    m_BaseSeed = convolute(m_BaseSeed, base_seed);
  }

  void InitChunkSeed(i64 x, i64 z) {
    m_ChunkSeed = m_WorldGenSeed;
    m_ChunkSeed = convolute(m_ChunkSeed, x);
    m_ChunkSeed = convolute(m_ChunkSeed, z);
    m_ChunkSeed = convolute(m_ChunkSeed, x);
    m_ChunkSeed = convolute(m_ChunkSeed, z);
  }

  i32 NextInt(i32 r) {
    auto i = (m_ChunkSeed >> 24) % r;
    if (i < 0) i += r;
    m_ChunkSeed = convolute(m_ChunkSeed, m_WorldGenSeed);
    return i;
  }

  template <typename T, typename... Ts>
  T SelectRandom(T x, Ts... xs) {
    constexpr auto N = 1 + sizeof...(xs);
    std::array<T, N> A = {x, xs...};
    return A[NextInt(N)];
  }

  cbn::DrawCanvas ProcessThisLayer(usz size) {
    i32 x, y, w, h;
    x = y = -size / 2;
    w = h = size + 1;
    auto dc = GetInts(x, y, w, h);
    for (usz i = 0; i < dc.width; ++i) {
      for (usz j = 0; j < dc.height; ++j) {
        dc(i, j) = c_Regions[dc(i, j)];
      }
    }
    return dc;
  }
};

struct Island : Layer {
  explicit Island(i64 base_seed) : Layer{base_seed} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    for (usz i = 0; i < dc.width; ++i) {
      for (usz j = 0; j < dc.height; ++j) {
        InitChunkSeed(x + i, y + j);
        dc(i, j) = !NextInt(10) ? 1 : 0;
      }
    }
    if (-w < x && x <= 0 && -h < y && y <= 0) dc(-x, -y) = 1;
    return dc;
  }
};

struct Zoom : Layer {
  explicit Zoom(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual u32 ModeOrRandom(u32 a, u32 b, u32 c, u32 d) {
    return c == d && a != b ? c : SelectRandom(a, b, c, d);
  }

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    auto px = x >> 1, py = y >> 1;
    auto pw = (w >> 1) + 2, ph = (h >> 1) + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    auto iw = (pw - 1) << 1, ih = (ph - 1) << 1;
    cbn::DrawCanvas i_dc {iw, ih};
    for (i32 j = 0; j < ph - 1; ++j) {
      for (i32 i = 0; i < pw - 1; ++i) {
        InitChunkSeed((px + i) * 2, (py + j) * 2);
        i_dc(2*i, 2*j) = p_dc(i, j);
        i_dc(2*i, 2*j + 1) = SelectRandom(p_dc(i, j), p_dc(i, j + 1));
        i_dc(2*i + 1, 2*j) = SelectRandom(p_dc(i, j), p_dc(i + 1, j));
        i_dc(2*i + 1, 2*j + 1) = ModeOrRandom(p_dc(i, j), p_dc(i + 1, j), p_dc(i, j + 1), p_dc(i + 1, j + 1));
      }
    }
    auto sx = x & 1, sy = y & 1;
    cbn::DrawCanvas dc {w, h};
    for (usz i = 0; i < dc.width; ++i) {
      for (usz j = 0; j < dc.height; ++j) {
        dc(i, j) = i_dc(sx + i, sy + j);
      }
    }
    return dc;
  }

  virtual cbn::DrawCanvas Process(usz size) override {
    if (m_Parent) m_Parent->Process(size/2);
    return ProcessThisLayer(size);
  }
};

struct Zoom_F : Zoom {
  explicit Zoom_F(i64 base_seed, Layer *parent) : Zoom{base_seed, parent} {}

  virtual u32 ModeOrRandom(u32 a, u32 b, u32 c, u32 d) override {
    return SelectRandom(a, b, c, d);
  }
};

struct AddIsland : Layer {
  explicit AddIsland(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto px = x - 1, py = y - 1;
    auto pw = w + 2, ph = h + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    for (i32 i = 0; i < w; ++i) {
      for (i32 j = 0; j < h; ++j) {
        InitChunkSeed(x + i, y + j);
        auto a = p_dc(i, j), b = p_dc(i + 2, j), c = p_dc(i, j + 2), d = p_dc(i + 2, j + 2), k = p_dc(i + 1, j + 1);
        if (k || (!a && !b && !c && !d)) {
          if (k > 0 && (!a || !b || !c || !d)) {
            if (!NextInt(5)) {
              if (k == 4) dc(i, j) = 4;
              else dc(i, j) = 0;
            }
            else dc(i, j) = k;
          }
          else dc(i, j) = k;
        }
        else {
          auto l2 = 1, n = 1;
          if (a) {
            if (!NextInt(l2)) n = a;
            ++l2;
          }
          if (b) {
            if (!NextInt(l2)) n = b;
            ++l2;
          }
          if (c) {
            if (!NextInt(l2)) n = c;
            ++l2;
          }
          if (d) {
            if (!NextInt(l2)) n = d;
            ++l2;
          }
          if (!NextInt(3)) dc(i, j) = n;
          else if (n == 4) dc(i, j) = 4;
          else dc(i, j) = 0;
        }
      }
    }
    return dc;
  }
};

struct RemoveTooMuchOcean : Layer {
  explicit RemoveTooMuchOcean(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto px = x - 1, py = y - 1;
    auto pw = w + 2, ph = h + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    for (i32 j = 0; j < h; ++j) {
      for (i32 i = 0; i < w; ++i) {
        InitChunkSeed(x + i, y + j);
        auto a = p_dc(i + 1, j), b = p_dc(i + 2, j + 1), c = p_dc(i, j + 1), d = p_dc(i + 1, j + 2), k = p_dc(i + 1, j + 1);
        dc(i, j) = k;
        if (!k && !a && !b && !c && !d && !NextInt(2)) dc(i, j) = 1;
      }
    }
    return dc;
  }
};

struct AddTemperatures : Layer {
  explicit AddTemperatures(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto p_dc = m_Parent->GetInts(x, y, w, h);
    for (i32 j = 0; j < h; ++j) {
      for (i32 i = 0; i < w; ++i) {
        InitChunkSeed(x + i, y + j);
        auto k = p_dc(i, j);
        if (!k) dc(i, j) = 0;
        else {
          auto r = NextInt(6);
          if (!r) r = 4;
          else if (r <= 1) r = 3;
          else r = 1;
          dc(i, j) = r;
        }
      }
    }
    return dc;
  }
};

struct WarmToLush : Layer {
  explicit WarmToLush(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto px = x - 1, py = y - 1;
    auto pw = w + 2, ph = h + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    for (i32 j = 0; j < h; ++j) {
      for (i32 i = 0; i < w; ++i) {
        auto k = p_dc(i + 1, j + 1);
        if (k == 1) {
          auto a = p_dc(i + 1, j), b = p_dc(i + 2, j + 1), c = p_dc(i, j + 1), d = p_dc(i + 1, j + 2);
          auto f1 = a == 3 || b == 3 || c == 3 || d == 3;
          auto f2 = a == 4 || b == 4 || c == 4 || d == 4;
          if (f1 || f2) k = 2;
        }
        dc(i, j) = k;
      }
    }
    return dc;
  }
};

struct IceToCold : Layer {
  explicit IceToCold(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto px = x - 1, py = y - 1;
    auto pw = w + 2, ph = h + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    for (i32 j = 0; j < h; ++j) {
      for (i32 i = 0; i < w; ++i) {
        auto k = p_dc(i + 1, j + 1);
        if (k == 4) {
          auto a = p_dc(i + 1, j), b = p_dc(i + 2, j + 1), c = p_dc(i, j + 1), d = p_dc(i + 1, j + 2);
          auto f1 = a == 2 || b == 2 || c == 2 || d == 2;
          auto f2 = a == 1 || b == 1 || c == 1 || d == 1;
          if (f1 || f2) k = 3;
        }
        dc(i, j) = k;
      }
    }
    return dc;
  }
};

struct AddDeepOcean : Layer {
  explicit AddDeepOcean(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto px = x - 1, py = y - 1;
    auto pw = w + 2, ph = h + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    for (i32 j = 0; j < h; ++j) {
      for (i32 i = 0; i < w; ++i) {
        auto a = p_dc(i + 1, j), b = p_dc(i + 2, j + 1), c = p_dc(i, j + 1), d = p_dc(i + 1, j + 2), k = p_dc(i + 1, j + 1);
        if (!k && !a && !b && !c && !d) dc(i, j) = 5;
        else dc(i, j) = k;
      }
    }
    return dc;
  }
};

struct Smooth : Layer {
  explicit Smooth(i64 base_seed, Layer *parent) : Layer{base_seed, parent} {}

  virtual cbn::DrawCanvas GetInts(i32 x, i32 y, usz w, usz h) override {
    cbn::DrawCanvas dc {w, h};
    auto px = x - 1, py = y - 1;
    auto pw = w + 2, ph = h + 2;
    auto p_dc = m_Parent->GetInts(px, py, pw, ph);
    for (i32 j = 0; j < h; ++j) {
      for (i32 i = 0; i < w; ++i) {
        auto a = p_dc(i, j + 1), b = p_dc(i + 2, j + 1), c = p_dc(i + 1, j), d = p_dc(i + 1, j + 2), k = p_dc(i + 1, j + 1);
        if (a == b && c == d) {
          InitChunkSeed(x + i, y + j);
          if (!NextInt(2)) k = a;
          else k = c;
        }
        else {
          if (a == b) k = a;
          if (c == d) k = c;
        }
        dc(i, j) = k;
      }
    }
    return dc;
  }
};

struct BiomeMap {
  BiomeMap(i64 seed = cbn::rng::MT19937())
    : m_Map{1, 1}
  {
    Recreate(seed);
  }

  void Recreate(i64 seed) {
    m_Seed = seed;
    Island l1 {1};
    Zoom_F l2 {2000, &l1};
    AddIsland l3 {1, &l2};
    Zoom l4 {2001, &l3};
    AddIsland l5 {2, &l4};
    AddIsland l6 {50, &l5};
    AddIsland l7 {70, &l6};
    RemoveTooMuchOcean l8 {2, &l7};
    AddTemperatures l9 {2, &l8};
    AddIsland l10 {3, &l9};
    WarmToLush l11 {2, &l10};
    IceToCold l12 {2, &l11};
    Zoom l13 {2002, &l12};
    Zoom l14 {2003, &l13};
    AddIsland l15 {4, &l14};
    AddDeepOcean l16 {4, &l15};
    Zoom l17 {1000, &l16};
    Zoom l18 {1001, &l17};
    Zoom l19 {1000, &l18};
    AddIsland l20 {3, &l19};
    Zoom l21 {1001, &l20};
    Zoom l22 {1002, &l21};
    Zoom l23 {1003, &l22};
    Smooth l24 {1000, &l23};
    l24.InitWorldGenSeed(m_Seed);
    m_Map = l24.Process(2048);  // Scale: 1:4
  }

  const cbn::DrawCanvas &Get(void) const { return m_Map; }

  void Save(const char *file) const {
    auto img = cbn::Image::make(m_Map);
    img.Save(CBN_FILE_FORMAT_PNG, file);
    img.Free();
  }

private:
  i64 m_Seed {0};
  cbn::DrawCanvas m_Map;
};

int main(void) {
  BiomeMap map;
  map.Save("biome-map.png");
}
