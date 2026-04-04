#include "LicenseManager.h"
#include "HardwareID.h"

#include <iostream>
#include <fstream>
#include <sstream>

LicenseManager::LicenseManager() {
    HardwareID hwid;
    currentMachineID = hwid.getMachineID();

    std::cout << "Device ID: " << currentMachineID << "\n"
              << "CPU ID: " << hwid.getCpuId() << "\n"
              << "MAC: " << hwid.getMacAddress() << "\n\n";
}

LicenseKey LicenseManager::createLicense(const std::string& userName, int daysValid) {
    LicenseKey license(currentMachineID, userName, daysValid);
    licenses.push_back(license);
    return license;
}

bool LicenseManager::verifyLicense(const std::string& licenseKey) const {
    for (const auto& lic : licenses) {
        if (lic.getKey() == licenseKey) {
            return lic.isValid(currentMachineID);
        }
    }

    return verifyFromFile(licenseKey);
}

bool LicenseManager::saveToFile(const std::string& licenseKey,
                               const std::string& filename) const {
    for (const auto& lic : licenses) {
        if (lic.getKey() == licenseKey) {
            std::ofstream file(filename);
            if (!file) return false;

            file << "KEY=" << lic.getKey() << "\n"
                 << "MACHINE=" << lic.getMachineID() << "\n"
                 << "TYPE=" << (lic.isTemporaryLicense() ? "TEMPORARY" : "PERPETUAL") << "\n"
                 << "EXPIRY=" << lic.getExpiryDate() << "\n";

            return true;
        }
    }
    return false;
}

bool LicenseManager::verifyFromFile(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file) return false;

    std::string line, key, machine, type;
    std::time_t expiry = 0;

    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string name = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (name == "KEY") key = value;
        else if (name == "MACHINE") machine = value;
        else if (name == "TYPE") type = value;
        else if (name == "EXPIRY") expiry = std::stoll(value);
    }

    if (machine != currentMachineID) {
        std::cout << "Invalid machine\n";
        return false;
    }

    if (type == "TEMPORARY" && std::time(nullptr) > expiry) {
        std::cout << "License expired\n";
        return false;
    }

    return true;
}

void LicenseManager::listAllLicenses() const {
    if (licenses.empty()) {
        std::cout << "No licenses found\n";
        return;
    }

    for (const auto& lic : licenses) {
        lic.printInfo();
    }
}

void LicenseManager::checkRemainingDays(const std::string& licenseKey) const {
    for (const auto& lic : licenses) {
        if (lic.getKey() != licenseKey) continue;

        if (!lic.isTemporaryLicense()) {
            std::cout << "Perpetual license\n";
            return;
        }

        std::time_t now = std::time(nullptr);
        std::time_t expiry = lic.getExpiryDate();

        if (now > expiry) {
            std::cout << "Expired: " << ctime(&expiry);
        } else {
            int days = static_cast<int>((expiry - now) / 86400);
            std::cout << "Active\n"
                      << "Expires: " << ctime(&expiry)
                      << "Days left: " << days << "\n";
        }
        return;
    }

    std::cout << "Key not found\n";
}
