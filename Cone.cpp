#include "og_pch.hpp"
#include "Cone.hpp"

namespace og
{
  Cone::Cone(const vec3f & apex, const vec3f & base, float half_angle, float min_distance)
    : apex_(apex),
      base_(base),
      dir_(base_ - apex_),
      min_distance_(min_distance),
      height_(dir_.length())
  {
    dir_ /= height_; //Normalise the direction.

    radius_ = height_ * tan(cml::rad(half_angle));

    cos_of_angle_ = height_ / cml::length (height_, radius_);

    CreateBB();
  }

  Cone::Cone(const vec3f & apex, const vec3f & dir, float length, float half_angle, float min_distance)
    : apex_(apex),
      base_(apex+(dir*length)),
      dir_(dir),
      min_distance_(min_distance),
      height_(length)
  {
    radius_ = height_ * tan( cml::rad ( half_angle ) );

    cos_of_angle_ = height_ / cml::length (height_, radius_);

    CreateBB();
  }

  bool Cone::isPointInside(const vec3i & point) const
  {
    vec3f dir (intToFloatSpace(point) - apex_);
    float dist = dir.length();

    if(cml::in_range(dist, min_distance_, height_) == false)
      return false;

    dir /= dist;

    if(cml::dot(dir, dir_) < cos_of_angle_) // Dot product decreases as we move away from the cone direction.
      return false;

    return true;
  }

  void Cone::Update(const vec3f & apex)
  {
    vec3f diff = apex - apex_;

    apex_ = apex;
    base_ += diff;

    bounds_.Translate(diff);
    CalcContainedPoints();
  }

  void Cone::Update(const vec3f & apex, const vec3f & dir, bool normalize)
  {
      apex_ = apex;
      dir_ = dir;

      if(normalize == true)
        dir_.normalize();

      base_ = apex_ + ( dir_ * height_ );
      CreateBB();
      CalcContainedPoints();
  }

  void Cone::CreateBB()
  {
    float cosX(cml::dot(dir_, cml::axis_3D(0)));
    float cosY(cml::dot(dir_, cml::axis_3D(1)));
    float cosZ(cml::dot(dir_, cml::axis_3D(2)));

    //Using the formula sin^2(x) + cos^2(x) = 1, cause 3 sqrts is better than 6 trig ops.
    vec3f half_size(sqrt(1.0f - (cosX * cosX)) * radius_, sqrt(1.0f - (cosY * cosY)) * radius_, sqrt(1.0f - (cosZ * cosZ)) * radius_);

    bounds_.FromCentreHalf(base_, half_size);
    bounds_.IncludePoint(apex_);
  }

  void Cone::CalcContainedPoints()
  {
    contained_points_.clear();

    const vec3i & min = bounds_.min_i();
    const vec3i & max = bounds_.max_i();
    vec3i check_point(min);

    for(; check_point[0] != max[0]; ++check_point[0])
    {
      for(; check_point[1] != max[1]; ++check_point[1])
      {
        for(; check_point[2] != max[2]; ++check_point[2])
        {
          if(isPointInside(check_point))
          {
            contained_points_.push_back(check_point);
          }
        }
        check_point[2] = min[2];
      }
      check_point[1] = min[1];
    }
  }
}
