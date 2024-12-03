#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Helper function to parse properties
std::map<std::string, std::string> parseProperties(const std::string & propertiesStr) {
  std::map<std::string, std::string> properties;
  std::istringstream ss(propertiesStr);
  std::string property;
  while (std::getline(ss, property, ',')) {
    size_t equalPos = property.find('=');
    if (equalPos != std::string::npos) {
      properties[property.substr(0, equalPos)] = property.substr(equalPos + 1);
    }
    else {
      properties[property] = "0";  // Default value for properties without values
    }
  }
  return properties;
}

// Abstract base class for ships
class Ship {
 protected:
  std::string name;
  std::string source;
  std::string destination;
  unsigned int capacity;

 public:
  Ship(const std::string & line) {
    std::istringstream ss(line);
    std::getline(ss, name, ':');
    std::string typeInfo;
    std::getline(ss, typeInfo, ':');
    std::getline(ss, source, ':');
    std::getline(ss, destination, ':');
    std::string capacityStr;
    std::getline(ss, capacityStr, ':');
    capacity = std::atoi(capacityStr.c_str());
    // The derived class will handle type-specific parsing
  }

  virtual void parseTypeInfo(const std::string & typeInfo) = 0;
  virtual bool canCarry(const std::string & cargoType,
                        const std::map<std::string, std::string> & properties) const = 0;
  virtual void loadCargo(const std::string & cargoType, unsigned int weight) = 0;
  virtual void printDetails() const = 0;

  virtual ~Ship() {}
};

// TankerShip class
class TankerShip : public Ship {
  int minTemp;
  int maxTemp;
  unsigned int numTanks;
  unsigned int tanksUsed;
  std::map<std::string, unsigned int> cargoMap;

 public:
  TankerShip(const std::string & line) : Ship(line), tanksUsed(0) {
    std::string typeInfo = line.substr(
        line.find(':') + 1, line.find(':', line.find(':') + 1) - line.find(':') - 1);
    parseTypeInfo(typeInfo);
  }

  void parseTypeInfo(const std::string & typeInfo) {
    std::istringstream ss(typeInfo);
    std::string temp;
    std::getline(ss, temp, ',');
    minTemp = std::atoi(temp.c_str());
    std::getline(ss, temp, ',');
    maxTemp = std::atoi(temp.c_str());
    std::getline(ss, temp, ',');
    numTanks = std::atoi(temp.c_str());
    if (capacity % numTanks != 0) {
      throw std::runtime_error("Capacity must be a multiple of the number of tanks.");
    }
  }

  bool canCarry(const std::string & cargoType,
                const std::map<std::string, std::string> & properties) const {
    if (cargoType != "liquid" && cargoType != "gas")
      return false;

    if (properties.find("mintemp") != properties.end() ||
        properties.find("maxtemp") != properties.end()) {
      int cargoMinTemp = properties.find("mintemp") != properties.end()
                             ? std::atoi(properties.find("mintemp")->second.c_str())
                             : -1000;
      int cargoMaxTemp = properties.find("maxtemp") != properties.end()
                             ? std::atoi(properties.find("maxtemp")->second.c_str())
                             : 1000;
      if (cargoMaxTemp < minTemp || cargoMinTemp > maxTemp)
        return false;
    }
    return true;
  }

  void loadCargo(const std::string & cargoType, unsigned int weight) {
    unsigned int tankCapacity = capacity / numTanks;
    unsigned int remainingWeight = weight;

    for (std::map<std::string, unsigned int>::iterator it = cargoMap.begin();
         it != cargoMap.end();
         ++it) {
      if (it->first == cargoType) {
        unsigned int freeSpace = tankCapacity - it->second;
        unsigned int toLoad = std::min(remainingWeight, freeSpace);
        it->second += toLoad;
        remainingWeight -= toLoad;
        if (remainingWeight == 0)
          break;
      }
    }

    while (remainingWeight > 0 && tanksUsed < numTanks) {
      unsigned int toLoad = std::min(remainingWeight, tankCapacity);
      cargoMap[cargoType] += toLoad;
      tanksUsed++;
      remainingWeight -= toLoad;
    }

    if (remainingWeight > 0) {
      throw std::runtime_error("Not enough tank capacity to load cargo.");
    }
  }

  void printDetails() const {
    std::cout << "The Tanker Ship " << name << "("
              << capacity - (numTanks * (capacity / numTanks - tanksUsed)) << "/"
              << capacity << ") is carrying :" << std::endl;
    for (std::map<std::string, unsigned int>::const_iterator it = cargoMap.begin();
         it != cargoMap.end();
         ++it) {
      std::cout << "  " << it->first << "(" << it->second << ")" << std::endl;
    }
    std::cout << "  " << tanksUsed << " / " << numTanks << " tanks used" << std::endl;
  }
};

// AnimalShip class
class AnimalShip : public Ship {
  unsigned int smallCargoLimit;
  bool hasRoamer;

 public:
  AnimalShip(const std::string & line) : Ship(line), hasRoamer(false) {
    std::string typeInfo = line.substr(
        line.find(':') + 1, line.find(':', line.find(':') + 1) - line.find(':') - 1);
    parseTypeInfo(typeInfo);
  }

  void parseTypeInfo(const std::string & typeInfo) {
    std::istringstream ss(typeInfo);
    std::string temp;
    std::getline(ss, temp, ',');
    smallCargoLimit = std::atoi(temp.c_str());
  }

  bool canCarry(const std::string & cargoType,
                const std::map<std::string, std::string> & properties) const {
    if (cargoType == "animal") {
      if (properties.find("roamer") != properties.end() && hasRoamer)
        return false;
      return true;
    }
    if (properties.find("hazardous") != properties.end() || cargoType == "liquid" ||
        cargoType == "gas")
      return false;
    unsigned int weight = std::atoi(properties.find("weight")->second.c_str());
    return weight <= smallCargoLimit;
  }

  void loadCargo(const std::string & cargoType, unsigned int weight) {
    if (cargoType == "animal") {
      if (hasRoamer)
        throw std::runtime_error("Cannot load multiple roamers.");
      hasRoamer = true;
    }
    capacity -= weight;
  }

  void printDetails() const {
    std::cout << "The Animals Ship " << name << "(" << capacity << "/" << capacity
              << ") is carrying :" << std::endl;
    if (hasRoamer) {
      std::cout << "  has a roamer" << std::endl;
    }
    else {
      std::cout << "  does not have a roamer" << std::endl;
    }
  }
};

// Reading ships
void readShips(const std::string & filename, std::vector<Ship *> & ships) {
  // Implementation
}

// Processing cargo
void processCargo(std::vector<Ship *> & ships, const std::string & cargoFile) {
  // Implementation
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships file> <cargo file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Ship *> ships;
  readShips(argv[1], ships);
  processCargo(ships, argv[2]);

  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
    delete ships[i];
  }
  return EXIT_SUCCESS;
}
