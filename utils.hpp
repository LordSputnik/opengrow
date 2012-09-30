#ifndef _UTILS_H_
#define _UTILS_H_

/** Converts a discrete input vector to a continuous output vector.
 * @param in The discrete input vector.
 */
vec3f intToFloatSpace ( const vec3i & in );

/** Converts a continuous input vector to a discrete output vector, by rounding all elements down.
 * @param in The continuous input vector.
 */
vec3i floatDownToIntSpace ( const vec3f & in );

/** Converts a continuous input vector to a discrete output vector, by rounding all elements up.
 * @param in The continuous input vector.
 */
vec3i floatUpToIntSpace ( const vec3f & in );

/**
 * Returns a floating point number within a certain range about a central point.
 * @param centre The central value.
 * @param variation The amount of variation in the output.
 */
float getRandomVariation(float centre, float variation);

/**
 * Returns an integer number within a certain range about a central point.
 * @param centre The central value.
 * @param variation The amount of variation in the output.
 */
int getRandomVariation(int centre, int variation);

/**
 * Prints a comma-delimited list of the three elements of the input vector.
 * Prepends a labels to the output, specified by the name parameter.
 * @param v The vector to print.
 * @param label The label to prepend to the vector output
 */
void PrintVector( const vec3f & v, const char* label );

/**
 * Prints a comma-delimited list of the three elements of the input vector.
 * Prepends a labels to the output, specified by the name parameter.
 * @param v The vector to print.
 * @param label The label to prepend to the vector output
 */
void PrintVector( const vec3i & v, const char* label );

#endif // _UTILS_H_
