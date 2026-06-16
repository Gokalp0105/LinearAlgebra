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

    // Copy constructor: performs a deep copy of another Vec.
    // Allocates new memory and copies each component individually,
    // ensuring the two Vec objects do not share the same data pointer.
    Vec(const Vec& other)
    {
        dim = other.dim;
        data = new float[dim];
        for (int i = 0; i < dim; i++)
            data[i] = other.data[i];
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

    // Assignment operator: deep copy.
    // Frees existing memory before allocating new storage,
    // then copies each component from the source vector.
    // Self-assignment is checked and handled safely.
    Vec& operator=(const Vec& other)
    {
        if (this == &other) return *this;
        delete[] data;
        dim = other.dim;
        data = new float[dim];
        for (int i = 0; i < dim; i++)
            data[i] = other.data[i];
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

    // Scalar multiplication: v * s
    // Returns a new vector where each component is multiplied by the scalar.
    // The original vector is not modified.
    Vec operator*(const float& scalar)
    {
        Vec newVec(dim);
        for (int i = 0; i < dim; i++)
        {
            newVec.data[i] = scalar * data[i];
        }
        return newVec;
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

    // Destructor: frees the heap-allocated data array.
    // Called automatically when the Vec goes out of scope.
    ~Vec() { delete[] data; }
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

// ============================================================
// Matrix: column-major matrix struct
// Stores an array of Vec objects as columns.
// dim  -> number of rows (component count of each column vector)
// vecN -> number of columns (number of Vec objects)
// ============================================================
struct Matrix
{
    int dim = 0;        // Row count: dimensionality of each column vector
    int vecN = 0;       // Column count: number of column vectors
    Vec* data = nullptr; // Heap array of column vectors

    // Construct an empty matrix with n columns and the given row dimension.
    // All components are default-initialized (uninitialized floats).
    Matrix(int n, int dim)
    {
        this->dim = dim;
        vecN = n;
        data = new Vec[vecN];

        for (int i = 0; i < vecN; i++)
        {
            data[i] = Vec(dim);
        }
    }

    // Construct from an existing Vec array.
    // Checks that all vectors share the same dimension before accepting them.
    // Throws std::invalid_argument if dimensions are inconsistent.
    // NOTE: stores a pointer to the original array (no deep copy).
    Matrix(int n, Vec* vecs)
    {
        int dimcontrol = vecs[0].dim;
        bool dimCont = true;
        for (int i = 0; i < n; i++)
        {
            if (dimcontrol != vecs[i].dim)
            {
                dimCont = false;
            }
        }
        if (dimCont)
        {
            dim = vecs[0].dim;
            vecN = n;
            data = vecs;
        }
        else
        {
            throw std::invalid_argument("These vectors can't create a matrix. This is because they are not in the same dimension.");
        }
    }

    // Forced constructor: accepts vectors of different dimensions.
    // Pads shorter vectors with zeros to match the largest dimension,
    // then stores them as a valid matrix.
    // The 'forced' parameter is a disambiguation tag; its value is unused.
    Matrix(bool forced, int n, Vec* vecs)
    {
        // Find the largest dimension among all vectors
        int biggestDim = 0;
        for (int i = 0; i < n; i++)
        {
            if (biggestDim < vecs[i].dim)
            {
                biggestDim = vecs[i].dim;
            }
        }

        // Pad each shorter vector with zeros to reach biggestDim
        for (int i = 0; i < n; i++)
        {
            if (biggestDim > vecs[i].dim)
            {
                float* newData = new float[biggestDim];

                // Copy existing components
                for (int j = 0; j < vecs[i].dim; j++)
                {
                    newData[j] = vecs[i].data[j];
                }

                // Fill the remaining slots with zero
                for (int j = 0; j < biggestDim - vecs[i].dim; j++)
                {
                    newData[j + vecs[i].dim] = 0;
                }

                delete[] vecs[i].data;      // Free old memory
                vecs[i].dim = biggestDim;   // Update dimension
                vecs[i].data = newData;     // Assign padded array
            }
        }

        dim = biggestDim;
        vecN = n;
        data = vecs;
    }

    // Initializer list constructor: allows Matrix m = {v1, v2, v3} syntax.
    // Performs a deep copy of each Vec from the list into owned heap storage.
    // Prints a warning if column dimensions are inconsistent.
    Matrix(std::initializer_list<Vec> list)
    {
        vecN = list.size();
        data = new Vec[vecN];

        int i = 0;
        for (const Vec& v : list)
            data[i++] = v;  // Uses Vec's deep-copy assignment operator

        // Verify that all column vectors share the same dimension
        int dimcontrol = data[0].dim;
        bool dimCont = true;
        for (int j = 0; j < vecN; j++)
        {
            if (data[j].dim != dimcontrol)
            {
                dimCont = false;
                break;
            }
        }

        if (dimCont)
            dim = data[0].dim;
        else
            std::cout << "Boyutlar uyuşmuyor!" << std::endl;
    }

    // Destructor: frees the heap-allocated Vec array.
    // Each Vec's own destructor handles its internal data pointer.
    ~Matrix()
    {
        delete[] data;
    }

    // Matrix-vector multiplication: M * v
    // Computes a linear combination of the column vectors of M,
    // weighted by the corresponding components of v.
    // Result dimension: dim (row count of M).
    // Requires: vecN == v.dim (column count must equal vector length).
    Vec operator*(const Vec& other)
    {
        if (vecN != other.dim)
        {
            throw std::invalid_argument("Matrix's and vector's dimension is not equal.");
        }
        Vec newVec(dim);
        for (int i = 0; i < dim; i++)
        {
            float sum = 0;
            for (int j = 0; j < vecN; j++)
            {
                // Row i of column j, weighted by the j-th component of other
                sum += data[j].data[i] * other.data[j];
            }
            newVec.data[i] = sum;
        }
        return newVec;
    }

    // Matrix addition: A + B
    // Adds corresponding column vectors element by element.
    // Requires both matrices to have the same dimensions.
    // Throws std::invalid_argument if shapes don't match.
    Matrix operator+(const Matrix& other) const
    {
        if (vecN != other.vecN || dim != other.dim)
        {
            throw std::invalid_argument("Can't sum these matrices. Control dimension and column number for each matrix.");
        }
        Matrix newMat(vecN, dim);

        for (int i = 0; i < vecN; i++)
        {
            newMat.data[i] = data[i] + other.data[i];
        }
        return newMat;
    }

    // Scalar multiplication: M * s
    // Returns a new matrix where every component of every column is scaled by s.
    // The original matrix is not modified.
    Matrix operator*(const float& scalar) const
    {
        Matrix newMat(vecN, dim);

        for (int i = 0; i < vecN; i++)
        {
            newMat.data[i] = data[i] * scalar;
        }
        return newMat;
    }
};

int main()
{
    Vec v1 = { 1, 1, 1, 1 };
    Vec v2 = { 1, -1, -1, 1 };
    Vec v3 = { -1, -1, 1, 1 };
    Vec v4 = { 1, 1, -1, -1 };

    Matrix m1 = { v1, v2, v3, v4 };

    Vec v5 = { 3, 8, 9, 6 };

    std::cout << m1 * v5 << std::endl;

    return 0;
}
