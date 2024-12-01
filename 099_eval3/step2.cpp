#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Ship {
 public:
  string name;
  int maxCapacity;
  int currentLoad;
  vector<string> cargo;

  Ship(string name, int capacity) : name(name), maxCapacity(capacity), currentLoad(0) {}

  bool canCarry(int cargoWeight) { return (currentLoad + cargoWeight <= maxCapacity); }

  void loadCargo(string cargoItem, int weight) {
    if (canCarry(weight)) {
      cargo.push_back(cargoItem);
      currentLoad += weight;
    }
  }

  void printStatus() {
    cout << "The Container Ship " << name << "(" << currentLoad << "/" << maxCapacity
         << ") is carrying :" << endl;
    for (size_t i = 0; i < cargo.size(); ++i) {
      cout << "  " << cargo[i] << endl;
    }
    cout << "(" << (maxCapacity - currentLoad) << ") slots remain" << endl;
  }
};

class Cargo {
 public:
  string name;
  string source;
  string destination;
  int weight;
  string type;

  Cargo(string name, string source, string destination, int weight, string type) :
      name(name), source(source), destination(destination), weight(weight), type(type) {}
};

class FleetManager {
 public:
  vector<Ship> ships;

  FleetManager(vector<Ship> ships) : ships(ships) {}

  struct CompareShipByName {
    bool operator()(const Ship & a, const Ship & b) const { return a.name < b.name; }
  };

  void sortShipsByName() { sort(ships.begin(), ships.end(), CompareShipByName()); }

  void loadCargoAndPrintResults(const Cargo & cargo) {
    cout << ships.size() << " ships can carry the " << cargo.name << " from "
         << cargo.source << " to " << cargo.destination << endl;
    vector<Ship *> eligibleShips;
    for (size_t i = 0; i < ships.size(); ++i) {
      if (ships[i].canCarry(cargo.weight)) {
        eligibleShips.push_back(&ships[i]);
      }
    }

    if (eligibleShips.empty()) {
      cout << "No ships can carry the " << cargo.name << " from " << cargo.source
           << " to " << cargo.destination << endl;
    }
    else {
      for (size_t i = 0; i < eligibleShips.size(); ++i) {
        cout << "  " << eligibleShips[i]->name << endl;
      }

      eligibleShips[0]->loadCargo(cargo.name, cargo.weight);
      cout << "**Loading the cargo onto " << eligibleShips[0]->name << "**" << endl;
    }

    cout << "---Done Loading---Here are the ships---" << endl;
    for (size_t i = 0; i < ships.size(); ++i) {
      ships[i].printStatus();
    }
  }
};

int main() {
  ifstream shipsFile("inputs/ships0.txt");
  vector<Ship> ships;
  string line;

  while (getline(shipsFile, line)) {
    stringstream ss(line);
    string name, type, source, destination;
    int maxCapacity;
    char dummy;

    ss >> name >> dummy >> type >> dummy >> source >> dummy >> destination >> dummy >>
        maxCapacity;
    ships.push_back(Ship(name, maxCapacity));
  }

  FleetManager manager(ships);
  manager.sortShipsByName();

  ifstream cargoFile("inputs/cargo0.txt");
  vector<Cargo> cargoList;
  while (getline(cargoFile, line)) {
    stringstream ss(line);
    string name, source, destination, type;
    int weight;
    char dummy;

    ss >> name >> dummy >> source >> dummy >> destination >> dummy >> weight >> dummy >>
        type;
    cargoList.push_back(Cargo(name, source, destination, weight, type));
  }

  for (size_t i = 0; i < cargoList.size(); ++i) {
    manager.loadCargoAndPrintResults(cargoList[i]);
  }

  return 0;
}
