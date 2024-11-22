#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "application.hpp"
#include "model.h"

void Model::load_shader(const std::string &shader_name) {
  // open files
  constexpr auto shaders_base_directory = ASSETS_DIRECTORY "/shaders/";
  std::string shader_directory = shaders_base_directory + shader_name + '/';

  std::string vertex_file_path = shader_directory + "vert.glsl";
  std::ifstream vert_file(vertex_file_path.data());
  if (not vert_file) {
    throw std::runtime_error("cant open shader" + vertex_file_path);
  }

  std::string fragment_file_path = shader_directory + "frag.glsl";
  std::ifstream fraq_file(fragment_file_path.data());
  if (not fraq_file) {
    throw std::runtime_error("cant open shader" + fragment_file_path);
  }

  // load data
  std::stringstream sstr_vert;
  sstr_vert << vert_file.rdbuf();
  std::string vert_code = sstr_vert.str();

  std::stringstream sstr_frag;
  sstr_frag << fraq_file.rdbuf();
  std::string frag_code = sstr_frag.str();

  // compile shaders
  int error_info_lenght = 0;

  // Compile Vertex Shader
  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  char const *vert_data = vert_code.c_str();
  glShaderSource(vert_id, 1, &vert_data, nullptr);
  glCompileShader(vert_id);

  // Check Vertex Shader
  glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &error_info_lenght);
  if (error_info_lenght > 0) {
    std::vector<char> error_msg(error_info_lenght + 1);
    glGetShaderInfoLog(vert_id, error_info_lenght, nullptr, error_msg.data());
    printf("%s\n", error_msg.data());
    // throw std::runtime_error("shader compile error");
  }

  // Compile Fragment Shader
  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  char const *frag_data = frag_code.c_str();
  glShaderSource(frag_id, 1, &frag_data, nullptr);
  glCompileShader(frag_id);

  // Check Fragment Shader
  glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &error_info_lenght);
  if (error_info_lenght > 0) {
    std::vector<char> error_msg(error_info_lenght + 1);
    glGetShaderInfoLog(frag_id, error_info_lenght, nullptr, error_msg.data());
    printf("%s\n", error_msg.data());
    // throw std::runtime_error("shader compile error");
  }

  // Link the program
  _program_id = glCreateProgram();
  glAttachShader(_program_id, vert_id);
  glAttachShader(_program_id, frag_id);
  glLinkProgram(_program_id);

  // Check the program
  glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &error_info_lenght);
  if (error_info_lenght > 0) {
    std::vector<char> error_msg(error_info_lenght + 1);
    glGetProgramInfoLog(_program_id, error_info_lenght, nullptr,
                        error_msg.data());
    printf("%s\n", error_msg.data());
    // throw std::runtime_error("shader compile error");
  }

  glDetachShader(_program_id, vert_id);
  glDetachShader(_program_id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);
}

static constexpr float float_threshold = 0.000001;

Model::Model(std::string_view filename) {
  default_position();
  _is_loaded = true;
}

void Model::load_sphere(std::vector<Vertex> &vertexes,
                        std::vector<uint32_t> &index, int W, int H) {
  vertexes.resize(W * H);
  index.resize((H - 1) * (W * 2 + 1) - 1);

  float pi = 3.14159265359;
  int i, j, k;
  float theta = 0, phi;
  for (i = 0; i < H; i++, theta += pi / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * pi / (W - 1)) {
      vertexes[i * W + j].normal = {
        sin(theta) * sin(phi),
        cos(theta),
        sin(theta) * cos(phi)
      };
      vertexes[i * W + j].position = {
        vertexes[i * W + j].normal.x,
        vertexes[i * W + j].normal.y,
        vertexes[i * W + j].normal.z
      };
      vertexes[i * W + j].texture_coords = {j / (W - 1.0f), i / (H - 1.0f)};
      vertexes[i * W + j].color = {1, 1, 1};
    }

  for (i = 0, k = 0; i < H - 1; i++) {
    for (j = 0; j < W; j++) {
      index[k++] = i * W + j;
      index[k++] = (i + 1) * W + j;
    }
    if (i != H - 2)
      index[k++] = -1;
  }
}

