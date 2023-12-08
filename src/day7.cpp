#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

const bool part1 = false;
const bool part2 = !part1; 

class Hand{
    public:
        Hand(std::string line){
            std::string hand_str = line.substr(0, line.find(' '));
            bid = std::stoll(line.substr(line.find(' ') + 1));
        }
        int get_value(){
            return hand_value;
        }
    private:
        int cards[5];
        int hand_value;
        long long bid;
};

bool compare_hands(Hand* a, Hand* b){
    if(a->get_value() == b->get_value()){

    }
    else return a->get_value() < b->get_value();
}

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream input_file("../inputs/test_input.txt");
    std::string line;
    std::vector<Hand*> hands;
    while(std::getline(input_file, line)){
        Hand* h = new Hand(line);
        hands.push_back(h);
    }
    long long result;
    std::sort(hands.begin(), hands.end(), compare_hands);
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}