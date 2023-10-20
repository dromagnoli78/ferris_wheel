#ifndef SettingInfo_h
#define SettingInfo_h
#include <Preferences.h>

class SettingInfo
{
  int value;
  int conversionFactor;
  const char *name;
  const char *memoryName;

public:
  SettingInfo() : value(-999){};
  SettingInfo(int iValue, const char *sName, const char *sMemoryName, int iConversionFactor);
  int getValue() { return value; }
  void setValue(int iValue) { value = iValue; }
  const char *getMemoryName() { return memoryName; }
  int getConversionFactor() { return conversionFactor; }
  int getMachineValue() { return getValue() * getConversionFactor(); }
  void store(Preferences* preferences, int iValue) {
    setValue(iValue);
    preferences->putUInt(getMemoryName(), iValue);
  };
  int reload(Preferences* preferences) {
    int settingValue = preferences->getUInt(getMemoryName(), 0U);
    setValue(settingValue);
    return settingValue;
  };

  const char *getName() { return name; }
};

SettingInfo::SettingInfo(int iValue, const char *sName, const char *sMemoryName, int iConversionFactor)
    : value(iValue), name(sName), memoryName(sMemoryName), conversionFactor(iConversionFactor){};

class BlinkingConfig
{
public:
  uint32_t colorA;
  uint32_t colorB;
  int interval;
  int duration;
  BlinkingConfig() : duration(-1){};
  BlinkingConfig(uint32_t cColorA, uint32_t cColorB, int iInterval, int iDuration);
};

BlinkingConfig::BlinkingConfig(uint32_t cColorA, uint32_t cColorB, int iInterval, int iDuration)
    : colorA(cColorA), colorB(cColorB), interval(iInterval), duration(iDuration){};

#endif