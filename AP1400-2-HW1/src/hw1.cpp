#include "hw1.h"
#include <algorithm>
#include <random>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <limits>
double epsilon = std::numeric_limits<double>::epsilon();

Matrix algebra::zeros(size_t n, size_t m) {
    Matrix matrix(n);
    auto generateZerosRow = [&m](){
        return std::vector<double>(m, 0.0);
    };
    std::generate(matrix.begin(), matrix.end(), generateZerosRow);
    return matrix;
    // Or simply using constructor
    // return Matrix(n, vector<double>(m, 0.0));
}

Matrix algebra::ones(size_t n, size_t m) {
    Matrix matrix(n);
    auto generateOnesRow = [&m](){
        return std::vector<double>(m, 1.0);
    };
    std::generate(matrix.begin(), matrix.end(), generateOnesRow);
    return matrix;
    // Or simply using constructor
    // return Matrix(n, vector<double>(m, 1.0));
}

Matrix algebra::random(size_t n, size_t m, double min, double max) {

    if (min > max) throw std::invalid_argument("min cannot be greater than max");
    
    Matrix matrix(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    auto generateRandomCol = [&](){
        std::vector<double> row(m);
        std::generate(row.begin(), row.end(), [&](){ return dis(gen); });
        return row;
    };
    std::generate(matrix.begin(), matrix.end(), generateRandomCol);
    return matrix;
}

void algebra::show(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (const double& element : row) {
            std::cout << std::fixed << std::setprecision(3) << element << " ";
        }
        std::cout << std::endl;
    }
}

Matrix algebra::multiply(const Matrix& matrix, double c) {
    Matrix newMatrix = matrix; // Copy one.
    /*
    auto multiplyByRow = [&c](auto& row) {
        auto multiplyByElement = [&c](auto & element) {
            return c * element;
        };
        std::transform(row.begin(), row.end(), row.begin(), multiplyByElement);
    };
    std::transform(newMatrix.begin(), newMatrix.end(), newMatrix.begin(), multiplyByRow);
    */
    
    /* Iterator Version */
    for (auto& row : newMatrix) {
        for (double& element : row) {
            element *= c;
        }
    }
    
    return newMatrix;
}

Matrix algebra::multiply(const Matrix& matrix1, const Matrix& matrix2) {
    
    if (matrix1.size() == 0 || matrix2.size() == 0) return Matrix{};

    const int n = matrix1.size();
    const int m = matrix2[0].size();

    if (matrix1[0].size() != matrix2.size()) throw std::logic_error("matrices with wrong dimensions cannot be multiplied");

    Matrix newMatrix = Matrix(n, std::vector<double>(m, 0.0));

    Matrix tranMatrix2 = algebra::transpose(matrix2);
    for (size_t i=0; i<n; ++i) {
        for (size_t j=0; j<m; ++j) {
            newMatrix[i][j] = std::inner_product(matrix1[i].begin(), matrix1[i].end(), tranMatrix2[j].begin(), 0.0);
        }
    }
    return newMatrix;
}

Matrix algebra::sum(const Matrix& matrix, double c) {
    if (matrix.size() == 0) return Matrix{};

    Matrix newMatrix = matrix;
    for (auto& row : newMatrix) {
        for (double& element : row) {
            element += c;
        }
    }

    return newMatrix;
}

Matrix algebra::sum(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.size() == 0 && matrix2.size() == 0) return Matrix{};
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) throw std::logic_error("matrices with wrong dimensions cannot be summed");

    Matrix newMatrix = matrix1;
    for (size_t i=0; i<matrix1.size(); i++) {
        for (size_t j=0; j<matrix1[0].size(); j++) {
            newMatrix[i][j] += matrix2[i][j];
        }
    }
    return newMatrix;
}

Matrix algebra::transpose(const Matrix& matrix) {

    if (matrix.size() == 0) return Matrix{};

    const int n = matrix.size();
    const int m = matrix[0].size();
    Matrix newMatrix;
    auto generateRow = [& matrix,  n](int index) {
        std::vector<double> row;
        for (size_t i=0; i<n; ++i) {
            row.push_back(matrix[i][index]);
        }
        return row;
    };
    for (size_t i=0; i<m; ++i) {
        newMatrix.push_back(generateRow(i));
    }
    return newMatrix;
}

Matrix algebra::minor(const Matrix& matrix, size_t n, size_t m) {
    if (matrix.size() == 0) return Matrix{};
    if (n > matrix.size() - 1 || m > matrix.size() - 1) throw std::logic_error("n, m cannot be larger than the matrix size");

    Matrix newMatrix = matrix;
    newMatrix.erase(newMatrix.begin() + n);
    std::for_each(newMatrix.begin(), newMatrix.end(), [m](auto& row){ row.erase(row.begin() + m); });
    return newMatrix;
}

