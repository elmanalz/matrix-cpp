#include "matrix.h"

#include <iomanip>
#include <iostream>

// Print a labelled matrix with a blank line after it.
static void show(const std::string& label, const Matrix& m)
{
    std::cout << label << " (" << m.rows() << "x" << m.cols() << ")\n" << m << '\n';
}

// Print a section heading so the output is skimmable.
static void heading(const std::string& title)
{
    std::cout << "== " << title << " ==\n\n";
}

// Fill a matrix from a list of values, row-major.
static Matrix make(size_t rows, size_t cols, std::initializer_list<double> values)
{
    if (values.size() != rows * cols)
        throw std::invalid_argument("make(): wrong number of values");

    Matrix m(rows, cols);
    size_t i = 0;
    for (double v : values) {
        m.at(i / cols, i % cols) = v;
        ++i;
    }
    return m;
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    heading("Construction");

    Matrix zero(2, 3);
    show("A new matrix is zero-filled", zero);

    Matrix a = make(2, 3, { 1, 2, 3,
                            4, 5, 6 });
    Matrix b = make(2, 3, { 10, 20, 30,
                            40, 50, 60 });
    show("A", a);
    show("B", b);

    
    heading("Element access");

    std::cout << "A.at(1, 2) = " << a.at(1, 2) << '\n';
    Matrix c = a;          
    c.at(0, 0) = 99.0;
    std::cout << "after c = A; c.at(0,0) = 99:  A.at(0,0) is still "
              << a.at(0, 0) << "\n\n";

              

    heading("Addition and subtraction");

    show("A + B", a + b);
    show("B - A", b - a);

    

    heading("Scalar multiplication");

    show("A * 2", a * 2.0);
    show("0.5 * A", 0.5 * a);    

    
    heading("Matrix multiplication");

    Matrix p = make(2, 3, { 1, 2, 3,
                            4, 5, 6 });
    Matrix q = make(3, 2, { 7,  8,
                            9,  10,
                            11, 12 });
    show("P", p);
    show("Q", q);
    show("P * Q  (2x3 times 3x2 gives 2x2)", p * q);

    
    heading("Transpose");

    show("P transposed", p.transpose());

    
    std::cout << "(P * Q)^T == Q^T * P^T ?  "
              << (((p * q).transpose() == q.transpose() * p.transpose()) ? "yes" : "no")
              << "\n\n";

              
    heading("Comparison");

    std::cout << "A == A               ?  " << (a == a ? "yes" : "no") << '\n';
    std::cout << "A == B               ?  " << (a == b ? "yes" : "no") << '\n';
    std::cout << "0.1 + 0.2 == 0.3     ?  "
              << ((make(1, 1, { 0.1 + 0.2 }) == make(1, 1, { 0.3 })) ? "yes" : "no")
              << "   (comparison is tolerance-based, not bitwise)\n\n";

              
    heading("Value semantics");

    
    Matrix original = make(2, 2, { 1, 2,
                                   3, 4 });
    Matrix copy = original;
    copy.at(0, 0) = -1.0;
    show("original (unchanged)", original);
    show("copy (independently modified)", copy);

    
    heading("Error handling");

    
    try {
        a.at(5, 0);
    } catch (const std::out_of_range& e) {
        std::cout << "A.at(5, 0)   -> " << e.what() << '\n';
    }

    try {
        Matrix wrong(3, 2);
        a + wrong;
    } catch (const std::invalid_argument& e) {
        std::cout << "A + (3x2)    -> " << e.what() << '\n';
    }

    try {
        Matrix wrong(2, 2);
        a * wrong;
    } catch (const std::invalid_argument& e) {
        std::cout << "A * (2x2)    -> " << e.what() << '\n';
    }

    std::cout << "\nDone.\n";
    return 0;
}
