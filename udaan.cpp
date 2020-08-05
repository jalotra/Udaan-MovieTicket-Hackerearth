#include <bits/stdc++.h>
using namespace std;

class MovieTicket{
    private :
        string screen_name;
        int total_cols;
        int total_rows;
        set<int>aisle_seats;
        vector<vector<bool>>matrix;
    
    public : 
        MovieTicket(string name, int rows, int cols, vector<int>aisle_Seats){
            this->screen_name = name;
            this->total_rows = rows;
            this->total_cols = cols ;
            // Initialise the matrix
            this->matrix.resize(this->total_rows + 1);
            for(int i = 0; i <= this->total_rows; i++){
                matrix[i].resize(this->total_cols + 1, 0);
            }
            // Push the aisle seats in the set
            for(auto seat : aisle_Seats){
                this->aisle_seats.insert(seat);
            }

        }

        // Now the Apis or the functionalities that are needed
        // 1. reserve_seats(reserves seats in a row iff possible)
        bool reserve_seats(int row_number, vector<int>seats){
            // First check if the matrix[row_number] has empty seats
            int total_available = 0;
            bool can_be_done = false;
            for(auto seat : seats){
                if(!this->matrix[row_number][seat]){
                    total_available += 1;
                }
            }
            if(total_available == (int)seats.size()){
                can_be_done = true;
                for(auto seat : seats){
                    this->matrix[row_number][seat] = 1;
                }
            }

            return can_be_done == true ? true : false;
        } 

        // 2. get_unreserved_seats
        vector<int> get_unreserved_seats(int row_number){
            vector<int>result;
            for(int i = 1; i <= this->total_cols; i++){
                if(!this->matrix[row_number][i]){
                    result.push_back(i);
                }
            }
            return result;
        }

        // 3. suggest_contiguous_seats
        vector<int>suggest_contiguous_seats(int number_of_seats, int row_number, int seat_number){
            // Keep in mind of Aisle seats
            // {seat_number, seat_number + 1, seat_number + 2 , ... seat_number + number_of_seats - 1}
            // or {seat_number - number_of_seats + 1, seat_number - number_of_seats ... , seat_number}

            // First check the forward direction
            
            
            int found = 0; 
            vector<int>result;
            for(int i = seat_number + 1; i < min(seat_number + number_of_seats, this->total_cols + 1); i++){
                if(this->aisle_seats.find(i) != this->aisle_seats.end() || this->matrix[row_number][i]){
                    break;
                }else{
                    found += 1;
                }
            } 
            if(!this->matrix[row_number][seat_number]){
                found += 1;
            }

            if(found == number_of_seats){
                for(int i = seat_number; i < min(seat_number + number_of_seats, this->total_cols + 1); i++){
                    result.push_back(i);
                }
                return result;
            }
            
            found = 0;
            // Lets check the backward direction 
            for(int i = max(1, seat_number - number_of_seats - 1); i < seat_number; i++){
                if(this->aisle_seats.find(i) != this->aisle_seats.end() || this->matrix[row_number][i]){
                    break;
                }else{
                    found += 1;
                }
            }
            if(!this->matrix[row_number][seat_number]){
                found += 1;
            }

            if(found == number_of_seats){
                for(int i = max(1, seat_number - number_of_seats - 1); i <= seat_number ; i++){
                    result.push_back(i);
                }
                return result;
            }

            return {};

        }

};

vector<string>get_values(string s){
    //cout << s << endl;
    vector<string>values;
    string current_word = "";
    for(int i = 0; i <= (int)s.size(); i++){
        if(s[i] != ' '){
            current_word += s[i];
        }else{
            values.push_back(current_word);
            //cout << current_word << " ";
            current_word = "";
        }
    }
    values.push_back(current_word);
    return values;
}

