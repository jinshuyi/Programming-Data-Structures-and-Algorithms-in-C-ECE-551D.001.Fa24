#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// A class represent a ship
class Ship {
 public:
  std::string name;
  std::string source;
  std::string destination;
  unsigned long capacity;

  // Constructor definedto initialize the objetc of ship
  Ship(const std::string & line) {
    std::istringstream iss(line);
    std::string typeInfo, capacityStr;
    if (!std::getline(iss, name, ':') || !std::getline(iss, typeInfo, ':') ||
        !std::getline(iss, source, ':') || !std::getline(iss, destination, ':') ||
        !std::getline(iss, capacityStr)) {
      throw std::invalid_argument("Invalid input line format.");
    }
    //!!! Ensure the name is unique(the readme reuqires)
    capacity = parseCapacity(capacityStr);
  }
  // key step: Static function to parse capacity from string
  static unsigned long parseCapacity(const std::string & str) {
    std::istringstream iss(str);
    unsigned long value;
    if (!(iss >> value) || !iss.eof()) {
      throw std::invalid_argument("Invalid capacity value.");
    }
    return value;
  }

  // Returns the route in "Source -> Destination" format
  std::string getRoute() const { return source + " -> " + destination; }
};
// define the  class fleetmanager to manage the Fleet
class FleetManager {
 private:
  std::vector<Ship> ships;

 public:
  // Function to read ships from file
  void loadShips(const std::string & filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
      throw std::runtime_error("Error opening file: " + filename);
    }

    std::vector<std::string> shipNames;  // To ensure unique names
    std::string line;
    //use getline just as C in AOP
    while (std::getline(file, line)) {
      try {
        Ship ship(line);

        // Check for duplicate ship names
        if (std::find(shipNames.begin(), shipNames.end(), ship.name) != shipNames.end()) {
          throw std::invalid_argument("Duplicate ship name: " + ship.name);
        }

        shipNames.push_back(ship.name);
        ships.push_back(ship);
      }
      catch (const std::exception & e) {
        std::cerr << "Error processing line: " << line << "\n";
        std::cerr << e.what() << "\n";
        exit(EXIT_FAILURE);
      }
    }
  }

  // Function to calculate and print total capacities by route
  void printRouteCapacities() const {
    std::vector<std::pair<std::string, unsigned long> > routeCapacities;

    // Accumulate capacities by route
    for (size_t i = 0; i < ships.size(); ++i) {
      std::string route = ships[i].getRoute();
      //use boolen to judge whether the route exists
      bool found = false;
      // Check if route already exists
      for (size_t j = 0; j < routeCapacities.size(); ++j) {
        if (routeCapacities[j].first == route) {
          routeCapacities[j].second += ships[i].capacity;
          found = true;
          break;
        }
      }

      // Add new route if not found
      if (!found) {
        routeCapacities.push_back(std::make_pair(route, ships[i].capacity));
      }
    }

    // Sort the results by route (source first, then destination)
    std::sort(routeCapacities.begin(), routeCapacities.end());

    // Print the results
    for (size_t i = 0; i < routeCapacities.size(); ++i) {
      std::cout << "(" << routeCapacities[i].first << ") has total capacity "
                << routeCapacities[i].second << "\n";
    }
  }
};

//use try,catch to meet the requirements of the exception
int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return EXIT_FAILURE;
  }

  const std::string filename = argv[1];

  try {
    FleetManager manager;
    manager.loadShips(filename);
    manager.printRouteCapacities();
  }
  catch (const std::exception & e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
