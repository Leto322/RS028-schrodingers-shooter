#include "../header/geometry.h"

//Definicije konstruktora i metoda za klasu Vector2D
Vector2D::Vector2D(double x, double y)
    : m_x(x), m_y(y)
{
}

double Vector2D::x() const
{
    return m_x;
}

void Vector2D::setX(double x)
{
    m_x = x;
}

double Vector2D::y() const
{
    return m_y;
}

void Vector2D::setY(double y)
{
    m_y = y;
}

void Vector2D::scale(void)
{
    m_x = m_x/200-9;
    m_y = m_y/200+9.2;
}

void Vector2D::add(double dx, double dy){
    m_x += dx;
    m_y += dy;
}

Vector2D Vector2D::operator + (const Vector2D & other) const
{
    return Vector2D(x() + other.x(), y() + other.y());
}

Vector2D Vector2D::operator - (const Vector2D & other) const
{
    return Vector2D(x() - other.x(), y() - other.y());
}


void Vector2D::operator += (const Vector2D & other)
{
    m_x += other.x();
    m_y += other.y();
}

double Vector2D::operator * (const Vector2D & other) const
{
    return x()*other.x() + y()*other.y();
}

std::ostream & operator << (std::ostream & out, const Vector2D & value)
{
    return out << "(" << value.x() << "," << value.y() << ")";
}


//Definicije konstruktora i metoda za klasu Line
Line::Line(Vector2D A, Vector2D B)
    : m_A(A), m_B(B)
{
}

Vector2D Line::A() const
{
    return m_A;
}

void Line::setA(const Vector2D & A)
{
    m_A = A;
}

Vector2D Line::B() const
{
    return m_B;
}

void Line::setB(const Vector2D & B)
{
    m_B = B;
}


//Definicije konstruktora i metoda za klasu Circle
Circle::Circle(Vector2D O, double r){
    if(r < 0){
        m_O = O;
        m_r = 0;
    }
    else {
        m_O = O;
        m_r = r;
    }
}


Vector2D Circle::O() const
{
    return m_O;
}

void Circle::setO(const Vector2D & O)
{
    m_O = O;
}

double Circle::r() const
{
    return m_r;
}

void Circle::setR(double r)
{
    if(r<0){
        m_r = 0;
    }
    else{
        m_r = r;
    }
}

//Presek izmedju dva kruga
bool Circle::intersection(const Circle & other) const
{
    double d = sqrt(pow((other.O().x() - O().x()), 2) + pow((other.O().y() - O().y()), 2));
    if (d > r() + other.r())
    {
        return false;
    }
    else
    {
        return true;
    }
}

//int main(int argc, const char *argv[])
//{
//    Vector2D A = Vector2D(0,0);
//    Vector2D B = Vector2D(2,0);
//    Circle C1 = Circle(A, 1);
//    Circle C2 = Circle(B, 0.999999);
//
//    // Line AB = Line(A,B);
//    // Circle C = Circle(A, 2.5);
//
//   // std::cout << A << "\n" << B << "\n" << (A+B) << "\n" << (A*B)<<std::endl;
//   // std::cout << AB.A() << "\n" << AB.B() << std::endl;
//   // std::cout << C.O() << "\n" << C.r() << std::endl;
//
//    A += A;
//    B.add(1, 1);
//    std::cout << A << "\n" << B  << std::endl;
//   return 0;
//}
