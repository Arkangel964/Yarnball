//
// Created by WillO on 2025-09-23.
//

#ifndef TUTORIAL1_VECTOR2D_H
#define TUTORIAL1_VECTOR2D_H

class Vector2D {
    public:
    float x;
    float y;

    Vector2D() {
        x = 0.0f;
        y = 0.0f;
    };

    Vector2D(float x, float y) : x(x), y(y) {};

    Vector2D& normalize();
    float length();

    //member operator function
    //Vector2D has to be on the left, float on the right
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    Vector2D operator+(const Vector2D vec) const;
    Vector2D operator-(const Vector2D vec) const;

    Vector2D& operator*=(float scalar);
    Vector2D& operator/=(float scalar);

    Vector2D& operator+=(const Vector2D vec);
    Vector2D& operator-=(const Vector2D vec);

    //unary negation
    Vector2D& operator-();

    bool operator==(const Vector2D vec) const;
    bool operator!=(const Vector2D vec) const;

    //Vector2D on the right, float on the left, needs to be non-member operator
    friend Vector2D operator*(float scalar, Vector2D& vec);
    friend Vector2D operator/(float scalar, Vector2D& vec);
};
#endif //TUTORIAL1_VECTOR2D_H