#version 410

layout(location = 0) in vec3 position;

uniform mat4 MVP;

void main() {
  vec4 pos = vec4(position, 1);
  gl_Position = MVP * pos;
}
