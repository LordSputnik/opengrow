#ifndef _OG_BOX_H_
#define _OG_BOX_H_

namespace og
{
  /**
   * The box class allows a box to be constructed from specified dimensions.
   * This box class is currently used as a bounding box in the sphere and cone classes.
   */
  class Box
  {
  public:

    /** Constructs a box from a maximum point and a minimum point.
     * @param max The maximum point of the box.
     * @param min The minimum point of the box.
     */
    void FromMaxMin(const vec3f & max, const vec3f & min);

    /** Constructs a box from a central point and a set of half-dimensions.
     * @param centre The centre of the box.
     * @param half The half-dimensions of the box. ie. (x/2,y/2,z/2).
     */
    void FromCentreHalf(const vec3f & centre, const vec3f & half);

    /** Extends the box to encompass the input point.
     * @param point The input point to expand the box to include.
     */
    void IncludePoint(const vec3f & point);

    /** Moves the box by the input displacement.
     * Rebuilds both the discrete and the continuous box bounds.
     * @param displacement The displacement to be applied to the box.
     */
    void Translate ( const vec3f & displacement );

    /** Returns the continuous minimum point of the box.
     * @return A floating point vector representing the minimum point.
     */
    inline const vec3f & min() const {return min_;}

    /** Returns the continuous maximum point of the box.
     * @return A floating point vector representing the maximum point.
     */
    inline const vec3f & max() const {return max_;}

    /** Returns the discrete minimum point of the box.
     * @return An integer vector representing the minimum point.
     */
    inline const vec3i & min_i() const {return min_i_;}

    /** Returns the discrete maximum point of the box.
     * @return An integer vector representing the maximum point.
     */
    inline const vec3i & max_i() const {return max_i_;}

  private:
    vec3f min_;
    vec3f max_;
    vec3i min_i_;
    vec3i max_i_;

    /// Generates the discrete min and max from the continuous min and max.
    void GenerateIntMaxMin();
  };
}

#endif // _OG_BOX_H_
