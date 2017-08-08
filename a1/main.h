//
// Created by yu on 30/07/17.
//
void parse_token (std::string line);
bool handle_number(std::string input);
int arithmetic(std::string input);
void parse_all(std::ifstream &in, std::string s);
void repeat(int postion, std::ifstream &in);
void format_print(const struct number &num);

struct number{
    double value;
    bool is_int;
};
