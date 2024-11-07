#include <array>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "model.h"

void Model::load_shader(const std::string &shader_name) {
  // open files
  constexpr auto shaders_base_directory = ASSETS_DIRECTORY"/shaders/";
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
    throw std::runtime_error("shader compile error");
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
    throw std::runtime_error("shader compile error");
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
		glGetProgramInfoLog(_program_id, error_info_lenght, nullptr, error_msg.data());
		printf("%s\n", error_msg.data());
    throw std::runtime_error("shader compile error");
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

void Model::load_sphere(std::vector<Vertex> &vertexes, std::vector<uint32_t> &index) {
}

void Model::load_cube(std::vector<Vertex> &vertexes, std::vector<uint32_t> &index) {
}

void Model::load_plane(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes) {
  vertexes.resize(4);

  for (auto &vert : vertexes) {
    vert.normal = {0, 1, 0};
    vert.color = {1, 1, 0};
  }

  float scale = 0.8;

  vertexes[0].position = {scale, scale, 0};
  vertexes[1].position = {scale, -scale, 0};
  vertexes[2].position = {-scale, -scale, 0};
  vertexes[3].position = {-scale, scale, 0};

  vertexes[0].texture_coords = {1, 1};
  vertexes[1].texture_coords = {1, 0};
  vertexes[2].texture_coords = {0, 0};
  vertexes[3].texture_coords = {0, 1};

  indexes = {0, 1, 2, 2, 3, 0};
}

Model::Model(GeometryType geom_type, const std::string &shader_path) : _type(geom_type) {
  default_position();
  _is_loaded = false;

  std::vector<Vertex> vertexes;
  std::vector<uint32_t> indexes;

  switch (geom_type) {
    case GeometryType::Cube:
      load_cube(vertexes, indexes);
      break;
    case GeometryType::Sphere:
      load_sphere(vertexes, indexes);
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

  calculate_bound_box(vertexes);
}

void Model::load_to_gpu(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes) {
  // create and bind vertex array
  glGenVertexArrays(1, &_vertex_array);
  glBindVertexArray(_vertex_array);

  // generate vertex buffer and bind it to vertex
  glGenBuffers(1, &_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);

  // fill buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexes.size(), vertexes.data(), GL_STATIC_DRAW);

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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, indexes.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer); // hz nado li
}

const Vec3 & Model::scale(const Vec3 &scale) {
  _scale.x *= abs(scale.x) < float_threshold ? 1 : scale.x;
  _scale.y *= abs(scale.y) < float_threshold ? 1 : scale.y;
  _scale.z *= abs(scale.z) < float_threshold ? 1 : scale.z;

  return _scale;
}

const Rotation & Model::rotate(const Vec3 &axis, float angle) {
  if (abs(angle) < float_threshold) {
    return _rotate;
  }

  _rotate.axis = axis;
  _rotate.angle = angle;

  return _rotate;
}

const Vec3 & Model::translate(const Vec3 &translate) {
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

const Model::BoundBox & Model::get_bound_box() {
  return _bound_box;
}

void Model::load_texture(std::string_view filename) {}

void Model::draw() {
  glUseProgram(_program_id);

  glBindVertexArray(_vertex_array);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
  glDrawElements(GL_TRIANGLES, _indexes_number, GL_UNSIGNED_INT, nullptr);
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

Model & Model::operator=(Model &&other) {
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
  std::array<uint32_t, shaders_size> shaders {};
  int real_shader_number;

  glGetAttachedShaders(_program_id, shaders_size, &real_shader_number, shaders.data());
  assert(shaders_size == real_shader_number);

  for (uint32_t shader : shaders) {
    if (shader != 0 && glIsShader(shader)) {
      glDetachShader(_program_id, shader);
      glDeleteShader(shader);
    }
  }
  glDeleteProgram(_program_id);
}