bool add_the_screen(map<string, MovieTicket*>&all_screens_map, string screen_name, int total_rows, int total_cols, vector<int>aisle_seats){
    if(all_screens_map.find(screen_name) == all_screens_map.end()){
        MovieTicket* new_obj = new MovieTicket(screen_name, total_rows, total_cols, aisle_seats);
        all_screens_map[screen_name] = new_obj;
        return true;
    }else{
        return false;
    }
}
bool book_seats(map<string, MovieTicket*>&all_screens_map, string screen_name, int row_number, vector<int>seats){
    if(all_screens_map.find(screen_name) != all_screens_map.end()){
        MovieTicket* obj = all_screens_map[screen_name];
        bool response = obj->reserve_seats(row_number, seats);
        
        return response;
    }else{
        return false;
    }
}

void unreserved_seats(map<string, MovieTicket*>&all_screens_map, string screen_name, int row_number){
    if(all_screens_map.find(screen_name) != all_screens_map.end()){
        MovieTicket* obj = all_screens_map[screen_name];
        vector<int>possible_options = obj->get_unreserved_seats(row_number);
        if(possible_options.size() != 0){
            for(auto option : possible_options){
                cout << option << " ";
            }cout << endl;
        }else{
            cout << "none" << endl;
        }
    }else{
        cout << "none" << endl;
    }
}

void contiguous_seats(map<string, MovieTicket*>&all_screens_map, string screen_name, int number_of_seats, int row_number, int seat_number){
    MovieTicket* obj = all_screens_map[screen_name];
    vector<int>possible_options = obj->suggest_contiguous_seats(number_of_seats, row_number, seat_number);
    if(possible_options.size() != 0){
        for(int i = 0; i < (int)possible_options.size(); i++){
            cout << possible_options[i] << " "; 
        }cout << endl;
    }else{
        cout << "none" << endl;
    }
}

int main(){
    // Lets define the main function 
    // This is hard in C++ 
    map<string, int>possible_options = {{"add-screen" ,0}, 
                                        {"reserve-seat" , 1},
                                        {"get-unreserved-seats" , 2},
                                        {"suggest-contiguous-seats", 3}
                                        };
    //int attempts;
    string s;
    //cin >> attempts;
    map<string, MovieTicket*>all_screens_map;
    int attempt = 0; 
    while(getline(cin, s)){
        //getline(cin, s);
        if(attempt == 0){
            attempt += 1;
            continue;
        }
        vector<string>values = get_values(s);
        if(possible_options[values[0]] == 0){
            string screen_name = values[1];
            int rows = stoi(values[2]);
            int cols = stoi(values[3]);
            vector<int>aisle_seats;
            //cout << screen_name << " " << rows << " " << cols << endl;
            for(int i = 4; i < (int)values.size(); i++){
                aisle_seats.push_back(stoi(values[i]));
            } 
            bool response = add_the_screen(all_screens_map, screen_name, rows, cols, aisle_seats);
            if(response){
                cout << "success" << endl;
            }else{
                cout << "failure" << endl;
            }
        }else if(possible_options[values[0]] == 1){
            string screen_name = values[1];
            int row_number = stoi(values[2]);
            vector<int>seats_to_book;
            for(int i = 3; i < (int)values.size(); i++){
                seats_to_book.push_back(stoi(values[i]));
            }
            bool response = book_seats(all_screens_map, screen_name, row_number, seats_to_book);
            if(response){
                cout << "success" << endl;
            }else{
                cout << "failure" << endl;
            }

        }else if(possible_options[values[0]] == 2){
            string screen_name = values[1];
            int row_number = stoi(values[2]);

            unreserved_seats(all_screens_map, screen_name, row_number);

        }else{
            string screen_name = values[1];
            int number_of_seats = stoi(values[2]);
            int row_number = stoi(values[3]);
            int seat_number = stoi(values[4]);
            //cout << screen_name << " " << number_of_seats << " " << row_number << " " << seat_number << endl;
            contiguous_seats(all_screens_map, screen_name, number_of_seats, row_number, seat_number);
        }
        attempt += 1;
    }
}
