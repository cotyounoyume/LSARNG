#include "LSARConfig.h"

std::vector<LSARItemARMA> LSARConfig::armas = std::vector<LSARItemARMA>();
std::vector<LSARItemARMO> LSARConfig::armos = std::vector<LSARItemARMO>();
std::unordered_map<std::string, LSARItemARMA> LSARConfig::arma_map = std::unordered_map<std::string, LSARItemARMA>();

uint8_t LSARConfig::modIndexes[5] = {};
bool LSARConfig::hasDlc[5] = {};
int LSARConfig::wOrLGlobal = 0;

class PapyrusActor;

LSARConfig::LSARConfig() {}

void LSARConfig::initArmorAddon(void) {
    auto playerRef = RE::TESForm::LookupByID<RE::TESObjectREFR>(0x14);
    auto cell = playerRef->GetParentCell();
    uint32_t num = cell->GetRuntimeData().objectList.size();
    for (uint32_t i = 0; i < cell->GetRuntimeData().objectList.size(); i++) {
        auto objRef = cell->GetRuntimeData().objectList[i];
        if (!objRef) continue;
        auto npc = RE::TESForm::LookupByID<RE::TESNPC>(objRef->formID);
        if (!npc) continue;
        if (npc->GetSex() == RE::SEX::kFemale)
            logs::info("npc:{}({:08X}) is Female", npc->GetFormEditorID(), npc->formID);
    }
}

void LSARConfig::setModIndexes(void) {
    setModIndex("Skyrim.esm", 0);
    setModIndex("Dawnguard.esm", 1);
    setModIndex("HearthFires.esm", 2);
    setModIndex("Dragonborn.esm", 3);
    setModIndex("zzLSAR.esp", 4);
}

void LSARConfig::setModIndex(std::string filename, int num) 
{
    auto dhnd = RE::TESDataHandler::GetSingleton();
    if (dhnd->LookupModByName(filename) != NULL) {
        hasDlc[num] = true;
        modIndexes[num] = dhnd->GetLoadedModIndex(filename).value();
        logs::info("{}'s index = {:02X}", filename, modIndexes[num]);
    } else {
        hasDlc[num] = false;
    }
}

void LSARConfig::readIniFileARMA() {
    logs::info("in  readIniFile() ARMA");
    std::string iniFile = _getIniFilePathARMA();
    logs::info("iniFilePath = {}", iniFile.c_str());
    std::ifstream ifs(iniFile);
    if (ifs.fail()) {
        logs::info("error, can't open {}", iniFile.c_str());
        return;
    }
    // ArchmageRobesAA	010146C3	C	L	1	1	1	1	3	0	zzLSARLCArchmageRobesAA

    std::regex reTAG("^#DO_NOT_EDIT_THIS_LINE:(\\w+).*$");
    std::smatch tagMatch;
    std::regex reComment("^\\s*#.*$");
    std::regex reConf(
        "^\\s*(\\w+)\\t(\\w+)\\t(\\w)\\t(\\w)\\t(\\d)\\t(\\d)\\t(\\d)\\t(\\d)\\t(\\d+)\\t(\\d)\\t(\\w+)\\s*$");

    std::string str;
    std::string currentTag = "";
    while (getline(ifs, str)) {
        logs::info("line = {}", str.c_str());
        if (std::regex_match(str, tagMatch, reTAG)) {
            currentTag = tagMatch.str(1);
            logs::info("currentTag = {}", currentTag.c_str());
        } else if (std::regex_match(str, reComment)) {
            //			nop
        } else if (std::regex_match(str, reConf)) {
            LSARItemARMA akItem = _str2arma(str);
            if (akItem.isEnabled) {
                arma_map[akItem.key] = akItem;
                armas.push_back(akItem);
            } else {
                logs::info("SKIP: {} needs dlc", akItem.name.c_str());
            }
        } else {
            logs::info("ERROR: invalid line: {}", str.c_str());
        }
    }
    logs::info("OUT readIniFile() ARMA");
}

