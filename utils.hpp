
#ifndef UTILS_HPP
#define UTILS_HPP

/**
* @brief Row-major matrix class
* 
*/
class Matrix {
private:
    std::vector< int > data;
    size_t n_rows;
    size_t n_cols;
public:

    Matrix() = default;
    Matrix(size_t n_rows, size_t n_cols);
    int& operator()(size_t i, size_t j);
    const int& operator()(size_t i, size_t j) const;
    std::vector< int >& get_data();

    void print() const;
};

inline Matrix::Matrix(size_t n_rows, size_t n_cols) :
    n_rows(n_rows), n_cols(n_cols) 
{
    data.resize(n_rows * n_cols);
};

inline int& Matrix::operator()(size_t i, size_t j) {
    return data[i * n_cols + j];
};

inline const int& Matrix::operator()(size_t i, size_t j) const {
    return data[i * n_cols + j];
};

inline std::vector< int >& Matrix::get_data() {
    return data;
};

inline void Matrix::print() const {
    for (size_t i = 0; i < n_rows; ++i) {
        for (size_t j = 0; j < n_cols; ++j) {
            std::cout << (*this)(i, j) << " ";
        }
        std::cout << std::endl;
    }
};
#endif
