#include <stdexcept>
#include <cmath>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "model.h"

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

  vertexes[0].position = {1, 1, 0};
  vertexes[1].position = {1, -1, 0};
  vertexes[2].position = {-1, -1, 0};
  vertexes[3].position = {-1, 1, 0};

  vertexes[0].texture_coords = {1, 1};
  vertexes[1].texture_coords = {1, 0};
  vertexes[2].texture_coords = {0, 0};
  vertexes[3].texture_coords = {0, 1};

  indexes = {0, 1, 2, 2, 3, 0};
}

Model::Model(GeometryType geom_type) : _type(geom_type) {
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

  calculate_bound_box(vertexes);
}

void Model::load_to_gpu(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes) {
  // create and bind vertex array
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  // generate vertex buffer and bind it to vertex
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

  // fill buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexes.size(), vertexes.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); // hz nado li

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
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  // fill data
  uint32_t index_buffer_size = sizeof(uint32_t) * indexes.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, indexes.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); // hz nado li
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

void Model::draw() {}

Model::Model(Model &&other) {
  std::swap(vertex_array, other.vertex_array);
  std::swap(vertex_buffer, other.vertex_buffer);
  std::swap(index_buffer, other.index_buffer);

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

  std::swap(vertex_array, other.vertex_array);
  std::swap(vertex_buffer, other.vertex_buffer);
  std::swap(index_buffer, other.index_buffer);

  _bound_box = other._bound_box;

  _rotate = other._rotate;
  _translate = other._translate;
  _scale = other._scale;

  _type = other._type;
  _is_loaded = other._is_loaded;

  return *this;
}

Model::~Model() {
  if (vertex_array != 0) {
    // enable vertex array for deleting vertex buffer
    glBindVertexArray(vertex_array);
    // delete vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (vertex_buffer != 0) {
      glDeleteBuffers(1, &vertex_buffer);
    }
    // disable vertex array and delete it
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertex_array);
  }

  if (index_buffer != 0) {
    glDeleteBuffers(1, &index_buffer);
  }
}
