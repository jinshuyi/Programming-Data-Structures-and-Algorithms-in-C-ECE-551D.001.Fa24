#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

class Cargo {
 public:
  std::string name, source, destination, category;
  unsigned int weight, slots;
  bool hazardous, roamer;

  Cargo(const std::string & line) : slots(1), hazardous(false), roamer(false) {
    std::istringstream ss(line);
    std::string temp;

    std::getline(ss, name, ',');
    std::getline(ss, source, ',');
    std::getline(ss, destination, ',');
    std::getline(ss, temp, ',');
    std::stringstream(temp) >> weight;

    while (std::getline(ss, temp, ',')) {
      if (temp == "roamer")
        roamer = true;
      if (temp.find("hazardous") != std::string::npos)
        hazardous = true;
      category = temp;  // 保存类别
    }
  }
};

class Ship {
 public:
  std::string name, type, source, destination;
  unsigned int totalCapacity, usedCapacity, totalSlots, usedSlots, maxTanks, usedTanks;
  bool isTanker, isAnimal, hasRoamer;

  Ship(const std::string & line) :
      usedCapacity(0), usedSlots(0), usedTanks(0), hasRoamer(false) {
    std::istringstream ss(line);
    std::string temp;

    std::getline(ss, name, ':');
    std::getline(ss, temp, ':');
    std::istringstream tempSS(temp);
    std::getline(tempSS, type, ',');  // 读取类型
    isTanker = (type == "Tanker");
    isAnimal = (type == "Animals");

    std::getline(tempSS, temp, ',');
    std::stringstream(temp) >> totalSlots;

    if (isTanker) {
      std::getline(tempSS, temp, ',');
      std::stringstream(temp) >> maxTanks;
    }
    else {
      maxTanks = 0;
    }

    std::getline(ss, source, ':');
    std::getline(ss, destination, ':');
    std::getline(ss, temp, ':');
    std::stringstream(temp) >> totalCapacity;
  }

  bool canCarry(const Cargo & cargo) const {
    if (source != cargo.source || destination != cargo.destination)
      return false;
    if (usedCapacity + cargo.weight > totalCapacity)
      return false;

    if (isTanker) {
      return usedTanks + cargo.slots <= maxTanks;
    }
    else if (isAnimal) {
      if (hasRoamer && cargo.roamer)
        return false;
      return usedSlots + cargo.slots <= totalSlots;
    }
    else {
      return usedSlots + cargo.slots <= totalSlots;
    }
  }

  void loadCargo(const Cargo & cargo) {
    usedCapacity += cargo.weight;
    usedSlots += cargo.slots;
    if (isTanker) {
      usedTanks += cargo.slots;
    }
    else if (isAnimal && cargo.roamer) {
      hasRoamer = true;
    }
  }

  unsigned int getRemainingCapacity() const { return totalCapacity - usedCapacity; }

  void printDetails() const {
    std::cout << "The " << type << " Ship " << name << "(" << usedCapacity << "/"
              << totalCapacity << ") is carrying :" << std::endl;
    if (isTanker) {
      std::cout << "  " << usedTanks << " / " << maxTanks << " tanks used" << std::endl;
    }
    else {
      std::cout << "  (" << totalSlots - usedSlots << ") slots remain" << std::endl;
    }
    if (isAnimal) {
      std::cout << "  " << (hasRoamer ? "has a roamer" : "does not have a roamer")
                << std::endl;
    }
  }
};

bool cargoComparator(const Cargo & a, const Cargo & b) {
  if (a.weight == b.weight)
    return a.name < b.name;
  return a.weight > b.weight;
}

Ship * findBestShip(
    AVLMultiMap<unsigned int, Ship *, std::less<unsigned int>, std::less<Ship *> > &
        shipMap,
    const Cargo & cargo) {
  std::vector<
      std::pair<std::pair<unsigned int, std::set<Ship *, std::less<Ship *> > >, int> >
      candidates = shipMap.preOrderDump();
  Ship * bestShip = NULL;

  for (size_t i = 0; i < candidates.size(); ++i) {
    const std::set<Ship *, std::less<Ship *> > & ships = candidates[i].first.second;
    for (std::set<Ship *>::const_iterator sit = ships.begin(); sit != ships.end();
         ++sit) {
      Ship * ship = *sit;
      if (ship->canCarry(cargo)) {
        if (!bestShip ||
            ship->getRemainingCapacity() < bestShip->getRemainingCapacity() ||
            (ship->getRemainingCapacity() == bestShip->getRemainingCapacity() &&
             ship->name < bestShip->name)) {
          bestShip = ship;
        }
      }
    }
  }

  return bestShip;
}

void processCargoAndShips(const std::string & shipFile, const std::string & cargoFile) {
  std::ifstream shipInput(shipFile.c_str());
  std::ifstream cargoInput(cargoFile.c_str());

  std::vector<Ship> ships;
  std::vector<Cargo> cargos;

  std::string line;
  while (std::getline(shipInput, line)) {
    ships.push_back(Ship(line));
  }

  while (std::getline(cargoInput, line)) {
    cargos.push_back(Cargo(line));
  }

  std::stable_sort(cargos.begin(), cargos.end(), cargoComparator);

  AVLMultiMap<unsigned int, Ship *, std::less<unsigned int>, std::less<Ship *> > shipMap;
  for (size_t i = 0; i < ships.size(); ++i) {
    shipMap.add(ships[i].getRemainingCapacity(), &ships[i]);
  }

  for (size_t i = 0; i < cargos.size(); ++i) {
    Ship * bestShip = findBestShip(shipMap, cargos[i]);
    if (bestShip) {
      bestShip->loadCargo(cargos[i]);
      std::cout << "Loading " << cargos[i].name << " onto " << bestShip->name << " from "
                << cargos[i].source << " to " << cargos[i].destination << " "
                << bestShip->getRemainingCapacity() << " capacity remains" << std::endl;
    }
    else {
      std::cout << "No ships can carry the " << cargos[i].name << " from "
                << cargos[i].source << " to " << cargos[i].destination << std::endl;
    }
  }

  std::cout << "---Done Loading---Here are the ships---" << std::endl;
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i].printDetails();
  }
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ship file> <cargo file>" << std::endl;
    return 1;
  }

  processCargoAndShips(argv[1], argv[2]);
  return 0;
}
