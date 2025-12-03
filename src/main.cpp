
struct myPapyrusUtil
{
    static auto getBookText(RE::StaticFunctionTag* a_tag, RE::TESForm* a_form)
    {
        std::string str("null");
        if (a_form->IsBook())
        {
            auto a_book = a_form->As<RE::TESObjectBOOK>();
            RE::BSString a_out;
            a_book->GetDescription(a_out, a_book);
            str = a_out.c_str();
        }
        return str;
    }

    static RE::SpellItem* getSpellByName(RE::StaticFunctionTag* a_tag, std::string a_spell)
    {
        auto& spellList = RE::TESDataHandler::GetSingleton()->GetFormArray(RE::FormType::Spell);
        for (auto& spell : spellList)
        {
            if (spell->GetName() == a_spell)
                return spell->As<RE::SpellItem>();
        }
        return nullptr;
    }

    static bool checkSpellType(RE::SpellItem* a_spell)
    {
        if (a_spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell)
            return true;
        return false;
    }

    static void setSchoolStrings(RE::SpellItem* a_spell, std::string* a_spellString)
    {
        if (!checkSpellType(a_spell))
            return;

        switch (a_spell->GetAssociatedSkill())
        {
            case RE::ActorValue::kAlteration:
                a_spellString[0] += a_spell->GetName();
                a_spellString[0] += ", ";
                break;
            case RE::ActorValue::kConjuration:
                a_spellString[1] += a_spell->GetName();
                a_spellString[1] += ", ";
                break;
            case RE::ActorValue::kDestruction:
                a_spellString[2] += a_spell->GetName();
                a_spellString[2] += ", ";
                break;
            case RE::ActorValue::kIllusion:
                a_spellString[3] += a_spell->GetName();
                a_spellString[3] += ", ";
                break;
            case RE::ActorValue::kRestoration:
                a_spellString[4] += a_spell->GetName();
                a_spellString[4] += ", ";
                break;
        }
    }

    static std::string getActorSpellListAsString(RE::StaticFunctionTag* a_tag, RE::Actor* a_actor)
    {
        std::string knownSpells[5] = { "" };

        auto& addedSpellList = a_actor->GetActorRuntimeData().addedSpells;
        for (auto& spell : addedSpellList)
        {
            setSchoolStrings(spell, knownSpells);
        }

        auto baseSpellList = a_actor->GetActorBase()->GetSpellList();
        for (auto& spell : std::span(baseSpellList->spells, baseSpellList->numSpells))
        {
            setSchoolStrings(spell, knownSpells);
        }

        std::string outputString = "";
        if (knownSpells[0] != "")
            outputString += "- Alteration: " + knownSpells[0] + "\n";
        if (knownSpells[1] != "")
            outputString += "- Conjuration: " + knownSpells[1] + "\n";
        if (knownSpells[2] != "")
            outputString += "- Destruction: " + knownSpells[2] + "\n";
        if (knownSpells[3] != "")
            outputString += "- Illusion: " + knownSpells[3] + "\n";
        if (knownSpells[4] != "")
            outputString += "- Restoration: " + knownSpells[4] + "\n";

        return outputString;
    }

