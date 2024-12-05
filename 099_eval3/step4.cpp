#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// Cargo class for handling cargo details
class Cargo {
 public:
  std::string name;
  std::string source;
  std::string destination;
  unsigned int weight;
  std::vector<std::string> properties;

  Cargo(const std::string & line) {
    std::istringstream ss(line);
    std::string temp;
    std::getline(ss, name, ',');
    std::getline(ss, source, ',');
    std::getline(ss, destination, ',');
    std::getline(ss, temp, ',');
    weight = std::strtoul(temp.c_str(), NULL, 10);
    while (std::getline(ss, temp, ',')) {
      properties.push_back(temp);
    }
  }
};

// Ship class for handling ship details
class Ship {
 protected:
  std::string name;
  std::string source;
  std::string destination;
  unsigned int capacity;
  unsigned int usedCapacity;
  unsigned int slots;
  unsigned int usedSlots;
  std::vector<std::string> capabilities;
  std::vector<Cargo> loadedCargo;

 public:
  Ship(const std::string & line) : usedCapacity(0), usedSlots(0) {
    std::istringstream ss(line);
    std::string temp;
    std::getline(ss, name, ':');
    std::getline(ss, temp, ':');  // Skip type info
    std::getline(ss, source, ':');
    std::getline(ss, destination, ':');
    std::getline(ss, temp, ':');
    capacity = std::strtoul(temp.c_str(), NULL, 10);
    std::getline(ss, temp, ':');
    slots = std::atoi(temp.c_str());
    while (std::getline(ss, temp, ',')) {
      capabilities.push_back(temp);
    }
  }

  virtual ~Ship() {}

  const std::string & getName() const { return name; }

  virtual bool canCarry(const Cargo & cargo) const {
    if (source != cargo.source || destination != cargo.destination) {
      return false;
    }
    if (usedCapacity + cargo.weight > capacity || usedSlots >= slots) {
      return false;
    }
    for (size_t i = 0; i < cargo.properties.size(); ++i) {
      const std::string & prop = cargo.properties[i];
      if (prop.find("hazardous-") == 0) {
        std::string hazard = prop.substr(10);
        if (std::find(capabilities.begin(), capabilities.end(), hazard) ==
            capabilities.end()) {
          return false;
        }
      }
    }
    return true;
  }

  virtual void loadCargo(const Cargo & cargo) {
    usedCapacity += cargo.weight;
    usedSlots++;  // Correctly increment usedSlots
    loadedCargo.push_back(cargo);
  }

  virtual void printDetails() const {
    std::cout << "The Container Ship " << name << "(" << usedCapacity << "/" << capacity
              << ") is carrying :" << std::endl;
    for (size_t i = 0; i < loadedCargo.size(); ++i) {
      std::cout << "  " << loadedCargo[i].name << "(" << loadedCargo[i].weight << ")"
                << std::endl;
    }
    std::cout << "  (" << slots - usedSlots << ") slots remain"
              << std::endl;  // Correctly print remaining slots
  }

  unsigned int getRemainingCapacity() const { return capacity - usedCapacity; }
  unsigned int getRemainingSlots() const {
    return slots - usedSlots;
  }  // Add this helper method
};

bool compareCargo(const Cargo & a, const Cargo & b) {
  return a.weight > b.weight;
}

// Best-fit strategy for selecting ships
class BestFitStrategy {
  AVLMultiMap<unsigned int, Ship *> shipMap;

 public:
  void addShip(Ship * ship) { shipMap.add(ship->getRemainingCapacity(), ship); }

  Ship * findBestShip(const Cargo & cargo) {
    std::vector<std::pair<std::pair<unsigned int, std::set<Ship *> >, int> >
        preOrderDump = shipMap.preOrderDump();
    for (size_t i = 0; i < preOrderDump.size(); ++i) {
      const std::set<Ship *> & ships = preOrderDump[i].first.second;
      for (std::set<Ship *>::iterator it = ships.begin(); it != ships.end(); ++it) {
        if ((*it)->canCarry(cargo)) {
          return *it;
        }
      }
    }
    return NULL;
  }

  void updateShip(Ship * ship, unsigned int oldCapacity) {
    shipMap.remove(oldCapacity, ship);
    shipMap.add(ship->getRemainingCapacity(), ship);
  }
};

// Process cargo and assign to best-fit ships
void processCargo(std::vector<Ship *> & ships, std::vector<Cargo> & cargoList) {
  BestFitStrategy strategy;
  for (size_t i = 0; i < ships.size(); ++i) {
    strategy.addShip(ships[i]);
  }

  std::stable_sort(cargoList.begin(), cargoList.end(), compareCargo);

  for (size_t i = 0; i < cargoList.size(); ++i) {
    Cargo & cargo = cargoList[i];
    Ship * bestShip = strategy.findBestShip(cargo);
    if (bestShip == NULL) {
      std::cout << "No ships can carry the " << cargo.name << " from " << cargo.source
                << " to " << cargo.destination << std::endl;
    }
    else {
      unsigned int oldCapacity = bestShip->getRemainingCapacity();
      bestShip->loadCargo(cargo);
      strategy.updateShip(bestShip, oldCapacity);
      std::cout << "Loading " << cargo.name << " onto " << bestShip->getName() << " from "
                << cargo.source << " to " << cargo.destination << " "
                << bestShip->getRemainingCapacity() << " capacity remains" << std::endl;
    }
  }

  std::cout << "---Done Loading---Here are the ships---" << std::endl;
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
  }
}

void readShips(const std::string & filename, std::vector<Ship *> & ships) {
  std::ifstream file(filename.c_str());
  if (!file) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (std::getline(file, line)) {
    ships.push_back(new Ship(line));
  }
}

void readCargo(const std::string & filename, std::vector<Cargo> & cargoList) {
  std::ifstream file(filename.c_str());
  if (!file) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (std::getline(file, line)) {
    cargoList.push_back(Cargo(line));
  }
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships_file> <cargo_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Ship *> ships;
  readShips(argv[1], ships);

  std::vector<Cargo> cargoList;
  readCargo(argv[2], cargoList);

  processCargo(ships, cargoList);

  for (size_t i = 0; i < ships.size(); ++i) {
    delete ships[i];
  }

  return EXIT_SUCCESS;
}
