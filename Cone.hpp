#ifndef _OG_CONE_H_
#define _OG_CONE_H_

#include "box.hpp"

namespace og
{
  /** A class to represent a conical container of points. Contains functions to test whether a point is in
   * a cone, and to get all discrete points within the cone. Note that this "cone" is actually a
   * spherical segment.
   */
  class Cone
  {
    public:
    /** Constructor. Also creates the cone's bounding box.
     * @param apex The point at which the sides of the cone converge; the tip of the cone.
     * @param base The point in the centre of the flat side of the cone.
     * @param half_angle The angle between a line from the apex to the base, and the side of the cone.
     * @param min_distance The distance from the apex to a plane which "chops off" the top of the cone.
     */
    Cone(const vec3f & apex, const vec3f & base, float half_angle, float min_distance = 0.0f);

    /** Constructor. Also creates the cone's bounding box.
     * @param apex The point at which the sides of the cone converge; the tip of the cone.
     * @param dir The direction from the apex to the base.
     * @param length The distance between the apex and the base.
     * @param half_angle The angle between a line from the apex to the base, and the side of the cone.
     * @param min_distance The distance from the apex to a plane which "chops off" the top of the cone.
     */
    Cone(const vec3f & apex, const vec3f & dir, float length, float half_angle, float min_distance = 0.0f);

    /** Tests whether a point is inside the cone.
     * @param point The point to test.
     * @return True if the point is in the cone.
     */
    bool isPointInside(const vec3i & point) const;

    /** Retrieves an array of the discrete points that the cone contains.
     * @return A reference to the array of points.
     */
    std::vector<vec3i> & contained_points() {return contained_points_;}

    /** Updates the position of the cone.
     * @param apex The location of the new apex.
     */
    void Update(const vec3f & apex);

    /** Updates the position of the cone.
     * @param apex The location of the new apex.
     * @param dir The new direction of the cone.
     * @param normalize True if the direction needs to be normalized.
     */
    void Update(const vec3f & apex, const vec3f & dir, bool normalize = false);

    private:
    std::vector<vec3i> contained_points_; ///< The points contained within the cone. Calculated using the CalcContainedPoints function.

    vec3f apex_; ///< The apex point of the cone.
    vec3f base_; ///< The point at the centre of the base of the cone.
    vec3f dir_; ///< The direction from the apex to the base.

    float cos_of_angle_; ///< The cosine of the half-angle.
    float min_distance_; ///< The distance from the apex to a plane which "chops off" the top of the cone.
    float height_; ///< The distance from the apex to the base of the cone.
    float radius_; ///< The radius of the circle at the base of the cone.

    Box bounds_; ///< A bounding box containing the whole cone.

    /// Creates a minimal (axis-aligned) bounding box that will completely contain the cone.
    void CreateBB();

    /// Calculates the positions of all the discrete points contained in the cone.
    void CalcContainedPoints();
  };
}
/*

class Cone
{
  public:

private:
  void CreateAABB();

};
*/

#endif // _OG_CONE_H_
