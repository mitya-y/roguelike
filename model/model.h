#pragma once

#include <vector>
#include <optional>
#include <vector>
#include <string>
#include <cstdint>

struct Vec3 { float x, y, z; };
struct Vec2 { float x, y; };
struct Rotation {
  Vec3 axis;
  float angle;
};

class Model {
public:
  enum struct GeometryType {
    Cube,
    Sphere,
    Plane
  };

  struct BoundBox {
    Vec3 min, max;
  };

private:
  struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texture_coords;
    Vec3 color;
  };

private:
  BoundBox _bound_box {};

  Rotation _rotate {1, 1, 1};
  Vec3 _translate {};
  Vec3 _scale {};

  GeometryType _type {};
  bool _is_loaded = false;

  // opengl stuff
  uint32_t vertex_array {};
  uint32_t vertex_buffer {};
  uint32_t index_buffer {};

public:
  Model(std::string_view filename);
  Model(GeometryType geom_type);

  Model(const Model &other) = delete;
  Model & operator=(const Model &other) = delete;

  Model(Model &&other);
  Model & operator=(Model &&other);

  ~Model();

private:
  void calculate_bound_box(const std::vector<Vertex> &vertexes);

  void load_sphere(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes);
  void load_cube(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes);
  void load_plane(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes);

  void load_to_gpu(std::vector<Vertex> &vertexes, std::vector<uint32_t> &indexes);

public:
  const Vec3 & scale(const Vec3 &scale = {1, 1, 1});
  const Rotation & rotate(const Vec3 &axis = {}, float angle = 0);
  const Vec3 & translate(const Vec3 &translate = {});
  void default_position();

  const BoundBox & get_bound_box();

  void load_texture(std::string_view filename);

  void draw();
};
