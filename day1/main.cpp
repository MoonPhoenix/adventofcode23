#include <fstream>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> input_vector;
std::string digits[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

void get_input(std::vector<std::string> &input, std::string filename){
    std::ifstream input_file(filename);
    std::string line;
    while(std::getline(input_file,line)){
        input.push_back(line);
    }
}

int eval_string(std::string input){
    int first = 0;
    bool first_found = false;
    int last = 0;
    for(int i = 0; i < input.length(); i++){
        if(isdigit(input[i])){
            if(!first_found){
                first_found = true;
                first = i;
            }
            last = i;
        }
    }
    int first_num = (int)(input[first] - 48);
    int last_num = (int)(input[last] - 48);
    std::string first_slice, second_slice;
    if(first_found){
        first_slice = input.substr(0,first);
        second_slice = input.substr(last);
    }
    else{
        first_slice = input;
        second_slice = input;
        first = input.size();
    }
    for(int i = 0; i < 9; i++){
        int index = first_slice.find(digits[i]);
        if(index < first && index >= 0){
            first_num = i + 1;
            first = index;
        }
    }
    last = 0;
    for(int i = 0; i < 9; i++){
        int index = second_slice.find(digits[i]);
        int final_index = 0;
        std::string fragment = second_slice;
        while(index >= 0){
            final_index += index;
            fragment = fragment.substr(index + 1);
            index = fragment.find(digits[i]);
        }
        if(final_index > last && final_index >= 0){
            last_num = i + 1;
            last = final_index;
        }
    }
    return (10 * first_num) + last_num;
}

int main(){
    get_input(input_vector, "./input_1.txt");
    int sum = 0;
    for(int i = 0; i < input_vector.size(); i++){
        int num = eval_string(input_vector[i]);
        if(num > 99 && num < 10){
            std::cout << "ERROR" << std::endl;
        }
        sum += num;
    }
    std::cout << sum << std::endl;
    return 0;
}