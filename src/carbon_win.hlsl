struct V2f {
  float4 pos : SV_Position;
  float2 uv : TEXCOORD0;
};

V2f vert(uint id : SV_VertexID) {
  float2 pos[] = {{-1, -1}, {3, -1}, {-1, 3}};
  float2 uvs[] = {{0, 1}, {2, 1}, {0, -1}};
  V2f o;
  o.pos = float4(pos[id], 0, 1);
  o.uv = uvs[id];
  return o;
}

Texture2D tex : register(t0);
SamplerState s : register(s0);

float4 frag(V2f in) : SV_Target {
  return tex.Sample(s, in.uv);
}
