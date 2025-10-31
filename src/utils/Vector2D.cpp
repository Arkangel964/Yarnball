//
// Created by WillO on 2025-09-23.
//

#include "Vector2D.h"

#include <cmath>

Vector2D &Vector2D::normalize() {
    float length = sqrtf(x * x + y * y);

    if (length > 0) {
        this->x /= length;
        this->y /= length;
    }

    return *this;
}


//member operator func
Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(x / scalar, y / scalar);
}

Vector2D Vector2D::operator+(const Vector2D vec) const {
    return Vector2D(x + vec.x, y + vec.y);
}

Vector2D Vector2D::operator-(const Vector2D vec) const {
    return Vector2D(x - vec.x, y - vec.y);
}

Vector2D& Vector2D::operator*=(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(float scalar) {
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

Vector2D& Vector2D::operator+=(Vector2D vec){
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2D & Vector2D::operator-=(const Vector2D vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D& Vector2D::operator-() {
    this->x = -this->x;
    this->y = -this->y;
    return *this;
}

bool Vector2D::operator==(const Vector2D vec) const {
    return x == vec.x && y == vec.y;
}

bool Vector2D::operator!=(const Vector2D vec) const {
    return x != vec.x || y != vec.y;
}

//non-member operator func
Vector2D operator*(float scalar, Vector2D& vec) {
    return Vector2D(vec.x * scalar, vec.y * scalar);
}

Vector2D operator/(float scalar, Vector2D& vec) {
    return Vector2D(vec.x / scalar, vec.y / scalar);
}
