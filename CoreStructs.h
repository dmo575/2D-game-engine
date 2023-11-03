#pragma once

struct Point
{
public:
    Point()
    : x(0.0f)
    , y(0.0f)
    {}

    Point(int x, int y)
        : x(static_cast<float>(x))
        , y(static_cast<float>(y))
    {}

    Point(float x, float y)
        : x(x)
        , y(y)
    {}


public:
    float x, y;

    Point& operator= (const Point& right)
    {
        this->x = right.x;
        this->y = right.y;

        return*this;
    }

    bool operator== (const Point& right) const
    {
        return (this->x == right.x && this->y == right.y);
    }

    bool operator!= (const Point& right) const
    {
        return !(*this == right);
    }

    Point operator+ (const Point& right) const
    {
        return Point(this->x + right.x, this->y + right.y);
    }

    Point operator- (const Point& right) const
    {
        return Point(this->x - right.x, this->y - right.y);
    }

    Point operator* (float right) const
    {
        return Point(this->x * right, this->y * right);
    }

    Point& operator+= (const Point& right)
    {
        *this = *this + right;
        return *this;
    }

    Point& operator-= (const Point& right)
    {
        *this = *this - right;
        return *this;
    }

 //         -----------------------------                 POINT ON FLOAT

    Point operator+ (float right) const
    {
        return Point(this->x + right, this->y + right);
    }

    Point operator/ (float right) const
    {
        return Point(this->x / right, this->y / right);
    }

    Point& operator/= (float right)
    {
        *this = *this / right;
        return *this;
    }

};


struct Point3D
{
public:
    Point3D()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    {}

    Point3D(int x, int y, int z)
        : x(static_cast<float>(x))
        , y(static_cast<float>(y))
        , z(static_cast<float>(z))
    {}

    Point3D(float x, float y, float z)
        : x(x)
        , y(y)
        , z(y)
    {}


public:
    float x, y, z;

    bool operator == (const Point3D& right) const
    {
        return right.x == x && right.y == y && right.z == z;
    }

    bool operator != (const Point3D& right) const
    {
        return !(*this == right);
    }

    Point3D operator += (const Point3D& right) const
    {
        return Point3D(x + right.x, y + right.y, z + right.z);
    }

    Point3D operator -= (const Point3D& right) const
    {
        return Point3D(x - right.x, y - right.y, z - right.z);
    }

    Point3D operator + (const Point3D& right) const
    {
        return Point3D(x + right.x, y + right.y, z + right.z);
    }

    Point3D operator - (const Point3D& right) const
    {
        return Point3D(x - right.x, y - right.y, z - right.z);
    }

    Point3D operator * (float right) const
    {
        return Point3D(x * right, y * right, z * right);
    }

    Point3D operator + (float right) const
    {
        return Point3D(x + right, y + right, z + right);
    }

    Point3D operator /= (float right) const
    {
        return Point3D(x / right, y / right, z / right);
    }

    Point3D operator / (float right) const
    {
        return Point3D(x / right, y / right, z / right);
    }
};


struct Rect
{
public:
    Rect()
        : x(0)
        , y(0)
        , w(0)
        , h(0)
    {}

    Rect(int x, int y, int width, int height)
        : x(x)
        , y(y)
        , w(width)
        , h(height)
    {}

    Rect(Point& pos, Point& dimensions)
        : x((int)pos.x)
        , y((int)pos.y)
        , w((int)dimensions.x)
        , h((int)dimensions.y)
    {}

public:
    int x, y, w, h;
};

struct Color
{
    enum class PreDefined {k_Red, k_Blue, k_Green, k_Purple, k_White, k_Black, k_Orange, k_Yellow, k_Cyan};

    Color(int r, int g, int b, int a)
        :r(r)
        ,g(g)
        ,b(b)
        ,a(a)
    {}
    Color(PreDefined color)
    {
        switch (color)
        {
        case Color::PreDefined::k_Red:
        {
            r = 255;
            g = 0;
            b = 0;
            a = 255;
        }
            break;
        case Color::PreDefined::k_Blue:
        {
            r = 0;
            g = 0;
            b = 255;
            a = 255;
        }
            break;
        case Color::PreDefined::k_Green:
        {
            r = 0;
            g = 255;
            b = 0;
            a = 255;
        }
            break;
        case Color::PreDefined::k_Purple:
        {
            r = 255;
            g = 0;
            b = 255;
            a = 255;
        }
            break;
        case Color::PreDefined::k_White:
        {
            r = 255;
            g = 255;
            b = 255;
            a = 255;
        }
            break;
        case Color::PreDefined::k_Black:
        {
            r = 0;
            g = 0;
            b = 0;
            a = 255;
        }
            break;
        case Color::PreDefined::k_Yellow:
        {
            r = 255;
            g = 255;
            b = 0;
            a = 255;
        }
        break;
        case Color::PreDefined::k_Orange:
        {
            r = 255;
            g = 120;
            b = 0;
            a = 255;
        }
        break;
        case Color::PreDefined::k_Cyan:
        {
            r = 0;
            g = 255;
            b = 255;
            a = 255;
        }
        break;
        default:
        {
            r = 125;
            g = 125;
            b = 125;
            a = 125;
        }
            break;
        }
    }
    int r, g, b, a;

    bool operator == (const Color& right)
    {
        return (r == right.r &&
            g == right.g &&
            b == right.b &&
            a == right.a);
    }

    bool operator != (const Color& right)
    {
        return !(r == right.r &&
            g == right.g &&
            b == right.b &&
            a == right.a);
    }
};


enum class KeyCode
{
    k_A, k_B, k_C, k_D, k_E, k_F, k_G, k_H, k_I, k_J, k_K, k_L, k_M, k_N, k_O, k_P, k_Q, k_R, k_S, k_T, k_U, k_V, k_W, k_X, k_Y, k_Z,
    k_Num_0, k_Num_1, k_Num_2, k_Num_3, k_Num_4, k_Num_5, k_Num_6, k_Num_7, k_Num_8, k_Num_9,
    k_Keypad_0, k_Keypad_1, k_Keypad_2, k_Keypad_3, k_Keypad_4, k_Keypad_5, k_Keypad_6, k_Keypad_7, k_Keypad_8, k_Keypad_9,
    k_Space, k_Ctrl_L, k_Esc,
    k_LMB, k_MMB, k_RMB,
    k_Win_Close,
    k_Unknown
};

enum class KeyState { k_KS_Null = -1, k_KS_Press, k_KS_Hold, k_KS_Release };

struct ColData
{
public:
    ColData(Point coloVector)
        :m_colVector(coloVector)
    {}

public:
    Point m_colVector;
};

struct Quad2D
{
public:
    Quad2D()
    {
        p[0].x = 0;
        p[0].y = 0;

        p[1].x = 0;
        p[1].y = 0;

        p[2].x = 0;
        p[2].y = 0;

        p[3].x = 0;
        p[3].y = 0;
    }
    ~Quad2D() {}

public:
    Point p[4];
};

struct Quad3D
{
public:
    Quad3D()
    {
        p[0].x = 0;
        p[0].y = 0;
        p[0].z = 0;

        p[1].x = 0;
        p[1].y = 0;
        p[1].z = 0;

        p[2].x = 0;
        p[2].y = 0;
        p[2].z = 0;

        p[3].x = 0;
        p[3].y = 0;
        p[3].z = 0;
    }
    ~Quad3D() {}

public:
    Point3D p[4];
};

enum class SoundType { k_Sound, k_Music };

enum class BasicShape { k_Rect, k_Circle };