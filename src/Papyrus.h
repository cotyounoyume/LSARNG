#pragma once
#include "LSARConfig.h"

namespace Papyrus {
    static inline const char CLASS_NAME[] = "LSARNG";

    static inline bool DebugOn = true;

    class LSARNG {
    public:
        // static inline RE::TESObjectREFR* GetSelectedReference(RE::RE::StaticFunctionTag*);
        static inline void test(RE::StaticFunctionTag*) { logs::info("Action from papyrus"); }

    	static inline bool HasSlotX(RE::StaticFunctionTag*, RE::TESObjectARMO* thisArmor, uint32_t slot) {
            if (!thisArmor) return NULL;
            auto aa = LSARFunc::GetLSARAA(thisArmor);
            if (!aa) return NULL;
            return LSARFunc::GetSlotMask(aa, slot);
            //slot -= 30;

            //UInt32 thisSlot = aa->biped.GetSlotMask();
            //UInt32 mask = 1 << slot;
            //if (thisSlot & mask) {
            //    logs::info("ture : ARMA(%08X)'s slot=%08X(%d), targetSlot=0x%08X(%d = %d), mask=%d", aa->formID, thisSlot,
            //             thisSlot, slot, slot, slot + 30, mask);
            //    return true;
            //} else {
            //    logs::info("false: ARMA(%08X)'s slot=%08X(%d), targetSlot=0x%08X(%d = %d), mask=%d", aa->formID, thisSlot,
            //             thisSlot, slot, slot, slot + 30, mask);
            //    return false;
            //}
        }

        static inline bool IsLSARArmor(RE::StaticFunctionTag*, RE::TESObjectARMO* thisArmor) {
            if (!thisArmor) return NULL;
            return LSARFunc::GetLSARAA(thisArmor) != NULL;
            //TESObjectARMA* aa = getLSARAA(thisArmor);
            //if (!aa)
            //    return false;
            //else
            //    return true;
        }

        static inline void ResetArmorAddon(RE::StaticFunctionTag*, uint32_t isLSARClothOn, uint32_t isLSARLArmorOn,
                             uint32_t isLSARHArmorOn, uint32_t isLoose, uint32_t slot49, uint32_t slot48,
                             uint32_t slot56, uint32_t slot53) {
            logs::info(
                "isLSARClothOn=%X, isLSARLArmorOn=%X, isLSARHArmorOn=%X, isLoose=%X, slot49=%X, slot48=%X, slot56=%X, "
                "slot53=%X",
                isLSARClothOn, isLSARLArmorOn, isLSARHArmorOn, isLoose, slot49, slot48, slot56, slot53);

            logs::info("in  ResetArmorAddon()");
            LSARConfig::resetArmorAddon(isLSARClothOn, isLSARLArmorOn, isLSARHArmorOn, isLoose, slot49, slot48, slot56, slot53);
            logs::info("out ResetArmorAddon()");

            logs::info("in  ResetArmor()");
            LSARConfig::resetArmor();
            logs::info("out ResetArmor()");
        }

        static inline void InitArmorAddon(RE::StaticFunctionTag*) {
            logs::info("in  initArmorAddon()");

            LSARConfig::initArmorAddon();

            logs::info("out initArmorAddon()");
        }

        static inline void SwapSlotX(RE::StaticFunctionTag*, RE::TESObjectARMO* thisArmor, uint32_t slot) {
            if (!thisArmor) return;
            auto aa = LSARFunc::GetLSARAA(thisArmor);
            //TESObjectARMA* aa = getLSARAA(thisArmor);
            if (!aa) {
                return;
            }
            //UInt32 thisSlot = aa->biped.GetSlotMask();
            //slot -= 30;
            //UInt32 mask = 1 << slot;
            //logs::info("Before  : ARMA(%08X)'s slot=%08X(%d), targetSlot=0x%08X(%d = %d), mask=%d", aa->formID, thisSlot,
            //         thisSlot, slot, slot, slot + 30, mask);
            //		logs::info("Before: ARMA(%08X)'s slot=%08X, targetSlot=%08X", aa->formID, thisSlot, slot);
            if (LSARFunc::GetSlotMask(aa, slot)) {
                LSARFunc::RemoveSlotFromMask(aa, slot);
                return;
            } else {
                LSARFunc::AddSlotToMask(aa, slot);
                return;
            }


            //if (thisSlot & mask) {
            //    aa->biped.RemoveSlotFromMask(mask);
            //    thisSlot = aa->biped.GetSlotMask();
            //    logs::info("AfterOFF: ARMA(%08X)'s slot=%08X(%d), targetSlot=0x%08X(%d = %d), mask=%d", aa->formID,
            //             thisSlot, thisSlot, slot, slot, slot + 30, mask);
            //    //			logs::info("AfterT:  ARMA(%08X)'s slot=%08X, targetSlot=%08X", aa->formID, thisSlot, slot);
            //    return;
            //} else {
            //    aa->biped.AddSlotToMask(mask);
            //    thisSlot = aa->biped.GetSlotMask();
            //    logs::info("AfterON : ARMA(%08X)'s slot=%08X(%d), targetSlot=0x%08X(%d = %d), mask=%d", aa->formID,
            //             thisSlot, thisSlot, slot, slot, slot + 30, mask);
            //    //			logs::info("AfterF:  ARMA(%08X)'s slot=%08X, targetSlot=%08X", aa->formID, thisSlot, slot);
            //    return;
            //}
        }

        static inline bool Register(RE::BSScript::IVirtualMachine* a_vm) {
            a_vm->RegisterFunction("test", CLASS_NAME, test);
            a_vm->RegisterFunction("HasSlotX", CLASS_NAME, HasSlotX);
            a_vm->RegisterFunction("IsLSARArmor", CLASS_NAME, IsLSARArmor);
            a_vm->RegisterFunction("ResetArmorAddon", CLASS_NAME, ResetArmorAddon);
            a_vm->RegisterFunction("InitArmorAddon", CLASS_NAME, InitArmorAddon);
            a_vm->RegisterFunction("SwapSlotX", CLASS_NAME, SwapSlotX);
            logs::info("Registered funcs for class {}", CLASS_NAME);
            return true;
        }
    };

    inline bool Register(RE::BSScript::IVirtualMachine* a_vm) {
        LSARNG::Register(a_vm);
        return true;
    }
}