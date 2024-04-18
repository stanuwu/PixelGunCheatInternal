#pragma once
#include <map>

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

static BKCSliderInt __aim_bot_target_size = BKCSliderInt("Target Marker Size", 5, 1, 20);
static BKCCheckbox __aim_bot_target_marker = BKCCheckbox("Target Marker", true);
static BKCCheckbox __aim_bot_through_walls = BKCCheckbox("Through Walls", false);
static BKCCheckbox __aim_bot_body_shot = BKCCheckbox("Body Shot", false);
static BKCModule __aim_bot = { "Aim Bot", COMBAT, 0x0, ImGuiKey_Insert, true, {&__aim_bot_target_marker, &__aim_bot_target_size, &__aim_bot_through_walls, &__aim_bot_body_shot} };

static ImU32 color_marker = ImGui::ColorConvertFloat4ToU32({1.00f, 0.00f, 1.00f, 1.00f});
static ImU32 color_border = ImGui::ColorConvertFloat4ToU32({0.00f, 0.00f, 0.00f, 1.00f});
static std::map<std::string, Unity::Vector3> player_pos_cache;
static RECT window_size_aim;

struct AimMarker
{
    Unity::Vector3 screen_pos;
};

static std::list<AimMarker> to_draw_aim;

static bool is_on_screen_aim(Unity::Vector3 pos)
{
    const int width = window_size_aim.right - window_size_aim.left;
    const int height = window_size_aim.bottom - window_size_aim.top;
    return pos.z > 0.01f && pos.x > -100 && pos.y > -100 && pos.x < (float)width + 100 && pos.y < (float)height + 100;
}

static bool is_zero(float* vector)
{
    return vector[0] == 0 && vector[1] == 0 && vector[2] == 0;
}

static float vec3_distance(Unity::Vector3& one, Unity::Vector3& two)
{
    return (float)abs(sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2) + pow(one.z - two.z, 2)));
}

static Unity::Vector3 crossProduct(const Unity::Vector3 &left, const Unity::Vector3 &right)
{
    return Unity::Vector3{
        left.y*right.z - left.z*right.y,
        left.z*right.x - left.x*right.z,
        left.x*right.y - left.y*right.x
    };
}

// https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
static Unity::Vector3 quaternation_mul(Unity::Quaternion& rotation, Unity::Vector3& value)
{
    Unity::Vector3 vector;
    float num12 = rotation.x + rotation.x;
    float num2 = rotation.y + rotation.y;
    float num = rotation.z + rotation.z;
    float num11 = rotation.w * num12;
    float num10 = rotation.w * num2;
    float num9 = rotation.w * num;
    float num8 = rotation.x * num12;
    float num7 = rotation.x * num2;
    float num6 = rotation.x * num;
    float num5 = rotation.y * num2;
    float num4 = rotation.y * num;
    float num3 = rotation.z * num;
    float num15 = ((value.x * ((1 - num5) - num3)) + (value.y * (num7 - num9))) + (value.z * (num6 + num10));
    float num14 = ((value.x * (num7 + num9)) + (value.y * ((1 - num8) - num3))) + (value.z * (num4 - num11));
    float num13 = ((value.x * (num6 - num10)) + (value.y * (num4 + num11))) + (value.z * ((1 - num8) - num5));
    vector.x = num15;
    vector.y = num14;
    vector.z = num13;
    return vector;
}

class ModuleAimBot : ModuleBase
{
public:
    ModuleAimBot() : ModuleBase(&__aim_bot) {}
    
