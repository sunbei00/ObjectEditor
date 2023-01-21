#include "Vector3.h"
#include <math.h>


Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3::Vector3(const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}
Vector3::Vector3() : Vector3(0, 0, 0) {}

GLfloat Vector3::magnitude() {
	return (GLfloat)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
Vector3 Vector3::normalize() {
	GLfloat mag = magnitude();
	return Vector3(x/mag, y/mag, z/mag);
}
Vector3 Vector3::crossProduct(const Vector3& vec)
{
	return Vector3(y * vec.z - z * vec.y, x * vec.z - z * vec.x, x * vec.y - y * vec.x);
}
Vector3 Vector3::crossProduct(Vector3&& vec)
{
	return Vector3(y * vec.z - z * vec.y, x * vec.z - z * vec.x, x * vec.y - y * vec.x);
}
Vector3 Vector3::operator*(const Vector3& vec) {
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}
Vector3 Vector3::operator*(Vector3&& vec) {
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}
Vector3 Vector3::operator/(const Vector3& vec) {
	return Vector3(x / vec.x, y / vec.y, z / vec.z);
}
Vector3 Vector3::operator/(Vector3&& vec) {
	return Vector3(x / vec.x, y / vec.y, z / vec.z);
}
Vector3 Vector3::operator-(const Vector3& vec) {
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}
Vector3 Vector3::operator-(Vector3&& vec) {
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}
Vector3 Vector3::operator+(const Vector3& vec) {
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}
Vector3 Vector3::operator+(Vector3&& vec) {
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}
Vector3& Vector3::operator=(const Vector3& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}
Vector3 operator+(const Vector3& vec, GLfloat f) {
	return Vector3(vec.x + f, vec.y + f, vec.z + f);
}
Vector3 operator+(GLfloat f, const Vector3& vec) {
	return Vector3(vec.x + f, vec.y + f, vec.z + f);
}
Vector3 operator-(const Vector3& vec, GLfloat f) {
	return Vector3(vec.x - f, vec.y - f, vec.z - f);
}
Vector3 operator-(GLfloat f, const Vector3& vec) {
	return Vector3(f - vec.x, f - vec.y, f - vec.z);
}
Vector3 operator*(const Vector3& vec, GLfloat f) {
	return Vector3(vec.x * f, vec.y * f, vec.z * f);
}
Vector3 operator*(GLfloat f, const Vector3& vec) {
	return Vector3(vec.x * f, vec.y * f, vec.z * f);
}
Vector3 operator/(const Vector3& vec, GLfloat f) {
	return Vector3(vec.x / f, vec.y / f, vec.z / f);
}
Vector3 operator/(GLfloat f, const Vector3& vec) {
	return Vector3(f / vec.x, f / vec.y, f / vec.z);
}
