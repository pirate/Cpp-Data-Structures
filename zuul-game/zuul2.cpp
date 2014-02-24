/*  Nick Sweeting 2013/10/09
    Zuul C++ (OOP)
    MIT Liscense

    Interactive text-based adventure game using classes.
*/

#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

#include "Item.cpp"
#include "Room.cpp"
//#include "Data.cpp"   sample data for rooms and items, contains an array of possible room names and item names

bool valid = false;
while (!valid) {
  int total_rooms = 15;
  int total_items = 5;
  cout << "Number of rooms(15): ";
  cin >> total_rooms;
  cout << "Number of items(5): ";
  cin >> total_items;

  if (total_rooms > 1 && total_rooms < 50 && total_items >= 1 && total_items < 50) {
    valid = true;
  }
  else cout << "Must have more than one room and item, cannot have greater than 50 of each.";
}

Room map[5][8] = {0};

int* find_adjacent(Room *map, int x, int y) {

  int exits[4] = {0};
}

#include "Player.cpp"

int main() {
  bool doors[4] = {true, false, true, false};
  Room LivingRoom(doors);
  LivingRoom.printDoors();

  while (true) {

  }

}
