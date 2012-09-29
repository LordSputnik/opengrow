#ifndef _OG_SPHERE_H_
#define _OG_SPHERE_H_

#include "box.hpp"

namespace og
{
  /**
   * A class to represent a spherical container of points. Contains
   * functions to test whether a point is in a sphere, and to get all
   * discrete points within the sphere.
   */
  class Sphere
  {
  public:
    /** Constructor. Also creates the sphere's bounding box.
     * @param centre The centre of the sphere.
     * @param radius The radius of the sphere.
     */
    Sphere(const vec3f & centre, float radius);

    /** Tests whether a point is inside the sphere.
     * @param point The point to test.
     * @return True if the point is in the sphere.
     */
    bool isPointInside(const vec3i & point) const;

    /** Retrieves an array of the discrete points that the sphere
     * contains.
     * @return A reference to the array of points.
     */
    std::vector<vec3i> & contained_points() {return contained_points_;}

    /** Updates the centre of the sphere to a new location.
     * @param centre The new central point.
     */
    void centre(const vec3f & centre);

  private:
    std::vector<vec3i> contained_points_; ///< The points contained within the sphere. Calculated using the CalcContainedPoints function.
    vec3f centre_; ///< The centre of the sphere.
    float radius_; ///< The radius of the sphere.

    Box bounds_; ///< A bounding box containing the whole sphere.

    /**
     * Calculates the positions of
     * all the discrete points contained in the sphere.
     */
    void CalcContainedPoints();
  };
}


#endif // _OG_SPHERE_H_
