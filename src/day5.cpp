#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

//There is something deeply wrong with this code.
//I have burnt off my fingerprints after writing this for penance (I accidentally touched the tea kettle)

const bool part1 = false;
const bool part2 = !part1; 

struct num_range{
    long long start;
    long long end;
};

bool struct_sort(num_range a, num_range b){
    return a.start < b.start;
}

enum conversion{ //Likely useless
    c_seed_to_soil = 0,
    c_soil_to_fertilizer = 1,
    c_fertilizer_to_water = 2,
    c_water_to_light = 3,
    c_light_to_temperature = 4,
    c_temperature_to_humidity = 5,
    c_humidity_to_location = 6,
    c_last
};

void get_input(std::vector<std::string> &input, std::string filename){
    std::ifstream input_file(filename);
    std::string line;
    while(std::getline(input_file,line)){
        input.push_back(line);
    }
}

void trim_range(std::vector<num_range> &output, std::vector<num_range> per_input_vec, num_range input){
    num_range whole_set = input;
    std::vector<num_range> test = per_input_vec;
    std::sort(test.begin(), test.end(), struct_sort);
    if(test.empty()){
        output.push_back(input);
        return;
    }
    for(int i = 0; i < test.size(); i++){
        num_range current_nm = test[i];
        if((current_nm.start) == whole_set.start){
            if(current_nm.end == whole_set.end) return;
            whole_set.start = current_nm.end;
        }else{
            num_range output_nm;
            output_nm.start = whole_set.start;
            output_nm.end = current_nm.start;
            output.push_back(output_nm);
            whole_set.start = current_nm.end;
        }
    }
    if(whole_set.start == whole_set.end) return;
    output.push_back(whole_set);
}

void seed_vector_init(std::vector<long long> &seed_vector, std::string s){
    int i = s.find(":") + 1;
    std::string snippet = "";
    bool whitespace = true;
    while(i < s.size()){
        if(s[i] == ' '){
            if(whitespace == false){
                seed_vector.push_back(std::stoll(snippet));
                snippet.clear();
            }
            whitespace = true;
        }
        else{
            whitespace = false;
            snippet.push_back(s[i]);
        }
        i++;
    }
    seed_vector.push_back(std::stoll(snippet));
    snippet.clear();
}

void seed_vector_init(std::vector<num_range> &seed_vector, std::string s){
    //Overloaded for Part 2
    int i = s.find(":") + 1;
    int count = 0;
    std::string snippet = "";
    bool whitespace = true;
    std::vector<long long> buffer;
    while(i < s.size()){
        if(s[i] == ' '){
            if(whitespace == false){
                buffer.push_back(std::stoll(snippet));
                snippet.clear();
            }
            whitespace = true;
        }
        else{
            whitespace = false;
            snippet.push_back(s[i]);
        }
        i++;
    }
    buffer.push_back(std::stoll(snippet));
    snippet.clear();
    for(int i = 0; i < buffer.size() / 2; i++){
        num_range nm;
        nm.start = buffer[2 * i];
        nm.end = buffer[(2 * i) + 1] + nm.start;
        seed_vector.push_back(nm);
    }
}

class Mapping{
    public:
        Mapping(long long dest, long long src, long long length, conversion c){
            offset = dest - src;
            src_start = src;
            src_end = src + length;
            map_type = c;
        }
        Mapping(std::string map_string, conversion c){
            //Weighing parsing them here instead of outside. Might be easier
            long long dest, src, length;
            std::string s = map_string;
            std::string snippet = s.substr(0, s.find(" "));
            dest = std::stoll(snippet);
            s = s.substr(s.find(" ") + 1);
            snippet = s.substr(0, s.find(" "));
            src = std::stoll(snippet);
            snippet = s.substr(s.find(" ") + 1);
            length = std::stoll(snippet);
            offset = dest - src;
            src_start = src;
            src_end = src + length;
            map_type = c;    
        }
        long long check_mapping(long long src){
            //check if in mapping range
            if(src < src_start || src >= src_end){
                return -1;
            }
            else{
                long long result = src + offset;
                return result;
            }
        }
        int check_mapping_range(num_range &output, num_range input, num_range &affected_input){
            if(input.start >= src_end || input.end <= src_start){
                return -1;
            }
            output.start = std::max(src_start, input.start) + offset;
            affected_input.start = std::max(src_start, input.start);
            output.end = std::min(src_end, input.end) + offset;
            affected_input.end = std::min(src_end, input.end);
            return 0;
        }
    private:
        long long src_start, src_end, offset;
        conversion map_type;
};

class Big_Map{
    public:
        Big_Map(int &line0, std::vector<std::string> &input_vector, conversion input_type){
            int i = line0 + 1;
            type = input_type;
            while(i < input_vector.size() && !input_vector[i].empty()){
                std::string s = input_vector[i];
                mappings.push_back(Mapping(s, type));
                i++;
            }
            line0 = i + 1;
        }
        long long query_dest(long long src){
            long long num = -1;
            for(Mapping m : mappings){
                num = m.check_mapping(src);
                if(num != -1){
                    return num;
                }
            }
            return src;
        }
        void query_dest_range(std::vector<num_range> &output, std::vector<num_range> input){
            for(num_range nm : input){
                std::vector<num_range> per_input_vec = {};
                for(Mapping m : mappings){
                    num_range dest;
                    num_range per_input;
                    if(m.check_mapping_range(dest, nm, per_input) != -1){
                        output.push_back(dest);
                        per_input_vec.push_back(per_input);
                    }
                }
                trim_range(output, per_input_vec, nm);
            }
        }
        conversion get_type(){
            return type;
        }
    private:
        std::vector<Mapping> mappings;
        conversion type;
};

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::string> input_vector;
    get_input(input_vector, "../inputs/day5.txt");
    std::vector<long long> seeds;
    std::vector<num_range> seed_ranges;
    if(part1) seed_vector_init(seeds, input_vector[0]);
    else seed_vector_init(seed_ranges, input_vector[0]);
    int line = 2;
    std::vector<Big_Map*> big_maps;
    for(conversion c = c_seed_to_soil; c < c_last; c = conversion(c + 1)){
        Big_Map* bm = new Big_Map(line, input_vector, c);
        big_maps.push_back(bm);
    }
    long long min_loc = LLONG_MAX;
    if(part1){
        for(long long seed : seeds){
            long long result = seed;
            for(Big_Map* bm : big_maps){
                result = bm->query_dest(result);
            }
            if(result < min_loc){
                min_loc = result;
            }
        }
    }
    else{
        std::vector<num_range> range_collections[c_last + 1];
        range_collections[0] = seed_ranges;
        for(int i = 0; i < c_last; i++){
            Big_Map* bm = big_maps[i];
            bm->query_dest_range(range_collections[i + 1], range_collections[i]);
        }
        for(int i = 0; i < range_collections[c_last].size(); i++){
            if(range_collections[c_last][i].start < min_loc) min_loc = range_collections[c_last][i].start;
        }
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  min_loc << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}