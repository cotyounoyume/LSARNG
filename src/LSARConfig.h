#pragma once

#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "PCH.h"
#include "LSARFunc.h"
#include "Utility.h"
//#include "skse64/GameData.h"
//#include "skse64/GameExtraData.h"
//#include "skse64/GameForms.h"
//#include "skse64/GameObjects.h"
//#include "skse64/GameRTTI.h"
//#include "skse64/GameReferences.h"
//#include "skse64/GameThreads.h"
//#include "skse64/NiExtraData.h"
//#include "skse64/PapyrusActor.h"
//#include "skse64_common/Utilities.h"

typedef struct _LSARItemARMO {
    std::string key;
    std::string name;
    uint32_t armoID;
    uint32_t armaOrgID;
    uint32_t armaDstID;
    bool isLoose;
    bool isLooseOrig;
    bool isEnabled;
    //uint32_t slotOrg;
    uint32_t num;
    uint32_t apparelType;  // 0:CLOTH, 1:LIGHT_ARMOR, 2:HEAVY_ARMOR, 9:HELMET
} LSARItemARMO;

typedef struct _LSARItemARMA {
    std::string key;
    std::string name;
    uint32_t formId;
    bool isEnabled;
    bool isLoose;
    bool isSkirtOn;
    bool isPantyOn;
    bool isBraOn;
    bool isGreavesOn;
    uint32_t priority;
    uint32_t dlcType;
    uint32_t apparelType;  // 0:CLOTH, 1:LIGHT_ARMOR, 2:HEAVY_ARMOR, 9:HELMET
    std::string note;

    bool isEnabledOrg;
    bool isLooseOrg;
    bool isSkirtOnOrg;
    bool isPantyOnOrg;
    bool isBraOnOrg;
    bool isGreavesOnOrg;
} LSARItemARMA;

class LSARConfig {
private:
    static std::vector<LSARItemARMO> armos;
    static std::vector<LSARItemARMA> armas;
    static std::unordered_map<std::string, LSARItemARMA> arma_map;
    static bool hasDlc[5];
    static uint8_t modIndexes[5];

    // public:
    static void _makeReal(LSARItemARMA akItem);
    static void _printItemTest(LSARItemARMA akItem);
    static void _printItemTest2(LSARItemARMA akItem);
    static LSARItemARMA _str2arma(std::string str);
    static LSARItemARMO _str2armo(std::string str);
    static void _setSlotLSAR(LSARItemARMA akItem);
    static void _printItemTest3(LSARItemARMA akItem);
    static void _printSlotState(uint32_t slot);
    static std::string _getIniFilePathARMA();
    static std::string _getIniFilePathARMO();
    static std::string _getIniFilePath(std::string type);
    static bool _getBooleanConfig(std::string str, std::string col);
    static bool _getLRConfig(std::string str, std::string col);
    static uint32_t _getFormID(std::string formIDstr, uint8_t modIndex, bool isARMAMode);
    static uint32_t _getApparelType(std::string str, std::string col);

public:
    LSARConfig();
    ~LSARConfig() { ; };
    static int wOrLGlobal;  // 0:Default, 1:Loose, 2:WellFitted
    static void readIniFileARMA();
    static void readIniFileARMO();
    static void debugPrint();
    static void setModIndexes();
    static void makeRealAll();
    static void resetArmorAddon(uint32_t isLSARClothOn, uint32_t isLSARLArmorOn, uint32_t isLSARHArmorOn, uint32_t isLoose,
                                uint32_t slot49, uint32_t slot48, uint32_t slot56, uint32_t slot53);
    static void resetArmor();
    static void initArmorAddon();
};
