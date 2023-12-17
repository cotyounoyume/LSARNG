ScriptName zzLSARUtil

;-- Functions ---------------------------------------

; Skipped compiler generated GetState

; Skipped compiler generated GotoState

Function ResetArmorAddon(Int changeLSARCloth, Int changeLSARLArmor, Int changeLSARHArmor, Int isLoose, Int slot49, Int slot48, Int slot56, Int slot53) Global
	LSARNG.ResetArmorAddon(changeLSARCloth, changeLSARLArmor, changeLSARHArmor, isLoose, slot49, slot48, slot56, slot53)
EndFunction

Bool Function HasSlotX(Armor thisArmor, Int slot) Global
	return LSARNG.HasSlotX(thisArmor, slot)
EndFunction

Function InitArmorAddon() Global
	LSARNG.InitArmorAddon()
EndFunction

Bool Function IsLSARArmor(Armor thisArmor) Global
	return LSARNG.IsLSARArmor(thisArmor)
EndFunction

Function SwapSlotX(Armor thisArmor, Int slot) Global
	LSARNG.SwapSlotX(thisArmor, slot)
EndFunction

Function test() Global
	LSARNG.test()
EndFunction
