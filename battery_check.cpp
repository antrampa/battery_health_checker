#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Helper function to read file content
string readFile(const string& path) {
    ifstream file(path);
    string content;
    if (file.is_open()) {
        getline(file, content);
    }
    return content;
}

int main(int argc, char *argv[]) {
    string powerPath = "/sys/class/power_supply/";
    
    if (argc >= 2 && ( std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cout << "Battery Tool Options:\n";
        std::cout << "  -h, --help   Show this help message\n";
        std::cout << "  -f, --full   Show uevent file content\n";
        return 0;
    }

    // Enumerate all BAT* directories
    for (const auto& entry : fs::directory_iterator(powerPath)) {
        string name = entry.path().filename().string();

        if (name.rfind("BAT", 0) == 0) { // starts with "BAT"
            cout << "Found battery: " << name << endl;

            string base = powerPath + name + "/";

            string energyFull = readFile(base + "charge_full");
            string energyDesign = readFile(base + "charge_full_design");
            string voltageNow = readFile(base + "voltage_now");
            string capacity = readFile(base + "capacity");
            string capacityLevel = readFile(base + "capacity_level");
            string status = readFile(base + "status");
            string serialNumber = readFile(base + "serial_number");
            string modelName = readFile(base + "model_name");
            string manufacturer = readFile(base + "manufacturer");
            string technology = readFile(base + "technology");
            string type = readFile(base + "type");
            string alarm = readFile(base + "alarm");

            if (!energyFull.empty() && !energyDesign.empty()) {
                double full = stod(energyFull);
                double design = stod(energyDesign);
                double health = (full / design) * 100.0;

                cout << "  Full Charge Capacity:  " << full << " uWh" << endl;
                cout << "  Design Capacity:       " << design << " uWh" << endl;
                cout << "  Battery Health:        " << health << "%\n";
            }

            if (!voltageNow.empty())
                cout << "  Voltage Now:           " << voltageNow << " uV" << endl;

            if (!capacity.empty())
                cout << "  Charge:                " << capacity << "%" << endl;

            if (!capacityLevel.empty())
                cout << "  Charge Level:          " << capacity << "%" << endl;

            if (!status.empty())
                cout << "  Status:                " << status << endl;

            if (!serialNumber.empty())
                cout << "  Serial Number:         " << serialNumber << endl;

            if (!modelName.empty())
                cout << "  Model Name:            " << modelName << endl;

            if (!manufacturer.empty())
                cout << "  Manufacturer:          " << manufacturer << endl;
            
            if (!technology.empty())
                cout << "  Technology:            " << technology << endl;

            if (!type.empty())
                cout << "  Type:                  " << type << endl;

            if (!alarm.empty() && alarm != "0")
                cout << "  Alarm:                 " << alarm << endl;
            
            if (argc >= 2 && (std::string(argv[1]) == "-f" || std::string(argv[1]) == "--full")) {
                std::cout << "UEVENT DATA:\n";

                string uevent = base + "uevent";
                ifstream f(uevent);
                string line;

                while (getline(f, line)) {
                    cout << line << endl;
                }
            }

            cout << "----------------------------------------\n";
        }
    }

    return 0;
}