void LSARConfig::readIniFileARMO() {
    logs::info("in  readIniFile() ARMO");
    std::string iniFile = _getIniFilePathARMO();
    logs::info("iniFilePath = {}", iniFile.c_str());
    std::ifstream ifs(iniFile);
    if (ifs.fail()) {
        logs::info("error, can't open {}", iniFile.c_str());
        return;
    }
    // #ARMO_ID	WorL	ARMA_NAME				ARMO_NAME
    // 020047D7	W		BlindMothPriestRobesAA	DLC1ClothesMothPriestRobes

    std::smatch tagMatch;
    std::regex reComment("^\\s*#.*$");
    std::regex reConf("^\\s*(\\w+)\\t(\\w+)\\t(\\d)\\t(\\w+)\\t(\\w+)\\s*$");

    std::string str;
    std::string currentTag = "";
    while (getline(ifs, str)) {
        logs::info("line = {}", str.c_str());
        if (std::regex_match(str, reComment)) {
            //			nop
        } else if (std::regex_match(str, reConf)) {
            LSARItemARMO akItem = _str2armo(str);
            if (arma_map[akItem.key].isEnabled) {
                armos.push_back(akItem);
            } else {
                logs::info("ERROR: key = {}, invalid line: {}", akItem.key.c_str(), str.c_str());
            }
        } else {
            logs::info("debug: invalid line: {}", str.c_str());
        }
    }
    logs::info("OUT readIniFile() ARMO");
}

LSARItemARMA LSARConfig::_str2arma(std::string str) {
    std::vector<std::string> items = Utility::Split(str, '\t');
    LSARItemARMA akItem;

    // ArchmageRobesAA	010146C3	C	L	1	1	1	1	3	0	zzLSARLCArchmageRobesAA

    akItem.name = items[0];
    akItem.formId = _getFormID(items[1], modIndexes[4], true);
    akItem.apparelType = _getApparelType(str, items[2]);
    akItem.isLoose = akItem.isLooseOrg = _getLRConfig(str, items[3]);
    akItem.isSkirtOn = akItem.isSkirtOnOrg = _getBooleanConfig(str, items[4]);
    akItem.isPantyOn = akItem.isPantyOnOrg = _getBooleanConfig(str, items[5]);
    akItem.isBraOn = akItem.isBraOnOrg = _getBooleanConfig(str, items[6]);
    akItem.isGreavesOn = akItem.isGreavesOnOrg = _getBooleanConfig(str, items[7]);
    akItem.priority = std::stoi(items[8]);
    akItem.dlcType = std::stoi(items[9]);
    akItem.note = items[10];
    akItem.key = items[3] + akItem.name;

    if (akItem.dlcType > 0 && hasDlc[akItem.dlcType] || akItem.dlcType == 0)
        akItem.isEnabled = akItem.isEnabledOrg = true;
    else
        akItem.isEnabled = akItem.isEnabledOrg = false;
    return akItem;
}

LSARItemARMO LSARConfig::_str2armo(std::string str) {
    std::vector<std::string> items = Utility::Split(str, '\t');
    LSARItemARMO akItem;

    // #ARMO_ID	WorL	DLC	ARMA_NAME				ARMO_NAME
    // 020047D7	W		0	BlindMothPriestRobesAA	DLC1ClothesMothPriestRobes

    akItem.isLoose = akItem.isLooseOrig = _getLRConfig(str, items[1]);
    akItem.name = items[3];
    akItem.key = items[1] + akItem.name;
    uint32_t dlcType = std::stoi(items[2]);
    uint8_t modIndex = modIndexes[dlcType];
    akItem.armoID = _getFormID(items[0], modIndexes[dlcType], false);
    akItem.isEnabled = arma_map[akItem.key].isEnabled;
    akItem.armaDstID = arma_map[akItem.key].formId;
    akItem.apparelType = arma_map[akItem.key].apparelType;
    auto srcAM = LSARFunc::GetArmorFromFormID(akItem.armoID);

    int target_num = 0;
    if (akItem.apparelType != 9) {
        for (int i = 0; i < srcAM->armorAddons.size(); i++) {
            if (LSARFunc::GetSlotMask(srcAM->armorAddons[i], 32)){
                target_num = i;
                break;
            }
        }
        akItem.num = target_num;

        akItem.armaOrgID = srcAM->armorAddons[target_num]->formID;
    } else {
        target_num = 1;  // hard coding for cultist mask
        akItem.num = target_num;
        akItem.armaOrgID = srcAM->armorAddons[target_num]->formID;
    }

    return akItem;
}

