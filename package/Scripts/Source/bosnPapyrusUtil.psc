Scriptname bosnPapyrusUtil Hidden

String Function readBook(Form a_form) global native 
Spell Function getSpellByName(String a_spell) global native 
String Function getActorSpellList(Actor a_actor) global native 
bool Function clonePackageSpell(Actor a_actor, String a_spell, Package a_package) global native 
String Function getActorInventoryAsString(ObjectReference a_actor, bool a_showEquip = false) global native 
String Function inventoryDecorator(Actor a_actor) global native 
ObjectReference Function getLoadedREFbyName(ObjectReference a_origin, float a_distance, String a_npcName) global native 
String Function getLoadedREFbyTypeAsString(ObjectReference a_origin, float a_distance, int a_formType, bool a_allowSteal) global native 
ObjectReference[] Function getLoadedREFListbyName(ObjectReference a_origin, float a_distance, String[] a_stringList, bool a_allowSteal) global native 
String Function getNearbyLoadedRefsAsString(ObjectReference a_origin, float a_distance, int a_numRefs, bool simple) global native 
