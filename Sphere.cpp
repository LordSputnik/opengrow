#include "og_pch.hpp"
#include "Sphere.hpp"

namespace og
{
  Sphere::Sphere(const vec3f & centre, float radius)
    : centre_(centre),
      radius_(radius)
  {
    bounds_.FromCentreHalf ( centre_, vec3f ( radius_, radius_, radius_ ) );
  }

  bool Sphere::isPointInside(const vec3i & point) const
  {
    // Check whether the distance between the point and the centre
    // is greater then the radius.
    if((intToFloatSpace(point) - centre_).length() > radius_)
      return false;

    return true;
  }

  void Sphere::CalcContainedPoints()
  {
    contained_points_.clear();

    const vec3i & min = bounds_.min_i();
    const vec3i & max = bounds_.max_i();
    vec3i check_point ( min );

    for( ; check_point[0] != max[0]; ++check_point[0] )
    {
      for( ; check_point[1] !=  max[1]; ++check_point[1] )
      {
        for( ; check_point[2] !=  max[2]; ++check_point[2] )
        {
          if ( isPointInside ( check_point ) )
          {
            contained_points_.push_back ( check_point );
          }
        }
        check_point[2] = min[2];
      }
      check_point[1] = min[1];
    }
  }

  void Sphere::centre(const vec3f & centre)
  {
    bounds_.Translate( centre - centre_ );

    centre_ = centre;

    CalcContainedPoints();
  }
}
