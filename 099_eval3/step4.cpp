#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// 船只结构体
struct Ship {
  std::string name;
  std::string type;
  int capacity;
  int used_capacity;
  std::string source;
  std::string destination;

  Ship(const std::string & n,
       const std::string & t,
       int c,
       const std::string & s,
       const std::string & d) :
      name(n), type(t), capacity(c), used_capacity(0), source(s), destination(d) {}

  int remainingCapacity() const { return capacity - used_capacity; }
};

// 货物结构体
struct Cargo {
  std::string name;
  std::string source;
  std::string destination;
  int weight;
  std::string type;

  Cargo(const std::string & n,
        const std::string & s,
        const std::string & d,
        int w,
        const std::string & t) :
      name(n), source(s), destination(d), weight(w), type(t) {}
};

// 按重量降序排序货物
bool compareCargo(const Cargo & a, const Cargo & b) {
  return a.weight > b.weight;
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships file> <cargo file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream shipFile(argv[1]);
  std::ifstream cargoFile(argv[2]);

  if (!shipFile.is_open() || !cargoFile.is_open()) {
    std::cerr << "Error opening input files!" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Ship> ships;
  std::vector<Cargo> cargos;

  // 读取船只数据
  std::string line;
  while (std::getline(shipFile, line)) {
    std::istringstream iss(line);
    std::string name, type, source, destination;
    int capacity;
    char colon;

    iss >> name >> colon >> type >> colon >> capacity >> colon >> source >> colon >>
        destination;
    ships.push_back(Ship(name, type, capacity, source, destination));
  }

  // 读取货物数据
  while (std::getline(cargoFile, line)) {
    std::istringstream iss(line);
    std::string name, source, destination, type;
    int weight;
    char colon;

    iss >> name >> colon >> source >> colon >> destination >> colon >> weight >> colon >>
        type;
    cargos.push_back(Cargo(name, source, destination, weight, type));
  }

  // 按重量排序货物
  std::stable_sort(cargos.begin(), cargos.end(), compareCargo);

  // 使用 AVL 树管理船只
  AVLMultiMap<int, Ship *> shipMap;
  for (std::vector<Ship>::iterator it = ships.begin(); it != ships.end(); ++it) {
    shipMap.add(it->remainingCapacity(), &(*it));
  }

  // 开始装载货物
  for (std::vector<Cargo>::iterator cargoIt = cargos.begin(); cargoIt != cargos.end();
       ++cargoIt) {
    bool loaded = false;

    // 遍历 AVL 树寻找最佳船只
    std::vector<std::pair<std::pair<int, std::set<Ship *, std::less<Ship *> > >, int> >
        preOrderDump = shipMap.preOrderDump();
    for (std::vector<std::pair<std::pair<int, std::set<Ship *, std::less<Ship *> > >,
                               int> >::iterator it = preOrderDump.begin();
         it != preOrderDump.end();
         ++it) {
      int remainingCapacity = it->first.first;
      if (remainingCapacity >= cargoIt->weight) {
        for (std::set<Ship *>::iterator shipIt = it->first.second.begin();
             shipIt != it->first.second.end();
             ++shipIt) {
          Ship * ship = const_cast<Ship *>(*shipIt);
          if (ship->source == cargoIt->source &&
              ship->destination == cargoIt->destination &&
              ship->remainingCapacity() >= cargoIt->weight) {
            // 装载货物
            ship->used_capacity += cargoIt->weight;
            std::cout << "Loading " << cargoIt->name << " onto " << ship->name << " from "
                      << cargoIt->source << " to " << cargoIt->destination << " "
                      << ship->remainingCapacity() << " capacity remains" << std::endl;

            // 更新 AVL 树
            shipMap.remove(ship->remainingCapacity() + cargoIt->weight, ship);
            shipMap.add(ship->remainingCapacity(), ship);

            loaded = true;
            break;
          }
        }
        if (loaded)
          break;
      }
    }

    if (!loaded) {
      std::cout << "No ships can carry " << cargoIt->name << " from " << cargoIt->source
                << " to " << cargoIt->destination << std::endl;
    }
  }

  // 输出船只最终状态
  std::cout << "---Done Loading---Here are the ships---" << std::endl;
  for (std::vector<Ship>::iterator it = ships.begin(); it != ships.end(); ++it) {
    std::cout << "The " << it->type << " Ship " << it->name << "(" << it->used_capacity
              << "/" << it->capacity << ") is carrying : " << std::endl;
    if (it->remainingCapacity() > 0) {
      std::cout << it->remainingCapacity() << " slots remain" << std::endl;
    }
  }

  return EXIT_SUCCESS;
}