    static std::string getActorInventoryAsString(RE::StaticFunctionTag* a_tag, RE::TESObjectREFR* a_actor, bool a_showEquip = false)
    {
        //auto dataHandler = RE::TESDataHandler::GetSingleton();
        //auto a_pckage = dataHandler->LookupForm<RE::TESPackage>(0x98b9b, "Skyrim.esm");
        //auto player = RE::PlayerCharacter::GetSingleton();
        //clonePackageSpell(a_tag, player, "Sparks", a_pckage);

        //auto s = getLoadedREFListbyName(a_tag, a_actor, 800, {"thistle", "Fly amanita", "Fly amanita" }, false);


        std::string inventoryString, weaponString, armorString, potionsString, scrollsString, foodString, ingredientsString, booksString, keysString, miscString = "";

        auto inventory = a_actor->GetInventory();
        for (auto& [item, data] : inventory)
        {
            const auto& [count, entry] = data;
            if (count > 0 && item->GetName() != "")
            {
                switch(item->GetFormType())
                {
                    case RE::FormType::Weapon:
                        if (!entry->IsWorn() || a_showEquip)
                        {
                            weaponString += item->GetName();
                            if (count > 1)
                                weaponString += "*" + std::to_string(count);
                            weaponString += ", ";
                        }
                        break;

                    case RE::FormType::Armor:
                        if (!entry->IsWorn() || a_showEquip)// || (item->As<RE::TESObjectARMO>() != a_body && item->As<RE::TESObjectARMO>() != a_hands && item->As<RE::TESObjectARMO>() != a_feet && item->As<RE::TESObjectARMO>() != a_hair))
                        {
                            if (item->GetName() != "")
                            {
                                armorString += item->GetName();
                                if (count > 1)
                                    armorString += "*" + std::to_string(count);
                                armorString += ", ";
                            }
                        }
                        break;

                    case RE::FormType::AlchemyItem:
                        if (item->As<RE::AlchemyItem>()->data.flags == RE::AlchemyItem::AlchemyFlag::kNone || item->As<RE::AlchemyItem>()->IsPoison() || item->As<RE::AlchemyItem>()->IsMedicine())
                        {
                            potionsString += item->GetName();
                            if (count > 1)
                                potionsString += "*" + std::to_string(count);
                            potionsString += ", ";
                        }

                        if (item->As<RE::AlchemyItem>()->IsFood())
                        {
                            foodString += item->GetName();
                            if (count > 1)
                                foodString += "*" + std::to_string(count);
                            foodString += ", ";
                        }
                        break;

                    case RE::FormType::Scroll:
                        scrollsString += item->GetName();
                        if (count > 1)
                            scrollsString += "*" + std::to_string(count);
                        scrollsString += ", ";
                        break;

                    case RE::FormType::Ingredient:
                        ingredientsString += item->GetName();
                        if (count > 1)
                            ingredientsString += "*" + std::to_string(count);
                        ingredientsString += ", ";
                        break;

                    case RE::FormType::Book:
                        booksString += item->GetName();
                        if (count > 1)
                            booksString += "*" + std::to_string(count);
                        booksString += ", ";
                        break;

                    case RE::FormType::KeyMaster:
                        keysString += item->GetName();
                        if (count > 1)
                            keysString += "*" + std::to_string(count);
                        keysString += ", ";
                        break;

                    default:
                        miscString += item->GetName();
                        if (count > 1)
                            miscString += "*" + std::to_string(count);
                        miscString += ", ";
                        break;
                }
            }
        }

        if (weaponString != "")
            inventoryString += "- Weapons: " + weaponString;
        if (armorString != "")
            inventoryString += "\n- Armor: " + armorString;
        if (potionsString != "")
            inventoryString += "\n- Potions: " + potionsString;
        if (scrollsString != "")
            inventoryString += "\n- Scrolls: " + scrollsString;
        if (foodString != "")
            inventoryString += "\n- Food: " + foodString;
        if (ingredientsString != "")
            inventoryString += "\n- Ingredients: " + ingredientsString;
        if (booksString != "")
            inventoryString += "\n- Books: " + booksString;
        if (keysString != "")
            inventoryString += "\n- Keys: " + keysString;
        if (miscString != "")
            inventoryString += "\n- Misc: " + miscString;

        return inventoryString;
    }

    static std::string inventoryDecorator(RE::StaticFunctionTag* a_tag, RE::Actor* a_actor)
    {
        return getActorInventoryAsString(a_tag, a_actor->As<RE::TESObjectREFR>());
    }

    static RE::SpellItem* getActorHasSpell(RE::Actor* a_actor, std::string a_spell)
    {
        auto& addedSpellList = a_actor->GetActorRuntimeData().addedSpells;
        for (auto& spell : addedSpellList)
        {
            if (spell->GetName() == a_spell)
                return spell;
        }

        auto baseSpellList = a_actor->GetActorBase()->GetSpellList();
        for (auto& spell : std::span(baseSpellList->spells, baseSpellList->numSpells))
        {
            if (spell->GetName() == a_spell)
                return spell;
        }

        return nullptr;
    }

    static void changePackageDataAddress(uint64_t* pData, uint64_t* a_source)
    {
        uint64_t* dst = (uint64_t*)(pData + 0x2);     //0x10 target-selector ptr 
        uint64_t* dstB = (uint64_t*)(*dst);
        uint64_t* dstC = (uint64_t*)(dstB + 0x1);     //0x08 spell ptr 
        *dstC = (uint64_t)a_source;
    }

