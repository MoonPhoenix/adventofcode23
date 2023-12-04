#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#define WINNING_NUMBER_COUNT 10
#define CARD_NUMBER_COUNT 25

#define PART1 false
#define PART2 !PART1

//Forgot to do some PART1/PART2 separation so won't work very well with PART1 needs something added.

int pow_2(int power){
    if(power == 0) return 1;
    int base = 2;
    for(int i = 1; i < power; i++){
        base *= 2;
    }
    return base;
}

class Card{
    public:
        Card(std::string input_line, int id, std::vector<Card*>* card_vector = nullptr){
            cards = card_vector;
            game_id = id;
            copy = 1;
            std::string snippet = "";
            std::string winning_number_str = "";
            std::string card_number_str = "";
            size_t snip_start, snip_end;
            snip_start = input_line.find_first_of(":") + 1;
            snip_end = input_line.find_first_of("|");
            winning_number_str = input_line.substr(snip_start, snip_end - snip_start);
            int found_count = 0;
            int i = 0;
            bool whitespace = true;
            while(found_count < WINNING_NUMBER_COUNT){
                char c = winning_number_str[i];
                if(c == ' '){
                    if(!whitespace){
                        winning_numbers[found_count] = std::stoi(snippet);
                        found_count++;
                        snippet.clear();
                        whitespace = true;
                    }
                }
                else{
                    if(whitespace){
                        whitespace = false;
                    }
                    snippet.push_back(c);
                }
                i++;
            }
            found_count = 0;
            i = 0;
            whitespace = true;
            card_number_str = input_line.substr(input_line.find('|') + 1);
            card_number_str.push_back(' '); // Easier than writing the case for the end of the line
            while(found_count < CARD_NUMBER_COUNT){
                char c = card_number_str[i];
                if(c == ' '){
                    if(!whitespace){
                        card_numbers[found_count] = std::stoi(snippet);
                        found_count++;
                        snippet.clear();
                        whitespace = true;
                    }
                }
                else{
                    if(whitespace){
                        whitespace = false;
                    }
                    snippet.push_back(c);
                }
                i++;
            }
        }
        int calculate_score(){
            int match_count = 0;
            //Actually want a vector for this so I can delete
            std::vector<int> winning_vector(&winning_numbers[0], &winning_numbers[WINNING_NUMBER_COUNT]);
            for(int i : card_numbers){
                for(int j = 0; j < winning_vector.size(); j++){
                    if (i == winning_vector[j]){
                        match_count++;
                        winning_vector.erase(winning_vector.begin() + j);
                        break;
                    }
                }
            }
            #if PART1
            if(match_count > 0) return pow_2(match_count - 1);
            else return 0;
            #elif PART2
            return match_count;
            #endif
        }
        void add_copy(int amount){
            copy += amount;
        }
        int play_card(){
            int wins = calculate_score();
            for(int i = 0; i < wins; i++){
                //if(game_id + i >= 100) break;
                Card* c = cards->at(game_id + i);
                c->add_copy(copy);
            }
            return copy;
        }
    private:
        int winning_numbers[WINNING_NUMBER_COUNT];
        int card_numbers[CARD_NUMBER_COUNT];
        std::vector<Card*>* cards;
        int game_id;
        int copy;
};

void get_input(std::vector<std::string> &input, std::string filename){
    std::ifstream input_file(filename);
    std::string line;
    while(std::getline(input_file,line)){
        input.push_back(line);
    }
}

int main(){
    std::vector<std::string> input_vector;
    std::vector<Card*> cards;
    get_input(input_vector, "../inputs/day4.txt");
    int sum = 0;
    for(int i = 0; i < input_vector.size(); i++){
        Card* c = new Card(input_vector[i], i + 1, &cards);
        #if PART1
        sum += c->calculate_score();
        #endif
        cards.push_back(c);
    }
    #if PART2
    for(int i = 0; i < cards.size(); i++){
        Card* c = cards[i];
        sum += c->play_card();
    }
    #endif
    std::cout << sum << std::endl;
}