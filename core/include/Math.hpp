#pragma once
#include "Config.hpp"

#include <cmath>



const unsigned int MaxUInt32 = 0xFFFFFFFF;
const int MinInt32 = 0x80000000;
const int MaxInt32 = 0x7FFFFFFF;
const float MaxFloat = 3.402823466e+38F;
const float MinPosFloat = 1.175494351e-38F;

const float Pi = 3.141592654f;
const float TwoPi = 6.283185307f;
const float PiHalf = 1.570796327f;

const float Epsilon = 0.000001f;
const float ZeroEpsilon = 32.0f * MinPosFloat;  // Very small epsilon for checking against 0.0f

const float M_INFINITY      =  1.0e30f;


#define powi(base,exp) (int)powf((float)(base), (float)(exp))

#define ToRadians(x) (float)(((x) * Pi / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / Pi))

inline float Sin( float a ){	return sin( a * Pi / 180 );}
inline float Cos( float a ){	return cos( a * Pi / 180 );}
inline float Tan( float a ){	return tan( a * Pi / 180 );}
inline float SinRad( float a ){	return sin( a );}
inline float CosRad( float a ){	return cos( a );}
inline float TanRad( float a ){	return tan( a );}
inline float ASin( float a ){	return asin( a ) * 180 / Pi;}
inline float ACos( float a ){	return acos( a ) * 180 / Pi;}
inline float ATan( float a ){	return atan( a ) * 180 / Pi;}
inline float ATan2( float y, float x ){	return atan2( y, x ) * 180 / Pi;}
inline float ASinRad( float a ){	return asin( a );}
inline float ACosRad( float a ){	return acos( a );}
inline float ATanRad( float a ){	return atan( a );}
inline float ATan2Rad( float y, float x ){	return atan2( y, x );}
inline int Floor( float a ){	return (int) ( floor( a ) );}
inline int Ceil( float a ){	return (int) ( ceil( a ) );}
inline int Trunc( float a ){	if ( a > 0 ) return Floor( a );	else return Ceil( a );}
inline int Round( float a ){	if ( a < 0 ) return (int) ( ceil( a-0.5f ) );	else return (int) ( floor( a+0.5f ) );}
inline float Sqrt( float a ){	if ( a > 0 ) return sqrt( a );	else return 0;}
inline float Abs( float a ){	if ( a < 0 ) a = -a;	return a;}
inline int Mod( int a, int b ){	if ( b == 0 ) return 0;	return a % b;}
inline float FMod( float a, float b ){	if ( b == 0 ) return 0;	return fmod( a, b );}
inline float Pow( float a, float b ){	return pow( a, b );}
inline int Sign( float a ){	if ( a < 0 ) return -1;	else if ( a > 0 ) return 1;	else return 0;}
inline float Min( float a, float b ){	return a < b ? a : b;}
inline float Max( float a, float b ){	return a > b ? a : b;}
inline int Min( int a, int b ){	return a < b ? a : b;}
inline int Max( int a, int b ){	return a > b ? a : b;}
inline float Clamp( float a, float min, float max ){	if ( a < min ) a = min;	else if ( a > max ) a = max;	return a;}
inline int   Clamp( int a, int min, int max ){	if ( a < min ) a = min;	else if ( a > max ) a = max;	return a;}


enum NoInitHint
{
    NO_INIT
};




inline float degToRad( float f )
{
	return f * 0.017453293f;
}

inline float radToDeg( float f )
{
	return f * 57.29577951f;
}







// -------------------------------------------------------------------------------------------------
// Vector
// -------------------------------------------------------------------------------------------------

class Vec2
{
public:
	float x, y;
	
	
	// ------------
	// Constructors
	// ------------
	Vec2() : x( 0.0f ), y( 0.0f )
	{
	}
	Vec2( const float v) : x( v ), y( v )
	{
	}
	
	explicit Vec2( NoInitHint )
	{
		// Constructor without default initialization
	}
	
	Vec2( const float x, const float y ) : x( x ), y( y )
	{
	}

	void set(  float x,  float y )
	{
		this->x = x;
		this->y = y;
	}
	
	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec2 &v ) const
	{
		return (x > v.x - Epsilon && x < v.x + Epsilon &&
				y > v.y - Epsilon && y < v.y + Epsilon);
	}
	
	bool operator!=( const Vec2 &v ) const
	{
		return (x < v.x - Epsilon || x > v.x + Epsilon ||
				y < v.y - Epsilon || y > v.y + Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec2 operator-() const
	{
		return Vec2( -x, -y );
	}
	
	Vec2 operator+( const Vec2 &v ) const
	{
		return Vec2( x + v.x, y + v.y );
	}
	
	Vec2 &operator+=( const Vec2 &v )
	{
		return *this = *this + v;
	}
	
	Vec2 operator-( const Vec2 &v ) const
	{
		return Vec2( x - v.x, y - v.y );
	}
	
	Vec2 &operator-=( const Vec2 &v )
	{
		return *this = *this - v;
	}
	
	Vec2 operator*( const float f ) const
	{
		return Vec2( x * f, y * f );
	}
	
	Vec2 &operator*=( const float f )
	{
		return *this = *this * f;
	}
	
	Vec2 operator/( const float f ) const
	{
		return Vec2( x / f, y / f );
	}
	
	Vec2 &operator/=( const float f )
	{
		return *this = *this / f;
	}
	
	// ----------------
	// Special products
	// ----------------
	float dot( const Vec2 &v ) const
	{
		return x * v.x + y * v.y;
	}
	
	// ----------------
	// Other operations
	// ----------------
	float length() const
	{
		return sqrtf( x * x + y * y );
	}
	
	Vec2 normalized() const
	{
		float invLen = 1.0f / length();
		return Vec2( x * invLen, y * invLen );
	}
	
	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
	}
	
	Vec2 lerp( const Vec2 &v, float f ) const
	{
		return Vec2( x + (v.x - x) * f, y + (v.y - y) * f );
	}
};


class Vec3
{
public:
	float x, y, z;

	Vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f )
	{ 
	}

	explicit Vec3( NoInitHint )
	{
		// Constructor without default initialization
	}

	Vec3( const float v) : x( v ), y( v ), z( v )
	{
	}
	
	Vec3( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) 
	{
	}

	void set(  float x,  float y,  float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec3 &v ) const
	{
		return (x > v.x - Epsilon && x < v.x + Epsilon && 
		        y > v.y - Epsilon && y < v.y + Epsilon &&
		        z > v.z - Epsilon && z < v.z + Epsilon);
	}

	bool operator!=( const Vec3 &v ) const
	{
		return (x < v.x - Epsilon || x > v.x + Epsilon || 
		        y < v.y - Epsilon || y > v.y + Epsilon ||
		        z < v.z - Epsilon || z > v.z + Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec3 operator-() const
	{
		return Vec3( -x, -y, -z );
	}

	Vec3 operator+( const Vec3 &v ) const
	{
		return Vec3( x + v.x, y + v.y, z + v.z );
	}

	Vec3 &operator+=( const Vec3 &v )
	{
		return *this = *this + v;
	}

	Vec3 operator-( const Vec3 &v ) const 
	{
		return Vec3( x - v.x, y - v.y, z - v.z );
	}

	Vec3 &operator-=( const Vec3 &v )
	{
		return *this = *this - v;
	}

	Vec3 operator*( const float f ) const
	{
		return Vec3( x * f, y * f, z * f );
	}

	Vec3 &operator*=( const float f )
	{
		return *this = *this * f;
	}

	Vec3 operator/( const float f ) const
	{
		return Vec3( x / f, y / f, z / f );
	}

	Vec3 &operator/=( const float f )
	{
		return *this = *this / f;
	}


	// ----------------
	// Special products
	// ----------------
	float dot( const Vec3 &v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 cross( const Vec3 &v ) const
	{
		return Vec3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	// ----------------
	// Other operations
	// ----------------


	float length() const 
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float length_squared() const
	{
		return x * x + y * y + z * z;
	}
	float distance( const Vec3 &v ) const
	{
		return sqrtf( (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) );
	}

	float distance_squared( const Vec3 &v ) const
	{
		return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z);
	}



	Vec3 normalized() const
	{
		float invLen = 1.0f / length();
		return Vec3( x * invLen, y * invLen, z * invLen );
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	Vec3 lerp( const Vec3 &v, float f ) const
	{
		return Vec3( x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f );
	}

	Vec3 toRotation() const
	{
		// Assumes that the unrotated view vector is (0, 0, -1)
		Vec3 v;
		
		if( y != 0 ) v.x = atan2f( y, sqrtf( x*x + z*z ) );
		if( x != 0 || z != 0 ) v.y = atan2f( -x, -z );

		return v;
	}


	Vec3 Min(const Vec3 &v) const
	{
		return Vec3( x < v.x ? x : v.x, y < v.y ? y : v.y, z < v.z ? z : v.z );
	}
	Vec3 Max(const Vec3 &v) const
	{
		return Vec3( x > v.x ? x : v.x, y > v.y ? y : v.y, z > v.z ? z : v.z );
	}

	static Vec3 Clamp(const Vec3 &v, const Vec3 &min, const Vec3 &max)
	{
		return Vec3( v.x < min.x ? min.x : v.x > max.x ? max.x : v.x, v.y < min.y ? min.y : v.y > max.y ? max.y : v.y, v.z < min.z ? min.z : v.z > max.z ? max.z : v.z );
	}

	static Vec3 Cross(const Vec3 &v1, const Vec3 &v2)
	{
		return Vec3( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
	}
	static float Dot(const Vec3 &v1, const Vec3 &v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static Vec3 Sub(const Vec3 &v1, const Vec3 &v2)
	{
		return Vec3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
	}	
	static Vec3 Add(const Vec3 &v1, const Vec3 &v2)
	{
		return Vec3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
	}
	static Vec3 Normalize(const Vec3 &v)
	{
		float invLen = 1.0f / v.length();
		return Vec3( v.x * invLen, v.y * invLen, v.z * invLen );
	}
	static float DistanceFromSq(const Vec3 &v1, const Vec3 &v2)
	{
		Vec3 v = v1 - v2;
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	static float DistanceFrom(const Vec3 &v1, const Vec3 &v2)
	{
		return sqrtf(DistanceFromSq(v1, v2));
	}

	static float Length(const Vec3 &v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	static Vec3 GetAngleWeights(const Vec3 &v, const Vec3 &v1, const Vec3 &v2)
	{
		float a = DistanceFromSq(v1, v2);
		float asqrt = sqrtf(a);
		
		float b = DistanceFromSq(v, v1);
		float bsqrt = sqrtf(b);
		
		float c = DistanceFromSq(v, v2);
		float csqrt = sqrtf(c);

		return Vec3(
		acosf((b + c - a) / (2.f * bsqrt * csqrt)),
		acosf((-b + c + a) / (2.f * asqrt * csqrt)),
		acosf((b - c + a) / (2.f * bsqrt * asqrt)));

		
	}
	static Vec3 Lerp(const Vec3 &v1, const Vec3 &v2, float t)
		{
			return Vec3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t);
		}
};


class Vec4
{
public:
	
	float x, y, z, w;


	Vec4() : x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{
	}
	
	explicit Vec4( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}

	explicit Vec4( Vec3 v ) : x( v.x ), y( v.y ), z( v.z ), w( 1.0f )
	{
	}

	explicit Vec4( Vec3 v, float w ) : x( v.x ), y( v.y ), z( v.z ), w( w )
	{
	}

	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec4 operator+( const Vec4 &v ) const
	{
		return Vec4( x + v.x, y + v.y, z + v.z, w + v.w );
	}

	Vec4 operator-() const
	{
		return Vec4( -x, -y, -z, -w );
	}
	
	Vec4 operator*( const float f ) const
	{
		return Vec4( x * f, y * f, z * f, w * f );
	}
	Vec4 operator/(float divisor) const 
	{
        if (divisor == 0) 
		{
            DEBUG_BREAK_IF(true);
            return *this;
        }
        return Vec4(x / divisor, y / divisor, z / divisor, w / divisor);
    }
	// Vec4 operator*(const Mat4& matrix) const 
	// {
    //     Vec4 result(0.0f, 0.0f, 0.0f, 0.0f);

    //     result.x = matrix.c[0][0] * x + matrix.c[0][1] * y + matrix.c[0][2] * z + matrix.c[0][3] * w;
    //     result.y = matrix.c[1][0] * x + matrix.c[1][1] * y + matrix.c[1][2] * z + matrix.c[1][3] * w;
    //     result.z = matrix.c[2][0] * x + matrix.c[2][1] * y + matrix.c[2][2] * z + matrix.c[2][3] * w;
    //     result.w = matrix.c[3][0] * x + matrix.c[3][1] * y + matrix.c[3][2] * z + matrix.c[3][3] * w;

    //     return result;
    // }
};


// -------------------------------------------------------------------------------------------------
// Quaternion
// -------------------------------------------------------------------------------------------------

class Quaternion
{
public:	
	
	float x, y, z, w;

	// ------------
	// Constructors
	// ------------
	Quaternion() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) 
	{ 
	}
	
	explicit Quaternion( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}
	
	Quaternion( const float eulerX, const float eulerY, const float eulerZ )
	{
		Quaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
	
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Quaternion operator*( const Quaternion &q ) const
	{
		return Quaternion(
			y * q.z - z * q.y + q.x * w + x * q.w,
			z * q.x - x * q.z + q.y * w + y * q.w,
			x * q.y - y * q.x + q.z * w + z * q.w,
			w * q.w - (x * q.x + y * q.y + z * q.z) );
	}

	Quaternion &operator*=( const Quaternion &q )
	{
		return *this = *this * q;
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
	}

	void identity()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	float length() const
	{
		return sqrtf( x * x + y * y + z * z + w * w );
	}

	void rotate( const float x, const float y, const float z )
	{
		*this = *this * Quaternion( x, y, z, 0 ) * Quaternion( -x, -y, -z, 0 );
	}

	void rotate(const Vec3 &rotation)
	{
		*this = *this * Quaternion( rotation.x, rotation.y, rotation.z, 0 ) * Quaternion( -rotation.x, -rotation.y, -rotation.z, 0 );
	}

	void rotate(float angle,float ax, float ay, float az)
	{
		*this = *this * Quaternion( angle, ax, ay, az ) * Quaternion( -angle, -ax, -ay, -az );
		

	}

	void setEuler( const float eulerX, const float eulerY, const float eulerZ )
	{
		Quaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
	
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	Quaternion Roll(float x)
	{
		return Quaternion( sinf( x * 0.5f ), 0, 0, cosf( x * 0.5f ) );
	}

	Quaternion Pitch(float x)
	{
		return Quaternion( 0, sinf( x * 0.5f ), 0, cosf( x * 0.5f ) );
	}

	Quaternion Yaw(float x)
	{
		return Quaternion( 0, 0, sinf( x * 0.5f ), cosf( x * 0.5f ) );
	}


	void setEuler(const Vec3 &rotation)
	{
		setEuler(rotation.x, rotation.y, rotation.z);
	}
	void set(const float x, const float y, const float z, const float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	// ----------------
	// Other operations
	// ----------------

	Quaternion slerp( const Quaternion &q, const float t ) const
	{
		// Spherical linear interpolation between two quaternions
		// Note: SLERP is not commutative
		
		Quaternion q1( q );

		// Calculate cosine
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

		// Use the shortest path
		if( cosTheta < 0 )
		{
			cosTheta = -cosTheta; 
			q1.x = -q.x; q1.y = -q.y;
			q1.z = -q.z; q1.w = -q.w;
		}

		// Initialize with linear interpolation
		float scale0 = 1 - t, scale1 = t;
		
		// Use spherical interpolation only if the quaternions are not very close
		if( (1 - cosTheta) > 0.001f )
		{
			// SLERP
			float theta = acosf( cosTheta );
			float sinTheta = sinf( theta );
			scale0 = sinf( (1 - t) * theta ) / sinTheta;
			scale1 = sinf( t * theta ) / sinTheta;
		} 
		
		// Calculate final quaternion
		return Quaternion( x * scale0 + q1.x * scale1, y * scale0 + q1.y * scale1,
		                   z * scale0 + q1.z * scale1, w * scale0 + q1.w * scale1 );
	}

	Quaternion nlerp( const Quaternion &q, const float t ) const
	{
		// Normalized linear quaternion interpolation
		// Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

		Quaternion qt;
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;
		
		// Use the shortest path and interpolate linearly
		if( cosTheta < 0 )
			qt = Quaternion( x + (-q.x - x) * t, y + (-q.y - y) * t,
							 z + (-q.z - z) * t, w + (-q.w - w) * t );
		else
			qt = Quaternion( x + (q.x - x) * t, y + (q.y - y) * t,
							 z + (q.z - z) * t, w + (q.w - w) * t );

		// Return normalized quaternion
		float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
		return Quaternion( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
	}

	Quaternion inverted() const
	{
		float len = x * x + y * y + z * z + w * w;
		if( len > 0 )
		{
			float invLen = 1.0f / len;
			return Quaternion( -x * invLen, -y * invLen, -z * invLen, w * invLen );
		}
		else return Quaternion();
	}
	static Quaternion Slerp(const Quaternion &a, const Quaternion &b, const float t ) 
	{
		return a.slerp(b, t);
	}

	static Quaternion Normalize(const Quaternion &q)
	{
		Quaternion out( q );
		out.normalize();
		return out;
	}
};


// -------------------------------------------------------------------------------------------------
// Matrix
// -------------------------------------------------------------------------------------------------

class Mat4
{
public:
	
	union
	{
		float c[4][4];	// Column major order for OpenGL: c[column][row]
		float x[16];
	};
	
	// --------------
	// Static methods
	// --------------
	static Mat4 Translate( float x, float y, float z )
	{
		Mat4 m;

		m.c[3][0] = x;
		m.c[3][1] = y;
		m.c[3][2] = z;

		return m;
	}
	static Mat4 Translate( const Vec3 &v )
	{
		return Translate( v.x, v.y, v.z );
	}

	static Mat4 Scale( float x, float y, float z )
	{
		Mat4 m;
		
		m.c[0][0] = x;
		m.c[1][1] = y;
		m.c[2][2] = z;

		return m;
	}

	static Mat4 Rotate( float x, float y, float z )
	{
		// Rotation order: YXZ [* Vector]
		return Mat4( Quaternion( x, y, z ) );
	}

	static Mat4 Rotate( const Quaternion &q )
	{
		return Mat4( q );
	}

	static Mat4 Rotate( Vec3 axis, float angle )
	{
		axis = axis * sinf( angle * 0.5f );
		return Mat4( Quaternion( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
	}
	static Mat4 Perspective(double fovY, double aspect, double nearPlane, double farPlane)
	{
		double height = 1.0 / tan(fovY * Pi / 360.0);
		double width = height / aspect;
		double f = farPlane;
		double n = nearPlane;

		Mat4 m;

		m.x[0] = width;
		m.x[5] = height;
		m.x[10] = (f + n) / (n - f);
		m.x[11] = -1;
		m.x[14] = 2 * f * n / (n - f);
		m.x[15] = 0;

		return m;
	}
	static Mat4 Perspective( float l, float r, float b, float t, float n, float f )
	{
		Mat4 m;

		m.x[0] = 2 * n / (r - l);
		m.x[5] = 2 * n / (t - b);
		m.x[8] = (r + l) / (r - l);
		m.x[9] = (t + b) / (t - b);
		m.x[10] = -(f + n) / (f - n);
		m.x[11] = -1;
		m.x[14] = -2 * f * n / (f - n);
		m.x[15] = 0;

		return m;
	}

	static Mat4 Ortho( float l, float r, float b, float t, float n, float f )
	{
		Mat4 m;

		m.x[0] = 2 / (r - l);
		m.x[5] = 2 / (t - b);
		m.x[10] = -2 / (f - n);
		m.x[12] = -(r + l) / (r - l);
		m.x[13] = -(t + b) / (t - b);
		m.x[14] = -(f + n) / (f - n);

		return m;
	}

	static Mat4 LookAt( const Vec3 &eye, const Vec3 &center, const Vec3 &up )
	{
		Vec3 f = (center - eye).normalized();
		Vec3 u = up.normalized();
		Vec3 s = Vec3::Cross( f, u ).normalized();
		u = Vec3::Cross( s, f );

		Mat4 m;

		m.c[0][0] = s.x;
		m.c[1][0] = s.y;
		m.c[2][0] = s.z;

		m.c[0][1] = u.x;
		m.c[1][1] = u.y;
		m.c[2][1] = u.z;

		m.c[0][2] = -f.x;
		m.c[1][2] = -f.y;
		m.c[2][2] = -f.z;

		m.c[3][0] = -s.dot( eye );
		m.c[3][1] = -u.dot( eye );
		m.c[3][2] = f.dot( eye );

		return m;
	}

	static void fastMult43( Mat4 &dst, const Mat4 &m1, const Mat4 &m2 )
	{
		// Note: dst may not be the same as m1 or m2

		float *dstx = dst.x;
		const float *m1x = m1.x;
		const float *m2x = m2.x;
		
		dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
		dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
		dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
		dstx[3] = 0.0f;

		dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
		dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
		dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
		dstx[7] = 0.0f;

		dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
		dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
		dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
		dstx[11] = 0.0f;

		dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
		dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
		dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
		dstx[15] = 1.0f;
	}

	static Mat4 Identity()
	{
		Mat4 m( NO_INIT );
		m.x[0] = 1; m.x[1] = 0; m.x[2] = 0; m.x[3] = 0;
		m.x[4] = 0; m.x[5] = 1; m.x[6] = 0; m.x[7] = 0;
		m.x[8] = 0; m.x[9] = 0; m.x[10] = 1; m.x[11] = 0;
		m.x[12] = 0; m.x[13] = 0; m.x[14] = 0; m.x[15] = 1;
		return m;
	
	}

	void set( const float *floatArray16 )
	{
		x[0] = floatArray16[0]; x[1] = floatArray16[1]; x[2] = floatArray16[2]; x[3] = floatArray16[3];
		x[4] = floatArray16[4]; x[5] = floatArray16[5]; x[6] = floatArray16[6]; x[7] = floatArray16[7];
		x[8] = floatArray16[8]; x[9] = floatArray16[9]; x[10] = floatArray16[10]; x[11] = floatArray16[11];
		x[12] = floatArray16[12]; x[13] = floatArray16[13]; x[14] = floatArray16[14]; x[15] = floatArray16[15];
	}

	// ------------
	// Constructors
	// ------------
	Mat4()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	}

	explicit Mat4( NoInitHint )
	{
		// Constructor without default initialization
	}

	Mat4( const float *floatArray16 )
	{
		for( unsigned int i = 0; i < 4; ++i )
		{
			for( unsigned int j = 0; j < 4; ++j )
			{
				c[i][j] = floatArray16[i * 4 + j];
			}
		}
	}

	Mat4( const Quaternion &q )
	{
		// Calculate coefficients
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
		float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
		float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

		c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;	
		c[2][0] = xz + wy;        c[3][0] = 0;
		c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
		c[2][1] = yz - wx;        c[3][1] = 0;
		c[0][2] = xz - wy;        c[1][2] = yz + wx;
		c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
		c[0][3] = 0;              c[1][3] = 0;
		c[2][3] = 0;              c[3][3] = 1;
	}

	// ----------
	// Matrix sum
	// ----------
	Mat4 operator+( const Mat4 &m ) const 
	{
		Mat4 mf( NO_INIT );
		
		mf.x[0] = x[0] + m.x[0];
		mf.x[1] = x[1] + m.x[1];
		mf.x[2] = x[2] + m.x[2];
		mf.x[3] = x[3] + m.x[3];
		mf.x[4] = x[4] + m.x[4];
		mf.x[5] = x[5] + m.x[5];
		mf.x[6] = x[6] + m.x[6];
		mf.x[7] = x[7] + m.x[7];
		mf.x[8] = x[8] + m.x[8];
		mf.x[9] = x[9] + m.x[9];
		mf.x[10] = x[10] + m.x[10];
		mf.x[11] = x[11] + m.x[11];
		mf.x[12] = x[12] + m.x[12];
		mf.x[13] = x[13] + m.x[13];
		mf.x[14] = x[14] + m.x[14];
		mf.x[15] = x[15] + m.x[15];

		return mf;
	}

	Mat4 &operator+=( const Mat4 &m )
	{
		return *this = *this + m;
	}

	// ---------------------
	// Matrix multiplication
	// ---------------------
	Mat4 operator*( const Mat4 &m ) const 
	{
		Mat4 mf( NO_INIT );
		
		mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
		mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
		mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
		mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

		mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
		mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
		mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
		mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

		mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
		mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
		mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
		mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

		mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
		mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
		mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
		mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

		return mf;
	}

	Mat4 operator*( const float f ) const
	{
		Mat4 m( *this );
		
		m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
		m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
		m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
		m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

		return m;
	}



	// ----------------------------
	// Vector-Matrix multiplication
	// ----------------------------
	Vec3 operator*( const Vec3 &v ) const
	{
		return Vec3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2] );
	}

	Vec4 operator*( const Vec4 &v ) const
	{
		return Vec4( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
		              v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3] );
	}

	Vec3 mult33Vec( const Vec3 &v ) const
	{
		return Vec3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
	}
	
	// ---------------
	// Transformations
	// ---------------
	void translate( const float tx, const float ty, const float tz )
	{
		*this = Translate( tx, ty, tz ) * *this;
	}

	void scale( const float sx, const float sy, const float sz )
	{
		*this = Scale( sx, sy, sz ) * *this;
	}

	void rotate( const float rx, const float ry, const float rz )
	{
		*this = Rotate( rx, ry, rz ) * *this;
	}

	void rotate( const float angle, const Vec3 &axis )
	{
		*this = Rotate(axis,angle ) * *this;
	}

	void rotate( const Quaternion &q )
	{
		*this = Rotate( q ) * *this;
	}

	// ---------------
	// Other
	// ---------------

	Mat4 transposed() const
	{
		Mat4 m( *this );
		
		for( unsigned int maty = 0; maty < 4; ++maty )
		{
			for( unsigned int matx = maty + 1; matx < 4; ++matx ) 
			{
				float tmp = m.c[matx][maty];
				m.c[matx][maty] = m.c[maty][matx];
				m.c[maty][matx] = tmp;
			}
		}

		return m;
	}

	float determinant() const
	{
		return 
			c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
			c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
			c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
			c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
			c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
			c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
	}

	Mat4 inverted() const
	{
		Mat4 m( NO_INIT );

		float d = determinant();
		if( d == 0 ) return m;
		d = 1.0f / d;
		
		m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
		m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
		m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
		m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
		m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
		m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
		m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
		m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
		m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
		m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
		m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
		m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
		m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
		m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
		m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
		m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);
		
		return m;
	}



	void decompose( Vec3 &trans, Vec3 &rot, Vec3 &scale ) const
	{
		// Getting translation is trivial
		trans = Vec3( c[3][0], c[3][1], c[3][2] );

		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );

		if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

		// Detect negative scale with determinant and flip one arbitrary axis
		if( determinant() < 0 ) scale.x = -scale.x;

		// Combined rotation matrix YXZ
		//
		// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
		// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
		// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

		rot.x = asinf( -c[2][1] / scale.z );
		
		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = fabsf( c[2][1] / scale.z );
		if( f > 0.999f && f < 1.001f )
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;
			
			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
		}
		// Standard case
		else
		{
			rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
			rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
		}
	}

	
	void setCol( unsigned int col, const Vec4& v ) 
	{
		x[col * 4 + 0] = v.x;
		x[col * 4 + 1] = v.y;
		x[col * 4 + 2] = v.z;
		x[col * 4 + 3] = v.w;
	}

	Vec4 getCol( unsigned int col ) const
	{
		return Vec4( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
	}

	Vec4 getRow( unsigned int row ) const
	{
		return Vec4( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
	}

	Vec3 getTrans() const
	{
		return Vec3( c[3][0], c[3][1], c[3][2] );
	}
	
	Vec3 getScale() const
	{
		Vec3 scale;
		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
		return scale;
	}
	void identity()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	
	}
	static Vec3 Transform(const Mat4 &m, const Vec3 &v ) 
	{
		return Vec3( v.x * m.c[0][0] + v.y * m.c[1][0] + v.z * m.c[2][0] + m.c[3][0],
		             v.x * m.c[0][1] + v.y * m.c[1][1] + v.z * m.c[2][1] + m.c[3][1],
		             v.x * m.c[0][2] + v.y * m.c[1][2] + v.z * m.c[2][2] + m.c[3][2] );
	}	

	static void Transform(const Mat4 &m, const Vec3 &v, Vec3 &out )
	{
		out.x = v.x * m.c[0][0] + v.y * m.c[1][0] + v.z * m.c[2][0] + m.c[3][0];
		out.y = v.x * m.c[0][1] + v.y * m.c[1][1] + v.z * m.c[2][1] + m.c[3][1];
		out.z = v.x * m.c[0][2] + v.y * m.c[1][2] + v.z * m.c[2][2] + m.c[3][2];
	}

	static Vec3 TranformNormal(const Mat4 &m, const Vec3 &v ) 
	{
		return Vec3( v.x * m.c[0][0] + v.y * m.c[1][0] + v.z * m.c[2][0],
		             v.x * m.c[0][1] + v.y * m.c[1][1] + v.z * m.c[2][1],
		             v.x * m.c[0][2] + v.y * m.c[1][2] + v.z * m.c[2][2] );
	}
	static Mat4 Inverse( const Mat4 &mat )
	{
		Mat4 m( NO_INIT );
        float d = mat.determinant();
		if( d == 0 ) return m;
		d = 1.0f / d;
		
		m.c[0][0] = d * (mat.c[1][2]*mat.c[2][3]*mat.c[3][1] - mat.c[1][3]*mat.c[2][2]*mat.c[3][1] + mat.c[1][3]*mat.c[2][1]*mat.c[3][2] - mat.c[1][1]*mat.c[2][3]*mat.c[3][2] - mat.c[1][2]*mat.c[2][1]*mat.c[3][3] + mat.c[1][1]*mat.c[2][2]*mat.c[3][3]);
		m.c[0][1] = d * (mat.c[0][3]*mat.c[2][2]*mat.c[3][1] - mat.c[0][2]*mat.c[2][3]*mat.c[3][1] - mat.c[0][3]*mat.c[2][1]*mat.c[3][2] + mat.c[0][1]*mat.c[2][3]*mat.c[3][2] + mat.c[0][2]*mat.c[2][1]*mat.c[3][3] - mat.c[0][1]*mat.c[2][2]*mat.c[3][3]);
		m.c[0][2] = d * (mat.c[0][2]*mat.c[1][3]*mat.c[3][1] - mat.c[0][3]*mat.c[1][2]*mat.c[3][1] + mat.c[0][3]*mat.c[1][1]*mat.c[3][2] - mat.c[0][1]*mat.c[1][3]*mat.c[3][2] - mat.c[0][2]*mat.c[1][1]*mat.c[3][3] + mat.c[0][1]*mat.c[1][2]*mat.c[3][3]);
		m.c[0][3] = d * (mat.c[0][3]*mat.c[1][2]*mat.c[2][1] - mat.c[0][2]*mat.c[1][3]*mat.c[2][1] - mat.c[0][3]*mat.c[1][1]*mat.c[2][2] + mat.c[0][1]*mat.c[1][3]*mat.c[2][2] + mat.c[0][2]*mat.c[1][1]*mat.c[2][3] - mat.c[0][1]*mat.c[1][2]*mat.c[2][3]);
		m.c[1][0] = d * (mat.c[1][3]*mat.c[2][2]*mat.c[3][0] - mat.c[1][2]*mat.c[2][3]*mat.c[3][0] - mat.c[1][3]*mat.c[2][0]*mat.c[3][2] + mat.c[1][0]*mat.c[2][3]*mat.c[3][2] + mat.c[1][2]*mat.c[2][0]*mat.c[3][3] - mat.c[1][0]*mat.c[2][2]*mat.c[3][3]);
		m.c[1][1] = d * (mat.c[0][2]*mat.c[2][3]*mat.c[3][0] - mat.c[0][3]*mat.c[2][2]*mat.c[3][0] + mat.c[0][3]*mat.c[2][0]*mat.c[3][2] - mat.c[0][0]*mat.c[2][3]*mat.c[3][2] - mat.c[0][2]*mat.c[2][0]*mat.c[3][3] + mat.c[0][0]*mat.c[2][2]*mat.c[3][3]);
		m.c[1][2] = d * (mat.c[0][3]*mat.c[1][2]*mat.c[3][0] - mat.c[0][2]*mat.c[1][3]*mat.c[3][0] - mat.c[0][3]*mat.c[1][0]*mat.c[3][2] + mat.c[0][0]*mat.c[1][3]*mat.c[3][2] + mat.c[0][2]*mat.c[1][0]*mat.c[3][3] - mat.c[0][0]*mat.c[1][2]*mat.c[3][3]);
		m.c[1][3] = d * (mat.c[0][2]*mat.c[1][3]*mat.c[2][0] - mat.c[0][3]*mat.c[1][2]*mat.c[2][0] + mat.c[0][3]*mat.c[1][0]*mat.c[2][2] - mat.c[0][0]*mat.c[1][3]*mat.c[2][2] - mat.c[0][2]*mat.c[1][0]*mat.c[2][3] + mat.c[0][0]*mat.c[1][2]*mat.c[2][3]);
		m.c[2][0] = d * (mat.c[1][1]*mat.c[2][3]*mat.c[3][0] - mat.c[1][3]*mat.c[2][1]*mat.c[3][0] + mat.c[1][3]*mat.c[2][0]*mat.c[3][1] - mat.c[1][0]*mat.c[2][3]*mat.c[3][1] - mat.c[1][1]*mat.c[2][0]*mat.c[3][3] + mat.c[1][0]*mat.c[2][1]*mat.c[3][3]);
		m.c[2][1] = d * (mat.c[0][3]*mat.c[2][1]*mat.c[3][0] - mat.c[0][1]*mat.c[2][3]*mat.c[3][0] - mat.c[0][3]*mat.c[2][0]*mat.c[3][1] + mat.c[0][0]*mat.c[2][3]*mat.c[3][1] + mat.c[0][1]*mat.c[2][0]*mat.c[3][3] - mat.c[0][0]*mat.c[2][1]*mat.c[3][3]);
		m.c[2][2] = d * (mat.c[0][1]*mat.c[1][3]*mat.c[3][0] - mat.c[0][3]*mat.c[1][1]*mat.c[3][0] + mat.c[0][3]*mat.c[1][0]*mat.c[3][1] - mat.c[0][0]*mat.c[1][3]*mat.c[3][1] - mat.c[0][1]*mat.c[1][0]*mat.c[3][3] + mat.c[0][0]*mat.c[1][1]*mat.c[3][3]);
		m.c[2][3] = d * (mat.c[0][3]*mat.c[1][1]*mat.c[2][0] - mat.c[0][1]*mat.c[1][3]*mat.c[2][0] - mat.c[0][3]*mat.c[1][0]*mat.c[2][1] + mat.c[0][0]*mat.c[1][3]*mat.c[2][1] + mat.c[0][1]*mat.c[1][0]*mat.c[2][3] - mat.c[0][0]*mat.c[1][1]*mat.c[2][3]);
		m.c[3][0] = d * (mat.c[1][2]*mat.c[2][1]*mat.c[3][0] - mat.c[1][1]*mat.c[2][2]*mat.c[3][0] - mat.c[1][2]*mat.c[2][0]*mat.c[3][1] + mat.c[1][0]*mat.c[2][2]*mat.c[3][1] + mat.c[1][1]*mat.c[2][0]*mat.c[3][2] - mat.c[1][0]*mat.c[2][1]*mat.c[3][2]);
		m.c[3][1] = d * (mat.c[0][1]*mat.c[2][2]*mat.c[3][0] - mat.c[0][2]*mat.c[2][1]*mat.c[3][0] + mat.c[0][2]*mat.c[2][0]*mat.c[3][1] - mat.c[0][0]*mat.c[2][2]*mat.c[3][1] - mat.c[0][1]*mat.c[2][0]*mat.c[3][2] + mat.c[0][0]*mat.c[2][1]*mat.c[3][2]);
		m.c[3][2] = d * (mat.c[0][2]*mat.c[1][1]*mat.c[3][0] - mat.c[0][1]*mat.c[1][2]*mat.c[3][0] - mat.c[0][2]*mat.c[1][0]*mat.c[3][1] + mat.c[0][0]*mat.c[1][2]*mat.c[3][1] + mat.c[0][1]*mat.c[1][0]*mat.c[3][2] - mat.c[0][0]*mat.c[1][1]*mat.c[3][2]);
		m.c[3][3] = d * (mat.c[0][1]*mat.c[1][2]*mat.c[2][0] - mat.c[0][2]*mat.c[1][1]*mat.c[2][0] + mat.c[0][2]*mat.c[1][0]*mat.c[2][1] - mat.c[0][0]*mat.c[1][2]*mat.c[2][1] - mat.c[0][1]*mat.c[1][0]*mat.c[2][2] + mat.c[0][0]*mat.c[1][1]*mat.c[2][2]);
		
		return m;
	}
};

// -------------------------------------------------------------------------------------------------
// Ray
// -------------------------------------------------------------------------------------------------
class Ray 
{
public:
    Vec3 origin;    
    Vec3 direction; // Direção  (normalizada)


    Ray() : origin(0, 0, 0), direction(1, 0, 0) {} 

    // Construtor com parâmetros
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) 
	{
        this->direction.normalize(); // Normaliza a direção do raio
    }

    // Método para obter um ponto em uma distância específica ao longo do raio
    Vec3 pointAt(float distance) const 
	{
        return origin + direction * distance;
    }


			bool Intersection(const Vec3 &vert0, const Vec3 &vert1, const Vec3 &vert2, Vec3 &intsPoint )
			{
				// Idea: Tomas Moeller and Ben Trumbore
				// in Fast, Minimum Storage Ray/Triangle Intersection 
				
				// Find vectors for two edges sharing vert0
				Vec3 edge1 = vert1 - vert0;
				Vec3 edge2 = vert2 - vert0;

				// Begin calculating determinant - also used to calculate U parameter
				Vec3 pvec = direction.cross( edge2 );

				// If determinant is near zero, ray lies in Plane3D of triangle
				float det = edge1.dot( pvec );


				// *** Culling branch ***
				/*if( det < Epsilon )return false;

				// Calculate distance from vert0 to ray origin
				Vec3 tvec = origin - vert0;

				// Calculate U parameter and test bounds
				float u = tvec.dot( pvec );
				if (u < 0 || u > det ) return false;

				// Prepare to test V parameter
				Vec3 qvec = tvec.cross( edge1 );

				// Calculate V parameter and test bounds
				float v = direction.dot( qvec );
				if (v < 0 || u + v > det ) return false;

				// Calculate t, scale parameters, ray intersects triangle
				float t = edge2.dot( qvec ) / det;*/


				// *** Non-culling branch ***
				if( det > -Epsilon && det < Epsilon ) return 0;
				float inv_det = 1.0f / det;

				// Calculate distance from vert0 to ray origin
				Vec3 tvec = origin - vert0;

				// Calculate U parameter and test bounds
				float u = tvec.dot( pvec ) * inv_det;
				if( u < 0.0f || u > 1.0f ) return 0;

				// Prepare to test V parameter
				Vec3 qvec = tvec.cross( edge1 );

				// Calculate V parameter and test bounds
				float v = direction.dot( qvec ) * inv_det;
				if( v < 0.0f || u + v > 1.0f ) return 0;

				// Calculate t, ray intersects triangle
				float t = edge2.dot( qvec ) * inv_det;


				// Calculate intersection point and test ray length and direction
				intsPoint = origin + direction * t;
				Vec3 vec = intsPoint - origin;
				if( vec.dot( direction ) < 0 || vec.length() > direction.length() ) return false;

				return true;
			}

};


// -------------------------------------------------------------------------------------------------
// Plane3D
// -------------------------------------------------------------------------------------------------

class Plane3D
{

private:
    float distToPoint(const Vec3& point) const 
	{
        return normal.dot(point) + dist;
    }

public:
	Vec3 normal; 
	float dist;

	// ------------
	// Constructors
	// ------------
	Plane3D() 
	{ 
		normal.x = 0; normal.y = 0; normal.z = 0; dist = 0; 
	};

	explicit Plane3D( const float a, const float b, const float c, const float d )
	{
		normal = Vec3( a, b, c );
		float invLen = 1.0f / normal.length();
		normal *= invLen;	// Normalize
		dist = d * invLen;
	}

	Plane3D( const Vec3 &v0, const Vec3 &v1, const Vec3 &v2 )
	{
		normal = v1 - v0;
		normal = normal.cross( v2 - v0 );
		normal.normalize();
		dist = -normal.dot( v0 );
	}

	bool intersect(const Ray& ray, Vec3& intersectionPoint) const 
	{
			float denom = normal.dot(ray.direction);

			// Verifica se o raio é paralelo ao plano
			if (std::abs(denom) < Epsilon) 
			{
				return false;
			}

			float t = -(normal.dot(ray.origin) + dist) / denom;

			// Verifica se a interseção está à frente do ponto de origem do raio
			if (t >= 0) 
			{
				intersectionPoint = ray.pointAt(t);
				return true;
			}

			return false;
		}

	// ----------------
	// Other operations
	// ----------------
	 float distanceToPoint(const Vec3& point) const 
	 {
        float distance = distToPoint(point);
        return Abs(distance);
    }
	 bool containsPoint(const Vec3& point) const 
	 {
        float distance = distToPoint(point);
        return Abs(distance) < Epsilon; 
    }
	Vec3 projectPoint(const Vec3& point) const 
	{
        float distance = distToPoint(point);
        return point - normal * distance;
    }
};


struct BoundingBox
{
	Vec3 min;
	Vec3 max;

	BoundingBox() : min(0, 0, 0), max(0, 0, 0) {}
	BoundingBox(const Vec3 &min, const Vec3 &max) : min(min), max(max) {}
	BoundingBox(const BoundingBox &box) : min(box.min), max(box.max) {}

	void Set(const Vec3 &min, const Vec3 &max)
	{
		this->min = min;
		this->max = max;
	}

	
	
	
	bool Merge( const Vec3 &_min, const Vec3 &_max )
	{
		bool changed = false;

		// Ignore zero-size boxes
		if( min == max )
		{
			changed = true;
			min = _min;
			max = _max;
		}
		else if( _min != _max )
		{
			if( _min.x < min.x ) { changed = true; min.x = _min.x; }
			if( _min.y < min.y ) { changed = true; min.y = _min.y; }
			if( _min.z < min.z ) { changed = true; min.z = _min.z; }

			if( _max.x > max.x ) { changed = true; max.x = _max.x; }
			if( _max.y > max.y ) { changed = true; max.y = _max.y; }
			if( _max.z > max.z ) { changed = true; max.z = _max.z; }
		}

		return changed;
	}
	bool Merge( const BoundingBox &b )
	{
		bool changed = false;

		// Ignore zero-size boxes
		if( min == max )
		{
			changed = true;
			min = b.min;
			max = b.max;
		}
		else if( b.min != b.max )
		{
			if( b.min.x < min.x ) { changed = true; min.x = b.min.x; }
			if( b.min.y < min.y ) { changed = true; min.y = b.min.y; }
			if( b.min.z < min.z ) { changed = true; min.z = b.min.z; }

			if( b.max.x > max.x ) { changed = true; max.x = b.max.x; }
			if( b.max.y > max.y ) { changed = true; max.y = b.max.y; }
			if( b.max.z > max.z ) { changed = true; max.z = b.max.z; }
		}

		return changed;
	}


	void AddPoint(const Vec3 &point)
	{
		min = min.Min(point);
		max = max.Max(point);
	}
	void Clear()
	{
		min = Vec3(0, 0, 0);
		max = Vec3(0, 0, 0);
	}

	Vec3 GetCorner( u32 index ) const
	{
		switch( index )
		{
		case 0:
			return Vec3( min.x, min.y, max.z );
		case 1:
			return Vec3( max.x, min.y, max.z );
		case 2:
			return Vec3( max.x, max.y, max.z );
		case 3:
			return Vec3( min.x, max.y, max.z );
		case 4:
			return Vec3( min.x, min.y, min.z );
		case 5:
			return Vec3( max.x, min.y, min.z );
		case 6:
			return Vec3( max.x, max.y, min.z );
		case 7:
			return Vec3( min.x, max.y, min.z );
		default:
			return Vec3();
		}
	}

	void Transform( const Mat4 &m )
	{
		// Efficient algorithm for transforming an AABB, taken from Graphics Gems
		
		float minA[3] = { min.x, min.y, min.z }, minB[3];
		float maxA[3] = { max.x, max.y, max.z }, maxB[3];

		for( u32 i = 0; i < 3; ++i )
		{
			minB[i] = m.c[3][i];
			maxB[i] = m.c[3][i];
			
			for( u32 j = 0; j < 3; ++j )
			{
				float x = minA[j] * m.c[j][i];
				float y = maxA[j] * m.c[j][i];
				minB[i] += Min( x, y );
				maxB[i] += Max( x, y );
			}
		}

		min = Vec3( minB[0], minB[1], minB[2] );
		max = Vec3( maxB[0], maxB[1], maxB[2] );
	}


		bool Intersection( const Ray &ray)
		{
			// SLAB based optimized ray/AABB intersection routine
			// Idea taken from http://ompf.org/ray/
			
			float l1 = (min.x - ray.origin.x) / ray.direction.x;
			float l2 = (max.x - ray.origin.x) / ray.direction.x;
			float lmin = Min( l1, l2 );
			float lmax = Max( l1, l2 );

			l1 = (min.y - ray.origin.y) / ray.direction.y;
			l2 = (max.y - ray.origin.y) / ray.direction.y;
			lmin = Max( Min( l1, l2 ), lmin );
			lmax = Min( Max( l1, l2 ), lmax );
				
			l1 = (min.z - ray.origin.z) / ray.direction.z;
			l2 = (max.z - ray.origin.z) / ray.direction.z;
			lmin = Max( Min( l1, l2 ), lmin );
			lmax = Min( Max( l1, l2 ), lmax );

			if( (lmax >= 0.0f) & (lmax >= lmin) )
			{
				// Consider length
				const Vec3 rayDest = ray.origin + ray.direction;
				Vec3 rayMins( Min( rayDest.x, ray.origin.x), Min( rayDest.y, ray.origin.y ), Min( rayDest.z, ray.origin.z ) );
				Vec3 rayMaxs( Max( rayDest.x, ray.origin.x), Max( rayDest.y, ray.origin.y ), Max( rayDest.z, ray.origin.z ) );
				return 
					(rayMins.x < max.x) && (rayMaxs.x > min.x) &&
					(rayMins.y < max.y) && (rayMaxs.y > min.y) &&
					(rayMins.z < max.z) && (rayMaxs.z > min.z);
			}
			else
				return false;
		}

		float Distance( const Vec3 &pos)
		{
			const Vec3 center = (min + max) * 0.5f;
			const Vec3 extent = (max - min) * 0.5f;
			
			Vec3 nearestVec;
			nearestVec.x = Max( 0.0f, fabsf( pos.x - center.x ) - extent.x );
			nearestVec.y = Max( 0.0f, fabsf( pos.y - center.y ) - extent.y );
			nearestVec.z = Max( 0.0f, fabsf( pos.z - center.z ) - extent.z );
			
			return nearestVec.length();
		}


	BoundingBox& operator=(const BoundingBox &box)
	{
		if (this == &box)
			return *this;
		min = box.min;
		max = box.max;
		return *this;
	}

	static BoundingBox TransformBoundingBox(const BoundingBox &box, const Mat4 &m)
	{
		BoundingBox result(box);
		result.Transform(m);
		return result;
	}
	static void TransformBoundingBox(const BoundingBox &box, const Mat4 &m,  BoundingBox &out)
	{
		out.Merge(box);
		out.Transform(m);
	}

	Vec3 corner(int n) const 
	{
		return Vec3( (n & 1) ? max.x : min.x, (n & 2) ? max.y : min.y, (n & 4) ? max.z : min.z );
	}
};


class Frustum
{
public:
	const Vec3 &getOrigin() const { return m_origin; }
	const Vec3 &getCorner( u32 index ) const { return m_corners[index]; }

	void build( const Mat4 &transMat, float left, float right,float bottom, float top, float nearPlane, float farPlane ) 
	{
			float left_f = left * farPlane / nearPlane;
			float right_f = right * farPlane / nearPlane;
			float bottom_f = bottom * farPlane / nearPlane;
			float top_f = top * farPlane / nearPlane;

	
			m_corners[0] = Vec3( left, bottom, -nearPlane );
			m_corners[1] = Vec3( right, bottom, -nearPlane );
			m_corners[2] = Vec3( right, top, -nearPlane );
			m_corners[3] = Vec3( left, top, -nearPlane );

			m_corners[4] = Vec3( left_f, bottom_f, -farPlane );
			m_corners[5] = Vec3( right_f, bottom_f, -farPlane );
			m_corners[6] = Vec3( right_f, top_f, -farPlane );
			m_corners[7] = Vec3( left_f, top_f, -farPlane );

			// Transform points to fit camera position and rotation
			m_origin = transMat * Vec3( 0, 0, 0 );
			for( u32 i = 0; i < 8; ++i )
				m_corners[i] = transMat * m_corners[i];

			m_planes[0] = Plane3D( m_origin, m_corners[3], m_corners[0] );		// Left
			m_planes[1] = Plane3D( m_origin, m_corners[1], m_corners[2] );		// Right
			m_planes[2] = Plane3D( m_origin, m_corners[0], m_corners[1] );		// Bottom
			m_planes[3] = Plane3D( m_origin, m_corners[2], m_corners[3] );		// Top
			m_planes[4] = Plane3D( m_corners[0], m_corners[1], m_corners[2] );	// Near
			m_planes[5] = Plane3D( m_corners[5], m_corners[4], m_corners[7] );	// Far
	}

	
	void build( const Mat4 &transMat, float fov, float aspect, float nearPlane, float farPlane )
	{
		float ymax = nearPlane * tanf( degToRad( fov / 2 ) );
		float xmax = ymax * aspect;
		
		build( transMat, -xmax, xmax, -ymax, ymax, nearPlane, farPlane );
	}
	void build( const Mat4 &viewMat, const Mat4 &projMat )
	{
		// This routine works with the OpenGL projection matrix
	// The view matrix is the inverse camera transformation matrix
	// Note: Frustum corners are not updated!
	
	Mat4 m = projMat * viewMat;
	
	m_planes[0] = Plane3D( -(m.c[0][3] + m.c[0][0]), -(m.c[1][3] + m.c[1][0]),
						-(m.c[2][3] + m.c[2][0]), -(m.c[3][3] + m.c[3][0]) );	// Left
	m_planes[1] = Plane3D( -(m.c[0][3] - m.c[0][0]), -(m.c[1][3] - m.c[1][0]),
						-(m.c[2][3] - m.c[2][0]), -(m.c[3][3] - m.c[3][0]) );	// Right
	m_planes[2] = Plane3D( -(m.c[0][3] + m.c[0][1]), -(m.c[1][3] + m.c[1][1]),
						-(m.c[2][3] + m.c[2][1]), -(m.c[3][3] + m.c[3][1]) );	// Bottom
	m_planes[3] = Plane3D( -(m.c[0][3] - m.c[0][1]), -(m.c[1][3] - m.c[1][1]),
						-(m.c[2][3] - m.c[2][1]), -(m.c[3][3] - m.c[3][1]) );	// Top
	m_planes[4] = Plane3D( -(m.c[0][3] + m.c[0][2]), -(m.c[1][3] + m.c[1][2]),
						-(m.c[2][3] + m.c[2][2]), -(m.c[3][3] + m.c[3][2]) );	// Near
	m_planes[5] = Plane3D( -(m.c[0][3] - m.c[0][2]), -(m.c[1][3] - m.c[1][2]),
						-(m.c[2][3] - m.c[2][2]), -(m.c[3][3] - m.c[3][2]) );	// Far

	m_origin = viewMat.inverted() * Vec3( 0, 0, 0 );

	// Calculate corners
	Mat4 mm = m.inverted();
	Vec4 corner = mm * Vec4( -1, -1,  -1, 1 );
	m_corners[0] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( 1, -1,  -1, 1 );
	m_corners[1] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( 1,  1,  -1, 1 );
	m_corners[2] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( -1,  1,  -1, 1 );
	m_corners[3] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( -1, -1, 1, 1 );
	m_corners[4] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( 1, -1, 1, 1 );
	m_corners[5] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( 1, 1, 1, 1 );
	m_corners[6] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	corner = mm * Vec4( -1, 1, 1, 1 );
	m_corners[7] = Vec3( corner.x / corner.w, corner.y / corner.w, corner.z / corner.w );
	}
	bool SphereInside( Vec3 pos, float rad ) const
	{
		for( u32 i = 0; i < 6; ++i )
		{
			if( m_planes[i].distanceToPoint( pos ) > rad ) return true;
		}

		return false;
	}
	bool BoxInside( BoundingBox &b ) const
	{
		for( u32 i = 0; i < 6; ++i )
		{
			const Vec3 &n = m_planes[i].normal;
			
			Vec3 positive = b.min;
			if( n.x <= 0 ) positive.x = b.max.x;
			if( n.y <= 0 ) positive.y = b.max.y;
			if( n.z <= 0 ) positive.z = b.max.z;

			if( m_planes[i].distanceToPoint( positive ) > 0 ) return true;
		}
		
		return false;
	}
	
	bool PointInside(const Vec3& point) const 
	{
		for( u32 i = 0; i < 6; ++i )
		{
			const Plane3D& plane = m_planes[i];
			if (plane.distanceToPoint(point) < 0) 
			{		
				return false;
			}
		}
    return true;
}

	void getAABB( Vec3 &mins, Vec3 &maxs ) const
	{
		mins.x = MaxFloat; mins.y = MaxFloat; mins.z = MaxFloat;
		maxs.x = -MaxFloat; maxs.y = -MaxFloat; maxs.z = -MaxFloat;
		
		for( u32 i = 0; i < 8; ++i )
		{
			if( m_corners[i].x < mins.x ) mins.x = m_corners[i].x;
			if( m_corners[i].y < mins.y ) mins.y = m_corners[i].y;
			if( m_corners[i].z < mins.z ) mins.z = m_corners[i].z;
			if( m_corners[i].x > maxs.x ) maxs.x = m_corners[i].x;
			if( m_corners[i].y > maxs.y ) maxs.y = m_corners[i].y;
			if( m_corners[i].z > maxs.z ) maxs.z = m_corners[i].z;
		}
	}
	void getAABB( BoundingBox &b ) const
	{
		Vec3 mins, maxs;
		getAABB( mins, maxs );
		b.min = mins;
		b.max = maxs;
	}

private:
	Plane3D  m_planes[6];  
	Vec3  m_origin;
	Vec3  m_corners[8];  
};





template<typename T>
struct  Rectangle
{

T x;
T y;
T width;
T height;

Rectangle() : x(0), y(0), width(0), height(0) {}
Rectangle(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}
Rectangle(const Rectangle &rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}

void Set(T x, T y, T width, T height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Merge(const Rectangle &rect)
{
	T right = x + width;
	T bottom = y + height;
	T rectRight = rect.x + rect.width;
	T rectBottom = rect.y + rect.height;
	x = Min(x, rect.x);
	y = Min(y, rect.y);
	right = Max(right, rectRight);
	bottom = Max(bottom, rectBottom);
	width = right - x;
	height = bottom - y;
}

void Merge(const Vec2 &point)
{
	T right = x + width;
	T bottom = y + height;
	x =Min(x, point.x);
	y = Min(y, point.y);
	right = Max(right, point.x);
	bottom = Max(bottom, point.y);
	width = right - x;
	height = bottom - y;
}

void Clear()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Rectangle& operator=(const Rectangle &rect)
{
	if (this == &rect)
		return *this;
	x = rect.x;
	y = rect.y;
	width = rect.width;
	height = rect.height;
	return *this;
}

};

template<typename T>
struct  Size
{
T width;
T height;

Size() : width(0), height(0) {}
Size(T w, T h) : width(w), height(h) {}
Size(const Size &size) : width(size.width), height(size.height) {}

Size& operator=(const Size &size)
{
	if (this == &size)
		return *this;
	width = size.width;
	height = size.height;
	return *this;
}


};

typedef Rectangle<int> IntRect;
typedef Rectangle<float> FloatRect;
typedef Size<int> IntSize;
typedef Size<float> FloatSize;