    static bool clonePackageSpell(RE::StaticFunctionTag* a_tag, RE::Actor* a_actor, std::string a_spell, RE::TESPackage* a_package)
    {
        auto copySpell = getActorHasSpell(a_actor, a_spell);
        if (a_actor && copySpell && a_package)
        {
            //manuall replace packagedata ptr in usemagic package
            auto pData = (RE::TESCustomPackageData*)a_package->data;
            changePackageDataAddress((uint64_t*)pData->data.data[1], (uint64_t*)copySpell); //spell target-selector
            /*  breaks concentration spells for now
            auto chargeTime = copySpell->GetChargeTime();
            if (copySpell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration)
                chargeTime = 1.0f;          
            uint64_t* dst = (uint64_t*)(pData->data.data[4] + 0x2);     //casttimemin + 0x10 
            *dst = (uint64_t)chargeTime;

            if (copySpell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration)
                chargeTime = 2.0f;
            dst = (uint64_t*)(pData->data.data[5] + 0x2);               //casttimemax + 0x10 
            *dst = (uint64_t)chargeTime;
            */



            return true;
        }
        return false;
    }

    static std::string to_lower(std::string s) {
        for (char& c : s)
            c = tolower(c);
        return s;
    }

    static RE::TESObjectREFR* getLoadedREFbyName(RE::StaticFunctionTag* a_tag, RE::TESObjectREFR* a_origin, float a_distance, std::string a_npcName)
    {
        auto tes = RE::TES::GetSingleton();
        RE::TESObjectREFR* a_result = nullptr;
        a_npcName = to_lower(a_npcName);

        tes->ForEachReferenceInRange(a_origin, a_distance, [&a_result, a_npcName](RE::TESObjectREFR* a_ref) -> RE::BSContainer::ForEachResult {
            if (!a_ref) return RE::BSContainer::ForEachResult::kContinue;

            RE::TESForm* base = a_ref->GetBaseObject();
            if (!base) return RE::BSContainer::ForEachResult::kContinue;

            //if (base->Is(RE::FormType::NPC)) 
                if (a_ref->Is3DLoaded() && to_lower(a_ref->GetDisplayFullName()) == a_npcName)
                {
                    logs::info("(Ref found: {} FormID: {}", a_ref->GetDisplayFullName(), std::format("0x{:x}", a_ref->GetFormID()));
                    a_result = a_ref;
                    return RE::BSContainer::ForEachResult::kStop;
                }

            //}
            return RE::BSContainer::ForEachResult::kContinue;
            });
        if (!a_result)
            logs::info("(Ref not found: {}", a_npcName);

        return a_result;
    }

    static RE::BSTArray<RE::TESObjectREFR*> getLoadedREFListbyName(RE::StaticFunctionTag* a_tag, RE::TESObjectREFR* a_origin, float a_distance, std::vector<std::string> a_stringList, bool a_allowSteal)
    {
        auto tes = RE::TES::GetSingleton();
        RE::BSTArray<RE::TESObjectREFR*> a_result;
        int counter = 0;

        tes->ForEachReferenceInRange(a_origin, a_distance, [&a_result, &a_stringList, a_allowSteal, &counter](RE::TESObjectREFR* a_ref) -> RE::BSContainer::ForEachResult {
            if (counter == a_stringList.size()) return RE::BSContainer::ForEachResult::kStop;
            if (!a_ref) return RE::BSContainer::ForEachResult::kContinue;

            RE::TESForm* base = a_ref->GetBaseObject();
            if (!base) return RE::BSContainer::ForEachResult::kContinue;

            for (auto& iString : a_stringList)
            {
                if (iString != "")
                {
                    auto s = to_lower(iString);
                    std::string displayName = to_lower(a_ref->GetDisplayFullName());
                    if (a_ref->Is3DLoaded() && displayName != "" && displayName == s)
                    {
                        //allow steal check
                        if (!a_allowSteal && a_ref->GetOwner() && a_ref->GetOwner() != RE::PlayerCharacter::GetSingleton()->As<RE::TESForm>())
                            return RE::BSContainer::ForEachResult::kContinue;

                        //

                        logs::info("(Ref found: {} FormID: {}", a_ref->GetDisplayFullName(), std::format("0x{:x}", a_ref->GetFormID()));
                        a_result.push_back(a_ref);
                        iString = "";
                        counter++;
                        return RE::BSContainer::ForEachResult::kContinue;
                    }
                }

             }
                return RE::BSContainer::ForEachResult::kContinue;
            });

        return a_result;
    }

