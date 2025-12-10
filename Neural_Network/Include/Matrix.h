/**
 * @file Matrix.h
 * @brief Lightweight 2D matrix class for numerical computations.
 *
 * @defgroup Math Mathematical Utilities
 * @brief Core math classes and helpers for neural networks and game AI.
 *
 * Features:
 *  - Row-major contiguous storage
 *  - Element-wise and Hadamard operations
 *  - Transpose and matrix multiplication
 *  - Random initialization
 *  - Interoperability with neural networks
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <functional>
#include <iostream>
#include <stdexcept>

/**
 * @class Matrix
 * @brief Lightweight 2D numeric matrix.
 *
 * @ingroup Math
 *
 * Designed for neural network computations:
 *  - Efficient indexing via flattened row-major storage
 *  - Element-wise operations and arithmetic
 *  - No implicit broadcasting
 *  - Shape validation for all operations
 *
 * @tparam T Numeric type (float, double, int, etc.)
 */
template <class T>
class Matrix {
public:
    int rows;                ///< Number of rows
    int cols;                ///< Number of columns
    std::vector<T> data;     ///< Row-major flattened storage (size = rows*cols)

    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    /**
     * @brief Construct a matrix of size (rows x cols).
     * @param rows Number of rows
     * @param cols Number of columns
     * @param zero Initialize to zero if true; else initialize to one
     */
    Matrix(int rows = 0, int cols = 0, bool zero = true);

    /**
     * @brief Construct a matrix from a flat row-major vector.
     * @param values Vector of size rows*cols
     * @param rows Number of rows
     * @param cols Number of columns
     *
     * @throws std::runtime_error if values.size() != rows*cols
     */
    Matrix(const std::vector<T>& values, int rows, int cols);

    // ---------------------------------------------------------------------
    // Factories
    // ---------------------------------------------------------------------

    /**
     * @brief Create a matrix filled with uniform random values in [min, max].
     * @param rows Number of rows
     * @param cols Number of columns
     * @param min Minimum value
     * @param max Maximum value
     * @return Matrix with random values
     */
    static Matrix random(int rows, int cols, T min, T max);

    // ---------------------------------------------------------------------
    // Element Access
    // ---------------------------------------------------------------------

    /**
     * @brief Access element (r, c) by reference.
     * @warning No bounds check in release mode
     */
    inline T& operator()(int r, int c) { return data[r * cols + c]; }

    /**
     * @brief Read-only access to element (r, c)
     */
    inline const T& operator()(int r, int c) const { return data[r * cols + c]; }

    /**
     * @brief Raw pointer to underlying data
     */
    T* dataPtr() { return data.data(); }
    const T* dataPtr() const { return data.data(); }

    // ---------------------------------------------------------------------
    // Element-wise operations
    // ---------------------------------------------------------------------

    /**
     * @brief Apply function f(x) to each element in place.
     * @param func Mapping function
     */
    void apply(const std::function<T(T)>& func);

    /**
     * @brief In-place Hadamard (element-wise) product: this = this ⊙ other
     * @param other Other matrix
     * @throws std::runtime_error if dimensions mismatch
     */
    void hadamard(const Matrix& other);

    // ---------------------------------------------------------------------
    // Arithmetic operators
    // ---------------------------------------------------------------------

    /**
     * @brief Matrix addition
     * @param other Other matrix
     * @return Sum matrix
     * @throws std::runtime_error if dimensions mismatch
     */
    Matrix operator+(const Matrix& other) const;

    /**
     * @brief Matrix subtraction
     * @param other Other matrix
     * @return Difference matrix
     * @throws std::runtime_error if dimensions mismatch
     */
    Matrix operator-(const Matrix& other) const;

    /**
     * @brief Matrix multiplication: C = A * B
     * @param other Right-hand side matrix
     * @return Resulting matrix
     * @throws std::runtime_error if inner dimensions mismatch
     */
    Matrix operator*(const Matrix& other) const;

    /**
     * @brief Scalar multiplication
     * @param val Scalar value
     * @return Scaled matrix
     */
    Matrix operator*(int val) const;

    // ---------------------------------------------------------------------
    // Structural operations
    // ---------------------------------------------------------------------

    /**
     * @brief Transpose of the matrix
     * @return Transposed matrix
     */
    Matrix transpose() const;

    /**
     * @brief Resize matrix and set all elements to zero
     * @param r Rows
     * @param c Columns
     */
    void resize(int r, int c);

    /**
     * @brief Multiply preallocated matrices without allocations
     * @param a Left matrix
     * @param b Right matrix
     * @param result Preallocated matrix (a.rows × b.cols)
     */
    static void multiply(const Matrix& a, const Matrix& b, Matrix& result);

    // ---------------------------------------------------------------------
    // I/O helpers
    // ---------------------------------------------------------------------

    /**
     * @brief Stream output for debugging
     */
    template <typename J>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<J>& m);
};

#endif // MATRIX_H