#include <iostream>
#include <vector>
#include <windows.h>
#include <highlevelmonitorconfigurationapi.h>
#include <physicalmonitorenumerationapi.h>
#include <thread> 
#include <chrono> 

#pragma comment(lib, "Dxva2.lib")

/*
- MC_COLOR_TEMPERATURE enumeration
typedef enum _MC_COLOR_TEMPERATURE {
  MC_COLOR_TEMPERATURE_UNKNOWN,
  MC_COLOR_TEMPERATURE_4000K,
  MC_COLOR_TEMPERATURE_5000K,
  MC_COLOR_TEMPERATURE_6500K,
  MC_COLOR_TEMPERATURE_7500K,
  MC_COLOR_TEMPERATURE_8200K,
  MC_COLOR_TEMPERATURE_9300K,
  MC_COLOR_TEMPERATURE_10000K,
  MC_COLOR_TEMPERATURE_11500K
} MC_COLOR_TEMPERATURE, *LPMC_COLOR_TEMPERATURE;
*/

class Monitor {
private:
    HANDLE monitorHandle;
    DWORD minBrightness;
    DWORD currentBrightness;
    DWORD maxBrightness;
    DWORD minContrast;
    DWORD currentContrast;
    DWORD maxContrast;
    DWORD minRedGain, currentRedGain, maxRedGain;
    DWORD minGreenGain, currentGreenGain, maxGreenGain;
    DWORD minBlueGain, currentBlueGain, maxBlueGain;
    DWORD minRedDrive, currentRedDrive, maxRedDrive;
    DWORD minGreenDrive, currentGreenDrive, maxGreenDrive;
    DWORD minBlueDrive, currentBlueDrive, maxBlueDrive;
    MC_COLOR_TEMPERATURE colorTemperature;


public:
    Monitor(HANDLE handle) : monitorHandle(handle), minBrightness(0), currentBrightness(0), maxBrightness(0),
        minContrast(0), currentContrast(0), maxContrast(0),
        minRedGain(0), currentRedGain(0), maxRedGain(0),
        minGreenGain(0), currentGreenGain(0), maxGreenGain(0),
        minBlueGain(0), currentBlueGain(0), maxBlueGain(0),
        colorTemperature(MC_COLOR_TEMPERATURE_UNKNOWN) {
        updateMonitorSettings();
    }

    void updateMonitorSettings() {
        if (!GetMonitorBrightness(monitorHandle, &minBrightness, &currentBrightness, &maxBrightness)) {
            std::cerr << "Failed to get monitor brightness. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorColorTemperature(monitorHandle, &colorTemperature)) {
            std::cerr << "Failed to get color temperature. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorContrast(monitorHandle, &minContrast, &currentContrast, &maxContrast)) {
            std::cerr << "Failed to get monitor contrast. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorRedGreenOrBlueGain(monitorHandle, MC_RED_GAIN, &minRedGain, &currentRedGain, &maxRedGain)) {
            std::cerr << "Failed to get red gain. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorRedGreenOrBlueGain(monitorHandle, MC_GREEN_GAIN, &minGreenGain, &currentGreenGain, &maxGreenGain)) {
            std::cerr << "Failed to get green gain. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorRedGreenOrBlueGain(monitorHandle, MC_BLUE_GAIN, &minBlueGain, &currentBlueGain, &maxBlueGain)) {
            std::cerr << "Failed to get blue gain. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorRedGreenOrBlueDrive(monitorHandle, MC_RED_DRIVE, &minRedDrive, &currentRedDrive, &maxRedDrive)) {
            std::cerr << "Failed to get red drive. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorRedGreenOrBlueDrive(monitorHandle, MC_GREEN_DRIVE, &minGreenDrive, &currentGreenDrive, &maxGreenDrive)) {
            std::cerr << "Failed to get green drive. Error: " << GetLastError() << std::endl;
        }
        if (!GetMonitorRedGreenOrBlueDrive(monitorHandle, MC_BLUE_DRIVE, &minBlueDrive, &currentBlueDrive, &maxBlueDrive)) {
            std::cerr << "Failed to get blue drive. Error: " << GetLastError() << std::endl;
        }
    }

    void resetToDefaultSettings() {
        if (!RestoreMonitorFactoryDefaults(monitorHandle)) {
            std::cerr << "Failed to restore monitor to factory defaults. Error: " << GetLastError() << std::endl;
        }
    }

