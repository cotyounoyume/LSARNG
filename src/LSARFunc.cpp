#pragma once
#include "LSARFunc.h"

namespace LSARFunc {
    RE::TESObjectARMA* GetLSARAA(RE::TESObjectARMO* thisArmor) {
        if (!thisArmor) return NULL;
        for (uint32_t i = 0; i < thisArmor->armorAddons.size(); i++) {
            if (IsLSARAA(thisArmor->armorAddons[i])) return thisArmor->armorAddons[i];
        }
        return NULL;
    }

    bool IsLSARAA(RE::TESObjectARMA* aa) {
        if (!aa) return NULL;
        return ArmorAddonStartsWith(aa, "zzLSAR");
    }

    bool IsLooseAA(RE::TESObjectARMA* aa) {
        if (!aa) return NULL;
        return ArmorAddonStartsWith(aa, "zzLSARL");
    }

    bool ArmorAddonStartsWith(RE::TESObjectARMA* aa, std::string word) {
        if (!aa) return false;
        return GetArmorAddonModelName(aa).find(word) != std::string::npos;
    }

    std::string GetArmorAddonModelName(RE::TESObjectARMA* aa, bool isFemale) 
    {
        if (!aa) return "";
        auto type = isFemale ? RE::SEXES::kFemale : RE::SEXES::kMale;
        return aa->bipedModels[type].model.c_str();
    }

    RE::TESObjectARMA* GetArmorAddonFromFormID(uint32_t formID) {
        return RE::TESForm::LookupByID<RE::TESObjectARMA>(formID);
    }

    RE::TESObjectARMO* GetArmorFromFormID(uint32_t formID) {
        return RE::TESForm::LookupByID<RE::TESObjectARMO>(formID);
    }

    RE::BGSBipedObjectForm::BipedObjectSlot Int2BOSlot(uint32_t slot) 
    {
        return static_cast<RE::BGSBipedObjectForm::BipedObjectSlot>(slot - 30);
    }

    uint32_t BOSlot2Int(RE::BGSBipedObjectForm::BipedObjectSlot BOSlot)
    {
        return ((int)BOSlot + 30);
    }

    bool GetSlotMask(RE::TESObjectARMA* aa, uint32_t slot) 
    {
        return aa->bipedModelData.bipedObjectSlots.any(Int2BOSlot(slot));
    }

    void AddSlotToMask(RE::TESObjectARMA* aa, uint32_t slot) 
    { 
        aa->AddSlotToMask(Int2BOSlot(slot));
    }

    void RemoveSlotFromMask(RE::TESObjectARMA* aa, uint32_t slot) 
    {
        aa->RemoveSlotFromMask(Int2BOSlot(slot));
    }
}