#include <fstream>
#include <iostream>
#include <vector>
#include <string>

//There is likely two different approaches to solving this. 
//Finding all symbols and looking for adjacent numbers seem to be simpler to implement.
//Finding all numbers and giving them boundaries to look for adjacent symbols would probably be more performant since you don't need to look for duplicate occurences.

#define IS_SYMBOL(c) !((c == '.') || std::isdigit(c))
#define COORDINATE_MATCH(a,b) ((a.x == b.x) && (a.y == b.y))
#define PART1 0
#define PART2 !PART1

struct coordinate{
    int x;
    int y;
    coordinate(int x_input, int y_input){
        x = x_input;
        y = y_input;
    }
};

class Schematic{
    public: 
        Schematic(std::vector<std::string> &input_vector){
            //assuming each string is identical
            x_lim = input_vector[0].size();
            y_lim = input_vector.size();
            map = new char*[y_lim];
            for(int i = 0; i < y_lim; i++){
                char* c = new char[x_lim];
                c = input_vector[i].data();
                map[i] = c;
            }
        }
        void find_symbols(){
            for(int i = 0; i < y_lim; i++){
                for(int j = 0; j < x_lim; j++){
                    if(IS_SYMBOL(map[i][j])){
                        symbol_coordinates.push_back(coordinate(j,i));
                    }
                }
            }
        }
        void find_stars(){
            for(int i = 0; i < y_lim; i++){
                for(int j = 0; j < x_lim; j++){
                    if(map[i][j] == '*'){
                        star_coordinates.push_back(coordinate(j,i));
                    }
                }
            }
        }
        int evaluate_gear(coordinate coor){
            //See if this turns out to be a gear
            //I really don't want to write new functions for this so will deviate from the standart strategy.
            std::vector<coordinate> hits;
            int num_count = 0;
            int num1 = 0, num2 = 0;
            int x_upper = 1, x_lower = -1, y_upper = 1, y_lower = -1;
            if(coor.x == 0) x_lower = 0;
            else if(coor.x == x_lim - 1) x_upper = 0;
            if(coor.y == 0) y_lower = 0;
            else if(coor.y == y_lim - 1) y_upper = 0;
            //This should adjust the searching ranges for corner and edge cases. Annoying that this has to be done every iteration though. 4 checks per regular pass
            //Business as usual
            for(int i = y_lower; i <= y_upper; i++){
                bool row_hit = false;
                for(int j = x_lower; j <= x_upper; j++){
                    if(i == 0 & j == 0){
                        row_hit = false;
                        continue;
                    }
                    if(std::isdigit(map[coor.y + i][coor.x + j]) && !row_hit){
                        hits.push_back(coordinate(coor.x + j, coor.y + i));
                        row_hit = true;
                    }else if(!std::isdigit(map[coor.y + i][coor.x + j])){
                        row_hit = false;
                    }
                }
            }
            if(hits.size() == 2){
                num1 = evaluate_number(hits[0]);
                num2 = evaluate_number(hits[1]);
                return num1 * num2;
            }
            else return 0;
        }
        int evaluate_symbol(coordinate coor){
            //Want to do this per coordinate because it feels more modular this way. Otherwise can just loop inside this
            //Corners and edges are somewhat annoying
            int sum = 0;
            int x_upper = 1, x_lower = -1, y_upper = 1, y_lower = -1;
            if(coor.x == 0) x_lower = 0;
            else if(coor.x == x_lim - 1) x_upper = 0;
            if(coor.y == 0) y_lower = 0;
            else if(coor.y == y_lim - 1) y_upper = 0;
            //This should adjust the searching ranges for corner and edge cases. Annoying that this has to be done every iteration though. 4 checks per regular pass
            //Business as usual
            for(int i = y_lower; i <= y_upper; i++){
                for(int j = x_lower; j <= x_upper; j++){
                    if(i == 0 & j == 0) continue;
                    if(std::isdigit(map[coor.y + i][coor.x + j])){
                        sum += evaluate_number(coordinate(coor.x + j,coor.y + i));
                    }
                }   
            }
            return sum;
        }
        int evaluate_number(coordinate coor){
            //To evaluate a number that was found during evaluate_symbol
            int x = coor.x;
            int x_start;
            while(x > 0){
                if(std::isdigit(map[coor.y][x - 1])) x--;
                else break;
            }
            #if PART1
            if(check_processed(coordinate(x, coor.y))) return 0;
            #endif
            x_start = x;
            std::string s = "";
            while(std::isdigit(map[coor.y][x])){
                s.push_back(map[coor.y][x]);
                x++;
            }
            processed_coordinates.push_back(coordinate(x_start, coor.y));
            return std::stoi(s);
        }
        bool check_processed(coordinate coor){
            for(int i = 0; i < processed_coordinates.size(); i++){
                if(COORDINATE_MATCH(coor, processed_coordinates[i])){
                    return true;
                }
            }
            return false;
        }
        int check_all_coordinates(){
            int sum = 0;
            for(int i = 0; i < symbol_coordinates.size(); i++){
                sum += evaluate_symbol(symbol_coordinates[i]);
            }
            return sum;
        }
        int check_all_stars(){
            int sum = 0;
            for(int i = 0; i < star_coordinates.size(); i++){
                sum += evaluate_gear(star_coordinates[i]);
            }
            return sum;
        }
        ~Schematic(){
            delete map;
        }

    private:
        int x_lim, y_lim;
        char** map;
        std::vector<coordinate> symbol_coordinates;
        std::vector<coordinate> processed_coordinates;
        std::vector<coordinate> star_coordinates;
};


void get_input(std::vector<std::string> &input, std::string filename){
    std::ifstream input_file(filename);
    std::string line;
    while(std::getline(input_file,line)){
        input.push_back(line);
    }
}

int main(){
    int sum;
    std::vector<std::string> input_vector;
    get_input(input_vector, "../inputs/day3.txt");
    Schematic schem = Schematic(input_vector);
    #if PART1
    schem.find_symbols();
    sum = schem.check_all_coordinates();
    #elif PART2
    schem.find_stars();
    sum = schem.check_all_stars();
    #endif
    std::cout << sum << std::endl;
}