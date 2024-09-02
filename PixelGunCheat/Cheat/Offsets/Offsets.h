#pragma once
#include <cstdint>

class Offsets
{
public:
    // Functions
    static inline uint64_t PlayerMoveCUpdate = 0x1BD7510;
    static inline uint64_t WeaponSoundsUpdate = 0x6CA9D0;
    static inline uint64_t SetNextCriticalHit = 0x6C2F90;
    static inline uint64_t MakeInvisibleForSeconds = 0x1B32AA0;
    static inline uint64_t InfiniteGemClaim = 0x4B9750;
    static inline uint64_t RapidFire = 0x1158280;
    static inline uint64_t TextMeshGetText = 0x44AC000;
    static inline uint64_t BehaviourGetEnabled = 0x43BEBE0;
    static inline uint64_t OnPreRender = 0x43363E0;
    static inline uint64_t TextMeshGetColor = 0x44ABDA0;
    static inline uint64_t WorldToScreenPoint = 0x439E1D0;
    static inline uint64_t ComponentGetTransform = 0x43BFC00;
    static inline uint64_t TransformGetRotation = 0x43D4B30;
    static inline uint64_t TransformGetPosition = 0x43D49C0;
    static inline uint64_t TransformLookAt = 0x43D2B30;
    static inline uint64_t PhysicsRayCast = 0x4432370;
    static inline uint64_t ObjectGetInstanceID = 0x43C7210;
    static inline uint64_t OnSceneUnload = 0x41B20B0;
    static inline uint64_t PriceModifier = 0x7E55C0;
    static inline uint64_t PlayerMoveCFixedUpdate = 0x1B32260;
    static inline uint64_t RewardMultiplier = 0xCA2670;
    static inline uint64_t DoubleRewards = 0xCA1CA0;
    static inline uint64_t PremiumPass = 0x191BCA0;
    static inline uint64_t GetItemRecordDict = 0x8046D0;
    static inline uint64_t AddWeapon = 0x68E160;
    static inline uint64_t ItemRecordGetShopId = 0x5F5320;
    static inline uint64_t GetDamageMultiplier = 0x116A970;
    static inline uint64_t GetPlayerSpeed = 0x115EB30;
    static inline uint64_t GetAmmoInClip = 0x116BA30;
    static inline uint64_t GetAmmo = 0x1178C00;
    static inline uint64_t PlayerGetImmortality = 0x1BC55D0;
    static inline uint64_t GiveWear = 0x208CF00;
    static inline uint64_t GiveWeapon = 0x2095530;
    static inline uint64_t GivePets = 0x2093D30;
    static inline uint64_t GiveGadget = 0x1F26CC0;
    static inline uint64_t AddGems = 0x20854D0;
    static inline uint64_t AddCoins = 0x2086510;
    static inline uint64_t AddSomeCurrency = 0x2084F10;
    static inline uint64_t AddClanChestPoints = 0x2085F80;
    static inline uint64_t ProgressUpdaterGetInstance = 0xA5C9B0;
    static inline uint64_t ProgressAddCurrency = 0xA3DF30;
    static inline uint64_t WeaponSoundsLateUpdate = 0x6C2B10;
    static inline uint64_t GetWeaponSkinSettings = 0x9518F0;
    static inline uint64_t GetIDListParentInstance = 0x14BFE90;
    static inline uint64_t GetIDList = 0x147C920;
    static inline uint64_t GadgetActivate = 0x1B4ABB0;
    static inline uint64_t GadgetDeactivate = 0x1BC7C50;
    static inline uint64_t SpoofModuleLevel = 0x15910A0;
    static inline uint64_t ModulePerkDuration = 0x1F22EF0;
    static inline uint64_t ThrowGadgetDamage = 0x1F22F90;
    static inline uint64_t GadgetDuration = 0x1F22FF0;
    static inline uint64_t GadgetCooldown = 0x1F22630;
    static inline uint64_t TeamKill = 0x1B1A440;
    static inline uint64_t LotteryDropCount = 0xBB04F0;
    static inline uint64_t LotteryDropId = 0xBB05F0;
    static inline uint64_t LotteryDropType = 0xBB0640;
    static inline uint64_t ForceItemDisplay = 0x143F4C0;
    static inline uint64_t CameraGetMain = 0x439E910;
    static inline uint64_t CameraGetCurrent = 0x439E7A0;
    static inline uint64_t CameraSetFieldOfView = 0x439F1F0;
    static inline uint64_t TransformSetRotation = 0x43D5360;
    
    // Fields
    static inline uint64_t bulletDelay = 0x1A4;
    static inline uint64_t playerMoveCPlayerDamageable = 0x678;
    static inline uint64_t nickLabel = 0x3D8;
    static inline uint64_t weaponSoundsPlayerMoveC = 0x500;
    static inline uint64_t myPlayerTransform = 0x3C0;
    static inline uint64_t headCollider = 0x130;
};
