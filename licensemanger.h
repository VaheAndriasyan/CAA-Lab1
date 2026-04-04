#pragma once

#include <vector>
#include <string>
#include "LicenseKey.h"

class LicenseManager {
private:
    std::vector<LicenseKey> licenses;
    std::string currentMachineID;

    bool verifyFromFile(const std::string& filename) const;

public:
    LicenseManager();

    // Create
    LicenseKey createLicense(const std::string& userName, int daysValid = 0);

    // Validate
    bool verifyLicense(const std::string& licenseKey) const;

    // Storage
    bool saveToFile(const std::string& licenseKey,
                    const std::string& filename) const;

    // Info
    void listAllLicenses() const;
    void checkRemainingDays(const std::string& licenseKey) const;

    // Getter
    const std::string& getCurrentMachineID() const noexcept {
        return currentMachineID;
    }
};
