#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <array>

enum hand_type{
    h_high_card,
    h_one_pair,
    h_two_pair,
    h_three_of_a_kind,
    h_full_house,
    h_four_of_a_kind,
    h_five_of_a_kind
};

const bool part1 = false;
const bool part2 = !part1; 

class Hand{
    public:
        Hand(std::string line){
            std::string hand_str = line.substr(0, line.find(' '));
            bid = std::stoll(line.substr(line.find(' ') + 1));
            joker_count = 0;
            for(int i = 0; i < hand_str.size(); i++){
                char c = hand_str[i];
                if(std::isdigit(c)){
                    cards[i] = ((int)c - 48);
                }else{
                    switch(c){
                        case 'T':
                            cards[i] = 10;
                            break;
                        case 'J': 
                            if(part1) cards[i] = 11;
                            else{
                                cards[i] = 1;
                                joker_count++;
                            } 
                            break;
                        case 'Q':
                            cards[i] = 12;
                            break;
                        case 'K':
                            cards[i] = 13;
                            break;
                        case 'A':
                            cards[i] = 14;
                            break;
                    }
                }
            }
            generate_value();
        }
        void generate_value(){
            if(part1){
                std::array<int, 13> h{};
                for(int i = 0; i < cards.size(); i++){
                    h[cards[i] - 2]++;
                }
                std::sort(h.begin(), h.end());
                switch(h[12]){
                    case 5: 
                        hand_value = h_five_of_a_kind;
                        break;
                    case 4:
                        hand_value = h_four_of_a_kind;
                        break;
                    case 3:
                        if(h[11] == 2) hand_value = h_full_house;
                        else hand_value = h_three_of_a_kind;
                        break;
                    case 2:
                        if(h[11] == 2) hand_value = h_two_pair;
                        else hand_value = h_one_pair;
                        break;
                    default:
                        hand_value = h_high_card;
                        break;
                }
            }else{
                std::array<int, 14> h{};
                for(int i = 0; i < cards.size(); i++){
                    if(cards[i] == 1) continue;
                    h[cards[i] - 1]++;
                }
                std::sort(h.begin(), h.end());
                switch(h[13]){
                    case 5: 
                        hand_value = h_five_of_a_kind;
                        break;
                    case 4:
                        if(joker_count == 1) hand_value = h_five_of_a_kind;
                        else hand_value = h_four_of_a_kind;
                        break;
                    case 3:
                        if(joker_count == 2) hand_value = h_five_of_a_kind;
                        else if(joker_count == 1) hand_value = h_four_of_a_kind;
                        else if(h[12] == 2) hand_value = h_full_house;
                        else hand_value = h_three_of_a_kind;
                        break;
                    case 2:
                        if(joker_count == 3) hand_value = h_five_of_a_kind;
                        else if(joker_count == 2) hand_value = h_four_of_a_kind;
                        else if(joker_count == 1){
                            if(h[12] == 2) hand_value = h_full_house;
                            else hand_value = h_three_of_a_kind;
                        } 
                        else{
                            if(h[12] == 2) hand_value = h_two_pair;
                            else hand_value = h_one_pair;
                        } 
                        break;
                    case 1:
                        if(joker_count == 4) hand_value = h_five_of_a_kind;
                        else if(joker_count == 3) hand_value = h_four_of_a_kind;
                        else if(joker_count == 2) hand_value = h_three_of_a_kind;
                        else if(joker_count == 1) hand_value = h_one_pair;
                        else hand_value = h_high_card;
                        break;
                    case 0:
                        hand_value = h_five_of_a_kind; //Five jokers
                        break;
                }
            }
        }
        long long get_bid(){
            return bid;
        }
        int get_value(){
            return hand_value;
        }
        std::array<int, 5> get_cards(){
            return cards;
        }
    private:
        std::array<int, 5> cards;
        hand_type hand_value;
        int joker_count;
        long long bid;
};

bool compare_hands(Hand* a, Hand* b){
    if(a->get_value() == b->get_value()){
        std::array<int, 5> a_cards = a->get_cards();
        std::array<int, 5> b_cards = b->get_cards();
        for(int i = 0; i < a_cards.size(); i++){
            if(a_cards[i] == b_cards[i]) continue;
            return a_cards[i] < b_cards[i];
        }
    }
    else return a->get_value() < b->get_value();
}

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream input_file("../inputs/day7.txt");
    std::string line;
    std::vector<Hand*> hands;
    while(std::getline(input_file, line)){
        Hand* h = new Hand(line);
        hands.push_back(h);
    }
    std::sort(hands.begin(), hands.end(), compare_hands);
    long long result = 0;
    std::sort(hands.begin(), hands.end(), compare_hands);
    for(int i = 0; i < hands.size(); i++){
        result += hands.at(i)->get_bid() * (i + 1);
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}