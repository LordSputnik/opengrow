#ifndef _DEFS_H_
#define _DEFS_H_

#ifdef _DEBUG
static const char PLUGINS_CFG[] = "plugins_d.cfg";
#else
static const char PLUGINS_CFG[] = "plugins.cfg";
#endif

static const float POINT_GRID_RES = 0.01f;
static const float INV_POINT_GRID_RES = 1/POINT_GRID_RES;

//Generic Typedefs
typedef std::string String;

typedef cml::vector3i vec3i; ///< An integer vector, to hold discrete points in space.
typedef cml::vector3f vec3f; ///< A floating point vector, to represent continuous space.

enum
{
  MID = 0,
  VAR,
  CUR
};

#endif // _DEFS_H_
