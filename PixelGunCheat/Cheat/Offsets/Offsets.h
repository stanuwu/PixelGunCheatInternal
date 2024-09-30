#pragma once
#include <cstdint>

class Offsets
{
public:
    // Functions
    static inline uint64_t PlayerMoveCUpdate = 0x1BE8B80;
    static inline uint64_t WeaponSoundsUpdate = 0x6CF7C0;
    static inline uint64_t SetNextCriticalHit = 0x6C7A60;
    static inline uint64_t MakeInvisibleForSeconds = 0x1B439C0;
    static inline uint64_t InfiniteGemClaim = 0x4BB300;
    static inline uint64_t RapidFire = 0x1185B00;
    static inline uint64_t TextMeshGetText = 0x44CF2A0;
    static inline uint64_t BehaviourGetEnabled = 0x43E1E90;
    static inline uint64_t OnPreRender = 0x4359630;
    static inline uint64_t TextMeshGetColor = 0x44CF040;
    static inline uint64_t WorldToScreenPoint = 0x43C1480;
    static inline uint64_t ComponentGetTransform = 0x43E2EB0;
    static inline uint64_t TransformGetRotation = 0x43F7DE0;
    static inline uint64_t TransformGetPosition = 0x43F7C70;
    static inline uint64_t TransformLookAt = 0x43F5DE0;
    static inline uint64_t PhysicsRayCast = 0x4455620;
    static inline uint64_t ObjectGetInstanceID = 0x43EA4C0;
    static inline uint64_t OnSceneUnload = 0x41D5300;
    static inline uint64_t PriceModifier = 0x7ED3A0;
    static inline uint64_t PlayerMoveCFixedUpdate = 0x1B43180;
    static inline uint64_t RewardMultiplier = 0xCA80A0;
    static inline uint64_t DoubleRewards = 0xCA9D70;
    static inline uint64_t PremiumPass = 0x18FEFC0;
    static inline uint64_t GetItemRecordDict = 0x80AAF0;
    static inline uint64_t AddWeapon = 0x6787D0;
    static inline uint64_t ItemRecordGetShopId = 0x5F7290;
    static inline uint64_t GetDamageMultiplier = 0x117F0F0;
    static inline uint64_t GetPlayerSpeed = 0x1163CF0;
    static inline uint64_t GetAmmoInClip = 0x116ACF0;
    static inline uint64_t GetAmmo = 0x11865F0;
    static inline uint64_t PlayerGetImmortality = 0x1B91B80;
    static inline uint64_t GiveWear = 0x20A4080;
    static inline uint64_t GiveWeapon = 0x209C9A0;
    static inline uint64_t GivePets = 0x20A3970;
    static inline uint64_t GiveGadget = 0x1F373D0;
    static inline uint64_t AddGems = 0x209A690;
    static inline uint64_t AddCoins = 0x2099180;
    static inline uint64_t AddSomeCurrency = 0x2099FD0;
    static inline uint64_t AddClanChestPoints = 0x20980D0;
    static inline uint64_t ProgressUpdaterGetInstance = 0xA3FC30;
    static inline uint64_t ProgressAddCurrency = 0xA6B3B0;
    static inline uint64_t WeaponSoundsLateUpdate = 0x6C75E0;
    static inline uint64_t GetWeaponSkinSettings = 0x9579A0;
    static inline uint64_t GetIDListParentInstance = 0x14FA4A0;
    static inline uint64_t GetIDList = 0x14961A0;
    static inline uint64_t GadgetActivate = 0x1BDA600;
    static inline uint64_t GadgetDeactivate = 0x1BD1EA0;
    static inline uint64_t SpoofModuleLevel = 0x15AB110;
    static inline uint64_t ModulePerkDuration = 0x1F33940;
    static inline uint64_t ThrowGadgetDamage = 0x1F33650;
    static inline uint64_t GadgetDuration = 0x1F33A40;
    static inline uint64_t GadgetCooldown = 0x1F32E80;
    static inline uint64_t TeamKill = 0x1B2B360;
    static inline uint64_t LotteryDropCount = 0xBB8CB0;
    static inline uint64_t LotteryDropId = 0xBB8DB0;
    static inline uint64_t LotteryDropType = 0xBB8E00;
    static inline uint64_t ForceItemDisplay = 0x143C300;
    static inline uint64_t CameraGetMain = 0x43C1BC0;
    static inline uint64_t CameraGetCurrent = 0x43C1A50;
    static inline uint64_t CameraSetFieldOfView = 0x43C24A0;
    static inline uint64_t TransformSetRotation = 0x43F8610;
    
    // Fields
    static inline uint64_t bulletDelay = 0x1A4;
    static inline uint64_t playerMoveCPlayerDamageable = 0x678;
    static inline uint64_t nickLabel = 0x3D8;
    static inline uint64_t weaponSoundsPlayerMoveC = 0x500;
    static inline uint64_t myPlayerTransform = 0x3C0;
    static inline uint64_t headCollider = 0x130;
};
