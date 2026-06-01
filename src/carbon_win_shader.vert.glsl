#version 330 core

out vec2 uv;

void main() {
  vec2 pos[] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));
  vec2 uvs[] = vec2[3](vec2(0, 1), vec2(2, 1), vec2(0, -1));
  gl_Position = vec4(pos[gl_VertexID], 0, 1);
  uv = uvs[gl_VertexID];
}
