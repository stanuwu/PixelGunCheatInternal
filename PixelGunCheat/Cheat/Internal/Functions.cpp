#include "Functions.h"

#include <cstdint>
#include <stdbool.h>
#include <stdbool.h>
#include <stdbool.h>

#include "../Hooks/Hooks.h"
#include "../Offsets/Offsets.h"

static uintptr_t GameBase_;
static uintptr_t GameAssembly_;
static uintptr_t UnityPlayer_;

void Functions::init(uintptr_t game_base, uintptr_t game_assembly, uintptr_t unity_player)
{
    GameBase_ = game_base;
    GameAssembly_ = game_assembly;
    UnityPlayer_ = unity_player;
}

void Functions::SetNextHitCritical(void* arg, bool arg1)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, bool)) (GameAssembly_ + Offsets::SetNextCriticalHit);
    return fn(arg, arg1);
}

void Functions::MakeInvisibleForSeconds(void* arg, float duration)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, float)) (GameAssembly_ + Offsets::MakeInvisibleForSeconds);
    return fn(arg, duration);
}

void* Functions::TextMeshGetText(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*)) (GameAssembly_ + Offsets::TextMeshGetText);
    return fn(arg);
}

void Functions::TextMeshGetColor(void* arg, void* color_ptr)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + Offsets::TextMeshGetColor);
    return fn(arg, color_ptr);
}

void Functions::CameraWorldToScreen(void* arg, void* world, void* screen)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*, int, void*)) (GameAssembly_ + Offsets::WorldToScreenPoint);
    return fn(arg, world, 2, screen);
}

void* Functions::ComponentGetTransform(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*)) (GameAssembly_ + Offsets::ComponentGetTransform);
    return fn(arg);
}

void Functions::TransformGetRotation(void* arg, void* quaternion)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + Offsets::TransformGetRotation);
    return fn(arg, quaternion);
}

void Functions::TransformGetPosition(void* arg, void* position)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*)) (GameAssembly_ + Offsets::TransformGetPosition);
    return fn(arg, position);
}

void Functions::TransformLookAt(void* arg, void* position, void* up)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, void*, void*)) (GameAssembly_ + Offsets::TransformLookAt);
    return fn(arg, position, up);
}

bool Functions::PhysicsRaycast(void* ray, void* hit_info, float max_distance)
{
    static const auto fn = (bool(*)(void*, void*, float)) (GameAssembly_ + Offsets::PhysicsRayCast);
    return fn(ray, hit_info, max_distance);
}

int Functions::ObjectGetInstanceID(void* arg)
{
    if (!arg) return -1;
    static const auto fn = (int(*)(void*))(GameAssembly_ + Offsets::ObjectGetInstanceID);
    return fn(arg);
}

bool Functions::BehaviourGetEnabled(void* arg)
{
    if (!arg) return false;
    static const auto fn = (bool(*)(void*))(GameAssembly_ + Offsets::BehaviourGetEnabled);
    return fn(arg);
}

void Functions::AddWeapon(void* arg, void* string, int source, bool bool1 = true, bool bool2 = false, void* class1 = nullptr, void* struct1 = nullptr)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*))(GameAssembly_ + Offsets::AddWeapon);
    fn(arg);
}

void* Functions::GetItemRecordDict()
{
    static const auto fn = (void*(*)())(GameAssembly_ + Offsets::GetItemRecordDict);
    return fn(); 
}

void* Functions::GetDataListStaticInstance()
{
    static const auto fn = (void*(*)())(GameAssembly_ + Offsets::GetIDListParentInstance);
    return fn();
}

void* Functions::GetDataList(void* arg, int type)
{
    static const auto fn = (void*(*)(void*, int))(GameAssembly_ + Offsets::GetIDList);
    return fn(arg, type); 
}

void* Functions::GetWeaponSkinSettings(void* id)
{
    static const auto fn = (void*(*)(void*))(GameAssembly_ + 0x8ff5b0);
    return fn(id);
}

