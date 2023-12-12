#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <algorithm>

const bool part1 = false;
const bool part2 = !part1;

enum direction{ //Pipes are never diagonal - These should have been N W S E my mistake
    d_up,
    d_left,
    d_down,
    d_right,
    d_last
};

struct coordinates{
    long long x;
    long long y;
    bool operator== (const coordinates rhs){
        return (this->x == rhs.x) && (this->y == rhs.y);
    }
};

struct pipe{
    coordinates coor;
    direction openings[2];
    char representation;
};

bool pipe_sort(pipe a, pipe b){
    if(a.coor.y != b.coor.y) return a.coor.y < b.coor.y;
    else return a.coor.x < b.coor.x;
}

pipe dirs_to_pipe(direction dir1, direction dir2, coordinates coor){
    //Will literally be used once but don't want to hard code animal pipe
    pipe p;
    p.openings[0] = std::min(dir1, dir2);
    p.openings[1] = std::max(dir1, dir2);
    p.coor = coor;
    if(p.openings[0] == d_up){
        if(p.openings[1] == d_left) p.representation = 'J';
        if(p.openings[1] == d_down) p.representation = '|';
        if(p.openings[1] == d_right) p.representation = 'L';
    }else if(p.openings[0] == d_left){
        if(p.openings[1] == d_down) p.representation = '7';
        if(p.openings[1] == d_right) p.representation = '-';
    }else if(p.openings[0] == d_down) p.representation = 'F';
    return p;
}

char peek_coordinate(coordinates origin, direction dir, std::vector<std::string> &map){
    int x_offset, y_offset;
    switch(dir){
        case d_up:
            x_offset = 0;
            y_offset = -1;
            break;
        case d_left:
            x_offset = -1;
            y_offset = 0;
            break;
        case d_down:
            x_offset = 0;
            y_offset = 1;
            break;
        case d_right:
            x_offset = 1;
            y_offset = 0;
            break;   
        default:
            throw;
            break;
    }
    origin.x += x_offset;
    origin.y += y_offset;
    if(origin.x < 0 || origin.y < 0 || origin.x >= map[0].size() || origin.y >= map.size()){
        //pipe leads outside
        return ' ';
    }
    else return map[origin.y][origin.x];
}

coordinates new_coordinates(coordinates origin, direction dir){
    int x_offset, y_offset;
    switch(dir){
        case d_up:
            x_offset = 0;
            y_offset = -1;
            break;
        case d_left:
            x_offset = -1;
            y_offset = 0;
            break;
        case d_down:
            x_offset = 0;
            y_offset = 1;
            break;
        case d_right:
            x_offset = 1;
            y_offset = 0;
            break;   
        default:
            throw;
            break;
    }
    origin.x += x_offset;
    origin.y += y_offset;
    return origin;
}

void char_to_pipe(pipe &output, char c){
    switch(c){
        case('|'):
            output.openings[0] = d_up;
            output.openings[1] = d_down;
            break;
        case('-'):
            output.openings[0] = d_left;
            output.openings[1] = d_right;
            break;
        case('L'):
            output.openings[0] = d_up;
            output.openings[1] = d_right;
            break;
        case('J'):
            output.openings[0] = d_up;
            output.openings[1] = d_left;
            break;
        case('7'):
            output.openings[0] = d_left;
            output.openings[1] = d_down;
            break;
        case('F'):
            output.openings[0] = d_down;
            output.openings[1] = d_right;
            break;
        default:
            output.openings[0] = d_last;
            output.openings[1] = d_last;
    }
    output.representation = c;
}

direction invert_direction(direction input){
    int i = (int)input;
    if(i < 2) return (direction)(i + 2);
    else return (direction)(i - 2);
}

bool check_pipe_adjacency(pipe a, pipe b){
    if(a.coor == b.coor) return true;
    coordinates a_opening_dest[2];
    a_opening_dest[0] = new_coordinates(a.coor, a.openings[0]);
    a_opening_dest[1] = new_coordinates(a.coor, a.openings[1]);
    if(a_opening_dest[0] == b.coor){
        direction a_approaching_dir = a.openings[0];
        direction a_inverted_dir = invert_direction(a_approaching_dir);
        if(b.openings[0] == a_inverted_dir || b.openings[1] == a_inverted_dir) return true;
    }else if(a_opening_dest[1] == b.coor){
        direction a_approaching_dir = a.openings[1];
        direction a_inverted_dir = invert_direction(a_approaching_dir);
        if(b.openings[0] == a_inverted_dir || b.openings[1] == a_inverted_dir) return true;
    }
    return false;
}


