#include <iostream>
#include <cmath>

class complex_number{
private:
    double Re;
    double Im;
public:
    explicit complex_number(double real = 0.0, double imaginary = 0.0) : Re(real), Im(imaginary){}

    complex_number addition(const complex_number &b) const{
        return complex_number(Re + b.Re, Im + b.Im);
    }

    complex_number subtraction(const complex_number &b) const{
        return complex_number(Re - b.Re, Im - b.Im);
    }

    complex_number multiplication(const complex_number &b) const{
        return complex_number(Re * b.Re - Im * b.Im, Re * b.Im + Im * b.Re);
    }

    complex_number division(const complex_number &b) const{
        if(b.Re == 0.0 && b.Im == 0.0)
            throw std::logic_error("Division by 0.\n");

        double divider = b.Re * b.Re + b.Im * b.Im; // делитель
        return complex_number((Re * b.Re + Im * b.Im) / divider, (Im * b.Re - Re * b.Im) / divider);
    }

    double abs_complex() const{
        return sqrt(Re * Re + Im + Im);
    }

    double argument_complex() const{
        return atan(Im / Re);
    }

    void cout_complex() const{
        std::cout << Re << " + " << Im << "i";
    }
};

int main(){
    complex_number z1(1.0, 2.0);
    complex_number z2(3.0, 4.0);

    // сложение двух чисел
    std::cout << "Addition of 2 numbers: (";
    z1.cout_complex();
    std::cout << ") + (";
    z2.cout_complex();
    std::cout << ") = ";
    complex_number z3 = z1.addition(z2);
    z3.cout_complex();
    std::cout << '\n';

    // вычитание двух чисел
    std::cout << "Subtraction of 2 numbers: (";
    z1.cout_complex();
    std::cout << ") - (";
    z2.cout_complex();
    std::cout << ") = ";
    z3 = z1.subtraction(z2);
    z3.cout_complex();
    std::cout << '\n';

    // умножение двух чисел
    std::cout << "Multiplication of 2 numbers: (";
    z1.cout_complex();
    std::cout << ") * (";
    z2.cout_complex();
    std::cout << ") = ";
    z3 = z1.multiplication(z2);
    z3.cout_complex();
    std::cout << '\n';

    // деление двух чисел
    std::cout << "Division of 2 numbers: (";
    z1.cout_complex();
    std::cout << ") / (";
    z2.cout_complex();
    std::cout << ") = ";
    try {
        z3 = z1.division(z2);
        z3.cout_complex();
        std::cout << '\n';
    }
    catch (std::logic_error &problem){
        std::cout << "Error. "<< problem.what();
    }

    // проверка ошибки деления на 0
    complex_number z0;
    std::cout << "Division of 2 numbers: (";
    z1.cout_complex();
    std::cout << ") / (";
    z0.cout_complex();
    std::cout << ") = ";
    try {
        z3 = z1.division(z0);
        z3.cout_complex();
        std::cout << '\n';
    }
    catch (std::logic_error &problem){
        std::cout << "Error. "<< problem.what();
    }

    // модуль
    std::cout << "Module of number: |";
    z1.cout_complex();
    std::cout << "| = " << z1.abs_complex() << "\n";

    // аргумент(угол)
    std::cout << "Argument of number: arg(";
    z1.cout_complex();
    std::cout << ") = " << z1.argument_complex() << "\n";

    return 0;
}