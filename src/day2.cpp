#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#define RED_LIMIT 12
#define GREEN_LIMIT 13 
#define BLUE_LIMIT 14

//I have created an abomination
//I have to turn the abomination into a class god help us
//I don't have to apparently, I just wanted to avoid passing an array to the function

std::string color_list[3] = {"red", "green", "blue"};
//int color_limits[3] = {RED_LIMIT, GREEN_LIMIT, BLUE_LIMIT};

std::vector<std::string> input_vector;

void get_input(std::vector<std::string> &input, std::string filename){
    std::ifstream input_file(filename);
    std::string line;
    while(std::getline(input_file,line)){
        input.push_back(line);
    }
}

bool evaluate_round(std::string round_text, int *mins){
    //max 3 
    std::string s = round_text;
    std::string snippet;
    for(int i = 0; i < 3; i++){
        int k = s.find(",");
        if(k != -1){
            snippet = s.substr(s.find_first_not_of(" "), k);
            s = s.substr(k + 2);
        }else{
            snippet = s.substr(s.find_first_not_of(" "));
        }
        for(int j = 0; j < 3; j++){
            int count = 0;
            if(snippet.find(color_list[j]) != -1){
                count = std::stoi(snippet.substr(0, snippet.find(" ")));
                if(count >= mins[j]){
                    mins[j] = count;
                }
                break;
            }
        }
    }
    return true;
}

int evaluate_bag(std::string bag_text, int *mins){
    int id;
    int snip_start = bag_text.find_first_of(" ");
    int snip_end = bag_text.find_first_of(":");
    std::string snippet = bag_text.substr(snip_start, (snip_end - snip_start));
    id = std::stoi(snippet);
    //Find all rounds
    std::vector<std::string> round_vector;
    snip_start = snip_end + 1;
    std::string eval_string = bag_text.substr(snip_start);
    bool final = false;
    do
    {
        snip_end = eval_string.find(";");
        if(snip_end == -1){
            final = true;
            snippet = eval_string;
        }else{
            snippet = eval_string.substr(1, snip_end);
            eval_string = eval_string.substr(snip_end + 1);
        }
        if(!evaluate_round(snippet, mins)) return 0;
    } while (!final);
    int pow = mins[0] * mins[1] * mins[2];
    return pow;
}

int main(){
    get_input(input_vector, "../inputs/day2.txt");
    int sum = 0;
    for(int i = 0; i < input_vector.size(); i++){
        int color_limits[3] = {0, 0, 0};
        sum += evaluate_bag(input_vector[i], color_limits);
    }
    std::cout << sum << std::endl;
}