void Model::load_cube(std::vector<Vertex> &vertexes,
                      std::vector<uint32_t> &index, const Vec3 &point0,
                      const Vec3 &point1) {
  vertexes.resize(24);
  index.resize(29);

  Vec3 normals[6] = {{0, -1, 0}, {0, 1, 0},  {-1, 0, 0},
                     {1, 0, 0},  {0, 0, -1}, {0, 0, 1}};

  vertexes[0].position = point0;
  vertexes[0].normal = normals[0];
  vertexes[0].texture_coords = {0, 2};
  vertexes[0].color = {1, 1, 1};

  vertexes[1].position = {point0.x, point0.y, point1.z};
  vertexes[1].normal = normals[0];
  vertexes[1].texture_coords = {0, 3};
  vertexes[1].color = {1, 1, 1};

  vertexes[2].position = {point1.x, point0.y, point1.z};
  vertexes[2].normal = normals[0];
  vertexes[2].texture_coords = {1, 3};
  vertexes[2].color = {1, 1, 1};

  vertexes[3].position = {point1.x, point0.y, point0.z};
  vertexes[3].normal = normals[0];
  vertexes[3].texture_coords = {1, 2};
  vertexes[3].color = {1, 1, 1};

  vertexes[4].position = point1;
  vertexes[4].normal = normals[1];
  vertexes[4].texture_coords = {1, 0};
  vertexes[4].color = {1, 1, 1};

  vertexes[5].position = {point1.x, point1.y, point0.z};
  vertexes[5].normal = normals[1];
  vertexes[5].texture_coords = {1, 1};
  vertexes[5].color = {1, 1, 1};

  vertexes[6].position = {point0.x, point1.y, point1.z};
  vertexes[6].normal = normals[1];
  vertexes[6].texture_coords = {0, 0};
  vertexes[6].color = {1, 1, 1};

  vertexes[7].position = {point0.x, point1.y, point0.z};
  vertexes[7].normal = normals[1];
  vertexes[7].texture_coords = {0, 1};
  vertexes[7].color = {1, 1, 1};

  vertexes[8].position = point0;
  vertexes[8].normal = normals[2];
  vertexes[8].texture_coords = {0, 3};
  vertexes[8].color = {1, 1, 1};

  vertexes[9].position = {point0.x, point0.y, point1.z};
  vertexes[9].normal = normals[2];
  vertexes[9].texture_coords = {0, 2};
  vertexes[9].color = {1, 1, 1};

  vertexes[10].position = {point0.x, point1.y, point1.z};
  vertexes[10].normal = normals[2];
  vertexes[10].texture_coords = {-1, 2};
  vertexes[10].color = {1, 1, 1};

  vertexes[11].position = {point0.x, point1.y, point0.z};
  vertexes[11].normal = normals[2];
  vertexes[11].texture_coords = {-1, 3};
  vertexes[11].color = {1, 1, 1};

  vertexes[12].position = {point1.x, point0.y, point1.z};
  vertexes[12].normal = normals[3];
  vertexes[12].texture_coords = {2, 3};
  vertexes[12].color = {1, 1, 1};

  vertexes[13].position = {point1.x, point0.y, point0.z};
  vertexes[13].normal = normals[3];
  vertexes[13].texture_coords = {2, 2};
  vertexes[13].color = {1, 1, 1};

  vertexes[14].position = point1;
  vertexes[14].normal = normals[3];
  vertexes[14].texture_coords = {1, 3};
  vertexes[14].color = {1, 1, 1};

  vertexes[15].position = {point1.x, point1.y, point0.z};
  vertexes[15].normal = normals[3];
  vertexes[15].texture_coords = {1, 2};
  vertexes[15].color = {1, 1, 1};

  vertexes[16].position = point0;
  vertexes[16].normal = normals[4];
  vertexes[16].texture_coords = {0, 2};
  vertexes[16].color = {1, 1, 1};

  vertexes[17].position = {point1.z, point0.y, point0.z};
  vertexes[17].normal = normals[4];
  vertexes[17].texture_coords = {1, 2};
  vertexes[17].color = {1, 1, 1};

  vertexes[18].position = {point1.z, point1.y, point0.z};
  vertexes[18].normal = normals[4];
  vertexes[18].texture_coords = {1, 1};
  vertexes[18].color = {1, 1, 1};

  vertexes[19].position = {point0.z, point1.y, point0.z};
  vertexes[19].normal = normals[4];
  vertexes[19].texture_coords = {0, 1};
  vertexes[19].color = {1, 1, 1};

  vertexes[20].position = {point0.z, point0.y, point1.z};
  vertexes[20].normal = normals[5];
  vertexes[20].texture_coords = {0, 3};
  vertexes[20].color = {1, 1, 1};

  vertexes[21].position = {point1.z, point0.y, point1.z};
  vertexes[21].normal = normals[5];
  vertexes[21].texture_coords = {1, 3};
  vertexes[21].color = {1, 1, 1};

  vertexes[22].position = point1;
  vertexes[22].normal = normals[5];
  vertexes[22].texture_coords = {1, 4};
  vertexes[22].color = {1, 1, 1};

  vertexes[23].position = {point0.z, point1.y, point1.z};
  vertexes[23].normal = normals[5];
  vertexes[23].texture_coords = {0, 4};
  vertexes[23].color = {1, 1, 1};

  index[0] = 0;
  index[1] = 1;
  index[2] = 3;
  index[3] = 2;
  index[4] = -1;

  index[5] = 7;
  index[6] = 6;
  index[7] = 5;
  index[8] = 4;
  index[9] = -1;

  index[10] = 8;
  index[11] = 9;
  index[12] = 11;
  index[13] = 10;
  index[14] = -1;

  index[15] = 15;
  index[16] = 14;
  index[17] = 13;
  index[18] = 12;
  index[19] = -1;

  index[20] = 16;
  index[21] = 17;
  index[22] = 19;
  index[23] = 18;
  index[24] = -1;

  index[25] = 20;
  index[26] = 21;
  index[27] = 23;
  index[28] = 22;
}

