#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Ship base class
class Ship {
 public:
  virtual ~Ship() {}
  virtual bool canCarry(const std::string & cargoType,
                        unsigned weight,
                        const std::map<std::string, std::string> & properties) const = 0;
  virtual void loadCargo(const std::string & cargoName, unsigned weight) = 0;
  virtual void printDetails() const = 0;
  virtual std::string getName() const = 0;
  virtual std::string getRoute() const = 0;
};

// Container ship class
class ContainerShip : public Ship {
  std::string name, source, destination;
  unsigned totalCapacity, usedCapacity;
  unsigned slots, usedSlots;
  std::set<std::string> hazmatCapabilities;
  std::vector<std::pair<std::string, unsigned> > loadedCargo;

 public:
  ContainerShip(const std::string & n,
                const std::string & src,
                const std::string & dest,
                unsigned cap,
                unsigned slot,
                const std::set<std::string> & hazmat) :
      name(n),
      source(src),
      destination(dest),
      totalCapacity(cap),
      usedCapacity(0),
      slots(slot),
      usedSlots(0),
      hazmatCapabilities(hazmat) {}

  bool canCarry(const std::string & cargoType,
                unsigned weight,
                const std::map<std::string, std::string> & properties) const {
    if (cargoType != "container")
      return false;
    if (usedCapacity + weight > totalCapacity)
      return false;
    if (usedSlots >= slots)
      return false;
    for (std::map<std::string, std::string>::const_iterator it = properties.begin();
         it != properties.end();
         ++it) {
      const std::string & propKey = it->first;
      if (propKey.find("hazardous-") == 0 &&
          hazmatCapabilities.count(propKey.substr(10)) == 0) {
        return false;
      }
    }
    return true;
  }

  void loadCargo(const std::string & cargoName, unsigned weight) {
    usedCapacity += weight;
    usedSlots++;
    loadedCargo.push_back(std::make_pair(cargoName, weight));
  }

  void printDetails() const {
    std::cout << "The Container Ship " << name << "(" << usedCapacity << "/"
              << totalCapacity << ") is carrying :\n";
    for (std::vector<std::pair<std::string, unsigned> >::const_iterator it =
             loadedCargo.begin();
         it != loadedCargo.end();
         ++it) {
      std::cout << "  " << it->first << "(" << it->second << ")\n";
    }
    std::cout << "  (" << (slots - usedSlots) << ") slots remain\n";
  }

  std::string getName() const { return name; }
  std::string getRoute() const { return source + " -> " + destination; }
};

// Function to compare ships by name (for sorting)
bool compareShipsByName(Ship * a, Ship * b) {
  return a->getName() < b->getName();
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships file> <cargo file>\n";
    return EXIT_FAILURE;
  }

  std::ifstream shipsFile(argv[1]);
  std::ifstream cargoFile(argv[2]);
  if (!shipsFile.is_open() || !cargoFile.is_open()) {
    std::cerr << "Error opening input files\n";
    return EXIT_FAILURE;
  }

  std::vector<Ship *> ships;
  std::string line;

  // Parse ships
  while (std::getline(shipsFile, line)) {
    std::istringstream iss(line);
    std::string name, type, src, dest, typeInfo;
    unsigned capacity;
    if (std::getline(iss, name, ':') && std::getline(iss, typeInfo, ':') &&
        std::getline(iss, src, ':') && std::getline(iss, dest, ':') && iss >> capacity) {
      if (typeInfo.find("Container") == 0) {
        size_t commaPos = typeInfo.find(',');
        unsigned slots = std::atoi(typeInfo.substr(commaPos + 1).c_str());
        std::set<std::string> hazmat;  // Empty for now
        ships.push_back(new ContainerShip(name, src, dest, capacity, slots, hazmat));
      }
      // Add Tanker and Animal ship parsing here
    }
    else {
      std::cerr << "Error: Malformed ships file\n";
      return EXIT_FAILURE;
    }
  }

  // Parse and process cargo
  while (std::getline(cargoFile, line)) {
    std::istringstream iss(line);
    std::string cargoName, src, dest, weightStr, props;
    unsigned weight;
    if (std::getline(iss, cargoName, ',') && std::getline(iss, src, ',') &&
        std::getline(iss, dest, ',') && std::getline(iss, weightStr, ',')) {
      weight = std::atoi(weightStr.c_str());
      std::getline(iss, props);

      std::map<std::string, std::string> properties;
      std::istringstream propStream(props);
      std::string prop;
      while (std::getline(propStream, prop, ',')) {
        size_t eqPos = prop.find('=');
        if (eqPos == std::string::npos) {
          properties[prop] = "0";
        }
        else {
          properties[prop.substr(0, eqPos)] = prop.substr(eqPos + 1);
        }
      }

      std::vector<Ship *> eligibleShips;
      for (std::vector<Ship *>::iterator it = ships.begin(); it != ships.end(); ++it) {
        if ((*it)->getRoute() == (src + " -> " + dest) &&
            (*it)->canCarry(props, weight, properties)) {
          eligibleShips.push_back(*it);
        }
      }

      if (!eligibleShips.empty()) {
        std::sort(eligibleShips.begin(), eligibleShips.end(), compareShipsByName);

        std::cout << eligibleShips.size() << " ships can carry the " << cargoName
                  << " from " << src << " to " << dest << "\n";
        for (std::vector<Ship *>::iterator it = eligibleShips.begin();
             it != eligibleShips.end();
             ++it) {
          std::cout << "  " << (*it)->getName() << "\n";
        }

        eligibleShips[0]->loadCargo(cargoName, weight);
        std::cout << "**Loading the cargo onto " << eligibleShips[0]->getName() << "**\n";
      }
      else {
        std::cout << "No ships can carry the " << cargoName << " from " << src << " to "
                  << dest << "\n";
      }
    }
    else {
      std::cerr << "Error: Malformed cargo file\n";
      return EXIT_FAILURE;
    }
  }

  // Print ship details
  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
    delete ships[i];
  }

  return EXIT_SUCCESS;
}
