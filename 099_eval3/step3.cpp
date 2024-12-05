#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Base class for Ship
class Ship {
 protected:
  std::string name;
  std::string type;
  unsigned int capacity;
  std::vector<std::string> routes;
  unsigned int used_capacity;

 public:
  Ship(const std::string & name,
       const std::string & type,
       unsigned int capacity,
       const std::vector<std::string> & routes) :
      name(name), type(type), capacity(capacity), routes(routes), used_capacity(0) {}

  virtual bool canCarry(
      const std::map<std::string, std::string> & cargoProperties) const = 0;
  virtual void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                         unsigned int weight) = 0;

  virtual void printStatus() const {
    std::cout << "The " << type << " Ship " << name << "(" << used_capacity << "/"
              << capacity << ") is carrying :\n";
  }

  virtual ~Ship() {}
};

// TankerShip class
class TankerShip : public Ship {
  int min_temp;
  int max_temp;
  unsigned int tanks;
  unsigned int used_tanks;

  struct Tank {
    unsigned int capacity;
    std::string cargoType;
    unsigned int used;
    Tank(unsigned int capacity, const std::string & cargoType, unsigned int used) :
        capacity(capacity), cargoType(cargoType), used(used) {}
  };

  std::vector<Tank> tankInfo;

 public:
  TankerShip(const std::string & name,
             unsigned int capacity,
             const std::vector<std::string> & routes,
             int min_temp,
             int max_temp,
             unsigned int tanks) :
      Ship(name, "Tanker", capacity, routes),
      min_temp(min_temp),
      max_temp(max_temp),
      tanks(tanks),
      used_tanks(0) {
    if (capacity % tanks != 0) {
      throw std::runtime_error(
          "Invalid tanker capacity: not divisible by number of tanks");
    }
    unsigned int tank_capacity = capacity / tanks;
    for (unsigned int i = 0; i < tanks; ++i) {
      tankInfo.push_back(Tank(tank_capacity, "", 0));  // Modified for C++03
    }
  }

  virtual bool canCarry(
      const std::map<std::string, std::string> & cargoProperties) const {
    if (cargoProperties.find("liquid") == cargoProperties.end() &&
        cargoProperties.find("gas") == cargoProperties.end()) {
      return false;
    }

    if (cargoProperties.find("mintemp") != cargoProperties.end()) {
      int required_min_temp = std::atoi(cargoProperties.at("mintemp").c_str());
      if (required_min_temp > max_temp)
        return false;
    }

    if (cargoProperties.find("maxtemp") != cargoProperties.end()) {
      int required_max_temp = std::atoi(cargoProperties.at("maxtemp").c_str());
      if (required_max_temp < min_temp)
        return false;
    }

    return true;
  }

  virtual void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                         unsigned int weight) {
    for (unsigned int i = 0; i < tanks; ++i) {
      if (tankInfo[i].cargoType == cargoProperties.at("name") ||
          tankInfo[i].cargoType.empty()) {
        unsigned int available_capacity = tankInfo[i].capacity - tankInfo[i].used;
        unsigned int load_amount = std::min(weight, available_capacity);

        tankInfo[i].used += load_amount;
        weight -= load_amount;

        if (tankInfo[i].cargoType.empty() && load_amount > 0) {
          tankInfo[i].cargoType = cargoProperties.at("name");
        }

        if (weight == 0)
          break;
      }
    }

    if (weight > 0) {
      throw std::runtime_error("Insufficient capacity in TankerShip");
    }

    used_capacity += weight;
  }

  virtual void printStatus() const {
    Ship::printStatus();
    unsigned int tanks_used = 0;
    for (unsigned int i = 0; i < tanks; ++i) {
      if (tankInfo[i].used > 0)
        ++tanks_used;
    }
    std::cout << " " << tanks_used << " / " << tanks << " tanks used\n";
  }
};

// AnimalShip class
class AnimalShip : public Ship {
  unsigned int smallCargoLimit;
  bool hasRoamer;

 public:
  AnimalShip(const std::string & name,
             unsigned int capacity,
             const std::vector<std::string> & routes,
             unsigned int smallCargoLimit) :
      Ship(name, "Animals", capacity, routes),
      smallCargoLimit(smallCargoLimit),
      hasRoamer(false) {}

  virtual bool canCarry(
      const std::map<std::string, std::string> & cargoProperties) const {
    if (cargoProperties.find("animal") != cargoProperties.end()) {
      if (cargoProperties.find("roamer") != cargoProperties.end() && hasRoamer) {
        return false;
      }
    }
    else {
      if (cargoProperties.find("liquid") != cargoProperties.end() ||
          cargoProperties.find("gas") != cargoProperties.end()) {
        return false;
      }
      if (cargoProperties.find("hazardous") != cargoProperties.end()) {
        return false;
      }
      unsigned int weight = std::atoi(cargoProperties.at("weight").c_str());
      if (weight > smallCargoLimit)
        return false;
    }
    return true;
  }

  virtual void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                         unsigned int weight) {
    if (!canCarry(cargoProperties))
      return;

    if (cargoProperties.find("animal") != cargoProperties.end()) {
      if (cargoProperties.find("roamer") != cargoProperties.end()) {
        hasRoamer = true;
      }
    }

    used_capacity += weight;
  }

  virtual void printStatus() const {
    Ship::printStatus();
    std::cout << (hasRoamer ? " has a roamer\n" : " does not have a roamer\n");
  }
};

// Utility functions
std::vector<std::map<std::string, std::string> > parseCargo(
    const std::string & filename) {
  std::ifstream file(filename.c_str());
  std::vector<std::map<std::string, std::string> > cargoList;
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::map<std::string, std::string> cargo;
    std::string field;
    std::getline(iss, field, ',');
    cargo["name"] = field;
    std::getline(iss, field, ',');
    cargo["origin"] = field;
    std::getline(iss, field, ',');
    cargo["destination"] = field;
    std::getline(iss, field, ',');
    cargo["weight"] = field;

    while (std::getline(iss, field, ',')) {
      size_t eq = field.find('=');
      if (eq != std::string::npos) {
        cargo[field.substr(0, eq)] = field.substr(eq + 1);
      }
      else {
        cargo[field] = "0";
      }
    }
    cargoList.push_back(cargo);
  }
  return cargoList;
}

// Main function
int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships_file> <cargo_file>\n";
    return EXIT_FAILURE;
  }

  std::vector<Ship *> ships;
  std::vector<std::map<std::string, std::string> > cargoList = parseCargo(argv[2]);

  // Read ships and initialize ship objects (parsing logic to be added)
  // Simulated for clarity.

  for (size_t i = 0; i < cargoList.size(); ++i) {
    const std::map<std::string, std::string> & cargo = cargoList[i];
    bool loaded = false;

    for (size_t j = 0; j < ships.size(); ++j) {
      if (ships[j]->canCarry(cargo)) {
        ships[j]->loadCargo(cargo, std::atoi(cargo.at("weight").c_str()));
        loaded = true;
        break;
      }
    }

    if (!loaded) {
      std::cout << "No ships can carry " << cargo.at("name") << " from "
                << cargo.at("origin") << " to " << cargo.at("destination") << "\n";
    }
  }

  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printStatus();
    delete ships[i];
  }

  return 0;
}
