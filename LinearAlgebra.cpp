#include <iostream>
#include <cmath>

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
        int smallestDim;
        Vec biggestVec;

        if (other.dim > dim)
        {
            smallestDim = dim;
            biggestVec = other;
        }
        else if (other.dim == dim)
        {
            // Equal dimensions: add component by component
            float* newData = new float[dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = other.data[i] + data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
        else
        {
            smallestDim = other.dim;
            biggestVec = *this;
        }

        // Different dimensions: add shared part, copy remainder from larger vec
        float* newData = new float[biggestVec.dim];
        for (int i = 0; i < smallestDim; i++)
        {
            newData[i] = other.data[i] + data[i];
        }
        for (int i = smallestDim; i < biggestVec.dim; i++)
        {
            newData[i] = biggestVec.data[i];
        }
        Vec newVec(biggestVec.dim, newData);
        return newVec;
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

int main()
{
    float arr[] = { 1.0f, 2.0f };
    float arr2[] = { 2.0f, 3.0f };

    Vec v1(2, arr);
    Vec v2(2, arr2);

    // (1,2) and (2,3) point in nearly the same direction, expect cos ~0.99
    std::cout << "cos(v1, v2) = " << v1.cos(v2) << std::endl;

    return 0;
}