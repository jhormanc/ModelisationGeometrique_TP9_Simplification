#pragma once

/*
* class utililitaire representant les vecteurs, points dans un espace 2D.
*/
template<typename T>
class Vec2
{
public:
	T x, y;

	Vec2(){};

	Vec2(T n) :x(n), y(n){};
	Vec2(T x_, T y_) :x(x_), y(y_){};
	Vec2(const Vec2<T> & p) :x(p.x), y(p.y){};

	/*
	* Surcharge des operateurs arithmetiques
	*/

	inline const Vec2 & operator=(const Vec2<T> & p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	inline const Vec2 & operator+=(const Vec2<T> & p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	inline const Vec2 & operator*=(const Vec2<T> & p)
	{
		x *= p.x;
		y *= p.y;
		return *this;
	}

	inline const Vec2 & operator*=(T k)
	{
		x *= k;
		y *= k;
		return *this;
	}

	inline const bool operator==(const Vec2<T> & p)
	{
		return (x == p.x && y == p.y) || (y == p.x && x == p.y);
	}

	inline Vec2 operator-()
	{
		return Vec2<T>(-x, -y);
	}

	// Produit Scalaire
	static inline float dotProduct(const Vec2<T> & v1, const Vec2<T> & v2) { return v1.x * v2.x + v1.y * v2.y; }
};

template <typename T>
inline const Vec2<T> operator * (const Vec2<T> & v, const T & k) { return Vec2<T>(v.x * k, v.y * k); }

template <typename T>
inline const Vec2<T> operator * (const T & k, const Vec2<T> & v) { return Vec2<T>(v.x * k, v.y * k); }

template <typename T>
inline const Vec2<T> operator * (const Vec2<T> & v1, const Vec2<T> & v2) { return Vec2<T>(v1.x * v2.x, v1.y * v2.y); }

template <typename T>
inline const Vec2<T> operator / (const Vec2<T> & v, const T & k) { return Vec2<T>(v.x / k, v.y / k); }

template <typename T>
inline const Vec2<T> operator + (const Vec2<T> & v1, const Vec2<T> & v2) { return Vec2<T>(v1.x + v2.x, v1.y + v2.y); }

template <typename T>
inline const Vec2<T> operator - (const Vec2<T> & v1, const Vec2<T> & v2) { return Vec2<T>(v1.x - v2.x, v1.y - v2.y); }

template <typename T>
inline float distanceSquared(const Vec2<T> & v1, const Vec2<T> & v2) { return (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y); }

template <typename T>
inline float distance(const Vec2<T> & v1, const Vec2<T> & v2) { return sqrt(distanceSquared(v1, v2)); }