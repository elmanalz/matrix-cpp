#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>


class Matrix {
private:
    size_t rows_;           //Store the rows of the Matrix
    size_t cols_;           //Store the columns of the Matrix
    std::vector<double> data_;      //Store the Matrix in a rows_ * columns std::vector
public:

    //Equality check of two matrices with a custom tolerance
    bool approx_equal(const Matrix& other, double tol = 1e-10) const
    {
        //Checking to see if the dimensions match
        if(rows_ != other.rows_ || cols_ != other.cols_)
            return false;
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < cols_; j++)
                //Comparison of two values with a dynamic tolerance
                //If the difference between two values is above the dynamic tolerance,
                //  the matrices are considered different and not equal
                if(std::fabs(at(i, j) - other.at(i, j)) >= (tol * std::max(1.0, std::max(std::abs(at(i,j)), std::abs(other.at(i,j))))))
                        return false;
        return true;
    }

    //Default constructor, assigning values to the rows_, columns_, 
    //and resizing the std::vector and assigning a default value of 0.0 to all the elements
    Matrix(size_t rows, size_t cols):rows_(rows), cols_(cols), data_(rows* cols, 0.0) {}

    //Getter function for rows
    size_t rows() const {return rows_;}
    //Getter function for columns
    size_t cols() const {return cols_;}

    //Modifiable getter for element of a specified position
    double& at(size_t r, size_t c)  
    {
        if(r < rows_ && c < cols_)
            return data_.at(r*cols_ + c);
        else
            throw std::out_of_range("Row and Column Arguments are higher than the capacity.");
    }

    //Constant getter for element of a specified position
    double  at(size_t r, size_t c) const  
    {
        if(r < rows_ && c < cols_)
            return data_.at(r*cols_ + c);
        else
            throw std::out_of_range("Row and Column Arguments are higher than the capacity.");
    }

    //Transpose function, swapping the rows and columns a(i, j) = aT(j, i)
    Matrix transpose() const
    {
        Matrix new_(cols_, rows_);
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < cols_; j++)
                new_.at(j, i) = at(i, j);
            
        return new_;
    }

    //Overloading the + operator for matrix addition
    Matrix operator+(const Matrix& other) const
    {
        //Checking to see if dimensions maatch
        if(rows_ == other.rows_ && cols_ == other.cols_)
        {
            Matrix new_(rows_, cols_);
            for (size_t i = 0; i < rows_; i++)
                for (size_t j = 0; j < cols_; j++)
                    new_.at(i, j) = at(i, j) + other.at(i, j);
            return new_;
        }
        else
            //Throwing when dimensions don't match
            throw std::invalid_argument("Rows and Columns of the matrices do not match");
    }

    //Overloading * operator for matrix multiplication
    Matrix operator*(const Matrix& other) const
    {
        if(cols_ == other.rows_)
        {
            Matrix new_(rows_, other.cols_);
            for (size_t i = 0; i < rows_; i++)
                //c(i, j) = dot(a(i, any), b(any, j)) 
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
            //Throwing in the case where dimensions do not meet the requirement
            throw std::invalid_argument("Rows and Columns of the matrices do not match");
    }

    //Overloading * operator for scalar multiplication
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

    //Overloading - operator for decrement operator
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

    //Overloading == operator for equality of two matrices
    bool operator==(const Matrix& other) const
    {
        return approx_equal(other);
    }

    //Overloading != operator for inequality of two matrices
    bool operator!=(const Matrix& other) const {return !(*this == other);}
};


//Overloading << operator for printing the matrix
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


//Overloading * operator for scalar multiplication of matrices
//Note: the operator has already been overloaded, the purpose of this overloading is to allow displacement in scalar multiplication
inline Matrix operator*(double s, const Matrix& m)
{
    return m*s;
}
