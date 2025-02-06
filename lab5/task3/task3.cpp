#include <iostream>

class logical_values_array{
private:
    unsigned int value; // поле класса
public:
    logical_values_array(unsigned int num = 0) : value(num){} // конструктор, инициализирует value значением num

    unsigned int get_value() const{ // accessor
        return value;
    }

    static logical_values_array inversion(const logical_values_array &a){
        return logical_values_array(~(a.value));
    }

    static logical_values_array disjunction(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(a.value | b.value);
    }

    static logical_values_array conjunction(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(inversion(disjunction(inversion(a.value), inversion(b.value))));
    }

    static logical_values_array implication(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(disjunction(inversion(a.value), b.value));
    }

    static logical_values_array coimplication(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(inversion(disjunction(inversion(a.value), b.value)));
    }

    static logical_values_array XOR(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(disjunction(
                conjunction(a.value, inversion(b.value)),
                conjunction(inversion(a.value), b.value)));
    }

    static logical_values_array equivalence(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(inversion(XOR(a.value, b.value)));
    }

    static logical_values_array Pierce_arrow(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(inversion(disjunction(a.value, b.value)));
    }

    static logical_values_array Sheffer_stroke(const logical_values_array &a, const logical_values_array &b){
        return logical_values_array(inversion(conjunction(a.value, b.value)));
    }

    static bool equals(const logical_values_array &a, const logical_values_array &b){
        return a.value == b.value;
    }

    static bool get_bit(const logical_values_array &a, unsigned int bit){
        if(bit >= sizeof(unsigned int) * 8)
            throw std::logic_error("Bit is more than size of unsigned int number.\n");
        return (a.value >> bit) & 1; // сдвигаем бит в младшую позицию и выделяем его с помощью &1
    }

    static void convertation_to_str(char *str, const logical_values_array &a){
        if(str == nullptr)
            throw std::logic_error("Null pointer.\n");

        int i;
        const short num_with_bits = sizeof(unsigned int) * 8;

        for(i = 0; i < num_with_bits; ++i){
            if((a.value & (1 << i)) == 0)
                str[num_with_bits - i - 1] = '0';
            else
                str[num_with_bits - i - 1] = '1';
        }

        str[num_with_bits] = '\0';
    }
};

int main(){
    bool answ;
    logical_values_array v1(13), v2(11), v3;
    char conv[33];

    v3 = logical_values_array::inversion(v1);
    std::cout << "Inversion: " << "~" << v1.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::conjunction(v1, v2);
    std::cout << "Conjunction: " << v1.get_value() << " & " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::disjunction(v1, v2);
    std::cout << "Disjunction: " << v1.get_value() << " || " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::implication(v1, v2);
    std::cout << "Implication: " << v1.get_value() << " -> " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::coimplication(v1, v2);
    std::cout << "Coimplication: " << v1.get_value() << " ~-> " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::XOR(v1, v2);
    std::cout << "XOR: " << v1.get_value() << " + " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::equivalence(v1, v2);
    std::cout << "Equivalence: " << v1.get_value() << " <=> " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::Pierce_arrow(v1, v2);
    std::cout << "Pierce arrow: " << v1.get_value() << " | " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::Sheffer_stroke(v1, v2);
    std::cout << "Sheffer stroke: " << v1.get_value() << " / " << v2.get_value() << " = " << v3.get_value() << "\n";

    v3 = logical_values_array::equals(v1, v2);
    std::cout << "Are equal: " << v1.get_value() << " == " << v2.get_value() << " = " << v3.get_value() << "\n";

    try{
        answ = logical_values_array::get_bit(v1, 2);
        std::cout << "Bit by position: " << v1.get_value() << " position 2" << " = " << answ << "\n";
    }
    catch (std::logic_error &problem){
        std::cout << "Error. " << problem.what();
    }

    try{
        logical_values_array::convertation_to_str(conv, v1);
        std::cout << "Binary num um in str: " << v1.get_value() << " = " << conv << "\n";
    }
    catch (std::logic_error &problem){
        std::cout << "Error. " << problem.what();
    }

    return 0;
}