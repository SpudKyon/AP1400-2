#include "hw1.h"

Matrix algebra::zeros(size_t n, size_t m) {
    Matrix matrix;
    matrix = Matrix(n, vector<double>(m, 0));
    return matrix;
}

Matrix algebra::ones(size_t n, size_t m) {
    Matrix matrix;
    matrix = Matrix(n, vector<double>(m, 1));
    return matrix;
}

Matrix algebra::random(size_t n, size_t m, double min, double max) {
    if (min > max) {
        throw std::logic_error("min cannot be greater than max");
    }

    Matrix result = zeros(n, m);

    std::random_device dev;
    std::uniform_real_distribution<> dis(min, max);

    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++) {
            std::mt19937 rng(dev());
            result[i][j] = dis(rng);
        }
    }
    return result;
}

void algebra::show(const Matrix &matrix) {
    auto se = setprecision(3);
    for (const auto &row: matrix) {
        for (const auto &elem: row) {
            cout << elem;
        }
    }
}

Matrix algebra::multiply(const Matrix &matrix, double c) {
    Matrix new_matrix(matrix);
    for (auto &row: new_matrix) {
        for (auto &elem: row) {
            elem *= c;
        }
    }
    return new_matrix;
}

Matrix algebra::multiply(const Matrix &matrix1, const Matrix &matrix2) {
    if (matrix1.empty() || matrix2.empty()) {
        return Matrix{};
    }
    int n1 = matrix1.size(), n2 = matrix2.size();
    int m1 = matrix1[0].size(), m2 = matrix2[0].size();

    if (m1 != n2) {
        throw logic_error("shape of matrices is illegal.");
    }

    Matrix res = zeros(n1, m2);
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            for (int k = 0; k < m1; k++) {
                res[i][j] += (matrix1[i][k] * matrix2[k][j]);
            }
        }
    }
    return res;
}

Matrix algebra::sum(const Matrix &matrix, double c) {
    Matrix res = Matrix(matrix);
    for (auto &row: res) {
        for (auto &elem: row) {
            elem += c;
        }
    }
    return res;
}

Matrix algebra::sum(const Matrix &matrix1, const Matrix &matrix2) {
    if (matrix1.empty()) {
        if (matrix2.empty()) {
            return Matrix{};
        } else {
            throw logic_error("can't add empty matrix.");
        }
    }

    int n1 = matrix1.size(), m1 = matrix1.at(0).size();
    int n2 = matrix1.size(), m2 = matrix1.at(0).size();
    if (!(n1 == n2 && m1 == m2)) {
        throw logic_error("can't add with different shape.");
    }
    Matrix res = Matrix(matrix1);

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            res[i][j] += matrix2.at(i).at(j);
        }
    }
    return res;
}

Matrix algebra::transpose(const Matrix &matrix) {
    if (matrix.empty()) {
        return matrix;
    }
    int n = matrix.size();
    int m = matrix[0].size();

    Matrix res = zeros(m, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[j][i] += matrix[i][j];
        }
    }

    return res;
}

Matrix algebra::minor(const Matrix &matrix, size_t n, size_t m) {
    int row = matrix.size();
    int col = matrix[0].size();

    Matrix res = zeros(row - 1, col - 1);
    for (int i = 0; i < row; ++i) {
        if (i == n) {
            continue;
        }
        for (int j = 0; j < col; ++j) {
            if (j == m) {
                continue;
            }
            int idx = i;
            int jdx = j;
            if (i > n)
                idx = i - 1;
            if (j > m)
                jdx = j - 1;
            res[idx][jdx] = matrix[i][j];
        }
    }

    return res;
}

double algebra::determinant(const Matrix &matrix) {
    if (matrix.empty()) {
        return 1;
    }
    int row = matrix.size();
    int col = matrix[0].size();
    if (row != col) {
        throw logic_error("non-square matrices have no determinant.");
    }

    if (row == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
    }

    double res = 0.0;
    for (int i = 0; i < row; i++) {
        res += matrix[i][0] * determinant(minor(matrix, i, 0)) * pow(-1, i);
    }
    return res;
}

Matrix algebra::inverse(const Matrix &matrix) {
    if (matrix.empty())
        return matrix;

    if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("non-square matrices have no inverse");
    }
    double det = determinant(matrix);
    if (det == 0) {
        throw std::logic_error("singular matrices have no inverse");
    }

    auto result = matrix;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            result[i][j] = determinant(minor(matrix, j, i)) * pow(-1, i + j);
        }
    }

    return multiply(result, 1 / det);
}

Matrix algebra::concatenate(const Matrix &matrix1, const Matrix &matrix2, int axis) {
    int n1 = matrix1.size(), m1 = matrix1[0].size();
    int n2 = matrix2.size(), m2 = matrix2[0].size();

    if (axis == 0 && (m1 != m2) || axis == 1 && (n1 != n2)) {
        throw logic_error("matrices with wrong dimensions cannot be concatenated.");
    }
    Matrix res = Matrix(matrix1);
    if (axis == 0) {
        for (int i = 0; i < n2; ++i) {
            res.push_back(vector<double>(matrix2[i]));
        }
    } else {
        for (int i = 0; i < n1; ++i) {
            for (int j = 0; j < m2; ++j) {
                res[i].push_back(matrix2[i][j]);
            }
        }
    }
    return res;
}

Matrix algebra::ero_swap(const Matrix &matrix, size_t r1, size_t r2) {
    if (matrix.size() <= r1 || matrix.size() <= r2) {
        throw std::logic_error("r1 or r2 inputs are out of range.");
    }

    auto res = matrix;
    res[r1] = matrix[r2];
    res[r2] = matrix[r1];
    return res;
}

Matrix algebra::ero_multiply(const Matrix &matrix, size_t r, double c) {
    int n = matrix.size();
    if (n < r) {
        throw logic_error("r inputs is out of range.");
    }
    auto res = matrix;
    for (auto &num: res[r]) {
        num *= c;
    }
    return res;
}

Matrix algebra::ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2) {
    if (matrix.size() <= r1 || matrix.size() <= r2) {
        throw std::logic_error("r1 or r2 inputs are out of range.");
    }
    auto res = matrix;
    for (int i = 0; i < matrix[0].size(); ++i) {
        res[r2][i] += matrix[r1][i] * c;
    }
    return res;
}

Matrix algebra::upper_triangular(const Matrix &matrix) {
    if (matrix.empty()) {
        return matrix;
    }
    int n = matrix.size();
    int m = matrix[0].size();
    if (n != m) {
        throw logic_error("non-square matrices have no upper triangular form.");
    }
    auto res = matrix;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double tmp = res[j][i] / res[i][i];
            for (int k = i; k < n; ++k) {
                res[j][k] -= res[i][k] * tmp;
            }
        }
    }
    return res;
}
