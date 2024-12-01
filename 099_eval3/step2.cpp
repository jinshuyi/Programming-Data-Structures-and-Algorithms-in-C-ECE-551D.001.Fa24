#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// 声明Ship和FleetManager类
class Ship {
 public:
  std::string name;
  unsigned int capacity;
  unsigned int slots;
  std::vector<std::string> hazmatCapabilities;
  unsigned int usedCapacity;
  unsigned int loadedCargo;

  Ship(std::string n, unsigned int cap, unsigned int s, std::vector<std::string> hazmat) :
      name(n),
      capacity(cap),
      slots(s),
      hazmatCapabilities(hazmat),
      usedCapacity(0),
      loadedCargo(0) {}

  bool canCarryCargo(unsigned int weight, std::vector<std::string> cargoHazmat) {
    if (usedCapacity + weight > capacity) {
      return false;
    }
    if (loadedCargo >= slots) {
      return false;
    }

    for (size_t i = 0; i < cargoHazmat.size(); i++) {
      bool matchFound = false;
      for (size_t j = 0; j < hazmatCapabilities.size(); j++) {
        if (cargoHazmat[i] == hazmatCapabilities[j]) {
          matchFound = true;
          break;
        }
      }
      if (!matchFound) {
        return false;
      }
    }
    return true;
  }

  void loadCargo(std::string cargoName, unsigned int weight) {
    usedCapacity += weight;
    loadedCargo++;
    std::cout << "  " << cargoName << "(" << weight << ")" << std::endl;
  }

  void printShipDetails() {
    std::cout << "The Container Ship " << name << "(" << usedCapacity << "/" << capacity
              << ") is carrying :" << std::endl;
  }
};

// 将CompareShipNames移到类外部
struct CompareShipNames {
  bool operator()(Ship * a, Ship * b) { return a->name < b->name; }
};

class FleetManager {
 private:
  std::vector<Ship> ships;

 public:
  void loadShips(const char * filename) {  // 使用const char*类型
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string name, temp;
      unsigned int capacity, slots;
      std::vector<std::string> hazmatCapabilities;

      std::getline(ss, name, ',');
      ss >> capacity >> slots;
      while (std::getline(ss, temp, ',')) {
        hazmatCapabilities.push_back(temp);
      }

      Ship ship(name, capacity, slots, hazmatCapabilities);
      ships.push_back(ship);
    }
  }

  void loadCargoAndPrintResults(const char * cargoFilename) {  // 使用const char*类型
    std::ifstream cargoFile(cargoFilename);
    std::string line;

    while (std::getline(cargoFile, line)) {
      std::stringstream ss(line);
      std::string cargoName, source, destination, properties;
      unsigned int weight;
      std::vector<std::string> cargoHazmat;

      std::getline(ss, cargoName, ',');
      std::getline(ss, source, ',');
      std::getline(ss, destination, ',');
      ss >> weight;
      ss.ignore();
      std::getline(ss, properties);

      // 解析cargo的hazmat信息
      std::stringstream propStream(properties);
      std::string prop;
      while (std::getline(propStream, prop, ',')) {
        cargoHazmat.push_back(prop);
      }

      // 寻找符合条件的船只
      std::vector<Ship *> eligibleShips;
      for (size_t i = 0; i < ships.size(); ++i) {
        if (ships[i].canCarryCargo(weight, cargoHazmat)) {
          eligibleShips.push_back(&ships[i]);
        }
      }

      if (eligibleShips.empty()) {
        std::cout << "No ships can carry the " << cargoName << " from " << source
                  << " to " << destination << std::endl;
      }
      else {
        std::cout << eligibleShips.size() << " ships can carry the " << cargoName
                  << " from " << source << " to " << destination << std::endl;

        // 排序船只
        std::sort(eligibleShips.begin(), eligibleShips.end(), CompareShipNames());

        for (size_t i = 0; i < eligibleShips.size(); ++i) {
          std::cout << "  " << eligibleShips[i]->name << std::endl;
        }

        // 装载货物
        eligibleShips[0]->loadCargo(cargoName, weight);
        std::cout << "**Loading the cargo onto " << eligibleShips[0]->name << "**"
                  << std::endl;
      }
    }

    // 完成加载后打印船只详细信息
    std::cout << "---Done Loading---Here are the ships---" << std::endl;
    for (size_t i = 0; i < ships.size(); ++i) {
      ships[i].printShipDetails();
      for (size_t j = 0; j < ships[i].loadedCargo; ++j) {
        // 输出已经加载的货物
        ships[i].loadCargo("Cargo", 10);  // 货物名称和重量按需更新
      }
      std::cout << "(" << (ships[i].slots - ships[i].loadedCargo) << ") slots remain"
                << std::endl;
    }
  }
};

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships file> <cargo file>" << std::endl;
    return 1;
  }

  FleetManager fleetManager;
  fleetManager.loadShips(argv[1]);
  fleetManager.loadCargoAndPrintResults(argv[2]);

  return 0;
}
