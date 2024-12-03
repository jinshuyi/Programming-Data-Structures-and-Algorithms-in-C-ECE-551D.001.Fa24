#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

  bool requiresProperty(const std::string & property) const {
    return std::find(properties.begin(), properties.end(), property) != properties.end();
  }

  int getPropertyValue(const std::string & property) const {
    for (size_t i = 0; i < properties.size(); ++i) {
      if (properties[i].find(property + "=") == 0) {
        return std::atoi(properties[i].substr(property.size() + 1).c_str());
      }
      else if (properties[i] == property) {
        return 0;
      }
    }
    return -1;  // Property not found
  }
};

class Ship {
 protected:
  std::string name;
  std::string source;
  std::string destination;
  unsigned int totalCapacity;
  unsigned int usedCapacity;

 public:
  Ship(const std::string & line) : usedCapacity(0) {
    std::istringstream ss(line);
    std::getline(ss, name, ':');
    std::getline(ss, source, ':');
    std::getline(ss, destination, ':');
    std::string temp;
    std::getline(ss, temp, ':');
    totalCapacity = std::strtoul(temp.c_str(), NULL, 10);
  }

  virtual ~Ship() {}

  const std::string & getName() const { return name; }

  virtual bool canCarry(const Cargo & cargo) const = 0;

  virtual void loadCargo(const Cargo & cargo) = 0;

  virtual void printDetails() const = 0;
};

// ContainerShip Class
class ContainerShip : public Ship {
 public:
  ContainerShip(const std::string & line) : Ship(line) {}

  bool canCarry(const Cargo & cargo) const {
    return cargo.requiresProperty("container") &&
           cargo.weight + usedCapacity <= totalCapacity;
  }

  void loadCargo(const Cargo & cargo) { usedCapacity += cargo.weight; }

  void printDetails() const {
    std::cout << "The Container Ship " << name << "(" << usedCapacity << "/"
              << totalCapacity << ") is carrying :\n";
    std::cout << "  (0) slots remain\n";
  }
};

// Define TankerShip and AnimalShip similarly
// ...

bool compareShipsByName(Ship * a, Ship * b) {
  return a->getName() < b->getName();
}

Ship * createShip(const std::string & line) {
  if (line.find("Container") != std::string::npos) {
    return new ContainerShip(line);
  }
  // Add logic for TankerShip and AnimalShip
  return NULL;  // Placeholder, add appropriate logic
}

void readShips(const std::string & filename, std::vector<Ship *> & ships) {
  std::ifstream file(filename.c_str());
  if (!file) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string line;
  while (std::getline(file, line)) {
    Ship * ship = createShip(line);
    if (ship) {
      ships.push_back(ship);
    }
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

void processCargo(std::vector<Ship *> & ships, const std::vector<Cargo> & cargoList) {
  for (size_t i = 0; i < cargoList.size(); ++i) {
    const Cargo & cargo = cargoList[i];
    std::vector<Ship *> possibleShips;

    for (size_t j = 0; j < ships.size(); ++j) {
      if (ships[j]->canCarry(cargo)) {
        possibleShips.push_back(ships[j]);
      }
    }

    if (possibleShips.empty()) {
      std::cout << "No ships can carry the " << cargo.name << " from " << cargo.source
                << " to " << cargo.destination << std::endl;
      continue;
    }

    std::sort(possibleShips.begin(), possibleShips.end(), compareShipsByName);

    std::cout << possibleShips.size() << " ships can carry the " << cargo.name << " from "
              << cargo.source << " to " << cargo.destination << std::endl;
    for (size_t k = 0; k < possibleShips.size(); ++k) {
      std::cout << "  " << possibleShips[k]->getName() << std::endl;
    }

    Ship * selectedShip = possibleShips[0];
    selectedShip->loadCargo(cargo);
    std::cout << "**Loading the cargo onto " << selectedShip->getName() << "**"
              << std::endl;
  }

  std::cout << "---Done Loading---Here are the ships---" << std::endl;
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
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
