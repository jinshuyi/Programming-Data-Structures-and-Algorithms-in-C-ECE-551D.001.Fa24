#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// 船只类
class Ship {
 private:
  std::string name, source, destination;
  unsigned int totalCapacity, usedCapacity, totalSlots, usedSlots;

 public:
  Ship(const std::string & line) : usedCapacity(0), usedSlots(0) {
    std::istringstream ss(line);
    std::string temp;
    std::getline(ss, name, ':');  // 读取船名
    std::getline(ss, temp, ':');  // 跳过类型
    std::istringstream tempSS(temp);
    std::getline(tempSS, temp, ',');  // 插槽数
    totalSlots = std::atoi(temp.c_str());
    std::getline(ss, source, ':');       // 起点
    std::getline(ss, destination, ':');  // 终点
    std::getline(ss, temp, ':');         // 总容量
    totalCapacity = std::strtoull(temp.c_str(), NULL, 10);
  }

  const std::string & getName() const { return name; }
  const std::string & getSource() const { return source; }
  const std::string & getDestination() const { return destination; }

  bool canCarry(unsigned int weight, unsigned int slots) const {
    return usedCapacity + weight <= totalCapacity && usedSlots + slots <= totalSlots;
  }

  void load(unsigned int weight, unsigned int slots) {
    usedCapacity += weight;
    usedSlots += slots;
  }

  unsigned int getRemainingCapacity() const { return totalCapacity - usedCapacity; }

  unsigned int getRemainingSlots() const { return totalSlots - usedSlots; }

  void printDetails() const {
    std::cout << "The Container Ship " << name << "(" << usedCapacity << "/"
              << totalCapacity << ") is carrying :" << std::endl;
    std::cout << "  (" << getRemainingSlots() << ") slots remain" << std::endl;
  }
};

// 货物类
class Cargo {
 public:
  std::string name, source, destination;
  unsigned int weight, slots;

  Cargo(const std::string & line) {
    std::istringstream ss(line);
    std::getline(ss, name, ',');
    std::getline(ss, source, ',');
    std::getline(ss, destination, ',');
    std::string temp;
    std::getline(ss, temp, ',');
    weight = std::strtoull(temp.c_str(), NULL, 10);
    slots = 1;  // 每个货物占一个插槽
  }
};

// 比较器
struct ShipPtrCompare {
  bool operator()(const Ship * a, const Ship * b) const {
    return a->getName() < b->getName();
  }
};

// 按货物重量排序
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
      Ship * ship = const_cast<Ship *>(*it);  // 解除 const 性
      if (ship->canCarry(cargo.weight, cargo.slots)) {
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
      bestShip->load(cargo.weight, cargo.slots);
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

  // 按重量从大到小排序货物
  std::stable_sort(cargoList.begin(), cargoList.end(), compareCargo);

  // 装载货物
  processCargo(ships, cargoList);

  // 清理内存
  for (size_t i = 0; i < ships.size(); ++i) {
    delete ships[i];
  }

  return EXIT_SUCCESS;
}
