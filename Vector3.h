#pragma once
typedef float GLfloat;

class Vector3 {
public:
	GLfloat x, y, z;
	Vector3(GLfloat x, GLfloat y, GLfloat z);
	Vector3(const Vector3& vec);
	Vector3();
	GLfloat magnitude();
	Vector3 normalize();
	Vector3 crossProduct(const Vector3& vec);
	Vector3 crossProduct(Vector3&& vec);
	Vector3 operator*(const Vector3& vec);
	Vector3 operator*(Vector3&& vec);
	Vector3 operator/(const Vector3& vec);
	Vector3 operator/(Vector3&& vec);
	Vector3 operator-(const Vector3& vec);
	Vector3 operator-(Vector3&& vec);
	Vector3 operator+(const Vector3& vec);
	Vector3 operator+(Vector3&& vec);
	Vector3& operator=(const Vector3& vec);
};
Vector3 operator+(const Vector3& vec, GLfloat f);
Vector3 operator+(GLfloat f, const Vector3& vec);
Vector3 operator-(const Vector3& vec, GLfloat f);
Vector3 operator-(GLfloat f, const Vector3& vec);
Vector3 operator*(const Vector3& vec, GLfloat f);
Vector3 operator*(GLfloat f, const Vector3& vec);
Vector3 operator/(const Vector3& vec, GLfloat f);
Vector3 operator/(GLfloat f, const Vector3& vec);