void Model::load_plane(std::vector<Vertex> &vertexes,
                       std::vector<uint32_t> &indexes) {
  vertexes.resize(4);

  for (auto &vert : vertexes) {
    vert.normal = {0, 1, 0};
    vert.color = {1, 1, 0};
  }

  float scale = 1;

  vertexes[0].position = {scale, 0, scale};
  vertexes[1].position = {scale, 0, -scale};
  vertexes[2].position = {-scale, 0, -scale};
  vertexes[3].position = {-scale, 0, scale};

  vertexes[0].texture_coords = {1, 1};
  vertexes[1].texture_coords = {1, 0};
  vertexes[2].texture_coords = {0, 0};
  vertexes[3].texture_coords = {0, 1};

  indexes = {0, 1, 2, 2, 3, 0};
}

Model::Model(GeometryType geom_type, const std::string &shader_path)
    : _type(geom_type) {
  default_position();
  _is_loaded = false;

  std::vector<Vertex> vertexes;
  std::vector<uint32_t> indexes;

  switch (geom_type) {
  case GeometryType::Cube:
    load_cube(vertexes, indexes, {0, 0, 0}, {});
    break;
  case GeometryType::Sphere:
    load_sphere(vertexes, indexes, 100, 100);
    break;
  case GeometryType::Plane:
    load_plane(vertexes, indexes);
    break;
  default:
    throw std::runtime_error("invalid GeometryType enum value");
  }

  load_to_gpu(vertexes, indexes);
  _indexes_number = indexes.size();

  load_shader(shader_path);
  load_texture(ASSETS_DIRECTORY "/textures/image_2024-08-04_00-57-12.png");

  calculate_bound_box(vertexes);
}

void Model::load_to_gpu(std::vector<Vertex> &vertexes,
                        std::vector<uint32_t> &indexes) {
  // create and bind vertex array
  glGenVertexArrays(1, &_vertex_array);
  glBindVertexArray(_vertex_array);

  // generate vertex buffer and bind it to vertex
  glGenBuffers(1, &_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);

  // fill buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexes.size(),
               vertexes.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer); // hz nado li

  // set vertex attributes
  char *offset = 0;
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), offset);
  offset += sizeof(Vec3);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), offset);
  offset += sizeof(Vec3);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), offset);
  offset += sizeof(Vec2);
  glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), offset);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  if (indexes.empty()) {
    return;
  }

  // generate index buffer and activate it
  glGenBuffers(1, &_index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);

  // fill data
  uint32_t index_buffer_size = sizeof(uint32_t) * indexes.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, indexes.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer); // hz nado li
}

const Vec3 &Model::scale(const Vec3 &scale) {
  _scale.x *= abs(scale.x) < float_threshold ? 1 : scale.x;
  _scale.y *= abs(scale.y) < float_threshold ? 1 : scale.y;
  _scale.z *= abs(scale.z) < float_threshold ? 1 : scale.z;

  return _scale;
}

const Rotation &Model::rotate(const Vec3 &axis, float angle) {
  if (abs(angle) < float_threshold) {
    return _rotate;
  }

  _rotate.axis = axis;
  _rotate.angle = angle;

  return _rotate;
}

