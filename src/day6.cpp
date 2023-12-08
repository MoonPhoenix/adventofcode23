#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <limits>

const bool part1 = false;
const bool part2 = !part1; 

const double EPSILON = std::numeric_limits<double>::epsilon();

std::vector<long long> extract_values(std::string s){
    std::vector<long long> output;
    s = s.substr(s.find(":") + 1);
    while(s.find_first_of(' ') != -1){
        s = s.substr(s.find_first_not_of(' '));
        std::string snippet = s.substr(0, s.find_first_of(' '));
        if(s.find_first_of(' ') != -1 ) s = s.substr(s.find_first_of(' '));
        output.push_back(stoll(snippet));
    }
    return output;
}

long long extract_single_value(std::string s){
    s = s.substr(s.find(":") + 1);
    std::string snippet = "";
    for(int i = 0; i < s.size(); i++){
        char c = s[i];
        if(c != ' '){
            snippet.push_back(c);
        }
    }
    return stoll(snippet);
};

long long calculate_records(long long time, long long dist){
    //let time pressed be tp and total time be time
    //1 mm/sec speed for each tp
    //so our inequality is - (tp * (time - tp) > dist) 
    //simplified to -tp^2 + time*tp - dist > 0;
    //Quadratic equation Where a = -1, b = total time, c = -dist
    //delta for this is b^2 - 4ac -> time^2 - 4dist
    //solve for x = -b ± sqrt(delta) / 2a -> (time ± sqrt(delta)) / 2
    double time_d = (double)time;
    double dist_d = (double)dist;
    double delta = (time_d * time_d) - (4 * dist_d);
    if(delta == EPSILON) {
        double time_result = (time_d / 2);
        if (abs(time_result - (int)time_result) <= EPSILON) return (int)time_result;
        return 0;
    }else if (delta < 0){
        return 0; //No possible records as the parabola does not intersect the x axis
    }else{
        int result;
        double root_1 = (time + sqrt(delta)) / 2.0;
        double root_2 = (time - sqrt(delta)) / 2.0;
        double root_min, root_max;
        root_min = std::min(root_1, root_2);
        root_max = std::max(root_1, root_2);
        //Function should never be true for t = 0 or t = time
        int min_bound = std::floor(root_min + 1);
        int max_bound = std::ceil(root_max - 1);
        if(min_bound < 0 && max_bound > time_d) return 0;
        result = max_bound - min_bound + 1;
        return result;
    }
}

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    long long result = 1;
    std::ifstream input_file("../inputs/day6.txt");
    std::string time_str, dist_str;
    std::getline(input_file, time_str);
    std::getline(input_file, dist_str);
    if(part1){
        std::vector<long long> time_vect = extract_values(time_str);
        std::vector<long long> dist_vect = extract_values(dist_str);
        for(int i = 0; i < time_vect.size(); i++){
            result = result * calculate_records(time_vect[i], dist_vect[i]);
        }
    }
    else if(part2){
        long long time = extract_single_value(time_str);
        long long distance = extract_single_value(dist_str);
        result = calculate_records(time, distance);
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
}