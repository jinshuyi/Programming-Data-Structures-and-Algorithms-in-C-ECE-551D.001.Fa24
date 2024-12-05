#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// 船只类
class Ship {
 private:
  std::string name, source, destination, type;
  unsigned int totalCapacity, usedCapacity, totalSlots, usedSlots, maxTanks, usedTanks;
  bool hasRoamer;

 public:
  Ship(const std::string & line) :
      usedCapacity(0), usedSlots(0), usedTanks(0), hasRoamer(false) {
    std::istringstream ss(line);
    std::string temp;

    // 解析船只信息
    std::getline(ss, name, ':');  // 船名
    std::getline(ss, type, ',');  // 类型

    if (type == "Container") {
      std::getline(ss, temp, ',');
      totalSlots = std::atoi(temp.c_str());
    }
    else if (type == "Tanker") {
      std::getline(ss, temp, ',');  // 温度参数
      std::getline(ss, temp, ',');  // 最大槽数
      maxTanks = std::atoi(temp.c_str());
      std::getline(ss, temp, ',');  // 当前使用槽数
      totalSlots = std::atoi(temp.c_str());
    }
    else if (type == "Animals") {
      std::getline(ss, temp, ',');
      totalSlots = std::atoi(temp.c_str());
    }

    std::getline(ss, source, ':');       // 起点
    std::getline(ss, destination, ':');  // 终点
    std::getline(ss, temp, ':');         // 总容量
    totalCapacity = std::strtoull(temp.c_str(), NULL, 10);
  }

  const std::string & getName() const { return name; }
  const std::string & getSource() const { return source; }
  const std::string & getDestination() const { return destination; }
  const std::string & getType() const { return type; }

  bool canCarry(unsigned int weight,
                unsigned int slots,
                bool hazardous,
                bool roamer) const {
    if (type == "Container") {
      return usedCapacity + weight <= totalCapacity && usedSlots + slots <= totalSlots;
    }
    else if (type == "Tanker") {
      return usedCapacity + weight <= totalCapacity && usedTanks + slots <= maxTanks;
    }
    else if (type == "Animals") {
      return usedCapacity + weight <= totalCapacity && usedSlots + slots <= totalSlots &&
             (!roamer || !hasRoamer);  // 不能超过一个roamer
    }
    return false;
  }

  void load(unsigned int weight, unsigned int slots, bool roamer) {
    usedCapacity += weight;
    if (type == "Container" || type == "Animals") {
      usedSlots += slots;
    }
    else if (type == "Tanker") {
      usedTanks += slots;
    }
    if (roamer) {
      hasRoamer = true;
    }
  }

  unsigned int getRemainingCapacity() const { return totalCapacity - usedCapacity; }

  unsigned int getRemainingSlots() const {
    if (type == "Tanker") {
      return maxTanks - usedTanks;
    }
    return totalSlots - usedSlots;
  }

  void printDetails() const {
    if (type == "Container") {
      std::cout << "The Container Ship " << name << "(" << usedCapacity << "/"
                << totalCapacity << ") is carrying :\n";
      std::cout << "  (" << getRemainingSlots() << ") slots remain\n";
    }
    else if (type == "Tanker") {
      std::cout << "The Tanker Ship " << name << "(" << usedCapacity << "/"
                << totalCapacity << ") is carrying :\n";
      std::cout << "  " << usedTanks << " / " << maxTanks << " tanks used\n";
    }
    else if (type == "Animals") {
      std::cout << "The Animals Ship " << name << "(" << usedCapacity << "/"
                << totalCapacity << ") is carrying :\n";
      if (hasRoamer) {
        std::cout << "  has a roamer\n";
      }
      else {
        std::cout << "  does not have a roamer\n";
      }
    }
  }
};

// 货物类
class Cargo {
 public:
  std::string name, source, destination;
  unsigned int weight, slots;
  bool hazardous, roamer;

