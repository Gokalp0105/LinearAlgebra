#include <iostream>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <utility>

// ============================================================
// Vec: N-dimensional vector struct.
// Uses dynamic heap memory; dimension is determined at runtime.
// Developed in parallel with Strang - Introduction to Linear Algebra.
// ============================================================

struct Vec
{
public:
    int dim;     // Number of components (dimensionality).
    float* data; // Heap-allocated array holding the components.

    // Construct with dimension only; components are left uninitialized.
    Vec(int dime)
    {
        dim = dime;
        data = new float[dim];
    }

    // Construct from a dimension and an existing float pointer.
    // Takes ownership of 'dat'; do not free it externally after this call.
    Vec(int dime, float* dat)
    {
        dim = dime;
        data = dat;
    }

    // Copy constructor: allocates independent memory and deep-copies
    // every component from 'other', so the two objects never share data.
    Vec(const Vec& other)
    {
        dim = other.dim;
        data = new float[dim];
        for (int i = 0; i < dim; i++)
            data[i] = other.data[i];
    }

    // Default constructor: zero-dimensional vector with a null data pointer.
    Vec() : dim(0), data(nullptr) {}

    // Initializer list constructor: supports Vec v = {1.0f, 2.0f, 3.0f} syntax.
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

    // Copy assignment: frees existing memory, then deep-copies from 'other'.
    // Self-assignment is detected and short-circuited before any deallocation.
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

    // Dot product: v · w = v1*w1 + v2*w2 + ... + vn*wn.
    // Geometric interpretation: v · w = ||v|| ||w|| cos(θ).
    // Returns 0 for perpendicular vectors, maximum magnitude for parallel ones.
    float operator*(const Vec& other)
    {
        float sum = 0;
        for (int i = 0; i < dim; i++)
        {
            sum += data[i] * other.data[i];
        }
        return sum;
    }

    // Scalar multiplication: returns a new vector with each component scaled by 'scalar'.
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

