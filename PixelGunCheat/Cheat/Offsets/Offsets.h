#pragma once
#include <cstdint>

class Offsets
{
public:
    // Functions
   static inline uint64_t PlayerMoveCUpdate = 0x1C0BC90;
    static inline uint64_t WeaponSoundsUpdate = 0x6CF690;
    static inline uint64_t SetNextCriticalHit = 0x6C7900;
    static inline uint64_t MakeInvisibleForSeconds = 0x1B62C70;
    static inline uint64_t InfiniteGemClaim = 0x4BE8B0;
    static inline uint64_t RapidFire = 0x1166D30;
    static inline uint64_t TextMeshGetText = 0x4504FE0;
    static inline uint64_t BehaviourGetEnabled = 0x44179F0;
    static inline uint64_t OnPreRender = 0x438F190;
    static inline uint64_t TextMeshGetColor = 0x4504D80;
    static inline uint64_t WorldToScreenPoint = 0x43F6FE0;
    static inline uint64_t ComponentGetTransform = 0x4418A10;
    static inline uint64_t TransformGetRotation = 0x442D9A0;
    static inline uint64_t TransformGetPosition = 0x442D830;
    static inline uint64_t TransformLookAt = 0x442B9A0;
    static inline uint64_t PhysicsRayCast = 0x448B360;
    static inline uint64_t ObjectGetInstanceID = 0x4420080;
    static inline uint64_t OnSceneUnload = 0x420AE60;
    static inline uint64_t PriceModifier = 0x7F9EF0;
    static inline uint64_t PlayerMoveCFixedUpdate = 0x1B62480;
    static inline uint64_t RewardMultiplier = 0xCB0AF0;
    static inline uint64_t DoubleRewards = 0xCB22E0;
    static inline uint64_t PremiumPass = 0x19497C0;
    static inline uint64_t GetItemRecordDict = 0x814EB0;
    static inline uint64_t AddWeapon = 0x675050;
    static inline uint64_t ItemRecordGetShopId = 0x5F5990;
    static inline uint64_t GetDamageMultiplier = 0x1182920;
    static inline uint64_t GetPlayerSpeed = 0x116C050;
    static inline uint64_t GetAmmoInClip = 0x1169D60;
    static inline uint64_t GetAmmo = 0x116D140;
    static inline uint64_t PlayerGetImmortality = 0x1B88650;
    static inline uint64_t GiveWear = 0x20C3A50;
    static inline uint64_t GiveWeapon = 0x20C4110;
    static inline uint64_t GivePets = 0x20CC200;
    static inline uint64_t GiveGadget = 0x1F5A4C0;
    static inline uint64_t AddGems = 0x20D34D0;
    static inline uint64_t AddCoins = 0x20D1090;
    static inline uint64_t AddSomeCurrency = 0x20D1FB0;
    static inline uint64_t AddClanChestPoints = 0x20D2FC0;
    static inline uint64_t ProgressUpdaterGetInstance = 0xA4E620;
    static inline uint64_t ProgressAddCurrency = 0xA6BDA0;
    static inline uint64_t WeaponSoundsLateUpdate = 0x6C7480;
    static inline uint64_t GetWeaponSkinSettings = 0x961B60;
    static inline uint64_t GetIDListParentInstance = 0x14FEF80;
    static inline uint64_t GetIDList = 0x14B2100;
    static inline uint64_t GadgetActivate = 0x1BCE240;
    static inline uint64_t GadgetDeactivate = 0x1BD6230;
    static inline uint64_t SpoofModuleLevel = 0x15AB9D0;
    static inline uint64_t ModulePerkDuration = 0x1F57AB0;
    static inline uint64_t ThrowGadgetDamage = 0x1F586B0;
    static inline uint64_t GadgetDuration = 0x1F58150;
    static inline uint64_t GadgetCooldown = 0x1F57DF0;
    static inline uint64_t TeamKill = 0x1B4A620;
    static inline uint64_t LotteryDropCount = 0xBC23A0;
    static inline uint64_t LotteryDropId = 0xBC24A0;
    static inline uint64_t LotteryDropType = 0xBC24F0;
    static inline uint64_t ForceItemDisplay = 0x14633A0;
    static inline uint64_t CameraGetMain = 0x43F7720;
    static inline uint64_t CameraGetCurrent = 0x43F75B0;
    static inline uint64_t CameraSetFieldOfView = 0x43F8000;
    static inline uint64_t TransformSetRotation = 0x442E1D0;
    
    // Fields
    static inline uint64_t bulletDelay = 0x1A4;
    static inline uint64_t playerMoveCPlayerDamageable = 0x690;
    static inline uint64_t nickLabel = 0x3e0;
    static inline uint64_t weaponSoundsPlayerMoveC = 0x500;
    static inline uint64_t myPlayerTransform = 0x3C0;
    static inline uint64_t headCollider = 0x130;
};
