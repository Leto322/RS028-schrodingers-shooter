#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath>

//Klasa Vektor
class Vector2D{
public:
    Vector2D(double x = 0, double y = 0);

    double x() const;
    void setX(double x);

    double y() const;
    void setY(double y);

    //Sabiranje tacaka
    Vector2D operator + (const Vector2D & other) const;

    //Oduzimanje tacaka
    Vector2D operator - (const Vector2D & other) const;

    //+= operator
    void operator += (const Vector2D & other);

    //Skalarni proizvod
    double operator * (const Vector2D & other) const;

    //Skaliranje koordinata (posto su sa slike koja predstavlja mapu) u koordinate scene
    void scale(void);

    //Dodavanje konstanti na vektor
    void add(double dx, double dy);

private:
    double m_x;
    double m_y;

    friend std::ostream & operator << (std::ostream & out, const Vector2D & value);
};

std::ostream & operator << (std::ostream & out, const Vector2D & value);


// Klasa Duz
class Line{
public:
    Line(Vector2D A, Vector2D B);

    Vector2D A() const;
    void setA(const Vector2D & A);

    Vector2D B() const;
    void setB(const Vector2D & B);




private:
    Vector2D m_A;
    Vector2D m_B;
};


// Klasa Krug
class Circle{
public:
    Circle(Vector2D O, double r);

    Vector2D O() const;
    void setO(const Vector2D & O);

    double r() const;
    void setR(double r);

    bool intersection(const Circle & other) const;


private:
    Vector2D m_O;
    double m_r;
};


#endif
