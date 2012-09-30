#ifndef _OG_WORLD_HPP_
#define _OG_WORLD_HPP_

class World
{
  public:
    //Probably best not to make this public in the long-term. When this class has developed a bit, will need to be private.
    std::map < vec3i, float > modified_points_; ///
};

#endif // _OG_WORLD_HPP_

