#ifndef SettingInfo_h
#define SettingInfo_h
#include <Preferences.h>

#define NOT_INITIALIZED -999

const char* NO_LABEL = nullptr;

class SettingInfo
{
  int value;
  int defaultValue;
  int conversionFactor;
  const char *name;
  const char *memoryName;
  const char ** labels;
  int numLabels = 0;
  int delta = 0;

public:
  SettingInfo() : value(NOT_INITIALIZED){};
  SettingInfo(int iValue, int iDefaultValue, const char *sName, const char *sMemoryName, int iConversionFactor, int iDelta, int iNumLabels, const char* cLabels[]);
  int getValue() { return value; }
  void setValue(int iValue) { value = iValue; }
  const char *getMemoryName() { return memoryName; }
  int getConversionFactor() { return conversionFactor; }
  int getMachineValue() { return getValue() * getConversionFactor(); }
  int getNumLabels() {return numLabels;}
  int getDelta(){return delta;}
  const char* getLabel(int index) {
    if (labels != nullptr && index >= 0 && index < numLabels) {
      return labels[index];
    } else {
      return NO_LABEL;
    }
  }

  void store(Preferences* preferences, int iValue) {
    setValue(iValue);
    preferences->putUInt(getMemoryName(), iValue);
  };
  int reload(Preferences* preferences) {
    int settingValue = preferences->getUInt(getMemoryName(), defaultValue);
    setValue(settingValue);
    return settingValue;
  };

  const char *getName() { return name; }
};

SettingInfo::SettingInfo(int iValue, int iDefaultValue, const char *sName, const char *sMemoryName, 
  int iConversionFactor, int iDelta, int iNumLabels, const char* cLabels[])
    : value(iValue), name(sName), memoryName(sMemoryName), conversionFactor(iConversionFactor), delta(iDelta), numLabels(iNumLabels), labels(cLabels){};

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