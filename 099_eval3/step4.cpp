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
  int slots, used_slots, tanks, used_tanks;
  std::vector<std::string> loaded_cargo;

  Ship() :
      capacity(0), used_capacity(0), slots(0), used_slots(0), tanks(0), used_tanks(0) {}

  // 计算剩余容量
  uint64_t remaining_capacity() const {
    return capacity > used_capacity ? capacity - used_capacity : 0;
  }

  // 判断是否可以装载货物
  bool can_load(const Cargo & cargo) const {
    if (origin != cargo.origin || destination != cargo.destination)
      return false;
    if (capacity < used_capacity + cargo.weight)
      return false;
    if (type == "Animals" && slots <= used_slots)
      return false;
    if (type == "Tanker" && tanks <= used_tanks)
      return false;

    for (size_t i = 0; i < cargo.properties.size(); i++) {
      if (std::find(restrictions.begin(), restrictions.end(), cargo.properties[i]) ==
          restrictions.end())
        return false;
    }
    return true;
  }

  // 装载货物
  void load_cargo(const Cargo & cargo) {
    used_capacity += cargo.weight;
    loaded_cargo.push_back(cargo.name + "(" + to_string(cargo.weight) + ")");
    if (type == "Animals") {
      used_slots++;
    }
    else if (type == "Tanker") {
      used_tanks++;
    }
  }

  // C++03 自定义整数转字符串
  std::string to_string(uint64_t num) const {
    std::ostringstream oss;
    oss << num;
    return oss.str();
  }
};

// 比较货物重量降序
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
    std::string capacity_str;

    if (type_details.find("Container") != std::string::npos) {
      ship.type = "Container";
      std::stringstream type_stream(type_details);
      std::string slots_str, restrictions_str;

      std::getline(type_stream, ship.type, ',');
      std::getline(type_stream, slots_str, ',');
      ship.slots = std::atoi(slots_str.c_str());

      std::getline(type_stream, restrictions_str, ':');
      if (!restrictions_str.empty()) {
        std::stringstream res_stream(restrictions_str);
        std::string restriction;
        while (std::getline(res_stream, restriction, ',')) {
          ship.restrictions.push_back(restriction);
        }
      }
    }
    else if (type_details.find("Animals") != std::string::npos) {
      ship.type = "Animals";
      std::stringstream type_stream(type_details);
      std::string slots_str;

      std::getline(type_stream, ship.type, ',');
      std::getline(type_stream, slots_str, ',');
      ship.slots = std::atoi(slots_str.c_str());
    }
    else if (type_details.find("Tanker") != std::string::npos) {
      ship.type = "Tanker";
      std::stringstream type_stream(type_details);
      std::string value;
      int count = 0;

      while (std::getline(type_stream, value, ',')) {
        count++;
        if (count == 4) {
          ship.tanks = std::atoi(value.c_str());
          break;
        }
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

// 装载货物
void load_cargo(const std::vector<Cargo> & cargoes, std::vector<Ship> & ships) {
  typedef AVLMultiMap<uint64_t, Ship *> ShipMap;
  ShipMap ship_map;

  for (size_t i = 0; i < ships.size(); i++) {
    ship_map.add(ships[i].remaining_capacity(), &ships[i]);
  }

  for (size_t i = 0; i < cargoes.size(); i++) {
    const Cargo & cargo = cargoes[i];
    Ship * best_ship = NULL;

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
