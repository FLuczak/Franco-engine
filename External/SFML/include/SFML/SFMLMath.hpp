//
// Created by Denis Wagner on 1/16/19.
//

#ifndef PONG_SFMLMATH_HPP
#define PONG_SFMLMATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <algorithm>

namespace sf
{

    template <typename TVector>
    concept VectorLike = requires(TVector Vector) {
        { Vector.x } -> std::convertible_to<float>;
        { Vector.y } -> std::convertible_to<float>;
    };

    template <typename T>
    concept IsScalar = std::is_scalar<T>::value;

    static const auto PI = acos( -1 );

    /*
     * Vector addition
     */
    template <VectorLike T>
    T operator+( const T& vec1, const T& vec2 )
    {
        const auto newX = vec1.x + vec2.x;
        const auto newY = vec1.y + vec2.y;

        return T( newX, newY );
    }

    /*
     * Vector subtraction
     */
    template <VectorLike T>
    T operator-( const T& vec1, const T& vec2 )
    {
        const auto newX = vec1.x - vec2.x;
        const auto newY = vec1.y - vec2.y;

        return T( newX, newY );
    }

    /*
     * Scalar multiplication
     */
    template <VectorLike T, IsScalar U>
    T operator*( const T& vec, U scalar )
    {
        const auto newX = vec.x * scalar;
        const auto newY = vec.y * scalar;

        return T( newX, newY );
    }

    /*
     * Scalar multiplication
     */
    template <IsScalar U, VectorLike T>
    T operator*( U scalar, const T& vec )
    {
        const auto newX = vec.x * scalar;
        const auto newY = vec.y * scalar;

        return T( newX, newY );
    }

    /*
     * Returns the dot product of two given vectors
     */
    template <VectorLike T>
    float operator*( const T& vec1, const T& vec2 )
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    template <VectorLike T>
    float dot(const T& vec1, const T& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }


    template <VectorLike T>
    T lerp(const T & vec1, const T & vec2,float t)
    {
        t = std::clamp(t, 0.0f, 1.0f);
    	return vec1 * (1.0f - t) + vec2 * t;
    }

    /*
     * Returns the square of a given value
     */
    template <IsScalar T>
    inline T sqr( T value )
    {
        return value * value;
    }

    /*
     * Converts radians to degrees
     */
    template <IsScalar T>
    inline float radToDeg( T radians )
    {
        return radians * 180.0 / PI;
    }

    /*
     * Converts degrees to radian
     */
    template <IsScalar T>
    inline float degToRad( T degree )
    {
        return degree / 180.0 * PI;
    }

    /*
     * Returns the length of a given vector
     */
    template <VectorLike T>
    inline float getLength( const T& vec )
    {
        return sqrt( sqr( vec.x ) + sqr( vec.y ) );
    }

    /*
	* Returns the length of a given vector
	*/
    template <VectorLike T>
    inline float length2(const T& vec)
    {
        return sqr(vec.x) + sqr(vec.y);
    }

    /*
     * Returns an inverted vector
     */
    template <VectorLike T>
    inline T getInverted( const T& vec )
    {
        return T( -vec.x, -vec.y );
    }

    /*
     * Inverts a given vector in-place
     */
    template <VectorLike T>
    inline T& invert( T& vec )
    {
        vec.x = -vec.x;
        vec.y = -vec.y;

        return vec;
    }

    /*
     * Returns a normalized vector
     */
    template <VectorLike T>
    inline T getNormalized( const T& vec )
    {
        const float length = getLength( vec );
        const float newX   = vec.x / length;
        const float newY   = vec.y / length;

        return T( newX, newY );
    }

    /*
     * Normalizes a given vector in-place
     */
    template <VectorLike T>
    inline T& normalize( T& vec ) 
    {
        const float length = getLength( vec );
        vec.x /= length;
        vec.y /= length;

        return vec;
    }

    template <VectorLike T>
    inline T& normalized(T vec)
    {
        const float length = getLength(vec);
        vec.x /= length;
        vec.y /= length;

        return vec;
    }

    template <VectorLike T>
    T min(const T& point1, const T& point2)
	{
        return T(std::min(point1.x, point2.x), std::min(point1.y, point2.y));
    }

    template <VectorLike T>
    T max(const T& point1, const T& point2)
    {
        return T(std::max(point1.x, point2.x), std::max(point1.y, point2.y));
    }

    /*
     * Returns the distance between two given points
     */
    template <VectorLike T>
    float distance(const T &point1, const T &point2) {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    /*
	 * Returns the distance between two given points
	*/
    template <VectorLike T>
    float distance2(const T& point1, const T& point2) {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        return dx * dx + dy * dy;
    }


    /*
     * Returns the angle of a given vector from 0 to 360° depending its direction on the unit circle
     */
    template <VectorLike T>
    inline float getRotationAngle( const T& vec )
    {
        const T      normalizedVec = getNormalized( vec );
        const float angleXRad     = acos( normalizedVec.x );
        const float angleYRad     = asin( normalizedVec.y );
        float       angleRad;

        if ( angleYRad < 0 )
            angleRad = degToRad( 360 ) - angleXRad;
        else
            angleRad = angleXRad;

        return radToDeg( angleRad );
    }

    /*
     * Returns the angle in degrees between two given vectors
     */
    template <VectorLike T>
    inline float getAngleBetween( const T& vec1, const T& vec2 )
    {
        const float angle = acos( ( vec1 * vec2 ) / ( getLength( vec1 ) * getLength( vec2 ) ) );

        return radToDeg( angle );
    }

    /*
     * Returns a vector rotated with a given angle in degrees
     */
    template <VectorLike T>
    inline sf::Vector2f getRotated( const T& vec, float angle )
    {
        const float angleRad = degToRad( -angle );
        const float newX     = vec.x * cos( angleRad ) - vec.y * sin( angleRad );
        const float newY     = vec.x * sin( angleRad ) + vec.y * cos( angleRad );

        return T( newX, newY );
    }

    /*
     * Rotates a vector in-place with a given angle in degrees
     */
    template <VectorLike T>
    inline T& rotate( T& vec, float angle )
    {
        const float angleRad = degToRad( -angle );
        
        vec.x = vec.x * cos( angleRad ) - vec.y * sin( angleRad );
        vec.y = vec.x * sin( angleRad ) + vec.y * cos( angleRad );

        return vec;
    }

    /*
     * Returns the projection of a vector on a given axis
     */
    template <VectorLike T>
    T projection( const T& vec, const T& axis ) {
        T NullVector(0, 0);
        if (axis == NullVector) {
            return NullVector;
        }
        float k = (vec * axis) / (axis * axis);
        return k * axis;
    }


    /**
     * Returns the reflection of a vector on a given normal
     */
    template <VectorLike T>
    T reflect(const T& vec, const T& normal) {
        return  normal * 2.0 * vec * normal;
    }


    /**
     * Returns the normal of a vector between two points
     */
    template <VectorLike T>
    T normalBetweenPoints(const T &point1, const T &point2) {
        T direction = point2 - point1;
        T normal = {direction.y, -direction.x};
        return sf::normalize(normal);
    }


}
#endif //PONG_SFMLMATH_HPP