double algebra::determinant(const Matrix& matrix) {
    if (matrix.size()==0) return 1.0;
    if (matrix.size() != matrix[0].size()) throw std::logic_error("only sqaure matrices have determinant");
    if (matrix.size()==1) return matrix[0][0];
    if (matrix.size()==2) return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];

    // choose row 0
    double determinant = 0;
    for (size_t i=0; i<matrix.size(); ++i) {
        determinant += matrix[0][i] * std::pow(-1, i) * algebra::determinant(algebra::minor(matrix, 0, i));
    }
    return determinant;
}

Matrix algebra::inverse(const Matrix& matrix) {
    if (matrix.size()==0) return Matrix{};
    if (matrix.size() != matrix[0].size()) throw std::logic_error("only sqaure matrices have inverse matrix");

    double determinant = algebra::determinant(matrix);
    if (std::abs(determinant - 0) < epsilon) throw std::logic_error("inverse matrix doesn't exist");

    if (matrix.size()==1) return matrix;
    // if (matrix.size() == 2) return algebra::multiply(Matrix{{matrix[1][1], -matrix[0][1]}, {-matrix[1][0], matrix[0][0]}}, 1 / determinant);
    Matrix adjugateMatrix = Matrix(matrix.size(), std::vector<double>(matrix.size(), 0.0));
    for (size_t i=0; i<matrix.size(); ++i) {
        for (size_t j=0; j<matrix.size(); ++j) {
            adjugateMatrix[i][j] = std::pow(-1, i+j) * algebra::determinant(algebra::minor(matrix, j, i));
        }
    }

    return algebra::multiply(adjugateMatrix, 1 / determinant);
}

Matrix algebra::concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
    if (matrix1.size() == 0 && matrix2.size() == 0) return Matrix{};
    Matrix newMatrix;
    if (axis == 0) {
        if (matrix1[0].size() != matrix2[0].size()) throw std::logic_error("m of two matrices must be the same");
        std::copy(matrix1.begin(), matrix1.end(), std::back_inserter(newMatrix));
        std::copy(matrix2.begin(), matrix2.end(), std::back_inserter(newMatrix));
    } else if (axis == 1) {
        if (matrix1.size() != matrix2.size()) throw std::logic_error("n of two matrices must be the same");
        for (size_t i=0; i<matrix1.size(); ++i) {
            std::vector<double> row;
            std::copy((matrix1.begin() + i)->begin(), (matrix1.begin() + i)->end(), std::back_inserter(row));
            std::copy((matrix2.begin() + i)->begin(), (matrix2.begin() + i)->end(), std::back_inserter(row));
            newMatrix.push_back(row);
        }
    } else {
        throw std::logic_error("axis value can only be 0 or 1.");
    }
    return newMatrix;
}

Matrix algebra::ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
    if (matrix.size() == 0) return Matrix{};
    if (r1 > matrix.size() - 1 || r2 > matrix.size() - 1|| r1 < 0 || r2 < 0) throw std::logic_error("illegal value of r1, r2");

    Matrix newMatrix = matrix;
    std::swap(newMatrix[r1], newMatrix[r2]);
    return newMatrix;
}

Matrix algebra::ero_multiply(const Matrix& matrix, size_t r, double c) {
    if (matrix.size() == 0) return Matrix{};
    if (r < 0 || r > matrix.size() - 1) throw std::logic_error("illegal value of r");
    Matrix newMatrix = matrix;
    std::transform((matrix.begin() + r)->begin(), (matrix.begin() + r)->end(), (newMatrix.begin() + r)->begin(), [c](double element) { return c * element; });
    return newMatrix;
}

Matrix algebra::ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
    if (matrix.size() == 0) return Matrix{};
    if (r1 > matrix.size() - 1 || r2 > matrix.size() - 1|| r1 < 0 || r2 < 0) throw std::logic_error("illegal value of r1, r2");
    Matrix newMatrix = matrix;
    for (size_t i=0; i<matrix[0].size(); ++i) {
        newMatrix[r2][i] += c * newMatrix[r1][i];
    }
    return newMatrix;
}

Matrix algebra::upper_triangular(const Matrix& matrix) {
    if (matrix.size() == 0) return Matrix{};
    if (matrix.size() < matrix[0].size()) throw std::logic_error("n should not be smaller than m");
    if (matrix.size() == 1) return matrix;
    Matrix newMatrix = matrix;
    for (size_t i=0; i<newMatrix.size(); ++i) {
        if (newMatrix[i][i] == 0) {
            for (size_t j=i+1; j<newMatrix.size(); ++j) {
                if (newMatrix[j][i] != 0) {
                    newMatrix = algebra::ero_swap(newMatrix, i, j);
                    break;
                }
            }
            
        }

        for (size_t j=i+1; j<newMatrix.size(); ++j) {
            double c = - newMatrix[j][i] / newMatrix[i][i];
            newMatrix = algebra::ero_sum(newMatrix, i, c, j);
        }

    }
    return newMatrix;
}