#pragma once

#include <optional>
#include <vector>
#include <string>

struct Vec3 { float x, y, z; };
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
  std::vector<Vec3> _points;
  std::optional<BoundBox> _bound_box = std::nullopt;

  Rotation _rotate {1, 1, 1};
  Vec3 _translate {};
  Vec3 _scale {};

  GeometryType _type {};
  bool _is_loaded = false;

private:
  Model(std::string_view filename);
  Model(GeometryType geom_type);

  ~Model();

private:
  void calculate_bound_box();

public:
  const Vec3 & scale(const Vec3 &scale = {1, 1, 1});
  const Rotation & rotate(const Vec3 &axis = {}, float angle = 0);
  const Vec3 & translate(const Vec3 &translate = {});
  void default_position();

  const BoundBox & get_bound_box();

  void load_texture(std::string_view filename);

  void draw();
};