void LSARConfig::resetArmorAddon(uint32_t isLSARClothOn, uint32_t isLSARLArmorOn, uint32_t isLSARHArmorOn, uint32_t isLoose,
                                 uint32_t slot49, uint32_t slot48, uint32_t slot56, uint32_t slot53) {
    logs::info("in  resetArmorAddon()");
    std::vector<LSARItemARMA>::iterator it;
    for (it = armas.begin(); it != armas.end(); it++) {
        if ((it->apparelType == 0 && isLSARClothOn == 0) || (it->apparelType == 1 && isLSARLArmorOn == 0) ||
            (it->apparelType == 2 && isLSARHArmorOn == 0)) {
            arma_map[it->key].isEnabled = false;
        } else if ((it->apparelType == 0 && isLSARClothOn == 1) || (it->apparelType == 1 && isLSARLArmorOn == 1) ||
                   (it->apparelType == 2 && isLSARHArmorOn == 1)) {
            arma_map[it->key].isEnabled = true;
        } else {
            arma_map[it->key].isEnabled = arma_map[it->key].isEnabledOrg;
        }
        if (isLoose != 2)
            wOrLGlobal = (isLoose == 1) ? 1 : 2;
        else
            wOrLGlobal = 0;

        if (slot49 != 2)
            arma_map[it->key].isSkirtOn = (slot49 == 1) ? true : false;
        else
            arma_map[it->key].isSkirtOn = arma_map[it->key].isSkirtOnOrg;
        if (slot48 != 2)
            arma_map[it->key].isPantyOn = (slot48 == 1) ? true : false;
        else
            arma_map[it->key].isPantyOn = arma_map[it->key].isPantyOnOrg;
        if (slot56 != 2)
            arma_map[it->key].isBraOn = (slot56 == 1) ? true : false;
        else
            arma_map[it->key].isBraOn = arma_map[it->key].isBraOnOrg;
        if (slot53 != 2)
            arma_map[it->key].isGreavesOn = (slot53 == 1) ? true : false;
        else
            arma_map[it->key].isGreavesOn = arma_map[it->key].isGreavesOnOrg;
    }
    makeRealAll();
    logs::info("out resetArmorAddon()");
}

void LSARConfig::resetArmor() {
    logs::info("in  resetArmor()");
    std::vector<LSARItemARMO>::iterator it;
    for (it = armos.begin(); it != armos.end(); it++) {
        if (!it->isEnabled) continue;

        std::string key = "";
        if (it->apparelType == 9 || wOrLGlobal == 0) {
            key = it->key;
        } else {
            if (wOrLGlobal == 1)
                key = "L" + it->name;
            else
                key = "W" + it->name;
        }

        LSARItemARMA arma = arma_map[key];
        auto targetARMO = LSARFunc::GetArmorFromFormID(it->armoID);
        uint32_t srcARMAFormId = targetARMO->armorAddons[it->num]->formID;
        uint32_t dstARMAFormId = 0;
        if (arma.isEnabled)
            dstARMAFormId = arma.formId;
        else
            dstARMAFormId = it->armaOrgID;

        auto dstARMA = LSARFunc::GetArmorAddonFromFormID(dstARMAFormId);
        targetARMO->armorAddons[it->num] = dstARMA;

        if (arma.isEnabled && it->apparelType == 9) {
            targetARMO->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet);
        }
    }

    logs::info("out resetArmor()");
}

void LSARConfig::makeRealAll() {
    logs::info("in  makeRealAll()");
    std::vector<LSARItemARMA>::iterator it;
    for (it = armas.begin(); it != armas.end(); it++) {
        _makeReal(arma_map[it->key]);
    }
    logs::info("out makeRealAll()");
}

