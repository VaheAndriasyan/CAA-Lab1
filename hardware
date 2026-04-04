#pragma once
#ifndef HARDWAREID_H
#define HARDWAREID_H

#include <string>


class HardwareID {
private:
    std::string cpuId;
    std::string motherboardId;
    std::string macAddress;

    std::string combineAndHash(const std::string& data);

#ifdef _WIN32
    std::string getCpuIdWindows();

    std::string getMacAddressWindows();
    std::string getMotherboardIdWindows();
#endif

public:
    HardwareID();

    std::string getMachineID();

    std::string getCpuId() { return cpuId; }
    std::string getMotherboardId() { return motherboardId; }
    std::string getMacAddress() { return macAddress; }
};

#endif
