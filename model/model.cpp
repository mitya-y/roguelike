#include <cmath>

#include "model.h"

static constexpr float float_threshold = 0.000001;

Model::Model(std::string_view filename) {
  default_position();
  _is_loaded = true;
}

Model::Model(GeometryType geom_type) : _type(geom_type) {
  default_position();
  _is_loaded = false;
}

Model::~Model() {}

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

void Model::calculate_bound_box() {
  if (_points.empty()) {
    _bound_box = {{}, {}};
  }

  _bound_box->min = _bound_box->max = _points[0];
  for (const auto &p : _points) {
    _bound_box->min.x = std::min(_bound_box->min.x, p.x);
    _bound_box->min.y = std::min(_bound_box->min.y, p.y);
    _bound_box->min.z = std::min(_bound_box->min.z, p.z);

    _bound_box->max.x = std::max(_bound_box->max.x, p.x);
    _bound_box->max.y = std::max(_bound_box->max.y, p.y);
    _bound_box->max.z = std::max(_bound_box->max.z, p.z);
  }
}

const Model::BoundBox & Model::get_bound_box() {
  if (!_bound_box.has_value()) {
    calculate_bound_box();
  }
  return _bound_box.value();
}

void Model::load_texture(std::string_view filename) {}

void Model::draw() {}
