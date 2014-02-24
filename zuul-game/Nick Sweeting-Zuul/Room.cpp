const char* exit_names[4] = {"North", "East", "South", "West"};

class Item {
public:
    Item(string inname) {
        name = inname;
    }
    ~Item() {};

    int id;
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

    // void PrintBoard() {
    //     int idx = 0;
    //     for (int y=0; y<y_size; y++) {
    //         for (int x=0; x<x_size; x++) {
    //             idx = RoomIndex(y,x);
    //             if (Player.curr_pos == idx) {
    //                 cout << "@";
    //             }
    //             cout << Rooms[idx].name << ": ";
    //             for (int m=0; m<Rooms[idx].Items.size(); m++) {
    //                 cout << Rooms[idx].Items[m].name << " ";
    //             }
    //             cout << " |    ";
    //         }
    //         cout << endl;
    //     }
    // }
};
