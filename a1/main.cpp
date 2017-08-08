#include <fstream>
#include <iostream>
#include <stack>
#include <cmath>


#include "main.h"

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



void parse_all(std::ifstream &in, std::string s){
    if(s == "repeat"){
        repeat(in.tellg(), in);
    }else{
        parse_token(s);
    }

}

void repeat(int position, std::ifstream &in) {

    const int times = int(stack.top().value);
    stack.pop();

    std::string s;
    for(auto i=1; i<times; i++){
       // std::cout<<i<<std::endl;
        while(in >> s){
            if(s == "endrepeat"){
                in.clear();
                in.seekg(position, in.beg);
                break;
            }
            parse_all(in, s);
        }
    }

}


void parse_token(std::string input){

    //handle float

    if(handle_number(input)) {
        return;
    }


    if(input == "pop"){
        stack.pop();
    }

    if(input == "add" || input == "sub" || input == "mult" || input == "div" || input == "sqrt"){
        arithmetic(input);
    }


    if(input == "reverse"){
        auto depth = int(stack.top().value);
        stack.pop();
        struct number queue[depth];
        int i;
        for(i = 0; i < depth; i++){
            queue[i] = stack.top();
            stack.pop();
        }

        for(auto j: queue){
            stack.push(j);
        }
    }
}

bool handle_number(std::string input){

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

int arithmetic(std::string input) {

    struct number x = stack.top();
    stack.pop();

    if(input == "sqrt"){
        if(x.value < 0){
            return 0;
        }
        struct number result{};
        result.is_int = x.is_int;
        result.value = std::sqrt(x.value);
        stack.push(result);
        std::cout << "sqrt ";

        format_print(x);
        std::cout << " = ";

        format_print(result);
        std::cout << std::endl;
        return 0;
    }

    struct number y = stack.top();
    stack.pop();
    struct number result{};
    result.is_int = x.is_int && y.is_int;

    format_print(x);

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

    format_print(y);
    std::cout << " = ";
    format_print(result);
    std::cout << std::endl;

    stack.push(result);

    return 0;
}

void format_print(const struct number &num){
    if(num.is_int){
        std::cout << int(num.value);
    } else {
        std::cout << num.value;
    }
    return;
}
