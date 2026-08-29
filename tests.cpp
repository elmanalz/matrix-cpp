

#include "matrix.h"
#include <initializer_list>
#include <string>

static int checks_run = 0;
static int checks_failed = 0;


template <typename Func>
static void check_expr(Func condition, const std::string& name)
{
    ++checks_run;
    try {
        if (condition()) {
            std::cout << "  pass   " << name << '\n';
        } else {
            ++checks_failed;
            std::cout << "  FAIL   " << name << '\n';
        }
    } catch (const std::exception& e) {
        ++checks_failed;
        std::cout << "  FAIL   " << name << "  (threw: " << e.what() << ")\n";
    } catch (...) {
        ++checks_failed;
        std::cout << "  FAIL   " << name << "  (threw an unknown exception)\n";
    }
}


#define CHECK(expr, msg) check_expr([&] { return static_cast<bool>(expr); }, msg)


template <typename Exception, typename Func>
static void check_throws(Func f, const std::string& name)
{
    ++checks_run;
    try {
        f();
        ++checks_failed;
        std::cout << "  FAIL   " << name << "  (nothing was thrown)\n";
    } catch (const Exception&) {
        std::cout << "  pass   " << name << '\n';
    } catch (...) {
        ++checks_failed;
        std::cout << "  FAIL   " << name << "  (wrong exception type)\n";
    }
}

static void section(const std::string& title)
{
    std::cout << "\n[" << title << "]\n";
}



static Matrix make(size_t r, size_t c, std::initializer_list<double> vals)
{
    if (vals.size() != r * c)
        throw std::invalid_argument("make(): wrong number of values");
    Matrix m(r, c);
    size_t i = 0;
    for (double v : vals) {
        m.at(i / c, i % c) = v;
        ++i;
    }
    return m;
}


static Matrix identity(size_t n)
{
    Matrix I(n, n);
    for (size_t i = 0; i < n; ++i)
        I.at(i, i) = 1.0;
    return I;
}




