#pragma once
#include <cstdint>

class Offsets
{
public:
    // Functions
    static inline uint64_t PlayerMoveCUpdate = 0x1BD8080;
    static inline uint64_t WeaponSoundsUpdate = 0x6CAA10;
    static inline uint64_t SetNextCriticalHit = 0x6C2FD0;
    static inline uint64_t MakeInvisibleForSeconds = 0x1B332F0;
    static inline uint64_t InfiniteGemClaim = 0x4B8EE0;
    static inline uint64_t RapidFire = 0x1151A20;
    static inline uint64_t TextMeshGetText = 0x44ABBA0;
    static inline uint64_t BehaviourGetEnabled = 0x43BE780;
    static inline uint64_t OnPreRender = 0x4335F80;
    static inline uint64_t TextMeshGetColor = 0x44AB940;
    static inline uint64_t WorldToScreenPoint = 0x439DD70;
    static inline uint64_t ComponentGetTransform = 0x43BF7A0;
    static inline uint64_t TransformGetRotation = 0x43D46D0;
    static inline uint64_t TransformGetPosition = 0x43D4560;
    static inline uint64_t TransformLookAt = 0x43D26D0;
    static inline uint64_t PhysicsRayCast = 0x4431F10;
    static inline uint64_t ObjectGetInstanceID = 0x43C6DB0;
    static inline uint64_t OnSceneUnload = 0x41B1C50;
    static inline uint64_t PriceModifier = 0x7E7A80;
    static inline uint64_t PlayerMoveCFixedUpdate = 0x1B32AB0;
    static inline uint64_t RewardMultiplier = 0xC9F810;
    static inline uint64_t DoubleRewards = 0xCA1230;
    static inline uint64_t PremiumPass = 0x1911720;
    static inline uint64_t GetItemRecordDict = 0x7FC5B0;
    static inline uint64_t AddWeapon = 0x692BF0;
    static inline uint64_t ItemRecordGetShopId = 0x5F5310;
    static inline uint64_t GetDamageMultiplier = 0x1156280;
    static inline uint64_t GetPlayerSpeed = 0x1163B10;
    static inline uint64_t GetAmmoInClip = 0x116D320;
    static inline uint64_t GetAmmo = 0x116E830;
    static inline uint64_t PlayerGetImmortality = 0x1B40640;
    static inline uint64_t GiveWear = 0x2084DB0;
    static inline uint64_t GiveWeapon = 0x2091630;
    static inline uint64_t GivePets = 0x20857B0;
    static inline uint64_t GiveGadget = 0x1F234D0;
    static inline uint64_t AddGems = 0x20958A0;
    static inline uint64_t AddCoins = 0x2093AB0;
    static inline uint64_t AddSomeCurrency = 0x2094920;
    static inline uint64_t AddClanChestPoints = 0x20966E0;
    static inline uint64_t ProgressUpdaterGetInstance = 0xA69B70;
    static inline uint64_t ProgressAddCurrency = 0xA3F9D0;
    static inline uint64_t WeaponSoundsLateUpdate = 0x6C2B50;
    static inline uint64_t GetWeaponSkinSettings = 0x9554F0;
    static inline uint64_t GetIDListParentInstance = 0x142A1E0;
    static inline uint64_t GetIDList = 0x1426230;
    static inline uint64_t GadgetActivate = 0x1B3EC70;
    static inline uint64_t GadgetDeactivate = 0x1BBB460;
    static inline uint64_t SpoofModuleLevel = 0x1579790;
    static inline uint64_t ModulePerkDuration = 0x1F2DC90;
    static inline uint64_t ThrowGadgetDamage = 0x1F2D770;
    static inline uint64_t GadgetDuration = 0x1F2D780;
    static inline uint64_t GadgetCooldown = 0x1F2DD40;
    static inline uint64_t TeamKill = 0x1B1AC50;
    static inline uint64_t LotteryDropCount = 0xBB0CD0;
    static inline uint64_t LotteryDropId = 0xBB0DD0;
    static inline uint64_t LotteryDropType = 0xBB0E20;
    static inline uint64_t ForceItemDisplay = 0x14BAE10;
    static inline uint64_t CameraGetMain = 0x439E4B0;
    static inline uint64_t CameraGetCurrent = 0x439E340;
    static inline uint64_t CameraSetFieldOfView = 0x439ED90;
    static inline uint64_t TransformSetRotation = 0x43D4F00;
    
    // Fields
    static inline uint64_t bulletDelay = 0x1A4;
    static inline uint64_t playerMoveCPlayerDamageable = 0x678;
    static inline uint64_t nickLabel = 0x3D8;
    static inline uint64_t weaponSoundsPlayerMoveC = 0x500;
    static inline uint64_t myPlayerTransform = 0x3C0;
    static inline uint64_t headCollider = 0x130;
};
