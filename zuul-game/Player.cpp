class Game {
public:
  Game(string player_name, );
  ~Game();

  void getPosition();
  void setPosition();

  // items in player backpack

private:
  // only accessible to other functions in the class
  static const char* exit_names[];
};

const char* Game::exit_names[] = {"North", "East", "South", "West"};

// c file

Game::Game(bool *doors) {

  for (int i=0; i<4; i++) {
    exits[i] = doors[i];
  }
}

Game::~Game() {}


void Game::printDoors() {
  for (int q=0;q<4;q++) {
    if (exits[q]) cout << exit_names[q] << endl;
  }
}


class Item {
  Item(char* name, Room destination_room);
  ~Item();

  char *name[];
  int dest_room;
};

Item::Item(char *name, Room destination_room) {

}