void LSARConfig::_makeReal(LSARItemARMA akItem) {
    auto srcAA = LSARFunc::GetArmorAddonFromFormID(akItem.formId);
    if (!srcAA) {
        logs::info("ERROR: srcAA(%X) is null", akItem.formId);
        return;
    }
    if (!akItem.isEnabled) return;
    srcAA->data.priorities[RE::SEXES::kFemale] = akItem.priority;
    _setSlotLSAR(akItem);
}

std::string LSARConfig::_getIniFilePathARMA() {
    return _getIniFilePath("ARMA");
}

std::string LSARConfig::_getIniFilePathARMO() {
     return _getIniFilePath("ARMO");
}

std::string LSARConfig::_getIniFilePath(std::string type) {
     return Utility::GetCurrentExecutableDirectory() + "\\Data\\SKSE\\plugins\\zzLSARSetting_" + type + ".ini";
}

bool LSARConfig::_getBooleanConfig(std::string str, std::string col) {
    bool result = false;
    int v = std::stoi(col);
    if (v == 0)
        result = false;
    else if (v == 1)
        result = true;
    else
        logs::info("ERROR: invalid argument %s in %s", col, str);
    return result;
}

bool LSARConfig::_getLRConfig(std::string str, std::string col) {
    bool result = false;
    if (col.compare("L") == 0)
        result = true;
    else if (col.compare("W") == 0)
        result = false;
    else
        logs::info("ERROR: invalid argument %s in %s", col, str);
    return result;
}

uint32_t LSARConfig::_getFormID(std::string formIDstr, uint8_t modIndex, bool isARMAMode) {
    uint32_t formID;
    uint8_t ioffset = 24;
    formID = std::strtoul(formIDstr.c_str(), NULL, 16);
    formID = formID & 0x00FFFFFF;

    formID = formID | (modIndex << ioffset);
    return formID;
}

uint32_t LSARConfig::_getApparelType(std::string str, std::string col) {
    uint32_t result = 99;
    if (col.compare("C") == 0)
        result = 0;
    else if (col.compare("L") == 0)
        result = 1;
    else if (col.compare("H") == 0)
        result = 2;
    else if (col.compare("O") == 0)
        result = 9;
    else
        logs::info("ERROR: invalid argument %s in %s", col, str);
    return result;
}

void LSARConfig::_setSlotLSAR(LSARItemARMA akItem) {
    auto srcAA = LSARFunc::GetArmorAddonFromFormID(akItem.formId);
    if (!srcAA) {
            logs::info("ERROR: formID:{:X} is null", akItem.formId);
            return;
    }
    //logs::info("Before: [:08X]:{}", srcAA->formID, srcAA->GetFormEditorID());
    //LSARConfig::PrintAllSlotFromARMA(srcAA);

    LSARFunc::RemoveSlotFromMask(srcAA, 49);
    LSARFunc::RemoveSlotFromMask(srcAA, 48);
    LSARFunc::RemoveSlotFromMask(srcAA, 53);
    LSARFunc::RemoveSlotFromMask(srcAA, 56);

    if (akItem.apparelType == 9) {
        if (akItem.isSkirtOn) LSARFunc::AddSlotToMask(srcAA, 30);
        if (akItem.isPantyOn) LSARFunc::AddSlotToMask(srcAA, 31);
        if (akItem.isBraOn) LSARFunc::AddSlotToMask(srcAA, 41);
        if (akItem.isGreavesOn) LSARFunc::AddSlotToMask(srcAA, 42);
    } else {
        if (akItem.isSkirtOn) LSARFunc::AddSlotToMask(srcAA, 49);
        if (akItem.isPantyOn) LSARFunc::AddSlotToMask(srcAA, 48);
        if (akItem.isBraOn) LSARFunc::AddSlotToMask(srcAA, 56);
        if (akItem.isGreavesOn) LSARFunc::AddSlotToMask(srcAA, 53);

        if (!akItem.isGreavesOn) LSARFunc::RemoveSlotFromMask(srcAA, 38);
    }
    //logs::info("After:  [:08X]:{}", srcAA->formID, srcAA->GetFormEditorID());
    //LSARConfig::PrintAllSlotFromARMA(srcAA);
}

