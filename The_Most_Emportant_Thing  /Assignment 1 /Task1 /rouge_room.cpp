//
//  rouge_room.cpp
//  Advanced Software Assignment
//
//  Created by Harry Powell-Evans on 27/01/2026.
//

#include <iostream>
#include <string> 
#include <thread>
#include <chrono>

using namespace std;

class Room{
private:
    int width;
    int height;
    int x;
    int y;
    char floor_marker;
    char player_marker;
    char wall_s_marker;
    char wall_t_marker;
    char corner_marker;

public:
    Room(int w, int h, char floor = '.', char player = '@', char wall_s = '|', char wall_t = '-', char corner = '+'){
        width = w;
        height = h;
        x=0;
        y=0;
        floor_marker = floor;
        player_marker = player;
        wall_s_marker = wall_s;
        wall_t_marker = wall_t;
        corner_marker = corner;
    }
    
    //Setters
    void set_player_marker(char marker){
        player_marker = marker;
    }
    void set_wall_marker(char marker_s, char marker_t){
        wall_s_marker = marker_s;
        wall_t_marker = marker_t;
    }
    void set_floor_marker(char marker){
        floor_marker = marker;
    }
    void set_corner_marker(char marker){
        corner_marker = marker;
    }

    //Draw Method 
    void draw(){
        //Draw the top wall
        cout << corner_marker << string(width, wall_t_marker) << corner_marker << endl;

        for (int i=0; i < height; i++){
            if (i != y){
                //Draw a row of floor
                cout << wall_s_marker << string(width, floor_marker) << wall_s_marker << endl;
            }
            else{
                //Draw a row with the player
                cout << wall_s_marker << string(x, floor_marker) << player_marker << string(width-x-1, floor_marker) << wall_s_marker << endl;
            }
        }

        //Draw the bottom wall
        cout << corner_marker << string(width, wall_t_marker) << corner_marker << endl;
    }

    //Movement
    void left(){
        if (x > 0){
            x--;
        }
        else{
            cout << "You can't move left!" << endl;
        }
    }

    void right(){
        if (x < width - 1){
            x++;
        }
        else{
            cout << "You can't move right!" << endl;
        }
    }

    void up(){
        if (y > 0){
            y--;
        }
        else{
            cout << "You can't move up!" << endl;
        }
    }

    void down(){
        if (y < height - 1){
            y++;
        }
        else{
            cout << "You can't move down!" << endl;
        }
    }
};

int main(){
    
    //Creat a room with dimensions 8X6

    Room room(8, 6);

    while (true){
        //draw the room 
        room.draw();

        //Get user input
        cout << "Enter command (w/a/s/d & x to exit): ";
        char input;
        cin >> input;   

        //Handle input
        if (input == 'w'){
            room.up();
        }
        else if (input == 'a'){
            room.left();
        }
        else if (input == 's'){
            room.down();
        }
        else if (input == 'd'){
            room.right();
        }
        else if (input == 'x'){
            cout << "Exiting game..." << endl;
            break;
        }
        else{
            cout << "Invalid command!" << endl;
        }

        //sleep for 0.5 seconds 
        this_thread::sleep_for(chrono::milliseconds(500));

        // Clear the screen (ANSI escape code
        cout << "\033[2J\033[1;1H";
    }
    return 0;
}
