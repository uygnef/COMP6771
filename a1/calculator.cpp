
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>

std::ostream& operator<<(std::ostream &out,  const struct number& A);
int arithmetic(const std::string &input);
bool handle_number(const std::string &input);
void parse_all(std::ifstream &in, const std::string &s);
void parse_token (const std::string &line);
void repeat(const int &position, std::ifstream &in);


struct number{
    double value;
    bool is_int;
};
std::stack<struct number> stack;


int main(int argc, char* argv[]) {
    // setup the print out format for the precision required.
    std::cout.setf(std::ios::fixed,std::ios::floatfield);
    std::cout.precision(3);

    // open the file for reading
    std::ifstream in;
    in.open(argv[1]);

    // string to be read into
    std::string s;
    // read the file while we have input.
    while (in >> s) {
        parse_all(in, s);
    }
    in.close();
}

std::ostream& operator<<(std::ostream &out, const struct number& A){
        if (A.is_int){
                out << static_cast<int>(A.value);
        }else{
                out << A.value;
        }
        return out;
}

void parse_all(std::ifstream &in, const std::string &s){
    if(s == "repeat"){
        repeat(in.tellg(), in);
    }else{
        parse_token(s);
    }

}

void repeat(const int &position, std::ifstream &in) {

    const int times = static_cast<int>(stack.top().value);
    stack.pop();

    std::string s;
    for(auto i=0; i<times; i++){
        while(in >> s){
            if(s == "endrepeat"){
                if(i != times - 1){
                    in.clear();
                    in.seekg(position, in.beg);
                }
                break;
            }
            parse_all(in, s);
        }
    }

}


void parse_token(const std::string &input){

    //handle float
    if(handle_number(input)) {
        return;
    }

    if(input == "pop"){
        stack.pop();
        return;
    }

    if(input == "add" || input == "sub" || input == "mult" || input == "div" || input == "sqrt"){
        arithmetic(input);
        return;
    }

    if(input == "reverse"){
        const auto depth = static_cast<int>(stack.top().value);
        stack.pop();
        struct number queue[depth];
        for(auto i = 0; i < depth; i++){
            queue[i] = stack.top();
            stack.pop();
        }

        for(auto j: queue){
            stack.push(j);
        }
        return;
    }
    std::cerr << "Unrecognized symbols: " << input << std::endl;
    exit(-1);

}

bool handle_number(const std::string &input){

    if (isdigit(input[0]) == 1){
        struct number new_number{};
        //handle double
        if (input.find('.') != std::string::npos){
            new_number.is_int = false;
            new_number.value = std::stod(input);
        }else{
            //handle int
            new_number.value = std::stoi(input);
            new_number.is_int = true;
        }
        stack.push(new_number);
        return true;
    }
    return false;
}

int arithmetic(const std::string &input) {

    struct number x = stack.top();
    stack.pop();

    if(input == "sqrt"){
        if(x.value < 0){
            std::cerr << "ValueError: sqrt domin error\n";
            exit(-1);
        }
        struct number result{};
        result.is_int = x.is_int;
        result.value = std::sqrt(x.value);
        stack.push(result);
        std::cout << "sqrt " << x << " = " << result << std::endl;
        return 0;
    }

    struct number y = stack.top();
    stack.pop();
    struct number result{};
    result.is_int = x.is_int && y.is_int;
    std::cout << x;

    if(input == "add"){
        std::cout << " + ";
        result.value = x.value + y.value;
    }
    if(input == "sub"){
        std::cout << " - ";
        result.value = x.value - y.value;
    }
    if(input == "mult"){
        std::cout << " * ";
        result.value = x.value * y.value;
    }
    if(input == "div"){
        std::cout << " / ";
        result.value = x.value / y.value;
    }
    std::cout << y << " = " << result << std::endl;
    stack.push(result);

    return 0;
}
