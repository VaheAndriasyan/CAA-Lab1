#pragma once

#include <string>
#include <ctime>

/**
 * LicenseKey - ներկայացնում է լիցենզիայի տվյալներն ու վարքագիծը
 */
class LicenseKey {
private:
    std::string key;
    std::string machineID;
    std::string licensee;

    std::time_t issueDate{};
    std::time_t expiryDate{}; // 0 → մշտական

    bool isTemporary{false};

    void generateKey();

public:
    // Միավորված կոնստրուկտոր
    explicit LicenseKey(const std::string& machine,
                        const std::string& user,
                        int daysValid = 0);

    // Validation
    bool isValid(const std::string& currentMachineID) const;

    // Info
    void printInfo() const;

    // Getters
    [[nodiscard]] const std::string& getKey() const noexcept { return key; }
    [[nodiscard]] const std::string& getMachineID() const noexcept { return machineID; }
    [[nodiscard]] std::time_t getExpiryDate() const noexcept { return expiryDate; }
    [[nodiscard]] bool isTemporaryLicense() const noexcept { return isTemporary; }
};
