#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>


class Matrix {
private:
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;
public:
    bool approx_equal(const Matrix& other, double tol = 1e-10) const
    {
        if(rows_ != other.rows_ || cols_ != other.cols_)
            return false;
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < cols_; j++)
                if(std::fabs(at(i, j) - other.at(i, j)) >= (tol * std::max(1.0, std::max(std::abs(at(i,j)), std::abs(other.at(i,j))))))
                        return false;
        return true;
    }
    Matrix(size_t rows, size_t cols):rows_(rows), cols_(cols), data_(rows* cols, 0.0) {}

    size_t rows() const {return rows_;}
    size_t cols() const {return cols_;}
    double& at(size_t r, size_t c)  
    {
        if(r < rows_ && c < cols_)
            return data_.at(r*cols_ + c);
        else
            throw std::out_of_range("Row and Column Arguments are higher than the capacity.");
    }
    double  at(size_t r, size_t c) const  
    {
        if(r < rows_ && c < cols_)
            return data_.at(r*cols_ + c);
        else
            throw std::out_of_range("Row and Column Arguments are higher than the capacity.");
    }
    Matrix transpose() const
    {
        Matrix new_(cols_, rows_);
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < cols_; j++)
                new_.at(j, i) = at(i, j);
            
        return new_;
    }
    Matrix operator+(const Matrix& other) const
    {
        if(rows_ == other.rows_ && cols_ == other.cols_)
        {
            Matrix new_(rows_, cols_);
            for (size_t i = 0; i < rows_; i++)
                for (size_t j = 0; j < cols_; j++)
                    new_.at(i, j) = at(i, j) + other.at(i, j);
            return new_;
        }
        else
            throw std::invalid_argument("Rows and Columns of the matrices do not match");
    }
    Matrix operator*(const Matrix& other) const
    {
        if(cols_ == other.rows_)
        {
            Matrix new_(rows_, other.cols_);
            for (size_t i = 0; i < rows_; i++)
                for (size_t j = 0; j < other.cols_; j++)
                {
                    double sum = 0.0;
                    for (size_t k = 0; k < cols_; k++)
                        sum += at(i, k) * other.at(k, j);
                    new_.at(i, j) = sum;
                }
    
            return new_;
        }
        else
            throw std::invalid_argument("Rows and Columns of the matrices do not match");
    }
    Matrix operator*(double mul) const
    {
        Matrix new_(rows_, cols_);
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < cols_; j++)
            {
                new_.at(i, j) = at(i, j) * mul;
            }

        return new_;
    }
    Matrix operator-(const Matrix& other) const
    {
        if(rows_ == other.rows_ && cols_ == other.cols_)
        {
            Matrix new_(rows_, cols_);
            for (size_t i = 0; i < rows_; i++)
                for (size_t j = 0; j < cols_; j++)
                    new_.at(i, j) = at(i, j) - other.at(i, j);
            return new_;
        }
        else
            throw std::invalid_argument("Rows and Columns of the matrices do not match");
    }
    bool operator==(const Matrix& other) const
    {
        return approx_equal(other);
    }

    bool operator!=(const Matrix& other) const {return !(*this == other);}
};

inline std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    for (size_t i = 0; i < m.rows(); i++)
    {
        for (size_t j = 0; j < m.cols(); j++)
            os<<m.at(i, j)<<' ';
        os<<'\n';
    }
    return os;
    
}

inline Matrix operator*(double s, const Matrix& m)
{
    return m*s;
}
