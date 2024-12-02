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
    weight = std::strtoull(temp.c_str(), NULL, 10);
    while (std::getline(ss, temp, ',')) {
      properties.push_back(temp);
    }
  }

  bool requiresProperty(const std::string & property) const {
    return std::find(properties.begin(), properties.end(), property) != properties.end();
  }
};

class Ship {
 protected:
  std::string name;
  std::string source;
  std::string destination;
  unsigned int totalCapacity;
  unsigned int usedCapacity;
  unsigned int slots;
  unsigned int usedSlots;
  std::vector<std::string> hazmatCapabilities;
  std::vector<Cargo> loadedCargo;

 public:
  Ship(const std::string & line) : usedCapacity(0), usedSlots(0) {
    std::istringstream ss(line);
    std::string typeInfo, temp;
    std::getline(ss, name, ':');
    std::getline(ss, typeInfo, ':');
    std::getline(ss, source, ':');
    std::getline(ss, destination, ':');
    std::getline(ss, temp, ':');
    totalCapacity = std::strtoull(temp.c_str(), NULL, 10);

    parseTypeInfo(typeInfo);
  }

  const std::string & getName() const { return name; }

  virtual ~Ship() {}

  virtual bool canCarry(const Cargo & cargo) const {
    if (cargo.source != source || cargo.destination != destination) {
      return false;
    }
    if (usedCapacity + cargo.weight > totalCapacity) {
      return false;
    }
    if (usedSlots >= slots) {
      return false;
    }
    for (size_t i = 0; i < cargo.properties.size(); ++i) {
      const std::string & property = cargo.properties[i];
      if (property.find("hazardous-") == 0) {
        std::string hazard = property.substr(10);
        if (std::find(hazmatCapabilities.begin(), hazmatCapabilities.end(), hazard) ==
            hazmatCapabilities.end()) {
          return false;
        }
      }
    }
    return true;
  }

  virtual void loadCargo(const Cargo & cargo) {
    usedCapacity += cargo.weight;
    usedSlots++;
    loadedCargo.push_back(cargo);
  }

  virtual void printDetails() const {
    std::cout << "The Container Ship " << name << " (" << usedCapacity << "/"
              << totalCapacity << ") is carrying:" << std::endl;
    for (size_t i = 0; i < loadedCargo.size(); ++i) {
      const Cargo & cargo = loadedCargo[i];
      std::cout << "  " << cargo.name << " (" << cargo.weight << ")" << std::endl;
    }
    std::cout << "  (" << (slots - usedSlots) << ") slots remain" << std::endl;
  }

 protected:
  virtual void parseTypeInfo(const std::string & typeInfo) {
    std::istringstream ss(typeInfo);
    std::string temp;
    std::getline(ss, temp, ',');  // Skip "Container"
    std::getline(ss, temp, ',');
    slots = static_cast<unsigned int>(std::atoi(temp.c_str()));
    while (std::getline(ss, temp, ',')) {
      hazmatCapabilities.push_back(temp);
    }
  }
};

// Helper function for sorting ships by name
bool compareShipsByName(const Ship * a, const Ship * b) {
  return a->getName() < b->getName();
}

void readShips(const std::string & filename, std::vector<Ship> & ships) {
  std::ifstream file(filename.c_str());
  if (!file) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (std::getline(file, line)) {
    ships.push_back(Ship(line));
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

void processCargo(std::vector<Ship> & ships, const std::vector<Cargo> & cargoList) {
  for (size_t i = 0; i < cargoList.size(); ++i) {
    const Cargo & cargo = cargoList[i];
    std::vector<Ship *> possibleShips;
    for (size_t j = 0; j < ships.size(); ++j) {
      if (ships[j].canCarry(cargo)) {
        possibleShips.push_back(&ships[j]);
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
    ships[i].printDetails();
  }
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships_file> <cargo_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Ship> ships;
  readShips(argv[1], ships);

  std::vector<Cargo> cargoList;
  readCargo(argv[2], cargoList);

  processCargo(ships, cargoList);

  return EXIT_SUCCESS;
}
