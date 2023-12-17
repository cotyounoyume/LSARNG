#pragma once
#include "PCH.h"
//#include "skse64/GameObjects.h"

namespace LSARFunc {
    RE::TESObjectARMA* GetLSARAA(RE::TESObjectARMO* thisArmor);
    bool IsLSARAA(RE::TESObjectARMA* aa);
    bool IsLooseAA(RE::TESObjectARMA* aa);
    bool ArmorAddonStartsWith(RE::TESObjectARMA* aa, std::string word);
    std::string GetArmorAddonModelName(RE::TESObjectARMA* aa, bool isFemal = true);
    RE::TESObjectARMA* GetArmorAddonFromFormID(uint32_t formID);
    RE::TESObjectARMO* GetArmorFromFormID(uint32_t formID);
    RE::BGSBipedObjectForm::BipedObjectSlot Int2BOSlot(uint32_t slot);
    uint32_t BOSlot2Int(RE::BGSBipedObjectForm::BipedObjectSlot BOSlot);
    bool GetSlotMask(RE::TESObjectARMA* aa, uint32_t slot);
    void AddSlotToMask(RE::TESObjectARMA* aa, uint32_t slot);
    void RemoveSlotFromMask(RE::TESObjectARMA* aa, uint32_t slot);
}