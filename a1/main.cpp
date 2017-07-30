#include <iostream>
#include <fstream>
#include "main.h"
#include <stack>

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
        std::cout<<s<<std::endl;
        parse_token(s);
    }
    in.close();
}



void parse_token(std::string input){

    //handle float

    if(!handle_number(input)){
        if((input.compare("add") == 0 || input.compare("sub") == 0
             || input.compare("mult") == 0|| input.compare("div") == 0)){
            arithmetic(input);
        }
    }



}

int handle_number(std::string input){

    if (isdigit(input[0])){
        struct number new_number;
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
        return 1;
    }
    return 0;
}

int arithmetic(std::string input) {

    struct number x,y,result;
    x = stack.top();
    stack.pop();
    y = stack.top();
    stack.pop();
    result.is_int = x.is_int && y.is_int;


    std::cout << (x.is_int ? std::to_string(int(x.value)) : std::to_string(x.value)) ;

    if(input.compare("add") == 0 ){
        std::cout << " + ";
        result.value = x.value + y.value;
    }
    if(input.compare("sub") == 0){
        std::cout << " - ";
        result.value = x.value - y.value;
    }
    if(input.compare("mult") == 0){
        std::cout << " * ";
        result.value = x.value * y.value;
    }
    if(input.compare("div") == 0){
        std::cout << " / ";
        result.value = x.value / y.value;
    }

    std::cout << (y.is_int ? std::to_string(int(y.value)) : std::to_string(y.value)) << " = "
              << (result.is_int ? std::to_string(int(result.value)) : std::to_string(result.value)) << std::endl;

    stack.push(result);

    return 0;
}