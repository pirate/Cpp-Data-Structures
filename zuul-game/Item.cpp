class Item {
public:
  Item(string name);
  ~Item();
  string item_name;
private:
};

Item::Item(string name) {
  item_name = name;
}

Item::~Item() {}
