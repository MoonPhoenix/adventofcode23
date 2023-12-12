#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <algorithm>

const bool part1 = false;
const bool part2 = !part1;

class Star{ //Might be better as a struct but class seems like a better investment
    public:
        Star(long long x, long long y){
            x_coordinate = x;
            y_coordinate = y;
        }
        long long get_x(){
            return x_coordinate;
        }
        long long get_y(){
            return y_coordinate;
        }
        long long measure_distance_to_star(Star* other_star, std::vector<int> &empty_x_list, std::vector<int> &empty_y_list){
            //manhattan distance is good
            int empty_offset = 1;
            if(part2) empty_offset = 1000000 - 1; //Better readability
            long long other_x = other_star->get_x();
            long long other_y = other_star->get_y();
            long long x_dist = abs(x_coordinate - other_x);
            long long y_dist = abs(y_coordinate - other_y);
            for(int i = 0; i < empty_x_list.size(); i++){
                if(x_coordinate < empty_x_list[i] ^ other_x < empty_x_list[i]){
                    x_dist += empty_offset;
                }
            }
            for(int i = 0; i < empty_y_list.size(); i++){
                if(y_coordinate < empty_y_list[i] ^ other_y < empty_y_list[i]){
                    y_dist += empty_offset;
                }
            }
            return x_dist + y_dist;
        }
    private:
        long long x_coordinate, y_coordinate;
};

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream input_file("../inputs/day11.txt");
    std::string line;
    long long result = 0;
    std::vector<std::string> galaxy_map;
    std::vector<int> empty_x_coordinates;
    std::vector<int> empty_y_coordinates;
    while(std::getline(input_file, line)){
        galaxy_map.push_back(line);
        if(line.find('#') == -1) empty_y_coordinates.push_back(galaxy_map.size() - 1);
    }
    {   //Experimenting a little with scopes
        std::vector<int> star_x_coordinates;
        for(std::string line : galaxy_map){
            while(line.find_last_of('#') != -1){
                star_x_coordinates.push_back(line.find_last_of('#'));
                line = line.substr(0, line.find_last_of('#'));
            }
        }
        std::sort(star_x_coordinates.begin(), star_x_coordinates.end()); //, [](int a, int b) {return a > b; }
        for(int i = galaxy_map[0].size() - 1; i >= 0; i--){
            while(i < star_x_coordinates.back()){
                star_x_coordinates.pop_back();
            }
            if(i == star_x_coordinates.back()) continue;
            empty_x_coordinates.push_back(i);
        }
        /*for(int i = 0; i < galaxy_map.size(); i++){
            for(int j = 0; j < empty_x_coordinates.size(); j++){
                galaxy_map[i].insert(galaxy_map[i].begin() + empty_x_coordinates[j], '.');
            }
        }*/
    }
    std::vector<Star*> stars;
    for(long long i = 0; i < galaxy_map.size(); i++){
        line = galaxy_map[i];
        while(line.find_last_of('#') != -1){
                long long x = line.find_last_of('#');
                Star* s = new Star(x, i);
                stars.push_back(s);
                line = line.substr(0, line.find_last_of('#'));
            }
    }
    for(int i = 0; i < stars.size(); i++){
        for(int j = i + 1; j < stars.size(); j++){
            int distance = stars[i]->measure_distance_to_star(stars[j], empty_x_coordinates, empty_y_coordinates);
            result += distance;
        }
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}