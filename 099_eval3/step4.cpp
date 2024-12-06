#include <stdint.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

// 定义货物结构体
struct Cargo {
  std::string name, origin, destination, category;
  uint64_t weight;
  int min_temp, max_temp;
  std::vector<std::string> properties;

  Cargo() : weight(0), min_temp(INT32_MIN), max_temp(INT32_MAX) {}
};

// 定义船只结构体
struct Ship {
  std::string name, type, origin, destination;
  uint64_t capacity, used_capacity;
  int slots, used_slots, tanks, used_tanks;
  int min_temp, max_temp;
  std::vector<std::string> restrictions;
  std::vector<std::string> loaded_cargo;

  Ship() :
      capacity(0),
      used_capacity(0),
      slots(0),
      used_slots(0),
      tanks(0),
      used_tanks(0),
      min_temp(INT32_MIN),
      max_temp(INT32_MAX) {}

  uint64_t remaining_capacity() const {
    return capacity > used_capacity ? capacity - used_capacity : 0;
  }

  bool can_load(const Cargo & cargo) const {
    if (origin != cargo.origin || destination != cargo.destination)
      return false;
    if (remaining_capacity() < cargo.weight)
      return false;

    if (type == "Container") {
      // Container 类型船需要检查危险物品限制
      for (size_t i = 0; i < cargo.properties.size(); i++) {
        if (cargo.properties[i].find("hazardous-") != std::string::npos &&
            std::find(restrictions.begin(), restrictions.end(), cargo.properties[i]) ==
                restrictions.end()) {
          return false;
        }
      }
    }
    else if (type == "Animals") {
      // Animals 类型船特殊逻辑
      if (cargo.category == "animal") {
        if (cargo.properties.end() !=
                std::find(cargo.properties.begin(), cargo.properties.end(), "roamer") &&
            used_slots > 0)
          return false;  // 已经有一个“roamer”动物
      }
      else {
        if (static_cast<int>(cargo.weight) > slots ||
            cargo.properties.end() !=
                std::find(cargo.properties.begin(), cargo.properties.end(), "hazardous-"))
          return false;
      }
    }
    else if (type == "Tanker") {
      // Tanker 类型船特殊逻辑
      if (cargo.category != "liquid" && cargo.category != "gas")
        return false;
      if (cargo.min_temp > max_temp || cargo.max_temp < min_temp)
        return false;
      if (used_tanks >= tanks)
        return false;
    }
    return true;
  }

  void load_cargo(const Cargo & cargo) {
    used_capacity += cargo.weight;
    std::ostringstream oss;
    oss << cargo.name << "(" << cargo.weight << ")";
    loaded_cargo.push_back(oss.str());
    if (type == "Animals") {
      used_slots++;
    }
    else if (type == "Tanker") {
      used_tanks++;
    }
  }
};

// 按重量降序排列货物
bool cargo_weight_desc(const Cargo & a, const Cargo & b) {
  return a.weight > b.weight;
}

// 解析船只文件
void parse_ships(std::ifstream & file, std::vector<Ship> & ships) {
  std::string line;
  while (std::getline(file, line)) {
    Ship ship;
    std::stringstream ss(line);
    std::string type_details;

    std::getline(ss, ship.name, ':');
    std::getline(ss, type_details, ':');

    if (type_details.find("Container") != std::string::npos) {
      ship.type = "Container";
      std::istringstream details(type_details);
      std::string slots;
      std::getline(details, ship.type, ',');
      std::getline(details, slots, ',');
      ship.slots = std::atoi(slots.c_str());
    }
    else if (type_details.find("Animals") != std::string::npos) {
      ship.type = "Animals";
      std::istringstream details(type_details);
      std::string slots;
      std::getline(details, ship.type, ',');
      std::getline(details, slots, ',');
      ship.slots = std::atoi(slots.c_str());
    }
    else if (type_details.find("Tanker") != std::string::npos) {
      ship.type = "Tanker";
      std::istringstream details(type_details);
      std::string temp, tanks;
      std::getline(details, temp, ',');
      ship.min_temp = std::atoi(temp.c_str());
      std::getline(details, temp, ',');
      ship.max_temp = std::atoi(temp.c_str());
      std::getline(details, tanks, ',');
      ship.tanks = std::atoi(tanks.c_str());
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
    Cargo cargo;
    std::stringstream ss(line);
    std::string weight;

    std::getline(ss, cargo.name, ',');
    std::getline(ss, cargo.origin, ',');
    std::getline(ss, cargo.destination, ',');
    std::getline(ss, weight, ',');
    cargo.weight = std::atoi(weight.c_str());
    std::getline(ss, cargo.category, ',');
    std::string property;
    while (std::getline(ss, property, ',')) {
      if (property.find("mintemp=") != std::string::npos) {
        cargo.min_temp = std::atoi(property.substr(8).c_str());
      }
      else if (property.find("maxtemp=") != std::string::npos) {
        cargo.max_temp = std::atoi(property.substr(8).c_str());
      }
      else {
        cargo.properties.push_back(property);
      }
    }
    cargoes.push_back(cargo);
  }
}

// 装载货物
void load_cargo(std::vector<Cargo> & cargoes, std::vector<Ship> & ships) {
  typedef AVLMultiMap<uint64_t, Ship *> ShipMap;
  ShipMap ship_map;

  for (size_t i = 0; i < ships.size(); i++) {
    ship_map.add(ships[i].remaining_capacity(), &ships[i]);
  }

  for (size_t i = 0; i < cargoes.size(); i++) {
    const Cargo & cargo = cargoes[i];
    Ship * best_ship = NULL;
    std::vector<std::pair<std::pair<uint64_t, std::set<Ship *> >, int> > dump =
        ship_map.preOrderDump();

    for (size_t j = 0; j < dump.size(); j++) {
      const std::set<Ship *> & ship_set = dump[j].first.second;
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

// 打印船只信息
void print_ships(const std::vector<Ship> & ships) {
  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); i++) {
    const Ship & ship = ships[i];
    std::cout << "The " << ship.type << " Ship " << ship.name << "(" << ship.used_capacity
              << "/" << ship.capacity << ") is carrying :\n";
    for (size_t j = 0; j < ship.loaded_cargo.size(); j++) {
      std::cout << "  " << ship.loaded_cargo[j] << "\n";
    }
    if (ship.type == "Animals") {
      if (ship.used_slots > 0) {
        std::cout << "  has a roamer\n";
      }
      else {
        std::cout << "  does not have a roamer\n";
      }
    }
    else if (ship.type == "Tanker") {
      std::cout << "  " << ship.used_tanks << " / " << ship.tanks << " tanks used\n";
    }
    else {
      std::cout << "  (" << ship.slots - ship.used_slots << ") slots remain\n";
    }
  }
}

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
  print_ships(ships);

  return EXIT_SUCCESS;
}
