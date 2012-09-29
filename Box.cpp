#include "pch.hpp"
#include "box.hpp"

namespace og
{
  void Box::FromMaxMin(const vec3f & max, const vec3f & min)
  {
    min_ = min;
    max_ = max;

    GenerateIntMaxMin();
  }

  void Box::FromCentreHalf(const vec3f & centre, const vec3f & half)
  {
    min_ = centre - half;
    max_ = centre + half;

    GenerateIntMaxMin();
  }

  void Box::IncludePoint(const vec3f & point)
  {
    min_.minimize(point);
    max_.maximize(point);

    GenerateIntMaxMin();
  }

  void Box::GenerateIntMaxMin()
  {
    min_i_ = floatDownToIntSpace(min_);
    max_i_ = floatUpToIntSpace(max_);
  }

  void Box::Translate(const vec3f & displacement)
  {
    min_ += displacement;
    max_ += displacement;

    GenerateIntMaxMin();
  }
}
