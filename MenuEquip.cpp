#include "MenuEquip.h"

#include "skse/GameData.h"
#include "skse/GameExtraData.h"
#include "skse/GameMenus.h"

MenuEquipManager* MenuEquipManager::GetSingleton() {
	static MenuEquipManager instance;
	return &instance;
}

void MenuEquipManager::queueEquipItem(MenuEquipData* data, UInt32 ticks) {
	data->equipAt = internalCounter + ticks  * 2;
	equipQueue.push(data);

	//_MESSAGE("EquipItem queued");
}
void MenuEquipManager::queueUnequipItem(MenuUnequipData* data, UInt32 ticks) {
	data->unequipAt = internalCounter + ticks * 2;
	unequipQueue.push(data);

	//_MESSAGE("UnequipItem queued");
}

bool MenuEquipManager::processEquipQueue(EquipItemMethod originalEquipItem) {
	//_MESSAGE("originalEquipItem: 0x%.8x", originalEquipItem);

	++internalCounter;

	if (equipQueue.empty()) {
		//_MESSAGE("empty");

		if (unequipQueue.empty()) {
			internalCounter = 0u;
		}
		return false;
	}
	else {
		//_MESSAGE("not empty (%i)", equipQueue.size());

		MenuEquipData* data = equipQueue.top();
		//_MESSAGE("data: 0x%.8x", data);
		if (data->equipAt <= internalCounter) {
			((data->equipManager)->*originalEquipItem)(data->actor, data->item, data->extraData, data->count, data->equipSlot, data->withEquipSound, data->preventEquip, data->showMsg, data->unk);
			
			delete data;
			equipQueue.pop();

			CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&UIStringHolder::GetSingleton()->inventoryMenu, 0x6, nullptr);
		}

		return true;
	}
}
bool MenuEquipManager::processUnequipQueue(UnequipItemMethod originalUnequipItem) {
	//_MESSAGE("originalUnequipItem: 0x%.8x", originalUnequipItem);

	++internalCounter;

	if (unequipQueue.empty()) {
		//_MESSAGE("empty");

		if (equipQueue.empty()) {
			internalCounter = 0u;
		}
		return false;
	}
	else {
		//_MESSAGE("not empty (%i)", unequipQueue.size());

		MenuUnequipData* data = unequipQueue.top();
		//_MESSAGE("data: 0x%.8x", data);
		if (data->unequipAt <= internalCounter) {
			((data->equipManager)->*originalUnequipItem)(data->actor, data->item, data->extraData, data->count, data->equipSlot, data->unkFlag1, data->preventEquip, data->unkFlag2, data->unkFlag3, data->unk);
			
			delete data;
			unequipQueue.pop();

			CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&UIStringHolder::GetSingleton()->inventoryMenu, 0x6, nullptr);
		}

		return true;
	}
}