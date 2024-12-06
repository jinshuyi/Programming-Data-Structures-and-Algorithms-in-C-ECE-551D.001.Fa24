#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Cargo class
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
    return 0;  // Default value
  }
};

// Base class: Ship
class Ship {
 protected:
  std::string name;
  std::string source;
  std::string destination;
  unsigned int totalCapacity;
  unsigned int usedCapacity;

 public:
  Ship(const std::string & name,
       const std::string & source,
       const std::string & destination,
       unsigned int capacity) :
      name(name),
      source(source),
      destination(destination),
      totalCapacity(capacity),
      usedCapacity(0) {}

  virtual ~Ship() {}

  virtual bool canCarry(const Cargo & cargo) const = 0;
  virtual void loadCargo(const Cargo & cargo) = 0;
  virtual void printDetails() const = 0;

  const std::string & getName() const { return name; }

  bool isOnRoute(const Cargo & cargo) const {
    return cargo.source == source && cargo.destination == destination;
  }
};

// Derived class: ContainerShip
class ContainerShip : public Ship {
  unsigned int slots;
  unsigned int usedSlots;
  std::vector<Cargo> loadedCargo;

 public:
  ContainerShip(const std::string & name,
                const std::string & source,
                const std::string & destination,
                unsigned int capacity,
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

// Derived class: AnimalsShip
class AnimalsShip : public Ship {
  unsigned int smallCargoLimit;
  bool hasRoamer;
  std::vector<Cargo> loadedCargo;

 public:
  AnimalsShip(const std::string & name,
              const std::string & source,
              const std::string & destination,
              unsigned int capacity,
              unsigned int smallCargoLimit) :
      Ship(name, source, destination, capacity),
      smallCargoLimit(smallCargoLimit),
      hasRoamer(false) {}

  bool canCarry(const Cargo & cargo) const {
    if (!isOnRoute(cargo) || usedCapacity + cargo.weight > totalCapacity) {
      return false;
    }
    if (cargo.requiresProperty("animal")) {
      if (!cargo.requiresProperty("roamer")) {
        return true;
      }
      else {
        return !hasRoamer;
      }
    }
    return cargo.weight <= smallCargoLimit && !cargo.requiresProperty("liquid") &&
           !cargo.requiresProperty("gas");
  }

  void loadCargo(const Cargo & cargo) {
    usedCapacity += cargo.weight;
    if (cargo.requiresProperty("animal") && cargo.requiresProperty("roamer")) {
      hasRoamer = true;
    }
    loadedCargo.push_back(cargo);
  }

  void printDetails() const {
    std::cout << "The Animals Ship " << name << " (" << usedCapacity << "/"
              << totalCapacity << ") is carrying:\n";
    for (size_t i = 0; i < loadedCargo.size(); ++i) {
      std::cout << "  " << loadedCargo[i].name << " (" << loadedCargo[i].weight << ")\n";
    }
    std::cout << "  " << (hasRoamer ? "has a roamer" : "does not have a roamer") << "\n";
  }
};

// Derived class: TankerShip
class TankerShip : public Ship {
  int minTemp;
  int maxTemp;
  unsigned int tanks;
  unsigned int usedTanks;
  std::vector<Cargo> loadedCargo;

 public:
  TankerShip(const std::string & name,
             const std::string & source,
             const std::string & destination,
             unsigned int capacity,
             int minTemp,
             int maxTemp,
             unsigned int tanks) :
      Ship(name, source, destination, capacity),
      minTemp(minTemp),
      maxTemp(maxTemp),
      tanks(tanks),
      usedTanks(0) {}

  bool canCarry(const Cargo & cargo) const {
    if (!isOnRoute(cargo) || usedCapacity + cargo.weight > totalCapacity) {
      return false;
    }
    if (cargo.requiresProperty("liquid") || cargo.requiresProperty("gas")) {
      int cargoMinTemp = cargo.getPropertyValue("mintemp");
      int cargoMaxTemp = cargo.getPropertyValue("maxtemp");
      return (cargoMaxTemp >= minTemp && cargoMinTemp <= maxTemp);
    }
    return false;
  }

  void loadCargo(const Cargo & cargo) {
    usedCapacity += cargo.weight;
    usedTanks++;
    loadedCargo.push_back(cargo);
  }

  void printDetails() const {
    std::cout << "The Tanker Ship " << name << " (" << usedCapacity << "/"
              << totalCapacity << ") is carrying:\n";
    for (size_t i = 0; i < loadedCargo.size(); ++i) {
      std::cout << "  " << loadedCargo[i].name << " (" << loadedCargo[i].weight << ")\n";
    }
    std::cout << "  " << usedTanks << " / " << tanks << " tanks used\n";
  }
};

// Comparator for sorting ships by name
bool compareShipsByName(Ship * a, Ship * b) {
  return a->getName() < b->getName();
}

// Function to create ships dynamically
Ship * createShip(const std::string & line) {
  std::istringstream ss(line);
  std::string name, typeInfo, source, destination, temp;
  unsigned int capacity;

  std::getline(ss, name, ':');
  std::getline(ss, typeInfo, ':');
  std::getline(ss, source, ':');
  std::getline(ss, destination, ':');
  std::getline(ss, temp, ':');
  capacity = std::strtoull(temp.c_str(), NULL, 10);

  std::istringstream typeStream(typeInfo);
  std::getline(typeStream, temp, ',');  // Read ship type

  if (temp == "Container") {
    unsigned int slots;
    std::getline(typeStream, temp, ',');
    slots = std::atoi(temp.c_str());
    return new ContainerShip(name, source, destination, capacity, slots);
  }
  else if (temp == "Animals") {
    unsigned int smallCargoLimit;
    std::getline(typeStream, temp, ',');
    smallCargoLimit = std::atoi(temp.c_str());
    return new AnimalsShip(name, source, destination, capacity, smallCargoLimit);
  }
  else if (temp == "Tanker") {
    int minTemp, maxTemp;
    unsigned int tanks;
    std::getline(typeStream, temp, ',');
    minTemp = std::atoi(temp.c_str());
    std::getline(typeStream, temp, ',');
    maxTemp = std::atoi(temp.c_str());
    std::getline(typeStream, temp, ',');
    tanks = std::atoi(temp.c_str());
    return new TankerShip(name, source, destination, capacity, minTemp, maxTemp, tanks);
  }

  std::cerr << "Unknown ship type: " << temp << std::endl;
  return NULL;
}

// Function to read ships from file
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
    else {
      std::cerr << "Error: Ship creation failed for line: " << line << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

// Function to read cargo from file
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

// Function to process and load cargo
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

    std::cout << "  **Loading the cargo onto " << selectedShip->getName() << "**\n";
  }

  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
  }
}

// Main function
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
