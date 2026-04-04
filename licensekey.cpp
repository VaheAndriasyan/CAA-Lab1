#include "LicenseKey.h"
#include <iostream>
#include <sstream>
#include <iomanip>

LicenseKey::LicenseKey(const std::string& machine, const std::string& user, int daysValid)
    : machineID(machine),
      licensee(user),
      issueDate(time(nullptr)),
      isTemporary(daysValid > 0)
{
    expiryDate = isTemporary ? issueDate + daysValid * 86400 : 0;
    generateKey();
}

void LicenseKey::generateKey() {
    std::string data = machineID + "|" + licensee + "|" + std::to_string(issueDate);

    unsigned long hash = 2166136261u;

    for (char ch : data) {
        hash ^= ch;
        hash *= 16777619;
    }

    if (isTemporary) {
        hash ^= expiryDate;
        hash *= 16777619;
    }

    std::stringstream ss;
    ss << std::uppercase << std::hex;

    for (int i = 0; i < 15; ++i) {
        if (i && i % 5 == 0) ss << '-';
        ss << ((hash >> (i * 4)) & 0xF);
    }

    key = ss.str();
}

bool LicenseKey::isValid(const std::string& currentMachineID) const {
    if (currentMachineID != machineID) return false;

    if (isTemporary && time(nullptr) > expiryDate) {
        return false;
    }

    return true;
}

void LicenseKey::printInfo() const {
    std::cout << "\n========== LICENSE INFO ==========\n";
    std::cout << "Key: " << key << "\n";
    std::cout << "User: " << licensee << "\n";
    std::cout << "Issued: " << ctime(&issueDate);

    if (isTemporary) {
        std::cout << "Expires: " << ctime(&expiryDate);
        std::cout << "Type: TEMPORARY\n";
    } else {
        std::cout << "Type: PERPETUAL\n";
    }

    std::cout << "=================================\n";
}