long long establish_loop(coordinates animal, std::vector<std::string> &map){
    std::vector<pipe> pipes;
    long long x_min = LLONG_MAX, x_max = 0, y_min = LLONG_MAX, y_max = 0;
    pipe paths[2];
    bool first_found = false;
    char surroundings[4];
    coordinates c;
    direction from[2];
    for(int i = d_up; i < d_last; i++){
        char c = peek_coordinate(animal, (direction)i, map);
        pipe p; 
        char_to_pipe(p, c);
        if(p.openings[0] == d_last) continue;
        direction inv = invert_direction((direction)i);
        if(!first_found && (inv == p.openings[0] || inv == p.openings[1])){
            p.coor = new_coordinates(animal, (direction)i);
            paths[0] = p;
            from[0] = (direction)i;
            first_found = true;
        }else if(first_found && (inv == p.openings[0] || inv == p.openings[1])){
            p.coor = new_coordinates(animal, (direction)i);
            paths[1] = p;
            from[1] = (direction)i;
        }
    }
    if(part2){
        pipe an = dirs_to_pipe(from[0], from[1], animal);
        pipes.push_back(an);
    }
    long long count = 1;
    while(true){
        if(part2){
            x_min = (std::min(paths[0].coor.x, paths[1].coor.x) < x_min) ? std::min(paths[0].coor.x, paths[1].coor.x) : x_min;
            x_max = (std::max(paths[0].coor.x, paths[1].coor.x) > x_max) ? std::max(paths[0].coor.x, paths[1].coor.x) : x_max;
            y_min = (std::min(paths[0].coor.y, paths[1].coor.y) < y_min) ? std::min(paths[0].coor.y, paths[1].coor.y) : y_min;
            y_max = (std::max(paths[0].coor.y, paths[1].coor.y) > y_max) ? std::max(paths[0].coor.y, paths[1].coor.y) : y_max;
            pipes.push_back(paths[0]);
            if(!(paths[0].coor == paths[1].coor)) pipes.push_back(paths[1]);
        }
        if(check_pipe_adjacency(paths[0], paths[1])){
            if(part1) return count;
            else break;
        }
        count++;
        direction d = from[0];
        d = (invert_direction(d) == paths[0].openings[0]) ? paths[0].openings[1] : paths[0].openings[0];
        char c = peek_coordinate(paths[0].coor, d, map);
        paths[0].coor = new_coordinates(paths[0].coor, d);
        char_to_pipe(paths[0], c);
        from[0] = d;
        d = from[1];
        d = (invert_direction(d) == paths[1].openings[0]) ? paths[1].openings[1] : paths[1].openings[0];
        c = peek_coordinate(paths[1].coor, d, map);
        paths[1].coor = new_coordinates(paths[1].coor, d);
        char_to_pipe(paths[1], c);
        from[1] = d;
    }
    if(part2){
        //Entrances from left are |, 7, J
        //Exits to the right are |, F, L
        //Try not to encounter any '-'
        //There are three types of entrance/exits | , L---7 and F---J
        //L---J and F---7 should not cause any state changes
        std::array<char, 2> segment_start = {'L', 'F'};
        std::array<char, 2> segment_end = {'7', 'J'};
        long long result = 0;
        std::sort(pipes.begin(), pipes.end(), pipe_sort);
        bool entered = false;
        bool pipe_segment_started = false;
        long long pipes_index = 0;
        std::vector<pipe> pipe_row;
        long long counter = 0;
        for(int y = y_min; y <= y_max; y++){ //adds redundant rows but easier to handle that way
            pipe_row.clear();
            while(pipes[counter].coor.y == y){
                pipe_row.push_back(pipes[counter]);
                counter++;
            }
            long long enter_point;
            long long offset = 1;
            entered = false;
            for(int i = 0; i < pipe_row.size(); i++){
                if(!entered){
                    if(pipe_row[i].representation == '|'){
                        enter_point = pipe_row[i].coor.x;
                        entered = true;
                    }else{
                        for(int s = 0; s < 2; s++){
                            if(pipe_row[i].representation == segment_start[s]){
                                i++;
                                while(pipe_row[i].representation == '-'){
                                    i++;
                                }
                                if(pipe_row[i].representation == segment_end[s]){
                                    enter_point = pipe_row[i].coor.x;
                                    entered = true;
                                    break;
                                }else{
                                    break;
                                }
                            }
                        }
                    }
                }else{
                    if(pipe_row[i].representation == '|'){
                        long long x_coor = pipe_row[i].coor.x; //for easier debugging
                        result += (x_coor - enter_point) - offset;
                        offset = 1;
                        entered = false;
                    }else{
                        for(int s = 0; s < 2; s++){
                            if(pipe_row[i].representation == segment_start[s]){
                                long long s_start = i;
                                i++;
                                offset++;
                                while(pipe_row[i].representation == '-'){
                                    i++;
                                    offset++;
                                }
                                if(pipe_row[i].representation == segment_end[s]){
                                    long long x_coor = pipe_row[i].coor.x;
                                    result += (x_coor - enter_point) - offset;
                                    offset = 1;
                                    entered = false;
                                    break;
                                }else{
                                    offset++;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        return result;
    }
    return 0;
}

int main(){
    const auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream input_file("../inputs/day10.txt");
    std::string line;
    std::vector<std::string> whole_map;
    long long result = 0;
    while(std::getline(input_file, line)){
        whole_map.push_back(line);
    }
    coordinates animal;
    for(int i = 0; i < whole_map.size(); i++){
        if(whole_map[i].find('S') != -1){
            animal.y = i;
            animal.x = whole_map[i].find('S');
        }
    }
    result = establish_loop(animal, whole_map);
    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds code_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Result: " <<  result << " - Executed in: " << code_time.count() << " us" << std::endl;
    return 0;
}