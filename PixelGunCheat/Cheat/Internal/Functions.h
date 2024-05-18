#pragma once

#include <cstdint>

#include "../Hooks/Hooks.h"

struct Ray
{
    float xo;
    float yo;
    float zo;
    float xd;
    float yd;
    float zd;
};

struct RaycastHit
{
    float px;
    float py;
    float pz;
    float nx;
    float ny;
    float nz;
    UINT face_id;
    float distance;
    float u;
    float v;
    int collider;
};

struct itemObtainParams
{
    int field1;
    int field2;
    int field3;
    int field4;
    int field5;
    int field6;
    int field7;
    bool field8;
    int field9;
    int field10;
    bool field11;
    int field12;
    int field13;
    bool field14;
    int field15;
    int field16;
    int field17;
    bool field18;
    int64_t field19;
    int field20;
    bool field21;
    bool field22;
    bool field23;
    bool field24;
    int field25;
};

class Functions
{
public:
    static void init(uintptr_t game_base, uintptr_t game_assembly, uintptr_t unity_player);
    static void SetNextHitCritical(void* arg, bool arg1);
    static void MakeInvisibleForSeconds(void* arg, float duration);
    static void* TextMeshGetText(void* arg);
    static void TextMeshGetColor(void* arg, void* color_ptr);
    static void CameraWorldToScreen(void* arg, void* world, void* screen);
    static void* ComponentGetTransform(void* arg);
    static void TransformGetRotation(void* arg, void* quaternion);
    static void TransformGetPosition(void* arg, void* position);
    static void TransformLookAt(void* arg, void* position, void* up);
    static bool PhysicsRaycast(void* ray, void* hit_info, float max_distance);
    static int ObjectGetInstanceID(void* arg);
    static bool BehaviourGetEnabled(void* arg);
    static void AddWeapon(void* arg, void* string, int source, bool bool1, bool bool2, void* class1, void* struct1);
    static void* GetItemRecordDict();
    static void* GetDataListStaticInstance();
    static void* GetDataList(void* arg, int type);
    static void* GetWeaponSkinSettings(void* id);
    static void* ItemRecordGetShopId(void* arg);
    static void GiveGadget(void* id, int level);
    static void GiveWear(void* id);
    static void GiveWeapon(void* id, bool event, bool auto_upgrade);
    static void GivePets(void* id, int count);
    static void AddGems(int amount, bool arg1, bool arg2, int enum1, int enum2, int enum3);
    static void AddCoins(int amount, bool arg1, bool arg2, int enum1, int enum2, int enum3);
    static void AddCoupons(int amount, int source, bool arg1, int arg2);
    static void AddClanLootboxPoints(int amount, int source, bool arg1, bool arg2, int arg3);
    static void AddSomeCurrency(void* currency, int amount, bool arg1, int enum1, int enum2, int enum3, int enum4);
    static void* ProgressUpdaterGetInstance();
    static void SendChat(void* arg, void* msg);
    static void ProgressAddCurrency(void* instance, void* currency, int amount, int enum1, bool bool1, bool bool2, itemObtainParams* obtainParams);
    static void ActivateGadget(void* arg, int gadget_id, int level);
    static void DeactivateGadget(void* arg, int gadget_id);
    static void* SystemObjectToString(void* arg);
    static ImVec4 ImVec4i(int r, int g, int b, int a = 255);
};
