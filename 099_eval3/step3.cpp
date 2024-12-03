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
  std::string routeStart;
  std::string routeEnd;
  unsigned int capacity;
  unsigned int usedCapacity;

 public:
  Ship(const std::string & n,
       const std::string & start,
       const std::string & end,
       unsigned int cap) :
      name(n), routeStart(start), routeEnd(end), capacity(cap), usedCapacity(0) {}

  virtual ~Ship() {}

  virtual bool canCarry(
      const std::map<std::string, std::string> & cargoProperties) const = 0;
  virtual void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                         unsigned int weight) = 0;
  virtual void printInfo() const = 0;

  const std::string & getName() const { return name; }
  bool isOnRoute(const std::string & start, const std::string & end) const {
    return routeStart == start && routeEnd == end;
  }
};

// Derived ContainerShip
class ContainerShip : public Ship {
 public:
  ContainerShip(const std::string & n,
                const std::string & start,
                const std::string & end,
                unsigned int cap) :
      Ship(n, start, end, cap) {}

  bool canCarry(const std::map<std::string, std::string> & cargoProperties) const {
    return (usedCapacity + std::atoi(cargoProperties.at("weight").c_str()) <= capacity) &&
           (cargoProperties.find("container") != cargoProperties.end());
  }

  void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                 unsigned int weight) {
    usedCapacity += weight;
  }

  void printInfo() const {
    std::cout << "The Container Ship " << name << " (" << usedCapacity << "/" << capacity
              << ") is carrying:\n";
  }
};

// Derived TankerShip
class TankerShip : public Ship {
  int minTemp, maxTemp;
  unsigned int numTanks, tanksUsed;

 public:
  TankerShip(const std::string & n,
             const std::string & start,
             const std::string & end,
             unsigned int cap,
             int minT,
             int maxT,
             unsigned int tanks) :
      Ship(n, start, end, cap),
      minTemp(minT),
      maxTemp(maxT),
      numTanks(tanks),
      tanksUsed(0) {}

  bool canCarry(const std::map<std::string, std::string> & cargoProperties) const {
    if (cargoProperties.find("liquid") == cargoProperties.end() &&
        cargoProperties.find("gas") == cargoProperties.end()) {
      return false;
    }
    if (cargoProperties.find("mintemp") != cargoProperties.end()) {
      int cargoMinTemp = std::atoi(cargoProperties.at("mintemp").c_str());
      if (cargoMinTemp > maxTemp)
        return false;
    }
    if (cargoProperties.find("maxtemp") != cargoProperties.end()) {
      int cargoMaxTemp = std::atoi(cargoProperties.at("maxtemp").c_str());
      if (cargoMaxTemp < minTemp)
        return false;
    }
    return (usedCapacity + std::atoi(cargoProperties.at("weight").c_str()) <= capacity);
  }

  void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                 unsigned int weight) {
    usedCapacity += weight;
    ++tanksUsed;
  }

  void printInfo() const {
    std::cout << "The Tanker Ship " << name << " (" << usedCapacity << "/" << capacity
              << ") is carrying:\n";
    std::cout << "  " << tanksUsed << " / " << numTanks << " tanks used\n";
  }
};

// Derived AnimalShip
class AnimalShip : public Ship {
  unsigned int smallEnoughThreshold;
  bool hasRoamer;

 public:
  AnimalShip(const std::string & n,
             const std::string & start,
             const std::string & end,
             unsigned int cap,
             unsigned int threshold) :
      Ship(n, start, end, cap), smallEnoughThreshold(threshold), hasRoamer(false) {}

  bool canCarry(const std::map<std::string, std::string> & cargoProperties) const {
    if (cargoProperties.find("animal") != cargoProperties.end()) {
      if (cargoProperties.find("roamer") != cargoProperties.end() && hasRoamer) {
        return false;
      }
      return true;
    }
    unsigned int weight = std::atoi(cargoProperties.at("weight").c_str());
    return (weight <= smallEnoughThreshold) && (usedCapacity + weight <= capacity);
  }

  void loadCargo(const std::map<std::string, std::string> & cargoProperties,
                 unsigned int weight) {
    usedCapacity += weight;
    if (cargoProperties.find("roamer") != cargoProperties.end()) {
      hasRoamer = true;
    }
  }

  void printInfo() const {
    std::cout << "The Animals Ship " << name << " (" << usedCapacity << "/" << capacity
              << ") is carrying:\n";
    std::cout << (hasRoamer ? "  has a roamer\n" : "  does not have a roamer\n");
  }
};

// Helper functions
std::vector<Ship *> loadShips(const std::string & filename) {
  std::ifstream file(filename);
  std::vector<Ship *> ships;
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string name, type, start, end;
    unsigned int capacity;
    iss >> name >> type >> start >> end >> capacity;

    if (type == "Container") {
      ships.push_back(new ContainerShip(name, start, end, capacity));
    }
    else if (type == "Tanker") {
      int minTemp, maxTemp;
      unsigned int numTanks;
      iss >> minTemp >> maxTemp >> numTanks;
      ships.push_back(
          new TankerShip(name, start, end, capacity, minTemp, maxTemp, numTanks));
    }
    else if (type == "Animals") {
      unsigned int threshold;
      iss >> threshold;
      ships.push_back(new AnimalShip(name, start, end, capacity, threshold));
    }
  }
  return ships;
}

std::vector<std::map<std::string, std::string> > loadCargo(const std::string & filename) {
  std::ifstream file(filename);
  std::vector<std::map<std::string, std::string> > cargoList;
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string name, start, end, weightStr;
    iss >> name >> start >> end >> weightStr;

    std::map<std::string, std::string> cargoProperties;
    cargoProperties["name"] = name;
    cargoProperties["start"] = start;
    cargoProperties["end"] = end;
    cargoProperties["weight"] = weightStr;

    std::string property;
    while (iss >> property) {
      size_t equalPos = property.find('=');
      if (equalPos != std::string::npos) {
        std::string key = property.substr(0, equalPos);
        std::string value = property.substr(equalPos + 1);
        cargoProperties[key] = value;
      }
      else {
        cargoProperties[property] = "0";
      }
    }
    cargoList.push_back(cargoProperties);
  }
  return cargoList;
}

int main() {
  std::vector<Ship *> fleet = loadShips("ships.txt");
  std::vector<std::map<std::string, std::string> > cargoList = loadCargo("cargo.txt");

  for (size_t i = 0; i < cargoList.size(); ++i) {
    const std::map<std::string, std::string> & cargo = cargoList[i];
    bool loaded = false;

    for (size_t j = 0; j < fleet.size(); ++j) {
      if (fleet[j]->isOnRoute(cargo.at("start"), cargo.at("end")) &&
          fleet[j]->canCarry(cargo)) {
        fleet[j]->loadCargo(cargo, std::atoi(cargo.at("weight").c_str()));
        std::cout << "**Loading the cargo onto " << fleet[j]->getName() << "**\n";
        loaded = true;
        break;
      }
    }
    if (!loaded) {
      std::cout << "No ships can carry the cargo " << cargo.at("name") << " from "
                << cargo.at("start") << " to " << cargo.at("end") << "\n";
    }
  }

  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < fleet.size(); ++i) {
    fleet[i]->printInfo();
    delete fleet[i];
  }

  return 0;
}
