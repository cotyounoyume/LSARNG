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

    //for (uint32_t n = 0; n < num; ++n) {
    //    RE::TESObjectREFR* pRef = NULL;
    //    pPC->parentCell->objectList.GetNthItem(n, pRef);
    //    if (pRef && (pRef->formType == 43 || pRef->baseForm->formType == 43)) {
    //        RE::TESNPC* rActor = DYNAMIC_CAST(pRef->baseForm, TESForm, TESNPC);
    //        RE::Actor* rrActor = DYNAMIC_CAST(pRef->baseForm, TESForm, Actor);
    //        uint32_t sex = rActor ? CALL_MEMBER_FN(rActor, GetSex)() : 0;
    //        if (sex == 1) {
    //            /*
    //            logs::info("NPC(%08X): sex=%d",pRef->formID,sex);
    //            //TESForm* akItem = papyrusActor::GetWornForm(rrActor, 0x4);
    //            TESForm* akItem = rrActor->
    //            papyrusActor::GetWornForm(rrActor, 0x4);

    //            logs::info("NPC(%08X): akItem:%08X",pRef->formID,akItem->formID);
    //            papyrusActor::EquipItemEx(rrActor, akItem, 0, false, false);
    //            logs::info("NPC(%08X): EquipItemEx:%08X",pRef->formID,akItem->formID);
    //            papyrusActor::QueueNiNodeUpdate(rrActor);
    //            logs::info("NPC(%08X): QueueNiNodeUpdate",pRef->formID);
    //            */
    //        }
    //        //			BSFixedString npcName = rActor->GetFullName();
    //        //			std::string str = std::string(npcName.data);
    //        logs::info("NPC found: %d, %08X, sex:%d", n, pRef->formID, sex);
    //        //			logs::info("NPC:%08X: sex:%d, name:%s", rActor->formID, sex, str);
    //        n++;
    //    }
    //}
}