const Vec3 &Model::translate(const Vec3 &translate) {
  _translate.x += translate.x;
  _translate.y += translate.y;
  _translate.z += translate.z;

  return _translate;
}

void Model::default_position() {
  _scale = {1, 1, 1};
  _rotate = {{}, 0};
  _translate = {};
}

void Model::calculate_bound_box(const std::vector<Vertex> &vertexes) {
  if (vertexes.empty()) {
    _bound_box = {{}, {}};
  }

  _bound_box.min = _bound_box.max = vertexes[0].position;
  for (const auto &v : vertexes) {
    const auto &p = v.position;

    _bound_box.min.x = std::min(_bound_box.min.x, p.x);
    _bound_box.min.y = std::min(_bound_box.min.y, p.y);
    _bound_box.min.z = std::min(_bound_box.min.z, p.z);

    _bound_box.max.x = std::max(_bound_box.max.x, p.x);
    _bound_box.max.y = std::max(_bound_box.max.y, p.y);
    _bound_box.max.z = std::max(_bound_box.max.z, p.z);
  }
}

const Model::BoundBox &Model::get_bound_box() { return _bound_box; }

void Model::load_texture(std::string_view filename) {
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
  //                 GL_LINEAR_MIPMAP_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  load and generate the texture
  int nrChannels;
  unsigned char *data = stbi_load(filename.data(), &_texture_widht,
                                  &_texture_height, &nrChannels, 0);
  if (data) {
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texture_widht, _texture_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

void Model::draw() {
  glUseProgram(_program_id);

  auto vp = Application::get_app().view_projection();
  uint32_t vp_id = glGetUniformLocation(_program_id, "MVP");
  glUniformMatrix4fv(vp_id, 1, false, &vp[0][0]);

  auto cam_pos = Application::get_app().camera_position();
  uint32_t cam_pos_id = glGetUniformLocation(_program_id, "CameraPos");
  glUniform3f(cam_pos_id, cam_pos[0], cam_pos[1], cam_pos[2]);

  float time = Application::get_app().timer();
  uint32_t time_id = glGetUniformLocation(_program_id, "Time");
  glUniform1f(time_id, time);

  glBindVertexArray(_vertex_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
  glBindTexture(GL_TEXTURE_2D, _texture);
  glDrawElements(GL_TRIANGLE_STRIP, _indexes_number, GL_UNSIGNED_INT, nullptr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Model::Model(Model &&other) {
  std::swap(_vertex_array, other._vertex_array);
  std::swap(_vertex_buffer, other._vertex_buffer);
  std::swap(_index_buffer, other._index_buffer);

  _bound_box = other._bound_box;

  _rotate = other._rotate;
  _translate = other._translate;
  _scale = other._scale;

  _type = other._type;
  _is_loaded = other._is_loaded;
}

Model &Model::operator=(Model &&other) {
  if (this == &other) {
    return *this;
  }

  std::swap(_vertex_array, other._vertex_array);
  std::swap(_vertex_buffer, other._vertex_buffer);
  std::swap(_index_buffer, other._index_buffer);

  _bound_box = other._bound_box;

  _rotate = other._rotate;
  _translate = other._translate;
  _scale = other._scale;

  _type = other._type;
  _is_loaded = other._is_loaded;

  return *this;
}

Model::~Model() {
  if (_vertex_array != 0) {
    // enable vertex array for deleting vertex buffer
    glBindVertexArray(_vertex_array);
    // delete vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (_vertex_buffer != 0) {
      glDeleteBuffers(1, &_vertex_buffer);
    }
    // disable vertex array and delete it
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vertex_array);
  }

  if (_index_buffer != 0) {
    glDeleteBuffers(1, &_index_buffer);
  }

  // delete shaders
  if (_program_id == 0 || not glIsProgram(_program_id)) {
    return;
  }

  const uint32_t shaders_size = 2;
  std::array<uint32_t, shaders_size> shaders{};
  int real_shader_number;

  glGetAttachedShaders(_program_id, shaders_size, &real_shader_number,
                       shaders.data());
  assert(shaders_size == real_shader_number);

  for (uint32_t shader : shaders) {
    if (shader != 0 && glIsShader(shader)) {
      glDetachShader(_program_id, shader);
      glDeleteShader(shader);
    }
  }
  glDeleteProgram(_program_id);
}
