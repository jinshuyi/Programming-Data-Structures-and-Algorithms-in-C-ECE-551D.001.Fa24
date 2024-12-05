#include <stdint.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// 货物结构体
struct Cargo {
  std::string name, origin, destination, category;
  uint64_t weight;
  std::vector<std::string> properties;
};

// 船只结构体
struct Ship {
  std::string name, type, origin, destination;
  uint64_t capacity, used_capacity;
  std::vector<std::string> restrictions;
  int slots, used_slots;

  Ship() : capacity(0), used_capacity(0), slots(0), used_slots(0) {}

  uint64_t remaining_capacity() const {
    return capacity > used_capacity ? capacity - used_capacity : 0;
  }

  bool can_load(const Cargo & cargo) const {
    if (origin != cargo.origin || destination != cargo.destination)
      return false;
    if (capacity < used_capacity + cargo.weight)
      return false;
    if (type == "Animals" && slots <= used_slots)
      return false;
    for (size_t i = 0; i < cargo.properties.size(); i++) {
      if (std::find(restrictions.begin(), restrictions.end(), cargo.properties[i]) ==
          restrictions.end())
        return false;
    }
    return true;
  }

  void load_cargo(const Cargo & cargo) {
    used_capacity += cargo.weight;
    if (type == "Animals") {
      used_slots++;
    }
  }
};

// 按货物重量降序排序
bool cargo_weight_desc(const Cargo & a, const Cargo & b) {
  return a.weight > b.weight;
}

// 解析船只文件
void parse_ships(std::ifstream & file, std::vector<Ship> & ships) {
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    Ship ship;
    std::string capacity_str, slots_str, restrictions_str;

    std::getline(ss, ship.name, ':');
    std::getline(ss, ship.type, ',');
    std::getline(ss, slots_str, ':');
    ship.slots = std::atoi(slots_str.c_str());
    std::getline(ss, restrictions_str, ':');
    if (!restrictions_str.empty()) {
      std::stringstream res_ss(restrictions_str);
      std::string restriction;
      while (std::getline(res_ss, restriction, ',')) {
        ship.restrictions.push_back(restriction);
      }
    }
    std::getline(ss, ship.origin, ':');
    std::getline(ss, ship.destination, ':');
    ss >> ship.capacity;
    ships.push_back(ship);
  }
}

// 解析货物文件
void parse_cargo(std::ifstream & file, std::vector<Cargo> & cargoes) {
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    Cargo cargo;
    std::string weight_str, properties_str;

    std::getline(ss, cargo.name, ',');
    std::getline(ss, cargo.origin, ',');
    std::getline(ss, cargo.destination, ',');
    std::getline(ss, weight_str, ',');
    cargo.weight = std::atoi(weight_str.c_str());
    std::getline(ss, cargo.category, ',');
    std::getline(ss, properties_str);
    if (!properties_str.empty()) {
      std::stringstream prop_ss(properties_str);
      std::string property;
      while (std::getline(prop_ss, property, ',')) {
        cargo.properties.push_back(property);
      }
    }
    cargoes.push_back(cargo);
  }
}

// 货物装载逻辑
void load_cargo(const std::vector<Cargo> & cargoes, std::vector<Ship> & ships) {
  typedef AVLMultiMap<uint64_t, Ship *> ShipMap;
  ShipMap ship_map;

  // 将所有船只添加到AVL树中
  for (size_t i = 0; i < ships.size(); i++) {
    ship_map.add(ships[i].remaining_capacity(), &ships[i]);
  }

  // 遍历货物列表，尝试装载
  for (size_t i = 0; i < cargoes.size(); i++) {
    const Cargo & cargo = cargoes[i];
    Ship * best_ship = NULL;

    // 使用preOrderDump来获取所有键值对
    std::vector<std::pair<std::pair<uint64_t, std::set<Ship *> >, int> > ship_data =
        ship_map.preOrderDump();

    for (size_t j = 0; j < ship_data.size(); j++) {
      const std::set<Ship *> & ship_set = ship_data[j].first.second;

      for (std::set<Ship *>::iterator it = ship_set.begin(); it != ship_set.end(); ++it) {
        Ship * ship = *it;
        if (ship->can_load(cargo)) {
          if (!best_ship ||
              ship->remaining_capacity() < best_ship->remaining_capacity() ||
              (ship->remaining_capacity() == best_ship->remaining_capacity() &&
               ship->name < best_ship->name)) {
            best_ship = ship;
          }
        }
      }
    }

    if (best_ship) {
      std::cout << "Loading " << cargo.name << " onto " << best_ship->name << " from "
                << cargo.origin << " to " << cargo.destination << " "
                << best_ship->remaining_capacity() - cargo.weight
                << " capacity remains\n";
      ship_map.remove(best_ship->remaining_capacity(), best_ship);
      best_ship->load_cargo(cargo);
      ship_map.add(best_ship->remaining_capacity(), best_ship);
    }
    else {
      std::cout << "No ships can carry the " << cargo.name << " from " << cargo.origin
                << " to " << cargo.destination << "\n";
    }
  }
}

// 主函数
int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ships_file> <cargo_file>\n";
    return EXIT_FAILURE;
  }

  std::ifstream ships_file(argv[1]);
  std::ifstream cargo_file(argv[2]);
  if (!ships_file.is_open() || !cargo_file.is_open()) {
    std::cerr << "Error: Unable to open input files.\n";
    return EXIT_FAILURE;
  }

  std::vector<Ship> ships;
  std::vector<Cargo> cargoes;
  parse_ships(ships_file, ships);
  parse_cargo(cargo_file, cargoes);

  std::sort(cargoes.begin(), cargoes.end(), cargo_weight_desc);
  load_cargo(cargoes, ships);

  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); i++) {
    const Ship & ship = ships[i];
    std::cout << "The " << ship.type << " Ship " << ship.name << "(" << ship.used_capacity
              << "/" << ship.capacity << ") is carrying :\n";
    // 输出每艘船的货物（此处留空，可以补充具体货物输出逻辑）
  }
  return EXIT_SUCCESS;
}