void LSARConfig::setModIndexes(void) {
    auto dhnd = RE::TESDataHandler::GetSingleton();
    if (dhnd->LookupModByName("Skyrim.esm") != NULL) {
        hasDlc[0] = true;
        //		modIndexes[0] = dhnd->GetModIndex("Skyrim.esm");
        //modIndexes[0] = dhnd->LookupModByName("Skyrim.esm")->modIndex;
        modIndexes[0] = dhnd->GetLoadedModIndex("Skyrim.esm").value();
        logs::info("Skyrim.esm's index = {:02X}", modIndexes[0]);
    } else {
        hasDlc[0] = false;
    }
    if (dhnd->LookupModByName("Dawnguard.esm") != NULL) {
        hasDlc[1] = true;
        //		modIndexes[1] = dhnd->GetModIndex("Dawnguard.esm");
        //modIndexes[1] = dhnd->LookupModByName("Dawnguard.esm")->modIndex;
        modIndexes[1] = dhnd->GetLoadedModIndex("Dawnguard.esm").value();
        logs::info("Dawnguard.esm's index = {:02X}", modIndexes[1]);
    } else {
        hasDlc[1] = false;
    }
    if (dhnd->LookupModByName("HearthFires.esm") != NULL) {
        hasDlc[2] = true;
        //		modIndexes[2] = dhnd->GetModIndex("HearthFires.esm");
        //modIndexes[2] = dhnd->LookupModByName("HearthFires.esm")->modIndex;
        modIndexes[2] = dhnd->GetLoadedModIndex("HearthFires.esm").value();
        logs::info("HearthFires.e's index = {:02X}", modIndexes[2]);
    } else {
        hasDlc[2] = false;
    }
    if (dhnd->LookupModByName("Dragonborn.esm") != NULL) {
        hasDlc[3] = true;
        //		modIndexes[3] = dhnd->GetModIndex("Dragonborn.esm");
        //modIndexes[3] = dhnd->LookupModByName("Dragonborn.esm")->modIndex;
        modIndexes[3] = dhnd->GetLoadedModIndex("Dragonborn.esm").value();
        logs::info("Dragonborn.esm's index = {:02X}", modIndexes[3]);
    } else {
        hasDlc[3] = false;
    }
    if (dhnd->LookupModByName("zzLSAR.esp") != NULL) {
        hasDlc[4] = true;
        //		modIndexes[4] = dhnd->GetModIndex("zzLSAR.esp");
        //modIndexes[4] = dhnd->LookupModByName("zzLSAR.esp")->modIndex;
        modIndexes[4] = dhnd->GetLoadedModIndex("zzLSAR.esm").value();
        logs::info("zzLSAR's index = {:02X}", modIndexes[4]);
    } else {
        hasDlc[4] = false;
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

    //	logs::info("input: formID=%s, modIndex=%02X => converted formID=%08X", items[1].c_str(), modIndexes[4],
    //akItem.formId);
    if (akItem.dlcType > 0 && hasDlc[akItem.dlcType] || akItem.dlcType == 0)
        akItem.isEnabled = akItem.isEnabledOrg = true;
    else
        akItem.isEnabled = akItem.isEnabledOrg = false;
    //	logs::info("modIndexes[0]=%X, modNum=%d, shiftedNum= %X",modIndexes[0], modNum, akItem.dstFormId);

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
    //	logs::info("input: formID=%s, modIndex=%02X => converted formID=%08X", items[0].c_str(), modIndex, akItem.armoID);

    int target_num = 0;
    if (akItem.apparelType != 9) {
        for (int i = 0; i < srcAM->armorAddons.size(); i++) {
            //uint32_t slotmask = srcAM->armorAddons[i]->biped.GetSlotMask();
            //if ((srcAM->armorAddons[i]->biped.GetSlotMask() & 0x4) == 0x4) {

            //if (srcAM->armorAddons[i]->bipedModelData.bipedObjectSlots.any(RE::BGSBipedObjectForm::BipedObjectSlot::kBody)) {
            if (LSARFunc::GetSlotMask(srcAM->armorAddons[i], 32)){
                target_num = i;
                //biped.GetSlotMask() & 0x4) == 0x4) {
                //    target_num = i;
            }
        }
        akItem.num = target_num;

        akItem.armaOrgID = srcAM->armorAddons[target_num]->formID;
    } else {
        target_num = 1;  // hard coding for cultist mask
        akItem.num = target_num;
        akItem.armaOrgID = srcAM->armorAddons[target_num]->formID;
        //akItem.slotOrg = srcAM->armorAddons[target_num]->biped.GetSlotMask();
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
        // nop 2:default
        //		logs::info("key=%s, isLSARClothOn=%d, appareltype=%d, name=%s, isEnabled=%X", it->key.c_str(),
        //isLSARClothOn, it->apparelType, it->name.c_str(), it->isEnabled);
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
        //		if (slot53 != 2) arma_map[it->key].isGreavesOn = (slot53 == 1) ? true : false; else
        //arma_map[it->key].isGreavesOn = arma_map[it->key].isGreavesOnOrg;
        /*
                logs::info("\tslot49org=%X, slot49=%X", arma_map[it->key].isSkirtOnOrg, arma_map[it->key].isSkirtOn);
                logs::info("\tslot48org=%X, slot48=%X", arma_map[it->key].isPantyOnOrg, arma_map[it->key].isPantyOn);
                logs::info("\tslot56org=%X, slot56=%X", arma_map[it->key].isBraOnOrg,   arma_map[it->key].isBraOn);
                logs::info("\tslot53org=%X, slot53=%X", arma_map[it->key].isGreavesOnOrg, arma_map[it->key].isGreavesOn);
        */
    }
    makeRealAll();
    logs::info("out resetArmorAddon()");
}

void LSARConfig::resetArmor() {
    logs::info("in  resetArmor()");
    std::vector<LSARItemARMO>::iterator it;
    for (it = armos.begin(); it != armos.end(); it++) {
        if (it->isEnabled) {
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
            //			logs::info("debug: %s's AA[%d] is changed from %08X(before) to %08X(after), armaOrgID=%08X",
            //it->name.c_str(), it->num, srcARMAFormId, dstARMAFormId, it->armaOrgID);

            if (arma.isEnabled && it->apparelType == 9) {
                //uint32_t newMask = 1 << (42 - 30);
                //targetARMO->bipedObject.SetSlotMask(newMask);

                targetARMO->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet);

                //				logs::info("debug: %s is helmet. slotmask is changed from %08X(original) to %08X(mod)",
                //it->name.c_str(), it->slotOrg, newMask);
            }
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
    // TESObjectARMA* srcAA = (TESObjectARMA*)LookupFormByID(akItem.formId);
    auto srcAA = LSARFunc::GetArmorAddonFromFormID(akItem.formId);
    if (!srcAA) {
        logs::info("ERROR: srcAA(%X) is null", akItem.formId);
        return;
    }
    if (akItem.isEnabled) {
        // srcAA->data.priority[1] = akItem.priority;
        srcAA->data.priorities[RE::SEXES::kFemale] = akItem.priority;
        _setSlotLSAR(akItem);
        //		_printItemTest3(akItem);
    }
}

std::string LSARConfig::_getIniFilePathARMA() {
    return _getIniFilePath("ARMA");
    // return GetRuntimeDirectory() + "Data\\SKSE\\plugins\\zzLSARSetting_ARMA.ini";
     //return Utility::GetCurrentExecutableDirectory() + "Data\\SKSE\\plugins\\zzLSARSetting_ARMA.ini";
}

std::string LSARConfig::_getIniFilePathARMO() {
     return _getIniFilePath("ARMO");
     //return GetRuntimeDirectory() + "Data\\SKSE\\plugins\\zzLSARSetting_ARMO.ini";
     //return Utility::GetCurrentExecutableDirectory() + "Data\\SKSE\\plugins\\zzLSARSetting_ARMO.ini";
}

std::string LSARConfig::_getIniFilePath(std::string type) {
     return Utility::GetCurrentExecutableDirectory() + "Data\\SKSE\\plugins\\zzLSARSetting_" + type + ".ini";
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
    //TESObjectARMA* srcAA = (TESObjectARMA*)LookupFormByID(akItem.formId);
    auto srcAA = LSARFunc::GetArmorAddonFromFormID(akItem.formId);
    if (!srcAA) {
            logs::info("ERROR: formID:{:X} is null", akItem.formId);
            return;
    }
    //srcAA->RemoveSlotFromMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModPelvisPrimary);   // 49
    //srcAA->RemoveSlotFromMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModMisc1);           // 48
    //srcAA->RemoveSlotFromMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModLegRight);        // 53
    //srcAA->RemoveSlotFromMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModChestSecondary);  // 56

    LSARFunc::RemoveSlotFromMask(srcAA, 49);
    LSARFunc::RemoveSlotFromMask(srcAA, 48);
    LSARFunc::RemoveSlotFromMask(srcAA, 53);
    LSARFunc::RemoveSlotFromMask(srcAA, 56);

    if (akItem.apparelType == 9) {
        //if (akItem.isSkirtOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kNone);       // 30
        //if (akItem.isPantyOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kHair);       // 31
        //if (akItem.isBraOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kLongHair);     // 41
        //if (akItem.isGreavesOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet);  // 42
        if (akItem.isSkirtOn) LSARFunc::AddSlotToMask(srcAA, 30);
        if (akItem.isPantyOn) LSARFunc::AddSlotToMask(srcAA, 31);
        if (akItem.isBraOn) LSARFunc::AddSlotToMask(srcAA, 41);
        if (akItem.isGreavesOn) LSARFunc::AddSlotToMask(srcAA, 42);
    } else {
        //if (akItem.isSkirtOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModPelvisPrimary);  // 49
        //if (akItem.isPantyOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModMisc1);          // 48
        //if (akItem.isBraOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModLegRight);         // 56
        //if (akItem.isGreavesOn) srcAA->AddSlotToMask(RE::BGSBipedObjectForm::BipedObjectSlot::kModChestSecondary);  // 53
        if (akItem.isSkirtOn) LSARFunc::AddSlotToMask(srcAA, 49);
        if (akItem.isPantyOn) LSARFunc::AddSlotToMask(srcAA, 48);
        if (akItem.isBraOn) LSARFunc::AddSlotToMask(srcAA, 56);
        if (akItem.isGreavesOn) LSARFunc::AddSlotToMask(srcAA, 53);

        if (!akItem.isGreavesOn) LSARFunc::RemoveSlotFromMask(srcAA, 38);
    }
            // slot = srcAA->biped.GetSlotMask();
    //if (!srcAA) {
    //    logs::info("ERROR: srcAA(%X) is null", akItem.formId);
    //    return;
    //}
    //if (akItem.apparelType == 9) {
    //    slot = (1 << (30 - 30)) * akItem.isSkirtOn + (1 << (31 - 30)) * akItem.isPantyOn +
    //           (1 << (41 - 30)) * akItem.isBraOn + (1 << (42 - 30)) * akItem.isGreavesOn;
    //    srcAA->biped.SetSlotMask(slot);
    //} else {
    //    uint32_t defaultSlot = (1 << (49 - 30)) + (1 << (48 - 30)) + (1 << (56 - 30)) + (1 << (53 - 30));
    //    srcAA->biped.RemoveSlotFromMask(defaultSlot);
    //    slot = srcAA->biped.GetSlotMask();

    //    slot = (1 << (49 - 30)) * akItem.isSkirtOn + (1 << (48 - 30)) * akItem.isPantyOn +
    //           (1 << (56 - 30)) * akItem.isBraOn + (1 << (53 - 30)) * akItem.isGreavesOn;
    //    srcAA->biped.AddSlotToMask(slot);
    //    slot = srcAA->biped.GetSlotMask();
    //    if (!akItem.isGreavesOn) srcAA->biped.RemoveSlotFromMask((1 << (38 - 30)));
    //}
    //slot = srcAA->biped.GetSlotMask();
    //	_printSlotState(slot);
}

void LSARConfig::debugPrint() {
    std::vector<LSARItemARMA>::iterator it;
    for (it = armas.begin(); it != armas.end(); it++) {
        _printItemTest2(*it);
        //		_printItemTest3(*it);
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
        logs::info("\t\tpath = {}", LSARFunc::GetArmorAddonModelName(srcAA));
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
        // logs::info("debug: priority=%d", srcAA->data.priority[1]);
        logs::info("debug: priority={}", srcAA->data.priorities[RE::SEXES::kFemale]);
        logs::info("debug: pathM={}", LSARFunc::GetArmorAddonModelName(srcAA, false));
        logs::info("debug: pathF={}", LSARFunc::GetArmorAddonModelName(srcAA));
        // logs::info("debug: pathM=%s", srcAA->models[0][0].GetModelName());
        //logs::info("debug: pathF=%s", srcAA->models[0][1].GetModelName());
    }
}

void LSARConfig::_printSlotState(uint32_t slot) {
    for (int i = 32; i <= 62; i++) {
        bool active = (slot & (1 << (i - 30))) > 0;
        //logs::info("\t%02d\t%X\t%08X", i, active, slot);
        logs::info("\t{:02d}\t{:X}\t{:08X}", i, active, slot);
    }
}

