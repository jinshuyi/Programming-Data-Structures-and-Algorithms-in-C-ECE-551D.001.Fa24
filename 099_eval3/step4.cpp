#include <stdint.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "avlmultimap03.hpp"

struct Cargo {
  std::string name, origin, destination, category;
  uint64_t weight;
  std::vector<std::string> properties;
};

struct Ship {
  std::string name, type, origin, destination;
  uint64_t capacity, used_capacity;
  std::vector<std::string> restrictions;
  int slots, used_slots, tanks, used_tanks;

  Ship() :
      capacity(0), used_capacity(0), slots(0), used_slots(0), tanks(0), used_tanks(0) {}

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
    if (type == "Tanker" && tanks <= used_tanks)
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
    else if (type == "Tanker") {
      used_tanks++;
    }
  }
};

bool cargo_weight_desc(const Cargo & a, const Cargo & b) {
  return a.weight > b.weight;
}

void parse_ships(std::ifstream & file, std::vector<Ship> & ships) {
  std::string line;
  while (std::getline(file, line)) {
    Ship ship;
    std::stringstream ss(line);
    std::string type_details;

    std::getline(ss, ship.name, ':');     // 船名
    std::getline(ss, type_details, ':');  // 类型和附加信息
    std::string capacity_str;

    if (type_details.find("Container") != std::string::npos) {
      ship.type = "Container";
      std::stringstream type_stream(type_details);
      std::string slots_str, restrictions_str;

      std::getline(type_stream, ship.type, ',');  // 类型
      std::getline(type_stream, slots_str, ',');  // 槽数
      ship.slots = std::atoi(slots_str.c_str());

      std::getline(type_stream, restrictions_str, ':');  // 限制条件
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

      std::getline(type_stream, ship.type, ',');  // 类型
      std::getline(type_stream, slots_str, ',');  // 槽数
      ship.slots = std::atoi(slots_str.c_str());
    }
    else if (type_details.find("Tanker") != std::string::npos) {
      ship.type = "Tanker";
      std::stringstream type_stream(type_details);
      std::string token;
      std::vector<int> tank_values;

      while (std::getline(type_stream, token, ',')) {
        tank_values.push_back(std::atoi(token.c_str()));
      }
      if (!tank_values.empty()) {
        ship.tanks = tank_values.back();  // 取最后一个值作为tanks的值
      }

      std::string restrictions_str;
      std::getline(type_stream, restrictions_str, ':');  // 限制条件
      if (!restrictions_str.empty()) {
        std::stringstream res_stream(restrictions_str);
        std::string restriction;
        while (std::getline(res_stream, restriction, ',')) {
          ship.restrictions.push_back(restriction);
        }
      }
    }

    std::getline(ss, ship.origin, ':');       // 起点
    std::getline(ss, ship.destination, ':');  // 终点
    ss >> ship.capacity;                      // 容量
    ships.push_back(ship);
  }
}

void print_ships(const std::vector<Ship> & ships) {
  std::cout << "---Done Loading---Here are the ships---\n";
  for (size_t i = 0; i < ships.size(); i++) {
    const Ship & ship = ships[i];
    std::cout << "The " << ship.type << " Ship " << ship.name << "(" << ship.used_capacity
              << "/" << ship.capacity << ") is carrying :\n";
    if (ship.type == "Animals") {
      if (ship.used_slots == 0) {
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

  print_ships(ships);

  return EXIT_SUCCESS;
}
