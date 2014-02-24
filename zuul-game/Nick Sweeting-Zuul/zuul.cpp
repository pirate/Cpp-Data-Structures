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

const int size = 20;                // total number of rooms
const int y_size = 5;               // length of room map
const int x_size = 4;               // width of room map
const int max_items = 15;                // total number of rooms
const int house_pos = 0;            // starting position (used in CheckWin and MakeMeal)

#include "Room.cpp"

vector<Item> items_to_win;

void init(BoardState &Game) {
    // for random item positioning (Define room_info[], item_info[], and is_room[] first)

    // vector<int> rnd(size);
    // for (int p=0;p<size;p++) rnd[p]=p;
    // random_shuffle(rnd.begin(), rnd.end());

    // for (int p=0;p<size;p++) {
    //     Game.Rooms[p].SetAttr(room_info[rnd[p]][0], room_info[rnd[p]][1], is_room[rnd[p]]);
    // }

    // [ 0, 1, 2, 3]
    // [ 4, 5, 6, 7]
    // [ 8, 9,10,11]
    // [12,13,14,15]
    // [16,17,18,19]

    items_to_win.push_back(Item("Stove"));
    items_to_win.push_back(Item("Bowl"));
    items_to_win.push_back(Item("Fish"));
    items_to_win.push_back(Item("Water"));
    items_to_win.push_back(Item("Salt"));
    items_to_win.push_back(Item("Potatoes"));
    items_to_win.push_back(Item("Tomatoes"));
    items_to_win.push_back(Item("Carrots"));
    items_to_win.push_back(Item("Wine"));

    Game.Rooms[house_pos].SetAttr("House", "Your house.", true);  // Name, Description, bool isRoom
    Game.Rooms[house_pos].Items.push_back(Item("Stove"));

    Game.Rooms[1].SetAttr("Field", "Where the mouse lives.", true);
    Game.Rooms[1].Items.push_back(Item("Mouse"));

    Game.Rooms[2].SetAttr("Lake", "", true);
    Game.Rooms[2].Items.push_back(Item("Fish"));

    Game.Rooms[3].SetAttr("Castle", "Dont disturb the king.", true);
    Game.Rooms[3].Items.push_back(Item("Potatoes"));

    Game.Rooms[4].SetAttr("Monster Sea", "Eek, not trying to kill the princess.", true);
    Game.Rooms[4].Items.push_back(Item("Poison"));

    Game.Rooms[5].SetAttr("Field", "Just a boring field.", true);

    Game.Rooms[6].SetAttr("Village", "The soup needs water.", true);
    Game.Rooms[6].Items.push_back(Item("Water"));

    Game.Rooms[7].SetAttr("Keep", "Doesn't wine go great with candlelight dinners?", true);
    Game.Rooms[7].Items.push_back(Item("Wine"));

    Game.Rooms[8].SetAttr("Bearing Sea", "The perfect seasoning...", true);
    Game.Rooms[8].Items.push_back(Item("Salt"));

    Game.Rooms[9].SetAttr("", "", false);

    Game.Rooms[10].SetAttr("Nice Forest", "Mmm I love carrots.", true);
    Game.Rooms[10].Items.push_back(Item("Carrots"));

    Game.Rooms[11].SetAttr("Lanau Forest", "", true);
    Game.Rooms[12].SetAttr("Fairon Woods", "", true);

    Game.Rooms[13].SetAttr("Castle", "The perfect fruit!", true);
    Game.Rooms[13].Items.push_back(Item("Tomatoes"));

    Game.Rooms[14].SetAttr("", "", false);

    Game.Rooms[15].SetAttr("Lake", "A frozen lake.", true);

    Game.Rooms[16].SetAttr("Village", "This might come in handy.", true);
    Game.Rooms[16].Items.push_back(Item("Bowl"));

    Game.Rooms[17].SetAttr("Hobbit Village", "I'm not trying to kill the princess!", true);
    Game.Rooms[17].Items.push_back(Item("Sword"));

    Game.Rooms[18].SetAttr("Evil Forest", "Disgusing witches, thats not how you make soup.", true);
    Game.Rooms[18].Items.push_back(Item("Eye of Newt"));

    Game.Rooms[19].SetAttr("Village", "My house has enough furniture.", true);
    Game.Rooms[19].Items.push_back(Item("Chair"));

    Game.Player.name = "Galbraith";
    Game.Player.curr_pos = house_pos;
}

bool CheckWin(BoardState &Game, int house_pos) {
    vector<Item> items_in_room = Game.Rooms[house_pos].Items;
    int count = 0;
    for (int i=0;i<items_in_room.size();i++) {
        for (int w=0;w<items_to_win.size();w++) {
            if (items_to_win[w].name == items_in_room[i].name) count ++;
        }
    }
    if (count == items_to_win.size()) return true;
    else return false;
}

string MakeMeal(BoardState &Game, int house_pos) {
    vector<Item> meal = Game.Rooms[house_pos].Items;
    string finished_meal;
    for (int f=0;f<meal.size();f++) {
        finished_meal += meal[f].name;
        if (f!=meal.size()-1) finished_meal += ", ";
    }
    return finished_meal;
}

main () {
    if (y_size*x_size != size) {trout << "INVALID BOARD DIMENTIONS: " << x_size << "*" << y_size << "!=" << size << endl; return(1);}

    BoardState Game = BoardState(20, 5, 4);  // total rooms, height, width of map in rooms
    init(Game);
    bool playing=true;

    trout << "You are Prince " << Game.Player.name << " the magnificent. You recently got \na call from your darling Princess Galbraith, who wants to come over \nfor dinner.  Oh no! Prince " << Game.Player.name << " the magnificent is a \nis a woefully unprepared bachelor with a fridge full of condiments \nand who-knows-how-old leftovers.  You need to have a meal ready in 1 hour \nbefore the princess arives! Scavenge around to find enough for a meal, \nthen bring the items fit to eat back home. Once you have enough for a meal, \nthe pricess will arrive :)" << endl;

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

        if (CheckWin(Game, house_pos)) {
            trout << "Congrats! You're collected enough for a candlelight dinner for two." << endl;
            trout << "Your meal consists of: " << MakeMeal(Game, house_pos) << endl;
            return 0;
        }
    }
    return 0;
}
