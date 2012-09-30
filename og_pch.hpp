#ifndef _PCH_H_
#define _PCH_H_

#ifdef _WIN32
	#pragma warning( push, 3 )
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wall"
	#pragma GCC diagnostic ignored "-Weffc++"
	#pragma GCC diagnostic ignored "-Wshadow"
#endif

//C STD Includes
#include <cstdio>
#include <cmath>

//C++ STD Includes
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <memory>
#include <algorithm>

//Boost Includes
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_container.hpp>

//External Includes
#include "cml/cml.h"
#include "pugixml.hpp"

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#define StrToStd boost::lexical_cast
#define StdToStr boost::lexical_cast<std::string>

#ifdef _WIN32
	#pragma warning( pop )
#else
	#pragma GCC diagnostic pop
#endif

#include "defs.hpp"
#include "utils.hpp"


/**
 * A namespace containing all the functionality that will be included in the final libary.
 */
namespace og{}

#endif // __PCH_H_