    void do_module(void* arg) override
    {
        if (Hooks::tick % 60 == 0)
        {
            GetWindowRect(GetActiveWindow(), &window_size_aim);
        }

        const int height = window_size_aim.bottom - window_size_aim.top;
        
        Unity::Vector3 prediction;
        void* target = nullptr;
        float distance = 9999999;
        Unity::CCamera* camera = (Unity::CCamera*)Hooks::main_camera;
        for (void* player : Hooks::player_list)
        {
            if (player == nullptr || Hooks::our_player == nullptr) continue;
            // Only Enemies
            if (!Hooks::is_player_enemy(player)) continue;
            
            Unity::CTransform* transform = (Unity::CTransform*)Hooks::get_player_transform(player);
            Unity::Vector3 world = transform->GetPosition();
            
            Unity::Vector3 screen;
            Functions::CameraWorldToScreen(camera, &world, &screen);

            if (!is_on_screen_aim(screen)) continue;
            if (screen.z <= 0) continue;
            
            Unity::Vector3 velocity = {0, 0, 0};

            std::string player_name = Hooks::get_player_name(player);
            
            if (player_pos_cache.contains(player_name))
            {
                Unity::Vector3 cached = player_pos_cache.at(player_name);
                velocity = {
                    velocity.x - cached.x,
                    velocity.y - cached.y,
                    velocity.z - cached.z
                };
            }

            player_pos_cache.insert_or_assign(player_name, velocity);

            void* camera_transform = Functions::ComponentGetTransform(camera);

            Unity::Vector3 camera_pos;
            Functions::TransformGetPosition(camera_transform, &camera_pos);
            
            Unity::Vector3 head_pos = {
                world.x + (velocity.Normalize().x / 10),
                world.y + (velocity.Normalize().y / 10) + 0.75f,
                world.z + (velocity.Normalize().z / 10)
            };
            
            const float distance_r = vec3_distance(world, camera_pos);
            if (distance_r > 800) continue;
            
            Unity::Vector3 one = Unity::Vector3 {1, 1, 1};
            Unity::Quaternion camera_rotation;
            Functions::TransformGetRotation(camera_transform, &camera_rotation);
            
            Unity::Vector3 aimDirection = quaternation_mul(camera_rotation, one);
            Unity::Vector3 v = {head_pos.x - camera_pos.x, head_pos.y - camera_pos.x, head_pos.z - camera_pos.x};
            float d = v.Dot(aimDirection);
            Unity::Vector3 closest_point = {
                camera_pos.x + aimDirection.x * d,
                camera_pos.y + aimDirection.y * d,
                camera_pos.z + aimDirection.z * d
            };

            float new_dist = vec3_distance(closest_point, head_pos);
            if (!(distance > new_dist)) continue;

            if (!__aim_bot_through_walls.enabled)
            {
                Unity::Vector3 diff = {
                    head_pos.x - camera_pos.x,
                    head_pos.y - camera_pos.y,
                    head_pos.z - camera_pos.z
                };
                Ray ray = {
                    camera_pos.x,
                    camera_pos.y,
                    camera_pos.z,
                    diff.Normalize().x,
                    diff.Normalize().y,
                    diff.Normalize().z
                };
                RaycastHit hit_info;
                if (Functions::PhysicsRaycast(&ray, &hit_info, 800))
                {
                    void* head_collider = (void*)*(uint64_t*)((uint64_t)player + Offsets::headCollider);
                    int id = Functions::ObjectGetInstanceID(head_collider);
                    if (hit_info.collider != id) continue;
                }
                else
                {
                    continue;
                }
            }

            distance = new_dist;
            target = player;
            prediction = {
                velocity.Normalize().x / 10,
                velocity.Normalize().y / 10,
                velocity.Normalize().z / 10
            };
        }
        
        if (target != nullptr && Hooks::main_camera != nullptr)
        {
            Unity::CTransform* target_t = (Unity::CTransform*)Hooks::get_player_transform(target);
            Unity::Vector3 target_p;
            Functions::TransformGetPosition(target_t, &target_p);
            Unity::Vector3 aim_at = {
                target_p.x + prediction.x,
                target_p.y + prediction.y + (__aim_bot_body_shot.enabled ? 0 : 0.75f),
                target_p.z + prediction.z
            };
            Unity::CTransform* t = (Unity::CTransform*)Functions::ComponentGetTransform(Hooks::main_camera);
            if (camera == nullptr) return;
            Hooks::fov_changer_module->run(nullptr);
            Unity::Vector3 screen;
            Functions::CameraWorldToScreen(camera, &aim_at, &screen);
            to_draw_aim.push_back({{screen.x, height - screen.y, screen.z}});
            Unity::Vector3 up = {0, 1, 0};
            Functions::TransformLookAt(t, &aim_at, &up);
            Hooks::aimed_pos = &aim_at;
        }
        else
        {
            Unity::Vector3 zero = {0, 0, 0};
            Hooks::aimed_pos = &zero;
        }
    }

    void draw_all_aim()
    {
        if (is_enabled())
        {
            std::list<AimMarker> list = to_draw_aim;
            for (auto draw : list)
            {
                draw_aim(draw.screen_pos);
            }
        }
        to_draw_aim.clear();
    }

    void draw_aim(Unity::Vector3 screen_pos)
    {
        if (is_enabled() && __aim_bot_target_marker.enabled)
        {
            ImGui::GetBackgroundDrawList()->AddCircleFilled({screen_pos.x, screen_pos.y}, (float)__aim_bot_target_size.value + 0.5f, color_border);
            ImGui::GetBackgroundDrawList()->AddCircleFilled({screen_pos.x, screen_pos.y}, (float)__aim_bot_target_size.value, color_marker);
        }
    }
};