  Cargo(const std::string & line) : slots(1), hazardous(false), roamer(false) {
    std::istringstream ss(line);
    std::string temp;

    std::getline(ss, name, ',');
    std::getline(ss, source, ',');
    std::getline(ss, destination, ',');
    std::getline(ss, temp, ',');
    weight = std::strtoull(temp.c_str(), NULL, 10);

    while (std::getline(ss, temp, ',')) {
      if (temp == "hazardous") {
        hazardous = true;
      }
      else if (temp == "roamer") {
        roamer = true;
      }
    }
  }
};

// 比较器
struct ShipPtrCompare {
  bool operator()(const Ship * a, const Ship * b) const {
    return a->getName() < b->getName();
  }
};

// 比较货物重量（从大到小）
bool compareCargo(const Cargo & a, const Cargo & b) {
  return a.weight > b.weight;
}

// 读取船只
void readShips(const std::string & filename, std::vector<Ship *> & ships) {
  std::ifstream file(filename.c_str());
  std::string line;
  while (std::getline(file, line)) {
    ships.push_back(new Ship(line));
  }
}

// 读取货物
void readCargo(const std::string & filename, std::vector<Cargo> & cargoList) {
  std::ifstream file(filename.c_str());
  std::string line;
  while (std::getline(file, line)) {
    cargoList.push_back(Cargo(line));
  }
}

// 查找最佳船只
Ship * findBestShip(
    AVLMultiMap<unsigned int, Ship *, std::less<unsigned int>, ShipPtrCompare> & shipMap,
    const Cargo & cargo) {
  std::vector<std::pair<std::pair<unsigned int, std::set<Ship *, ShipPtrCompare> >, int> >
      preOrderDump = shipMap.preOrderDump();

  for (size_t i = 0; i < preOrderDump.size(); ++i) {
    const std::set<Ship *, ShipPtrCompare> & ships = preOrderDump[i].first.second;
    for (std::set<Ship *, ShipPtrCompare>::const_iterator it = ships.begin();
         it != ships.end();
         ++it) {
      Ship * ship = const_cast<Ship *>(*it);
      if (ship->canCarry(cargo.weight, cargo.slots, cargo.hazardous, cargo.roamer)) {
        return ship;
      }
    }
  }
  return NULL;
}

// 处理货物装载
void processCargo(std::vector<Ship *> & ships, const std::vector<Cargo> & cargoList) {
  AVLMultiMap<unsigned int, Ship *, std::less<unsigned int>, ShipPtrCompare> shipMap;
  for (size_t i = 0; i < ships.size(); ++i) {
    shipMap.add(ships[i]->getRemainingCapacity(), ships[i]);
  }

  for (size_t i = 0; i < cargoList.size(); ++i) {
    const Cargo & cargo = cargoList[i];
    Ship * bestShip = findBestShip(shipMap, cargo);

    if (bestShip == NULL) {
      std::cout << "No ships can carry the " << cargo.name << " from " << cargo.source
                << " to " << cargo.destination << std::endl;
    }
    else {
      std::cout << "Loading " << cargo.name << " onto " << bestShip->getName() << " from "
                << cargo.source << " to " << cargo.destination << " "
                << (bestShip->getRemainingCapacity() - cargo.weight)
                << " capacity remains" << std::endl;

      shipMap.remove(bestShip->getRemainingCapacity(), bestShip);
      bestShip->load(cargo.weight, cargo.slots, cargo.roamer);
      shipMap.add(bestShip->getRemainingCapacity(), bestShip);
    }
  }

  std::cout << "---Done Loading---Here are the ships---" << std::endl;
  for (size_t i = 0; i < ships.size(); ++i) {
    ships[i]->printDetails();
  }
}

// 主函数
int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships_file> <cargo_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Ship *> ships;
  readShips(argv[1], ships);

  std::vector<Cargo> cargoList;
  readCargo(argv[2], cargoList);

  // 使用普通函数进行稳定排序
  std::stable_sort(cargoList.begin(), cargoList.end(), compareCargo);

  processCargo(ships, cargoList);

  // 清理内存
  for (size_t i = 0; i < ships.size(); ++i) {
    delete ships[i];
  }

  return EXIT_SUCCESS;
}