/*
void Functions::PurchaseSkin(void* thing)
{
    static const auto fn = (void(*)(void*))(GameAssembly_ + 0x8ff420);
    return fn(thing);
}
*/

void* Functions::ItemRecordGetShopId(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*))(GameAssembly_ + Offsets::ItemRecordGetShopId);
    return fn(arg); 
}

void Functions::GiveWear(void* id)
{
    static const auto fn = (void(*)(void*))(GameAssembly_ + Offsets::GiveWear);
    return fn(id); 
}

void Functions::GiveWeapon(void* id, bool event, bool auto_upgrade)
{
    static const auto fn = (void(*)(void*))(GameAssembly_ + Offsets::GiveWeapon);
    return fn(id); 
}

void Functions::GivePets(void* id, int count)
{
    static const auto fn = (void(*)(void*, int))(GameAssembly_ + Offsets::GivePets);
    return fn(id, count); 
}

void Functions::AddGems(int amount, bool arg1, bool arg2, int enum1, int enum2, int enum3)
{
    static const auto fn = (void(*)(int, bool, bool, int, int, int))(GameAssembly_ + Offsets::AddGems);
    return fn(amount, arg1, arg2, enum1, enum2, enum3); 
}

void Functions::AddCoins(int amount, bool arg1, bool arg2, int enum1, int enum2, int enum3)
{
    static const auto fn = (void(*)(int, bool, bool, int, int, int))(GameAssembly_ + Offsets::AddCoins);
    return fn(amount, arg1, arg2, enum1, enum2, enum3); 
}

void Functions::AddCoupons(int amount, int source, bool arg1, int arg2)
{
    static const auto fn = (void(*)(int, int, bool, int))(GameAssembly_ + Offsets::AddCoupons);
    return fn(amount, source, arg1, arg2); 
}

void Functions::AddClanLootboxPoints(int amount, int source, bool arg1, bool arg2, int arg3)
{
    static const auto fn = (void(*)(int, int, bool, bool, int))(GameAssembly_ + Offsets::AddClanChestPoints);
    return fn(amount, source, arg1, arg2, arg3); 
}

void Functions::AddSomeCurrency(void* currency, int amount, bool arg1, int enum1, int enum2, int enum3, int enum4)
{
    static const auto fn = (void(*)(void*, int, bool, int, int, int, int))(GameAssembly_ + Offsets::AddSomeCurrency);
    return fn(currency, amount, arg1, enum1, enum2, enum3, enum4); 
}

void Functions::ActivateGadget(void* arg, int gadget_id, int level)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, int, void*, int))(GameAssembly_ + Offsets::GadgetActivate);
    return fn(arg, gadget_id, Hooks::create_system_string("TrustMeBroThisIsARealCallToThisMethod"), level);
}

void Functions::DeactivateGadget(void* arg, int gadget_id)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, int))(GameAssembly_ + Offsets::GadgetDeactivate);
    return fn(arg, gadget_id);
}

void* Functions::SystemObjectToString(void* arg)
{
    if (!arg) return nullptr;
    static const auto fn = (void*(*)(void*))(GameAssembly_ + 0x3970900);
    return fn(arg);
}

void* Functions::ProgressUpdaterGetInstance()
{
    static const auto fn = (void*(*)())(GameAssembly_ + Offsets::ProgressUpdaterGetInstance);
    return fn(); 
}

void Functions::DoSomething(void* arg, int eff_id, float test)
{
    if (!arg) return;
    static const auto fn = (void(*)(void*, int, float))(GameAssembly_ + 0x1b209c0);
    return fn(arg, eff_id, test);
}

void Functions::ProgressAddCurrency(void* instance, void* currency, int amount, int enum1, bool bool1, bool bool2, itemObtainParams* obtainParams)
{
    static const auto fn = (void(*)(void*, void*, int, int, bool, bool, void*))(GameAssembly_ + Offsets::ProgressAddCurrency);
    return fn(instance, currency, amount, enum1, bool1, bool2, obtainParams); 
}

ImVec4 Functions::ImVec4i(const int r, const int g, const int b, const int a)
{
    return { (float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f };
}