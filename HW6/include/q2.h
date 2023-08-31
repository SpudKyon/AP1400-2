#ifndef Q2_H
#define Q2_H

#include <fstream>

namespace q2 {
    struct Patient {
        std::string name;
        size_t age;
        size_t smokes;
        size_t area_q;
        size_t alkhol;
    };

    inline std::vector<Patient> read_file(const std::string &filename) {
        std::vector<Patient> patients;
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string cell[6];
            int idx = 0;
            while (getline(ss, cell[idx], ',')) {
                idx++;
            }
            Patient patient;
            if (cell[0].at(cell[0].length() - 1) == ' ')
                patient.name = cell[0] + cell[1];
            else
                patient.name = cell[0] + " " + cell[1];
            patient.age = std::atoi(cell[2].c_str());
            patient.smokes = std::atoi(cell[3].c_str());
            patient.area_q = std::atoi(cell[4].c_str());
            patient.alkhol = std::atoi(cell[5].c_str());
            patients.push_back(patient);
        }

        file.close();
        return patients;
    }

    inline size_t possibilities(const Patient& patient) {
        return 3 * patient.age + 5 * patient.smokes + 2 * patient.area_q + 4 * patient.alkhol;
    }

    inline void sort(std::vector<Patient>& vector) {
        std::sort(vector.begin(), vector.end(), [](const Patient& a, const Patient& b) {
            return possibilities(a) > possibilities(b);
        });
    }
}

#endif //Q2_H