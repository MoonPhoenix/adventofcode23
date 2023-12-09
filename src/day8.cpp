#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <array>
#include <numeric>

const bool part1 = false;
const bool part2 = !part1;

class Location{
    public:
        Location(std::string input, std::vector<Location*> &loc_vector){
            //Original entry that the function is called from a string AAA = (BBB,CCC)
            //Otherwise it is called from a function 
            location_vector = &loc_vector;
            loc_id = input;
            left = nullptr;
            right = nullptr;
            loc_vector.push_back(this);

        }
        void generate_lr(){
            
        }
        void set_left(Location* input){
            left = input;
        }
        void set_right(Location* input){
            right = input;
        }
        std::string get_id(){
            return loc_id;
        }
        Location* go_left(){
            return left;
        }
        Location* go_right(){
            return right;
        }
    private:
        Location* left;
        Location* right;
        std::vector<Location*>* location_vector;
        std::string loc_id;
};

bool loc_sort(Location* a, Location* b){
    //Don't know if I'll be needing this. Putting it in just in case
    //Might make the map easier to navigate in the following stages?
    std::string a_id = a->get_id();
    std::string b_id = b->get_id();
    for(int i = 0; i < a_id.size(); i++){
        if(a_id[i] == b_id[i]) continue;
        return (int)a_id[i] < (int)b_id[i];
    }
    return false;
}

bool loc_find(Location* &output, std::string input_id, std::vector<Location*> &loc_vector){
    for(int i = 0; i < loc_vector.size(); i++){
        if(!input_id.compare(loc_vector[i]->get_id())){
            output = loc_vector[i];
            return true;
        }
    }
    return false;
}

void add_location(Location* &root_loc, std::string input, std::vector<Location*> &loc_vector){
    //root_loc is merely an output
    std::string main_id = input.substr(0, input.find("=") - 1);
    Location* loc;
    if(!loc_find(loc, main_id, loc_vector)){
        loc = new Location(main_id, loc_vector);
    }
    if(!main_id.compare("AAA")) root_loc = loc;
    Location *left, *right;
    std::string left_str = input.substr(input.find("(") + 1, 3);
    std::string right_str = input.substr(input.find(",") + 2, 3);
    if(!loc_find(left, left_str, loc_vector)){
        left = new Location(left_str, loc_vector);
    }
    loc->set_left(left);
    if(!loc_find(right, right_str, loc_vector)){
        right = new Location(right_str, loc_vector);
    }
    loc->set_right(right);
}

void add_location(std::vector<Location*> &root_locs, std::string input, std::vector<Location*> &loc_vector){
    //root_loc is merely an output
    std::string main_id = input.substr(0, input.find("=") - 1);
    Location* loc;
    if(!loc_find(loc, main_id, loc_vector)){
        loc = new Location(main_id, loc_vector);
    }
    if(main_id[2] == 'A') root_locs.push_back(loc);
    Location *left, *right;
    std::string left_str = input.substr(input.find("(") + 1, 3);
    std::string right_str = input.substr(input.find(",") + 2, 3);
    if(!loc_find(left, left_str, loc_vector)){
        left = new Location(left_str, loc_vector);
    }
    loc->set_left(left);
    if(!loc_find(right, right_str, loc_vector)){
        right = new Location(right_str, loc_vector);
    }
    loc->set_right(right);
}

long long traversal(Location* root_loc, std::string directions){
    int dir_size = directions.size();
    const std::string destination = "ZZZ";    
    long long count = 0;
    Location* current_loc = root_loc;
    if(part1){
        while(true){
            if(current_loc->get_id().compare(destination)){
                if(directions[count%dir_size] == 'L'){
                    current_loc = current_loc->go_left();
                }
                else{
                    current_loc = current_loc->go_right();
                }
                count++;
                continue;
            }
            return count;
        }
    }else{
        while(true){
            if(current_loc->get_id()[2] != 'Z'){
                if(directions[count%dir_size] == 'L'){
                    current_loc = current_loc->go_left();
                }
                else{
                    current_loc = current_loc->go_right();
                }
                count++;
                continue;
            }
            return count;
        }
    }
    return -1;
}

long long vector_lcm(std::vector<long long> input){
    long long first = input[0];
    for(int i = 1; i < input.size(); i++){
        first = std::lcm(first, input[i]);
    }
    return first;
}

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream input_file("../inputs/day8.txt");
    std::string directions;
    std::getline(input_file, directions);
    std::string line;
    std::getline(input_file, line); //Get rid of newline
    std::vector<Location*> locations;
    std::vector<Location*> root_locs; //For Part 2
    Location* root_loc;
    long long result = 0;
    if(part1){
        while(std::getline(input_file, line)){
            add_location(root_loc, line, locations);
        }
        result = traversal(root_loc, directions);
    }else{
        std::vector<long long> periods;
        while(std::getline(input_file, line)){
            add_location(root_locs, line, locations);
        }
        for(int i = 0; i < root_locs.size(); i++){
            periods.push_back(traversal(root_locs[i], directions));
        }
        result = vector_lcm(periods);
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}
