#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <vector>

using namespace std;

bool isIn(char item, string container) {
    for (int i = 0; i < container.length(); i++) {
        if (item == container[i]) {
            return true;
        }
    }
    return false;
}

vector<string> tokenize(string split_exp) {
    vector<string> token_list;
    int prev_index = 0;
    for (int i = 0; i < split_exp.length(); i++) {
        if (string(1, split_exp[i]) == " ") {
            token_list.push_back(split_exp.substr(prev_index, i - prev_index));
            prev_index = i + 1;
        }
    }
    token_list.push_back(split_exp.substr(prev_index, split_exp.length() - 1));
    return token_list;
}

vector<string> convert_exp_to_arr(string exp) {
    string ops = "+-*/^";
    string nums = "1234567890";
    string left_parentheses = "(";
    string right_parentheses = ")";
    string converting_string = "";
    for (int i = 0; i < exp.length(); i++) {
        if (isIn(exp[i], nums)) {
            converting_string += exp[i];
        } else if (isIn(exp[i], left_parentheses)) {
            converting_string += exp[i];
            converting_string += " ";
        } else if (isIn(exp[i], right_parentheses)) {
            converting_string += " ";
            converting_string += exp[i];
        } else if (isIn(exp[i], ops)) {
            converting_string += " ";
            converting_string += exp[i];
            converting_string += " ";
        }
    }
    return tokenize(converting_string);
}

double evaluate(double num1, double num2, string op) {
    if (op == "+") {
        return num1 + num2;
    } else if (op == "-") {
        return num1 - num2;
    } else if (op == "*") {
        return num1 * num2;
    } else if (op == "/") {
        return num1 / num2;
    } else if (op == "^") {
        return pow(num1, num2);
    }
    return 0;
}

int precedence(string op) {
    if (op == "+" or op == "-") {
        return 0;
    } else if (op == "*" or op == "/") {
        return 1;
    } else if (op == "^") {
        return 2;
    }
    return -1;
}

bool is_token_num(string token) {
    return ((int)token[0] >= 48 and (int)token[0] <= 57);
}

bool can_pop(string op1, string op2) {
    return (op2 != "(" and precedence(op1) <= precedence(op2));
}

void pop_and_evaluate(stack<float>* num_stack, stack<string>* op_stack) {
    float num1 = num_stack->top();
    num_stack->pop();
    float num2 = num_stack->top();
    num_stack->pop();
    string op = op_stack->top();
    op_stack->pop();
    num_stack->push(evaluate(num2, num1, op));
}

float calculate(string exp) {
    vector<string> tokenized_exp = convert_exp_to_arr(exp);
    stack<float> numbers;
    stack<string> operators;
    for (int i = 0; i < tokenized_exp.size(); i++) {
        if(is_token_num(tokenized_exp[i])) {
            numbers.push(stof(tokenized_exp[i]));
        } else if (tokenized_exp[i] == "(") {
            operators.push(tokenized_exp[i]);
        } else if (tokenized_exp[i] == ")") {
            while (operators.top() != "(") {
                float num1 = numbers.top();
                numbers.pop();
                float num2 = numbers.top();
                numbers.pop();
                string op = operators.top();
                operators.pop();
                numbers.push(evaluate(num2, num1, op));
            }
            operators.pop();
        } else {
            while (operators.size() > 0 and can_pop(tokenized_exp[i], operators.top())) {
                float num1 = numbers.top();
                numbers.pop();
                float num2 = numbers.top();
                numbers.pop();
                string op = operators.top();
                operators.pop();
                numbers.push(evaluate(num2, num1, op));
                //pop_and_evaluate(numbers, operators);
            }
            operators.push(tokenized_exp[i]);
        }
    }
    while(operators.size() > 0) {
        float num1 = numbers.top();
        numbers.pop();
        float num2 = numbers.top();
        numbers.pop();
        string op = operators.top();
        operators.pop();
        numbers.push(evaluate(num2, num1, op));
    }
    return numbers.top();
}

int main() {
    //prints out -6
    cout << calculate("(4*(1+2)/6)^3-(((5+1)/2)^2)*2") << endl;
    return 0;
}

