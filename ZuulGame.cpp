/*  Nick Sweeting 2013/10/09
    Zuul C++ (OOP)
    MIT License
    g++ ZuulGame.cpp -o main && ./main

    Interactive text-based adventure game with room navigation, items, and a story!
*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

const int size = 20;                // total number of rooms
const int y_size = 5;               // length of room map
const int x_size = 4;               // width of room map
const int max_items = 15;           // max items the player can carry
const int house_pos = 0;            // starting position (used in check_win and make_meal)
const char* exit_names[4] = {"North", "East", "South", "West"};

class Item {
    public:
        Item(string inname) {
            name = inname;
        }
        ~Item() {};

        string name;
};

class BoardState {
    public:

        class Room {
            public:
                Room(string in_name) {
                    name = in_name;
                    isRoom = true;
                }

                ~Room() {};

                void SetAttr(string inname, string indetails, bool inisRoom) {
                    name = inname;
                    details = indetails;
                    isRoom = inisRoom;
                }

                string name;
                string details;
                bool isRoom;
                vector<Item> Items;
        };

        int current_room;
        int size;
        int y_size;
        int x_size;
        vector<Room> Rooms;

        void AddRoom(Room o) {
            Rooms.push_back(o);
        }

        class Character {
            public:
                int curr_pos;
                string name;
                vector<Item> Backpack;

                Character() {
                    curr_pos=0;
                };

                ~Character() {};
        };

        Character Player;

        BoardState(int in_size, int in_y_size, int in_x_size) {
            size = in_size; y_size = in_y_size; x_size = in_x_size;

            Player = Character();

            for (int q=0; q<size; q++) {
                AddRoom(Room("Room"));
            }
        };

        ~BoardState() {};

        int* RoomPos(int position) {
            int *pos = new int[2];
            pos[0] = position/(y_size-1);                 // y
            pos[1] = position-(pos[0]*x_size);        // x
            return pos;
        }

        int RoomIndex(int y_pos, int x_pos) {
            if (x_pos > x_size-1 || x_pos < 0) return -1;
            if (y_pos > y_size-1 || y_pos < 0) return -1;

            return (y_pos*x_size)+x_pos;
        }

        bool IsRoom(int position) {
            if (position >= 0 && position < size) {
                return Rooms[position].isRoom;
            }
            else return false;
        }

        int* RoomDoors(int position) {
            int y_pos = RoomPos(position)[0];
            int x_pos = RoomPos(position)[1];

            int *doors = new int[4];
            doors[0] = RoomIndex(y_pos-1, x_pos); // n
            doors[1] = RoomIndex(y_pos, x_pos+1); // e
            doors[2] = RoomIndex(y_pos+1, x_pos); // s
            doors[3] = RoomIndex(y_pos, x_pos-1); // w

            for (int l=0;l<4;l++) if (!IsRoom(doors[l])) doors[l] = -1;
            return doors;
        }

        void PickItem(BoardState &Game, int pos, string item_to_take) {
            for (int l=0; l<Rooms[pos].Items.size(); l++) {
                Item item = Rooms[pos].Items[l];
                if (item.name == item_to_take) {
                    Game.Player.Backpack.push_back(item);
                    Rooms[pos].Items.erase(Rooms[pos].Items.begin() + l);
                }
            }
        }

        void DropItem(BoardState &Game, int pos, string item_to_drop) {
            for (int l=0; l<Game.Player.Backpack.size(); l++) {
                Item item = Game.Player.Backpack[l];
                if (item.name == item_to_drop) {
                    Rooms[pos].Items.push_back(item);
                    Game.Player.Backpack.erase(Game.Player.Backpack.begin() + l);
                }
            }
        }

        void PrintBoard() {
            for (int y=0; y<y_size; y++) {
                cout << "| ";
                for (int x=0; x<x_size; x++) {
                    if (Player.curr_pos == RoomIndex(y,x)) cout << "@"; else cout << " ";
                    cout << setw(14) << Rooms[RoomIndex(y,x)].name << setw(11) << " | ";
                }
                cout << endl;
            }
        }
};


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

bool check_win(BoardState &Game, int house_pos) {
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

string make_meal(BoardState &Game, int house_pos) {
    vector<Item> meal = Game.Rooms[house_pos].Items;
    string finished_meal;
    for (int f=0;f<meal.size();f++) {
        finished_meal += meal[f].name;
        if (f!=meal.size()-1) finished_meal += ", ";
    }
    return finished_meal;
}

int main () {
    if (y_size*x_size != size) {cout << "INVALID BOARD DIMENTIONS: " << x_size << "*" << y_size << "!=" << size << endl; return(1);}

    BoardState Game = BoardState(20, 5, 4);  // total rooms, height, width of map in rooms
    init(Game);
    bool playing=true;

    cout << "You are Prince " << Game.Player.name << " the magnificent. You recently got \na call from your darling Princess Galbraith, who wants to come over \nfor dinner.  Oh no! Prince " << Game.Player.name << " the magnificent is a \nis a woefully unprepared bachelor with a fridge full of condiments \nand who-knows-how-old leftovers.  You need to have a meal ready in 1 hour \nbefore the princess arives! Scavenge around to find enough for a meal, \nthen bring the items fit to eat back home. Once you have enough for a meal, \nthe pricess will arrive :)" << endl;

    cout << endl << "[n/s/e/w] to move, [p] to pickup, [d] to drop, [q] to exit" << endl << endl;

    while (playing) {
        int curr_pos = Game.Player.curr_pos;
        BoardState::Room curr_room = Game.Rooms[curr_pos];

        // Print current gamestate
        cout << endl << "Current Room: " << curr_room.name << " [" << Game.RoomPos(curr_pos)[0] << "," << Game.RoomPos(curr_pos)[1] << "]" << endl;
        cout << "Exits: ";
        int *doors = Game.RoomDoors(curr_pos);
        for (int b=0; b<4; b++) if (doors[b] > -1) cout << exit_names[b] << " ";

        cout << endl << "Items: ";
        for (int m=0; m<curr_room.Items.size(); m++) cout << curr_room.Items[m].name << ", ";

        cout << endl << "Details: " << curr_room.details << endl;

        cout << endl;
        Game.PrintBoard();

        cout << endl << "Backpack: ";
        for (int m=0; m<Game.Player.Backpack.size(); m++) cout << Game.Player.Backpack[m].name << ", ";

        // Get player's next action from input
        string in;
        cout << endl << "-> ";
        cin >> in;
        if (in == "p") {
            cout << endl << "To Pick up an item from the room, type it's name: ";
            string item_to_take;
            cin >> item_to_take;
            Game.PickItem(Game, curr_pos, item_to_take);
        }
        if (in == "d") {
            cout << endl << "To drop an item from your backpack into the room, type it's name: ";
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
        cout << endl;

        // Check for game ending conditions
        if (check_win(Game, house_pos)) {
            cout << "Congrats! You're collected enough for a candlelight dinner for two." << endl;
            cout << "Your meal consists of: " << make_meal(Game, house_pos) << endl;
            return 0;
        }
    }
    return 0;
}