void LSARConfig::debugPrint() {
    std::vector<LSARItemARMA>::iterator it;
    for (it = armas.begin(); it != armas.end(); it++) {
        _printItemTest2(*it);
    }
}

void LSARConfig::_printItemTest(LSARItemARMA akItem) {
    logs::info("debug: {}", akItem.key.c_str());
    logs::info("debug: {}", akItem.name.c_str());
    logs::info("debug: {:08X}", akItem.formId);
    logs::info("debug: {}", akItem.apparelType);
    logs::info("debug: {:X}", akItem.isLoose);
    logs::info("debug: {:X}", akItem.isSkirtOn);
    logs::info("debug: {:X}", akItem.isPantyOn);
    logs::info("debug: {:X}", akItem.isBraOn);
    logs::info("debug: {:X}", akItem.isGreavesOn);
    logs::info("debug: {}", akItem.priority);
    logs::info("debug: {}", akItem.dlcType);
    logs::info("debug: {}", akItem.note.c_str());
}

void LSARConfig::_printItemTest2(LSARItemARMA akItem) {
    auto srcAA = LSARFunc::GetArmorAddonFromFormID(akItem.formId);
    if (!srcAA) {
        logs::info("ERROR: akItem not found(formId={:08X})", akItem.formId);
    } else {
        logs::info("\t{}: name={}, key={}, formId={:08X}", 
            akItem.note.c_str(), akItem.name.c_str(), akItem.key.c_str(), akItem.formId);
        logs::info("\t\t(dlcType, apparelType, isEnabled, priority)=({:X}, {:X}, {:X}, {})", 
            akItem.dlcType, akItem.apparelType, akItem.isEnabled, akItem.priority);
        logs::info("\t\t(isLoose, isSkirtOn, isPantyOn, isBraOn, isGreavesOn)=({:X}, {:X}, {:X}, {:X}, {:X})",
            akItem.isLoose, akItem.isSkirtOn, akItem.isPantyOn, akItem.isBraOn, akItem.isGreavesOn);
        std::string modelname = LSARFunc::GetArmorAddonModelName(srcAA);
        logs::info("\t\tpath = {}", modelname);
    }
}

void LSARConfig::_printItemTest3(LSARItemARMA akItem) {
    //RE::TESObjectARMA* srcAA = (RE::TESObjectARMA*)LookupFormByID(akItem.formId);
    auto srcAA = LSARFunc::GetArmorAddonFromFormID(akItem.formId);
    if (!srcAA) {
        logs::info("ERROR: akItem not found(formId={:08X})", akItem.formId);
    } else {
        logs::info("debug: srcFormID={:08X}", akItem.formId);
        logs::info("debug: key={}", akItem.key.c_str());
        logs::info("debug: name={}", akItem.name.c_str());
        logs::info("debug: note={}", akItem.note.c_str());
        logs::info("debug: priority={}", srcAA->data.priorities[RE::SEXES::kFemale]);
        std::string modelnameM = LSARFunc::GetArmorAddonModelName(srcAA, false);
        std::string modelnameF = LSARFunc::GetArmorAddonModelName(srcAA);
        logs::info("debug: pathM={}", modelnameM);
        logs::info("debug: pathF={}", modelnameF);
    }
}

void LSARConfig::PrintAllSlotFromARMA(RE::TESObjectARMA* aa) 
{
    if (!aa) return;
    logs::info("ARMA: [{:08X}]: {}", aa->formID, aa->GetFormEditorID());
    for (int i = 30; i <= 61; i++) {
        auto isOn = LSARFunc::GetSlotMask(aa, i);
        logs::info("  {:02d}: {}", i, isOn);
    }
}

void LSARConfig::_printSlotState(uint32_t slot) {
    for (int i = 32; i <= 62; i++) {
        bool active = (slot & (1 << (i - 30))) > 0;
        //logs::info("\t%02d\t%X\t%08X", i, active, slot);
        logs::info("\t{:02d}\t{:X}\t{:08X}", i, active, slot);
    }
}

