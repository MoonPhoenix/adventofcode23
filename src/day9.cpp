#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <array>

const bool part1 = false;
const bool part2 = !part1;

std::vector<long long> generate_sequence(std::string input){
    std::vector<long long> sequence;
    while(input.find(' ') != -1){
        sequence.push_back(std::stoll(input.substr(0,input.find_first_of(' '))));
        input = input.substr(input.find_first_of(' ') + 1);
    }
    sequence.push_back(std::stoll(input));
    return sequence;
}

long long generate_prediction(std::vector<long long> input_sequence){
    std::vector<long long> prediction_sequence;
    bool all_zeroes = true;
    for(int i = 1; i < input_sequence.size(); i++){
        long long num = input_sequence[i] - input_sequence[i - 1];
        if(all_zeroes && num != 0) all_zeroes = false;
        prediction_sequence.push_back(num);
    }
    if(all_zeroes){
        if(part1) return input_sequence.back();
        else return input_sequence[0];
    }else{
        if(part1){
            long long pred_number = generate_prediction(prediction_sequence);
            return pred_number + input_sequence.back();
        }else{
            long long pred_number = generate_prediction(prediction_sequence);
            return input_sequence[0] - pred_number;
        }
    }
}

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream input_file("../inputs/day9.txt");
    std::string line;
    long long result = 0;
    while(std::getline(input_file, line)){
        std::vector<long long> initial_sequence = generate_sequence(line);
        result += generate_prediction(initial_sequence);
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}