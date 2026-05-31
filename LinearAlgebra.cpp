#include <iostream>
#include <cmath>
#include <initializer_list>
#include <ostream>

// ============================================================
// Vec: N-dimensional vector struct
// Uses dynamic heap memory; dimension is determined at runtime.
// Developed in parallel with Strang - Introduction to Linear Algebra.
// ============================================================

struct Vec
{
public:
    int dim;      // Number of components (dimensionality)
    float* data;  // Heap pointer holding the components

    // Construct with dimension only: components are uninitialized
    Vec(int dime)
    {
        dim = dime;
        data = new float[dim];
    }

    // Construct with dimension and an existing float array
    Vec(int dime, float* dat)
    {
        dim = dime;
        data = dat;
    }

    // Default constructor: empty vector
    Vec() : dim(0), data(nullptr) {}

    // Initializer list constructor: allows Vec v = {1.0f, 2.0f, 3.0f} syntax
    Vec(std::initializer_list<float> list)
    {
        dim = list.size();
        data = new float[dim];
        int i = 0;
        for (float val : list)
        {
            data[i++] = val;
        }
    }

    // Assignment operator: copies dim and data pointer (shallow copy).
    // NOTE: both Vec objects share the same memory after assignment.
    // Switch to deep copy once a destructor is added.
    Vec operator=(const Vec& other)
    {
        this->dim = other.dim;
        this->data = other.data;
        return *this;
    }

    // Dot product: v * w = v1*w1 + v2*w2 + ... + vn*wn
    // Geometric meaning: measures how aligned two vectors are.
    // Returns 0 for perpendicular vectors, maximum for parallel ones.
    float operator*(const Vec& other)
    {
        float sum = 0;
        for (int i = 0; i < dim; i++)
        {
            sum += data[i] * other.data[i];
        }
        return sum;
    }

    // Vector addition: v + w
    // If dimensions are equal, components are added pairwise.
    // If dimensions differ, the extra components of the larger
    // vector are carried over unchanged.
    Vec operator+(const Vec& other)
    {
        if (other.dim > dim)
        {
            float* newData = new float[other.dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = other.data[i] + data[i];
            }
            // Carry over remaining components from the larger vector
            for (int i = dim; i < other.dim; i++)
            {
                newData[i] = other.data[i];
            }
            Vec newVec(other.dim, newData);
            return newVec;
        }
        else if (other.dim == dim)
        {
            // Equal dimensions: add component by component
            float* newData = new float[dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = data[i] + other.data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
        else
        {
            float* newData = new float[dim];
            for (int i = 0; i < other.dim; i++)
            {
                newData[i] = data[i] + other.data[i];
            }
            // Carry over remaining components from this vector
            for (int i = other.dim; i < dim; i++)
            {
                newData[i] = data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
    }

    // Unary minus: returns a new vector with all components negated
    // Does not modify the original vector.
    Vec operator-()
    {
        float* newData = new float[dim];
        for (int i = 0; i < dim; i++)
        {
            newData[i] = -data[i];
        }
        return Vec(dim, newData);
    }

    // Vector subtraction: v - w
    // Same dimension logic as addition.
    // For the larger vector's remaining components:
    //   if other is larger -> negate (subtracting something that has no counterpart)
    //   if this is larger  -> keep as is (subtracting zero)
    Vec operator-(const Vec& other)
    {
        if (other.dim > dim)
        {
            float* newData = new float[other.dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = data[i] - other.data[i];
            }
            // No counterpart in this: treat as 0 - other[i]
            for (int i = dim; i < other.dim; i++)
            {
                newData[i] = -other.data[i];
            }
            Vec newVec(other.dim, newData);
            return newVec;
        }
        else if (other.dim == dim)
        {
            // Equal dimensions: subtract component by component
            float* newData = new float[dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = data[i] - other.data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
        else
        {
            float* newData = new float[dim];
            for (int i = 0; i < other.dim; i++)
            {
                newData[i] = data[i] - other.data[i];
            }
            // No counterpart in other: treat as this[i] - 0
            for (int i = other.dim; i < dim; i++)
            {
                newData[i] = data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
    }

    // Vector length (L2 norm): ||v|| = sqrt(v1^2 + v2^2 + ... + vn^2)
    // Generalization of the Pythagorean theorem to N dimensions.
    float length()
    {
        float sum = 0;
        for (int i = 0; i < dim; i++)
        {
            sum += data[i] * data[i];
        }
        return sqrt(sum);
    }

    // Cosine of the angle between two vectors
    // Formula: cos(theta) = (v . w) / (||v|| * ||w||)
    // Range: -1 to 1
    //   1  -> same direction (0 degrees)
    //   0  -> perpendicular (90 degrees)
    //  -1  -> opposite direction (180 degrees)
    float cos(Vec& other)
    {
        float dotprod = *this * other;
        float len = this->length() * other.length();
        return dotprod / len;
    }
};

// Overloads std::cout << v to print vector components in (x, y, z) format
std::ostream& operator<<(std::ostream& os, const Vec& v)
{
    os << "(";
    for (int i = 0; i < v.dim; i++)
    {
        os << v.data[i];
        if (i < v.dim - 1) os << ", ";
    }
    os << ")";
    return os;
}

int main()
{
    Vec v1 = { 1.0f, 2.0f };
    Vec v2 = { 2.0f, 3.0f };

    std::cout << "v1 = " << v1 << std::endl;
    std::cout << "v2 = " << v2 << std::endl;
    std::cout << "v1 + v2 = " << (v1 + v2) << std::endl;
    std::cout << "v1 - v2 = " << (v1 - v2) << std::endl;
    std::cout << "v1 * v2 = " << (v1 * v2) << std::endl;
    std::cout << "cos(v1, v2) = " << v1.cos(v2) << std::endl;

    return 0;
}