/*  Nick Sweeting 2013/10/09
    Zuul C++ (OOP)
    MIT Liscense

    Interactive text-based adventure game using classes.
*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

#define trout cout

const int size = 20;                    // total number of rooms
const int y_size = 5;                   // length of room map
const int x_size = 4;                   // width of room map
const int max_items = size-(size/4);    // total number of rooms


#include "Room.cpp"

int rnd(int max) {return rand() % max;}

int init(BoardState &Game) {
    srand(time(NULL));
    sranddev();


    int rand_ord[size] = {-1};
    for (int p=0;p<size;p++) {
        while (rand_ord[p] == -1) {
            rand_ord[p] = rnd(size);
        }
        cout << rand_ord[p] << " ";
    }

    cout << endl;

    // for (int p=0;p<size;p++) {
    //     Game.Rooms[] = room_info[rnd(size)
    // }

    //string item_info[max_items] = {a};

    Game.Rooms[0].SetAttr("House", "Your house.", true);  // Name, Description, bool isRoom
    Game.Rooms[1].SetAttr("Castle", "Where the king lives.", true);
    Game.Rooms[2].SetAttr("Field", "Where the mouse lives.", true);
    Game.Rooms[3].SetAttr("Lake", "Where the fish lives.", true);
    Game.Rooms[4].SetAttr("Sea", "I wonder what the sea tastes like?", true);
    Game.Rooms[5].SetAttr("Box", "Special Room", true);
    Game.Rooms[6].SetAttr("Land", "Special Room", true);
    Game.Rooms[7].SetAttr("Roof", "Special Room", true);
    Game.Rooms[8].SetAttr("", "", false);
    Game.Rooms[8].SetAttr("Potato", "Special Room", true);
    Game.Rooms[9].SetAttr("", "", false);
    Game.Rooms[10].SetAttr("Potato", "Special Room", true);
    Game.Rooms[11].SetAttr("Potato", "Special Room", true);
    Game.Rooms[12].SetAttr("Potato", "Special Room", true);
    Game.Rooms[13].SetAttr("Potato", "Special Room", true);
    Game.Rooms[14].SetAttr("Potato", "Special Room", true);
    Game.Rooms[15].SetAttr("Potato", "Special Room", true);
    Game.Rooms[16].SetAttr("Potato", "Special Room", true);
    Game.Rooms[17].SetAttr("Potato", "Special Room", true);
    Game.Rooms[18].SetAttr("Potato", "Special Room", true);
    Game.Rooms[19].SetAttr("Supermarket", "Special Room", true);

    Game.Rooms[0].Items.push_back(Item("Sword"));
    Game.Rooms[0].Items.push_back(Item("Monacle"));
    Game.Rooms[1].Items.push_back(Item("Shield"));
    Game.Rooms[2].Items.push_back(Item("Water"));
    Game.Rooms[2].Items.push_back(Item("Poison"));
    Game.Rooms[3].Items.push_back(Item("Fish"));
    Game.Rooms[4].Items.push_back(Item("Fish"));
    Game.Rooms[5].Items.push_back(Item("Fish"));
    Game.Rooms[6].Items.push_back(Item("Fish"));

    Game.Player.name = "Galbraith";
    Game.Player.curr_pos = 1;

    return 2;
}

main () {
    if (y_size*x_size != size) {trout << "INVALID BOARD DIMENTIONS: "<< x_size << "*" << y_size << "!=" << size << endl; return(1);}

    BoardState Game = BoardState(20, 5, 4);  // total rooms, height, width of map in rooms
    init(Game);
    bool playing=true;


    cout << "You are Prince " << Game.Player.name << " the magnificent. You recently got \na call from your darling Princess Galbraith, who wants to come over \nfor dinner.  Oh no! Prince " << Game.Player.name << " the magnificent is a \nis a woefully unprepared bachelor with a fridge full of condiments \nand who-knows-how-old leftovers.  You need to have a meal ready in 1 hour \nbefore the princess arives! Scavenge around to find enough for a meal, \nthen bring the items fit to eat back home. Once you have enough for a meal, \nthe pricess will arrive :)" << endl;



    trout << endl << "[n/s/e/w] to move, [p] to pickup, [d] to drop, [q] to exit" << endl << endl;

    while (playing) {
        int curr_pos = Game.Player.curr_pos;
        BoardState::Room curr_room = Game.Rooms[curr_pos];

        trout << endl << "Current Room: " << curr_room.name << " [" << Game.RoomPos(curr_pos)[0] << "," << Game.RoomPos(curr_pos)[1] << "]" << endl;
        trout << "Exits: ";
        int *doors = Game.RoomDoors(curr_pos);
        for (int b=0; b<4; b++) if (doors[b] > -1) trout << exit_names[b] << " ";

        trout << endl << "Items: ";
        for (int m=0; m<curr_room.Items.size(); m++) trout << curr_room.Items[m].name << ", ";

        trout << endl << "Details: " << curr_room.details << endl;

        trout << endl;
        Game.PrintBoard();

        trout << endl << "Backpack: ";
        for (int m=0; m<Game.Player.Backpack.size(); m++) trout << Game.Player.Backpack[m].name << ", ";

        string in;
        trout << endl << "-> ";
        cin >> in;

        if (in == "p") {
            trout << endl << "To Pick up an item from the room, type it's name: ";
            string item_to_take;
            cin >> item_to_take;
            Game.PickItem(Game, curr_pos, item_to_take);
        }
        if (in == "d") {
            trout << endl << "To drop an item from your backpack into the room, type it's name: ";
            string item_to_drop;
            cin >> item_to_drop;
            Game.DropItem(Game, curr_pos, item_to_drop);
        }
        else if (in == "n" || in == "e" || in == "s" || in == "w") {
            int *doors = Game.RoomDoors(curr_pos);
            if (in == "n" && doors[0] > -1) Game.Player.curr_pos = doors[0];
            if (in == "e" && doors[1] > -1) Game.Player.curr_pos = doors[1];
            if (in == "s" && doors[2] > -1) Game.Player.curr_pos = doors[2];
            if (in == "w" && doors[3] > -1) Game.Player.curr_pos = doors[3];
        }
        else if (in == "q") return 0;
        trout << endl;
    }
    return 0;
}


    // [ 0, 1, 2, 3]
    // [ 4, 5, 6, 7]
    // [ 8, 9,10,11]
    // [12,13,14,15]
    // [16,17,18,19]
