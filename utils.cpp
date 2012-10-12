#include "og_pch.hpp"
#include "utils.hpp"

vec3f intToFloatSpace ( const vec3i & in )
{
  return vec3f(static_cast<float>(in[0]) * POINT_GRID_RES,
                static_cast<float>(in[1]) * POINT_GRID_RES,
                static_cast<float>(in[2]) * POINT_GRID_RES);
}

vec3i floatDownToIntSpace ( const vec3f & in )
{
  return vec3i(static_cast<int>(floor(in[0]*INV_POINT_GRID_RES)),
                static_cast<int>(floor(in[1]*INV_POINT_GRID_RES)),
                static_cast<int>(floor(in[2]*INV_POINT_GRID_RES)));
}

vec3i floatUpToIntSpace ( const vec3f & in )
{
  return vec3i(static_cast<int>(ceil(in[0]*INV_POINT_GRID_RES)),
                static_cast<int>(ceil(in[1]*INV_POINT_GRID_RES)),
                static_cast<int>(ceil(in[2]*INV_POINT_GRID_RES)));
}

float getRandomVariation(float centre, float variation)
{
  if(variation == 0.0f)
    return centre;

  return cml::random_real ( centre - variation, centre + variation );
}

int getRandomVariation(int centre, int variation)
{
  if(variation == 0)
    return centre;

  return cml::random_integer ( centre - variation, centre + variation );
}

void PrintVector( const vec3f & v, const char* label )
{
  printf("%s: ( %f, %f, %f )\n",label,v[0],v[1],v[2]);
}

void PrintVector( const vec3i & v, const char* label )
{
  printf("%s: ( %i, %i, %i )\n",label,v[0],v[1],v[2]);
}
