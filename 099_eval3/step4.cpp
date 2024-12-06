#include <stdint.h>  // For uint64_t

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// Cargo class
class Cargo {
 public:
  std::string name;
  std::string source;
  std::string destination;
  uint64_t weight;
  std::vector<std::string> properties;

  Cargo(const std::string & line) {
    std::istringstream ss(line);
    std::string temp;
    std::getline(ss, name, ',');
    std::getline(ss, source, ',');
    std::getline(ss, destination, ',');
    std::getline(ss, temp, ',');
    weight = std::strtoull(temp.c_str(), NULL, 10);
    while (std::getline(ss, temp, ',')) {
      properties.push_back(temp);
    }
  }

  bool requiresProperty(const std::string & property) const {
    return std::find(properties.begin(), properties.end(), property) != properties.end();
  }

  int getPropertyValue(const std::string & key) const {
    for (size_t i = 0; i < properties.size(); ++i) {
      if (properties[i].find(key + "=") == 0) {
        std::string value = properties[i].substr(key.length() + 1);
        return std::atoi(value.c_str());
      }
    }
    return 0;
  }
};

// Base Ship class
class Ship {
 protected:
  std::string name;
  std::string source;
  std::string destination;
  uint64_t totalCapacity;
  uint64_t usedCapacity;

 public:
  Ship(const std::string & name,
       const std::string & source,
       const std::string & destination,
       uint64_t capacity) :
      name(name),
      source(source),
      destination(destination),
      totalCapacity(capacity),
      usedCapacity(0) {}

  virtual ~Ship() {}

  virtual bool canCarry(const Cargo & cargo) const = 0;
  virtual void loadCargo(const Cargo & cargo) = 0;
  virtual void printDetails() const = 0;

  uint64_t remainingCapacity() const {
    return totalCapacity > usedCapacity ? totalCapacity - usedCapacity : 0;
  }

  const std::string & getName() const { return name; }

  bool isOnRoute(const Cargo & cargo) const {
    return cargo.source == source && cargo.destination == destination;
  }
};

// Container Ship
class ContainerShip : public Ship {
  unsigned int slots;
  unsigned int usedSlots;
  std::vector<Cargo> loadedCargo;

 public:
  ContainerShip(const std::string & name,
                const std::string & source,
                const std::string & destination,
                uint64_t capacity,
                unsigned int slots) :
      Ship(name, source, destination, capacity), slots(slots), usedSlots(0) {}

  bool canCarry(const Cargo & cargo) const {
    if (!isOnRoute(cargo) || usedCapacity + cargo.weight > totalCapacity ||
        usedSlots >= slots) {
      return false;
    }
    return cargo.requiresProperty("container");
  }

  void loadCargo(const Cargo & cargo) {
    usedCapacity += cargo.weight;
    usedSlots++;
    loadedCargo.push_back(cargo);
  }

  void printDetails() const {
    std::cout << "The Container Ship " << name << " (" << usedCapacity << "/"
              << totalCapacity << ") is carrying:\n";
    for (size_t i = 0; i < loadedCargo.size(); ++i) {
      std::cout << "  " << loadedCargo[i].name << " (" << loadedCargo[i].weight << ")\n";
    }
    std::cout << "  (" << (slots - usedSlots) << ") slots remain\n";
  }
};

// Function to process and load cargo
void processCargo(std::vector<Ship *> & ships, const std::vector<Cargo> & cargoList) {
  typedef AVLMultiMap<uint64_t, Ship *> ShipMap;
  ShipMap shipMap;

  for (size_t i = 0; i < ships.size(); ++i) {
    shipMap.add(ships[i]->remainingCapacity(), ships[i]);
  }

  for (size_t i = 0; i < cargoList.size(); ++i) {
    const Cargo & cargo = cargoList[i];
    Ship * bestShip = NULL;

    std::vector<std::pair<std::pair<uint64_t, std::set<Ship *> >, int> > dump =
        shipMap.preOrderDump();
    for (size_t j = 0; j < dump.size(); ++j) {
      const std::set<Ship *> & shipSet = dump[j].first.second;
      for (std::set<Ship *>::iterator it = shipSet.begin(); it != shipSet.end(); ++it) {
        Ship * ship = *it;
        if (ship->canCarry(cargo)) {
          if (!bestShip || ship->remainingCapacity() < bestShip->remainingCapacity() ||
              (ship->remainingCapacity() == bestShip->remainingCapacity() &&
               ship->getName() < bestShip->getName())) {
            bestShip = ship;
          }
        }
      }
    }

    if (bestShip) {
      std::cout << "Loading " << cargo.name << " onto " << bestShip->getName() << " from "
                << cargo.source << " to " << cargo.destination << " "
                << bestShip->remainingCapacity() - cargo.weight << " capacity remains\n";
      shipMap.remove(bestShip->remainingCapacity(), bestShip);
      bestShip->loadCargo(cargo);
      shipMap.add(bestShip->remainingCapacity(), bestShip);
    }
    else {
      std::cout << "No ships can carry the " << cargo.name << " from " << cargo.source
                << " to " << cargo.destination << "\n";
    }
  }

  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
  }
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships_file> <cargo_file>\n";
    return EXIT_FAILURE;
  }

  std::vector<Ship *> ships;
  std::vector<Cargo> cargoList;

  std::ifstream shipsFile(argv[1]);
  std::ifstream cargoFile(argv[2]);
  if (!shipsFile.is_open() || !cargoFile.is_open()) {
    std::cerr << "Error opening input files\n";
    return EXIT_FAILURE;
  }

  std::string line;
  while (std::getline(shipsFile, line)) {
    std::istringstream ss(line);
    std::string name, type, source, destination, temp;
    uint64_t capacity;
    std::getline(ss, name, ':');
    std::getline(ss, type, ':');
    std::getline(ss, source, ':');
    std::getline(ss, destination, ':');
    std::getline(ss, temp, ':');
    capacity = std::strtoull(temp.c_str(), NULL, 10);

    if (type.find("Container") == 0) {
      unsigned int slots = std::atoi(type.substr(type.find(',') + 1).c_str());
      ships.push_back(new ContainerShip(name, source, destination, capacity, slots));
    }
  }

  while (std::getline(cargoFile, line)) {
    cargoList.push_back(Cargo(line));
  }

  processCargo(ships, cargoList);

  for (size_t i = 0; i < ships.size(); ++i) {
    delete ships[i];
  }

  return EXIT_SUCCESS;
}