    void setBrightness(DWORD newBrightness) {
        if (newBrightness >= minBrightness && newBrightness <= maxBrightness) {
            if (!SetMonitorBrightness(monitorHandle, newBrightness)) {
                std::cerr << "Failed to set monitor brightness. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Brightness value out of range. Must be between " << minBrightness << " and " << maxBrightness << "." << std::endl;
        }
    }

    void setContrast(DWORD newContrast) {
        if (newContrast >= minContrast && newContrast <= maxContrast) {
            if (!SetMonitorContrast(monitorHandle, newContrast)) {
                std::cerr << "Failed to set monitor contrast. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Contrast value out of range. Must be between " << minContrast << " and " << maxContrast << "." << std::endl;
        }
    }

    void setColorTemperature(MC_COLOR_TEMPERATURE newColorTemperature) {
        if (!SetMonitorColorTemperature(monitorHandle, newColorTemperature)) {
            std::cerr << "Failed to set monitor color temperature. Error: " << GetLastError() << std::endl;
        }
    }

    void setRGBGain(DWORD newRedGain, DWORD newGreenGain, DWORD newBlueGain) {
        if (newRedGain >= minRedGain && newRedGain <= maxRedGain) {
            if (!SetMonitorRedGreenOrBlueGain(monitorHandle, MC_RED_GAIN, newRedGain)) {
                std::cerr << "Failed to set red gain. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Red gain value out of range. Must be between " << minRedGain << " and " << maxRedGain << "." << std::endl;
        }

        if (newGreenGain >= minGreenGain && newGreenGain <= maxGreenGain) {
            if (!SetMonitorRedGreenOrBlueGain(monitorHandle, MC_GREEN_GAIN, newGreenGain)) {
                std::cerr << "Failed to set green gain. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Green gain value out of range. Must be between " << minGreenGain << " and " << maxGreenGain << "." << std::endl;
        }

        if (newBlueGain >= minBlueGain && newBlueGain <= maxBlueGain) {
            if (!SetMonitorRedGreenOrBlueGain(monitorHandle, MC_BLUE_GAIN, newBlueGain)) {
                std::cerr << "Failed to set blue gain. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Blue gain value out of range. Must be between " << minBlueGain << " and " << maxBlueGain << "." << std::endl;
        }
    }

    void setRGBDrive(DWORD newRedDrive, DWORD newGreenDrive, DWORD newBlueDrive) {
        if (newRedDrive >= minRedDrive && newRedDrive <= maxRedDrive) {
            if (!SetMonitorRedGreenOrBlueDrive(monitorHandle, MC_RED_DRIVE, newRedDrive)) {
                std::cerr << "Failed to set red drive. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Red drive value out of range. Must be between " << minRedDrive << " and " << maxRedDrive << "." << std::endl;
        }

        if (newGreenDrive >= minGreenDrive && newGreenDrive <= maxGreenDrive) {
            if (!SetMonitorRedGreenOrBlueDrive(monitorHandle, MC_GREEN_DRIVE, newGreenDrive)) {
                std::cerr << "Failed to set green drive. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Green drive value out of range. Must be between " << minGreenDrive << " and " << maxGreenDrive << "." << std::endl;
        }

        if (newBlueDrive >= minBlueDrive && newBlueDrive <= maxBlueDrive) {
            if (!SetMonitorRedGreenOrBlueDrive(monitorHandle, MC_BLUE_DRIVE, newBlueDrive)) {
                std::cerr << "Failed to set blue drive. Error: " << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "Blue drive value out of range. Must be between " << minBlueDrive << " and " << maxBlueDrive << "." << std::endl;
        }
    }


    HANDLE getHandle() const { return monitorHandle; }
    DWORD getMinBrightness() const { return minBrightness; }
    DWORD getCurrentBrightness() const { return currentBrightness; }
    DWORD getMaxBrightness() const { return maxBrightness; }
    DWORD getMinContrast() const { return minContrast; }
    DWORD getCurrentContrast() const { return currentContrast; }
    DWORD getMaxContrast() const { return maxContrast; }
    DWORD getMinRedGain() const { return minRedGain; }
    DWORD getCurrentRedGain() const { return currentRedGain; }
    DWORD getMaxRedGain() const { return maxRedGain; }
    DWORD getMinGreenGain() const { return minGreenGain; }
    DWORD getCurrentGreenGain() const { return currentGreenGain; }
    DWORD getMaxGreenGain() const { return maxGreenGain; }
    DWORD getMinBlueGain() const { return minBlueGain; }
    DWORD getCurrentBlueGain() const { return currentBlueGain; }
    DWORD getMaxBlueGain() const { return maxBlueGain; }
    DWORD getMinRedDrive() const { return minRedDrive; }
    DWORD getCurrentRedDrive() const { return currentRedDrive; }
    DWORD getMaxRedDrive() const { return maxRedDrive; }
    DWORD getMinGreenDrive() const { return minGreenDrive; }
    DWORD getCurrentGreenDrive() const { return currentGreenDrive; }
    DWORD getMaxGreenDrive() const { return maxGreenDrive; }
    DWORD getMinBlueDrive() const { return minBlueDrive; }
    DWORD getCurrentBlueDrive() const { return currentBlueDrive; }
    DWORD getMaxBlueDrive() const { return maxBlueDrive; }
    MC_COLOR_TEMPERATURE getColorTemperature() const { return colorTemperature; }



};


class MonitorControl {
private:
    std::vector<Monitor> monitors;

    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
        MonitorControl* pThis = reinterpret_cast<MonitorControl*>(dwData);
        DWORD numPhysicalMonitors;
        if (GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numPhysicalMonitors)) {
            std::vector<PHYSICAL_MONITOR> physicalMonitors(numPhysicalMonitors);
            if (GetPhysicalMonitorsFromHMONITOR(hMonitor, numPhysicalMonitors, physicalMonitors.data())) {
                for (DWORD i = 0; i < numPhysicalMonitors; ++i) {
                    pThis->monitors.push_back(Monitor(physicalMonitors[i].hPhysicalMonitor));
                }
            }
        }
        return TRUE;
    }

public:
    MonitorControl() {
        updateMonitors();
    }

    void updateMonitors() {
        monitors.clear();
        EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(this));
    }

    void resetToDefaultSettings() {
        for (Monitor& monitor : monitors) {
            monitor.resetToDefaultSettings();
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        for (Monitor& monitor : monitors) {
            monitor.updateMonitorSettings();
        }
    }

    void setBrightness(DWORD newBrightness) {
        for (Monitor& monitor : monitors) {
            monitor.setBrightness(newBrightness);
            monitor.updateMonitorSettings();
        }
    }

    void setContrast(DWORD newContrast) {
        for (Monitor& monitor : monitors) {
            monitor.setContrast(newContrast);
            monitor.updateMonitorSettings();
        }
    }

    void setColorTemperature(MC_COLOR_TEMPERATURE newColorTemperature) {
        for (Monitor& monitor : monitors) {
            monitor.setColorTemperature(newColorTemperature);
            monitor.updateMonitorSettings();
        }
    }

    void setRGBGain(DWORD newRedGain, DWORD newGreenGain, DWORD newBlueGain) {
        for (Monitor& monitor : monitors) {
            monitor.setRGBGain(newRedGain, newGreenGain, newBlueGain);
            monitor.updateMonitorSettings();
        }
    }

    void setRGBDrive(DWORD newRedDrive, DWORD newGreenDrive, DWORD newBlueDrive) {
        for (Monitor& monitor : monitors) {
            monitor.setRGBDrive(newRedDrive, newGreenDrive, newBlueDrive);
            monitor.updateMonitorSettings();
        }
    }


    int getNumberMonitors() const { return monitors.size(); }

    void printMonitorInfo() const {
        std::cout << "======================" << std::endl;
        std::cout << "Number of monitors: " << getNumberMonitors() << std::endl;
        for (size_t i = 0; i < monitors.size(); ++i) {
            std::cout << "======================" << std::endl;
            std::cout << "Monitor: " << i + 1 << std::endl;
            std::cout << "Handle: " << monitors[i].getHandle() << std::endl;
            std::cout << "Minimum Brightness: " << monitors[i].getMinBrightness() << std::endl;
            std::cout << "Current Brightness: " << monitors[i].getCurrentBrightness() << std::endl;
            std::cout << "Maximum Brightness: " << monitors[i].getMaxBrightness() << std::endl;
            std::cout << "Minimum Contrast: " << monitors[i].getMinContrast() << std::endl;
            std::cout << "Current Contrast: " << monitors[i].getCurrentContrast() << std::endl;
            std::cout << "Maximum Contrast: " << monitors[i].getMaxContrast() << std::endl;
            std::cout << "Red Gain: " << monitors[i].getCurrentRedGain() << std::endl;
            std::cout << "Green Gain: " << monitors[i].getCurrentGreenGain() << std::endl;
            std::cout << "Blue Gain: " << monitors[i].getCurrentBlueGain() << std::endl;
            std::cout << "Red Drive: " << monitors[i].getCurrentRedDrive() << std::endl;
            std::cout << "Green Drive: " << monitors[i].getCurrentGreenDrive() << std::endl;
            std::cout << "Blue Drive: " << monitors[i].getCurrentBlueDrive() << std::endl;
            std::cout << "Color Temperature: " << monitors[i].getColorTemperature() << std::endl;
        }
    }
};

int main() {
    MonitorControl monitorConntrol;
    monitorConntrol.printMonitorInfo();
    return 0;
}