    // Vector addition. When dimensions differ the shorter operand is treated as
    // if zero-padded; excess components of the longer operand are copied unchanged.
    Vec operator+(const Vec& other)
    {
        if (other.dim > dim)
        {
            float* newData = new float[other.dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = other.data[i] + data[i];
            }
            // 'other' has no counterpart in this; carry its components as-is.
            for (int i = dim; i < other.dim; i++)
            {
                newData[i] = other.data[i];
            }
            Vec newVec(other.dim, newData);
            return newVec;
        }
        else if (other.dim == dim)
        {
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
            // 'this' has no counterpart in other; carry its components as-is.
            for (int i = other.dim; i < dim; i++)
            {
                newData[i] = data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
    }

    // Unary negation: returns a new vector with every component sign-flipped.
    // The original vector is not modified.
    Vec operator-()
    {
        float* newData = new float[dim];
        for (int i = 0; i < dim; i++)
        {
            newData[i] = -data[i];
        }
        return Vec(dim, newData);
    }

    // Vector subtraction. Same dimension-mismatch logic as operator+:
    // excess components of 'other' become (0 - other[i]),
    // excess components of 'this' become (this[i] - 0).
    Vec operator-(const Vec& other)
    {
        if (other.dim > dim)
        {
            float* newData = new float[other.dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = data[i] - other.data[i];
            }
            // No counterpart in this; treat missing entries as 0 - other[i].
            for (int i = dim; i < other.dim; i++)
            {
                newData[i] = -other.data[i];
            }
            Vec newVec(other.dim, newData);
            return newVec;
        }
        else if (other.dim == dim)
        {
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
            // No counterpart in other; treat missing entries as this[i] - 0.
            for (int i = other.dim; i < dim; i++)
            {
                newData[i] = data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
    }

    // Euclidean length (L2 norm): sqrt(v1² + v2² + ... + vn²).
    float length()
    {
        float sum = 0;
        for (int i = 0; i < dim; i++)
        {
            sum += data[i] * data[i];
        }
        return sqrt(sum);
    }

    // Returns the cosine of the angle between this vector and 'other'.
    // Formula: cos(θ) = (v · w) / (||v|| * ||w||).
    // Output range: [-1, 1]. 1 → same direction, 0 → perpendicular, -1 → opposite.
    float cos(Vec& other)
    {
        float dotprod = *this * other;
        float len = this->length() * other.length();
        return dotprod / len;
    }

    // Destructor: releases the heap-allocated component array.
    ~Vec() { delete[] data; }
};

// Prints a Vec as (x, y, z, ...) to the given output stream.
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
// Matrix: column-major matrix struct.
// 'data' is an array of Vec objects, each representing one column.
// dim  = number of rows (length of each column vector).
// vecN = number of columns (number of Vec objects in data).
// ============================================================
struct Matrix
{
    int dim = 0;         // Row count: number of components in each column vector.
    int vecN = 0;        // Column count: number of column vectors stored in data.
    Vec* data = nullptr; // Heap-allocated array of column vectors.

    // Construct an uninitialized matrix with 'n' columns and 'dim' rows.
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

    // Construct from an existing Vec array; validates that all vectors share
    // the same dimension. Stores the raw pointer directly without deep-copying.
    // Throws std::invalid_argument if any dimension is inconsistent.
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

    // Forced constructor: accepts vectors of mixed dimensions by zero-padding
    // every shorter vector up to the largest dimension found in the array.
    // The 'forced' bool is a disambiguation tag; its value is not used.
    Matrix(bool forced, int n, Vec* vecs)
    {
        int biggestDim = 0;
        for (int i = 0; i < n; i++)
        {
            if (biggestDim < vecs[i].dim)
            {
                biggestDim = vecs[i].dim;
            }
        }

        for (int i = 0; i < n; i++)
        {
            if (biggestDim > vecs[i].dim)
            {
                float* newData = new float[biggestDim];

                for (int j = 0; j < vecs[i].dim; j++)
                {
                    newData[j] = vecs[i].data[j];
                }

                // Fill trailing slots with zero to reach biggestDim.
                for (int j = 0; j < biggestDim - vecs[i].dim; j++)
                {
                    newData[j + vecs[i].dim] = 0;
                }

                delete[] vecs[i].data;
                vecs[i].dim = biggestDim;
                vecs[i].data = newData;
            }
        }

        dim = biggestDim;
        vecN = n;
        data = vecs;
    }

    // Initializer list constructor: Matrix m = {v1, v2, v3}.
    // Deep-copies each Vec from the list into owned heap storage via Vec's
    // assignment operator. Prints a diagnostic if column dimensions differ.
    Matrix(std::initializer_list<Vec> list)
    {
        vecN = list.size();
        data = new Vec[vecN];

        int i = 0;
        for (const Vec& v : list)
            data[i++] = v;

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
    // Each Vec's own destructor handles its internal float* data.
    ~Matrix()
    {
        delete[] data;
    }

    // Matrix-vector multiplication: M * v.
    // Computes a linear combination of the columns of M weighted by v's components.
    // Result: a new Vec of length 'dim'. Requires vecN == v.dim.
    Vec operator*(const Vec& other) const
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
                sum += data[j].data[i] * other.data[j];
            }
            newVec.data[i] = sum;
        }
        return newVec;
    }

    // Matrix addition: A + B, performed column-by-column.
    // Both matrices must have identical shape (same dim and vecN).
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

    // Transpose: returns a new matrix where entry [row i][col j] of m
    // becomes entry [row j][col i] of the result.
    // The result is allocated with the same vecN and dim as m; for square
    // matrices this is equivalent to the true mathematical transpose shape.
    Matrix transpose(Matrix& m)
    {
        Matrix m2(m.vecN, m.dim);
        for (int i = 0; i < m.vecN; i++)
        {
            for (int j = 0; j < m.dim; j++)
            {
                m2.data[i].data[j] = m.data[j].data[i];
            }
        }
        return m2;
    }

    // Scalar multiplication: every component of every column is scaled by 'scalar'.
    // Returns a new matrix; the original is not modified.
    Matrix operator*(const float& scalar) const
    {
        Matrix newMat(vecN, dim);

        for (int i = 0; i < vecN; i++)
        {
            newMat.data[i] = data[i] * scalar;
        }
        return newMat;
    }

    // Matrix-matrix multiplication: A * B.
    // Each column of B is multiplied by A via operator*(Vec), producing
    // the corresponding column of the result.
    // Result dimensions: dim rows, other.vecN columns.
    Matrix operator*(const Matrix& other) const
    {
        Matrix newMat(other.vecN, dim);
        for (int i = 0; i < other.vecN; i++)
        {
            newMat.data[i] = (*this) * other.data[i];
        }
        return newMat;
    }

    // Reduces m to upper triangular form via Gaussian elimination.
    // Internally transposes m to iterate over rows directly, applies forward
    // elimination column by column, then transposes the result back.
    // Partial pivoting: when the current pivot is near zero (< 1e-9),
    // the first row below it with a non-zero entry in that column is swapped in.
    // Throws std::runtime_error if no valid pivot exists (singular matrix).
    Matrix upperTriangularMatrix(Matrix& m)
    {
        Matrix t = transpose(m);
        for (int i = 0; i < vecN; i++)
        {
            if (std::abs(t.data[i].data[i]) < 1e-9f)
            {
                int swapRow = -1;
                for (int k = i + 1; k < vecN; k++)
                {
                    if (std::abs(t.data[k].data[i]) > 1e-9f)
                    {
                        swapRow = k;
                        break;
                    }
                }
                if (swapRow == -1)
                    throw std::runtime_error("Matrix is singular");
                std::swap(t.data[i], t.data[swapRow]);
            }
            for (int j = i + 1; j < dim; j++)
            {
                float multiplier = t.data[j].data[i] / t.data[i].data[i];
                t.data[j] = t.data[j] - t.data[i] * multiplier;
            }
        }
        return t.transpose(t);
    }

    // Overload of upperTriangularMatrix that simultaneously tracks row operations
    // on the right-hand side vector 'equals', keeping the augmented system
    // [m | equals] consistent throughout elimination.
    // Partial pivoting: every row swap is mirrored in 'equals'.
    // Throws std::runtime_error if the matrix is singular.
    Matrix upperTriangularMatrix(Matrix& m, Vec& equals)
    {
        Matrix t = transpose(m);
        for (int i = 0; i < vecN; i++)
        {
            if (std::abs(t.data[i].data[i]) < 1e-9f)
            {
                int swapRow = -1;
                for (int k = i + 1; k < vecN; k++)
                {
                    if (std::abs(t.data[k].data[i]) > 1e-9f)
                    {
                        swapRow = k;
                        break;
                    }
                }
                if (swapRow == -1)
                    throw std::runtime_error("Matrix is singular");
                std::swap(t.data[i], t.data[swapRow]);
                std::swap(equals.data[i], equals.data[swapRow]); // Mirror row swap in the rhs.
            }
            for (int j = i + 1; j < dim; j++)
            {
                float multiplier = t.data[j].data[i] / t.data[i].data[i];
                t.data[j] = t.data[j] - t.data[i] * multiplier;
                equals.data[j] -= multiplier * equals.data[i]; // Apply the same operation to the rhs.
            }
        }
        return t.transpose(t);
    }

    // Solves the square linear system m * x = equals via Gaussian elimination
    // followed by back substitution.
    // 'equals' is taken by value so the caller's copy remains unmodified.
    // After reduction to upper triangular form, back substitution proceeds from
    // the last row upward, substituting already-solved unknowns into each equation.
    // Throws std::runtime_error if a zero pivot is encountered at any stage.
    Vec elimination(Matrix& m, Vec equals)
    {
        Matrix solution = upperTriangularMatrix(m, equals);
        Vec sl(solution.dim);

        if (std::abs(solution.data[solution.vecN - 1].data[solution.dim - 1]) < 1e-9f)
            throw std::runtime_error("Matrix is singular");
        sl.data[solution.dim - 1] = equals.data[solution.dim - 1] / solution.data[solution.vecN - 1].data[solution.dim - 1];

        for (int i = solution.vecN - 1; i >= 0; i--)
        {
            float sum = equals.data[i];
            for (int j = i + 1; j < solution.dim; j++)
            {
                sum -= sl.data[j] * solution.data[j].data[i]; // Subtract contributions of already-solved unknowns.
            }
            if (std::abs(solution.data[i].data[i]) < 1e-9f)
                throw std::runtime_error("Matrix is singular");
            sl.data[i] = sum / solution.data[i].data[i];
        }
        return sl;
    }
};

// Prints a Matrix row by row (one row per line), enclosed in parentheses.
std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    os << "(";
    for (int i = 0; i < m.vecN; i++)
    {
        for (int j = 0; j < m.dim; j++)
        {
            os << m.data[i].data[j];
            if (j < m.dim - 1) os << ", ";
        }
        std::cout << std::endl;
    }
    os << ")";
    return os;
}

int main()
{
    Vec v1 = { 1, 2, 5, 8 };
    Vec v2 = { 2, 6, 9, 12 };
    Vec v3 = { 4, 1, 3, 14 };
    Vec v4 = { 2, 7, 5, 21 };

    Matrix m1 = { v1, v2, v3, v4 };

    std::cout << m1 * m1;

    return 0;
}