int main()
{
    std::cout << "Running Matrix tests\n";

    
    try {

    // -----------------------------------------------------------------------
    section("construction");

    Matrix z(2, 3);
    CHECK(z.rows() == 2, "constructor sets rows()");
    CHECK(z.cols() == 3, "constructor sets cols()");

    bool all_zero = true;
    for (size_t i = 0; i < z.rows(); ++i)
        for (size_t j = 0; j < z.cols(); ++j)
            if (z.at(i, j) != 0.0) all_zero = false;
    CHECK(all_zero, "new matrix is zero-filled");

    // -----------------------------------------------------------------------
    section("at()");

    Matrix a = make(2, 3, {1, 2, 3,
                           4, 5, 6});
    CHECK(a.at(0, 0) == 1 && a.at(1, 2) == 6, "at() reads what was written");

    a.at(1, 1) = 99.0;
    CHECK(a.at(1, 1) == 99.0, "at() returns a usable reference");
    a.at(1, 1) = 5.0;  

    
    check_throws<std::out_of_range>([&] { a.at(2, 0); }, "at() rejects row out of range");
    check_throws<std::out_of_range>([&] { a.at(0, 3); }, "at() rejects column out of range");

    
    check_throws<std::out_of_range>([&] { a.at(static_cast<size_t>(-1), 0); },
                                    "at() rejects wrapped negative index");

    // -----------------------------------------------------------------------
    section("addition and subtraction");

    Matrix b = make(2, 3, {10, 20, 30,
                           40, 50, 60});

    CHECK(a + b == make(2, 3, {11, 22, 33, 44, 55, 66}), "A + B is elementwise");
    CHECK(b - a == make(2, 3, { 9, 18, 27, 36, 45, 54}), "A - B is elementwise");

    
    CHECK((a + b) - b == a, "(A + B) - B == A");

    Matrix wrong(3, 2);
    check_throws<std::invalid_argument>([&] { a + wrong; }, "A + B rejects shape mismatch");
    check_throws<std::invalid_argument>([&] { a - wrong; }, "A - B rejects shape mismatch");

    
    section("multiplication");

    Matrix p = make(2, 3, {1, 2, 3,
                           4, 5, 6});
    Matrix q = make(3, 2, {7,  8,
                           9, 10,
                           11, 12});
                           
    CHECK(p * q == make(2, 2, {58, 64, 139, 154}), "A * B against a hand-computed result");
    CHECK((p * q).rows() == 2 && (p * q).cols() == 2, "A(2x3) * B(3x2) has shape 2x2");

    CHECK(p * identity(3) == p, "A * I == A");
    CHECK(identity(2) * p == p, "I * A == A");

    check_throws<std::invalid_argument>([&] { p * make(2, 2, {1, 2, 3, 4}); },
                                        "A * B rejects inner-dimension mismatch");

                                        
    section("scalar multiplication");

    CHECK(p * 2.0 == make(2, 3, {2, 4, 6, 8, 10, 12}), "A * scalar scales every element");
    CHECK(2.0 * p == p * 2.0, "scalar * A == A * scalar (symmetry)");
    CHECK(p * 0.0 == Matrix(2, 3), "A * 0 is the zero matrix");

    
    section("transpose");

    
    Matrix t(1, 1);
    CHECK((t = p.transpose(), true), "transpose() runs without throwing");
    CHECK(t.rows() == p.cols() && t.cols() == p.rows(), "transpose swaps the shape");

    
    CHECK(([&] {
        for (size_t i = 0; i < p.rows(); ++i)
            for (size_t j = 0; j < p.cols(); ++j)
                if (t.at(j, i) != p.at(i, j)) return false;
        return true;
    }()), "transpose maps (i,j) -> (j,i) for every element");

    CHECK(p.transpose().transpose() == p, "transpose(transpose(A)) == A");

    
    CHECK((p * q).transpose() == q.transpose() * p.transpose(),
          "(A * B)^T == B^T * A^T");

          
    section("equality");

    CHECK(p == p, "A == A");
    CHECK(!(p == make(3, 2, {1, 2, 3, 4, 5, 6})), "matrices of different shape are not equal");
    CHECK(p != make(2, 3, {1, 2, 3, 4, 5, 7}), "one differing element makes them unequal");
    CHECK(!(p != p), "operator!= is consistent with operator==");

    
    CHECK(make(1, 1, {0.1 + 0.2}) == make(1, 1, {0.3}), "0.1 + 0.2 == 0.3 within tolerance");

    
    CHECK(make(1, 1, {1.0}) != make(1, 1, {1.5}), "tolerance does not hide a 0.5 difference");

    
    section("const-correctness");

    
    const Matrix cm = make(2, 3, {1, 2, 3,
                                  4, 5, 6});
    CHECK(cm.rows() == 2 && cm.cols() == 3, "rows()/cols() callable on a const Matrix");
    CHECK(cm.at(1, 2) == 6.0,               "at() callable on a const Matrix");
    CHECK(cm.transpose().rows() == 3,       "transpose() callable on a const Matrix");
    CHECK(cm + cm == cm * 2.0,              "operator+ and scalar* callable on a const Matrix");
    CHECK(cm - cm == Matrix(2, 3),          "operator- callable on a const Matrix");
    CHECK(cm * cm.transpose() == make(2, 2, {14, 32, 32, 77}),
                                            "operator* callable on a const Matrix");
    CHECK(cm == cm && !(cm != cm),          "comparisons callable on a const Matrix");

    
    section("value semantics (the Rule of Zero payoff)");

    Matrix original = make(2, 2, {1, 2, 3, 4});
    Matrix copy = original;        
    copy.at(0, 0) = 999.0;
    CHECK(original.at(0, 0) == 1.0, "copying makes a deep copy (original unchanged)");
    CHECK(copy.at(0, 0) == 999.0,   "the copy is independently mutable");

    Matrix assigned(1, 1);
    assigned = original;         
    CHECK(assigned == original,     "copy assignment produces an equal matrix");
    assigned.at(1, 1) = -7.0;
    CHECK(original.at(1, 1) == 4.0, "copy assignment is also a deep copy");

    } catch (const std::exception& e) {
        ++checks_failed;
        std::cout << "\n!! test run ABORTED by an unexpected exception: "
                  << e.what() << '\n';
    }

    
    std::cout << "\n----------------------------------------\n"
              << checks_run - checks_failed << " / " << checks_run << " checks passed\n";

    if (checks_failed == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << checks_failed << " CHECK(S) FAILED\n";
    return 1;      
}
