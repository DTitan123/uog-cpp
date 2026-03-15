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

class room{
private:
    int width;
    int height;
    int x;
    int y;
    char floor_marker;
    char player_marker;
    char wall_marker;
    char corner_marker;

public:
    room(int w, int h, char floor = '.', char player = '@', char wall = '|', char corner = '+'){
        width = w;
        height = h;
        x=0;
        y=0;
        floor_marker = floor;
        player_marker = player;
        wall_marker = wall;
        corner_marker = corner;
    }
    
    //Setters
    void set_player_marker(char marker){
        player_marker = marker;
    }
    void set_wall_marker(char marker){
        wall_marker = marker;
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
        cout << corner_marker << string(width, wall_marker) << corner_marker <<end l;

        for (int i=0; i < height; i++){
            if (int i !=y){
                //Draw a row of floor
                cout << wall_marker << string(width, floor_marker) << wall_marker << endl;
            }
            else{
                //Draw a row with the player
                cout << wall_marker << string(x, floor_marker) << player_marker << string(width-x-1, floor_marker) << wall_marker << endl;
            }
        }

        //Draw the bottom wall
        cout << corner_marker << string(width, wall_marker) << corner_marker << endl;
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
    int room[] = {2, 2, 0, 1};
    for (int i = 0; i < size(room); i++){
            
    }
    
}
        

 
Hello 