    static std::string getLoadedREFbyTypeAsString(RE::StaticFunctionTag* a_tag, RE::TESObjectREFR* a_origin, float a_distance, int a_formType, bool a_allowSteal = true)
    {
        auto tes = RE::TES::GetSingleton();
        std::string a_result = "";
        RE::FormType formType;

        switch (a_formType)
        {
            case 26:
                formType = RE::FormType::Armor;
                break;
            case 42:
                formType = RE::FormType::Ammo;
                break;
            case 38:
                formType = RE::FormType::Tree;
                break;
            case 39:
                formType = RE::FormType::Flora;
                break;
            case 30:
                formType = RE::FormType::Ingredient;
                break;
            case 32:
                formType = RE::FormType::Misc;
                break;
            case 48:
                formType = RE::FormType::Note;
                break;
            case 46:
                formType = RE::FormType::AlchemyItem;
                break;
            case 23:
                formType = RE::FormType::Scroll;
                break;
            case 52:
                formType = RE::FormType::SoulGem;
                break;
            case 41:
                formType = RE::FormType::Weapon;
                break;
            case 27:
                formType = RE::FormType::Book;
                break;
        };


        tes->ForEachReferenceInRange(a_origin, a_distance, [&a_result, &formType, a_allowSteal](RE::TESObjectREFR* a_ref) -> RE::BSContainer::ForEachResult {
            if (!a_ref) return RE::BSContainer::ForEachResult::kContinue;

            RE::TESForm* base = a_ref->GetBaseObject();
            if (!base) return RE::BSContainer::ForEachResult::kContinue;

            std::string displayName = a_ref->GetDisplayFullName();
            if (a_ref->Is3DLoaded() && base->GetFormType() == formType && displayName != "")
            {
                if (!a_allowSteal && a_ref->GetOwner() && a_ref->GetOwner() != RE::PlayerCharacter::GetSingleton()->As<RE::TESForm>())
                    return RE::BSContainer::ForEachResult::kContinue;

                logs::info("(Ref found: {} FormID: {}", displayName, std::format("0x{:x}", a_ref->GetFormID()));
                a_result += displayName + ",";
            }

            //}
            return RE::BSContainer::ForEachResult::kContinue;
            });

        return a_result;
    }


    static bool RegisterFuncsForSKSE(RE::BSScript::IVirtualMachine* a_vm) {

        if (!a_vm) {
            logs::info("VirtualMachineError!");
            return false;
        }

        a_vm->RegisterFunction("readBook", "bosnPapyrusUtil", getBookText, false);
        a_vm->RegisterFunction("getSpellByName", "bosnPapyrusUtil", getSpellByName, false);
        a_vm->RegisterFunction("getActorSpellList", "bosnPapyrusUtil", getActorSpellListAsString, false);
        a_vm->RegisterFunction("clonePackageSpell", "bosnPapyrusUtil", clonePackageSpell, false);
        a_vm->RegisterFunction("getActorInventoryAsString", "bosnPapyrusUtil", getActorInventoryAsString, false);
        a_vm->RegisterFunction("inventoryDecorator", "bosnPapyrusUtil", inventoryDecorator, false);
        a_vm->RegisterFunction("getLoadedREFbyName", "bosnPapyrusUtil", getLoadedREFbyName, false);
        a_vm->RegisterFunction("getLoadedREFbyTypeAsString", "bosnPapyrusUtil", getLoadedREFbyTypeAsString, false);
        a_vm->RegisterFunction("getLoadedREFListbyName", "bosnPapyrusUtil", getLoadedREFListbyName, false);

        return true;
    }
};

struct hooks
{
    struct TESCamera_Update
    {
        static void thunk(RE::TESCamera* a_camera)
        {
            func(a_camera);
        }
        static inline REL::Relocation<decltype(thunk)> func;
    };

    static void install()
    {
        REL::Relocation<std::uintptr_t> hook1{ RELOCATION_ID(49852, 50784) };  // 84AB90, 876700
        stl::write_thunk_call<TESCamera_Update>(hook1.address() + REL::Relocate(0x1A6, 0x1A6));
    }
};



SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	//logs::info("Hello World!!");
    SKSE::GetPapyrusInterface()->Register(myPapyrusUtil::RegisterFuncsForSKSE);  // register papyrus functions

    //hooks::install();
	return true;
}
