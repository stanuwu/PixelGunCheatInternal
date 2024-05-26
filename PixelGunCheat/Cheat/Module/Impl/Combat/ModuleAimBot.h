#pragma once
#include <map>
#include <math.h>

#include "../../ModuleBase.h"
#include "../../../Internal/Functions.h"
#include "../../../Offsets/Offsets.h"
#include "../../../Util/ClientUtil.h"
#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

static BKCDropdown __aim_bot_body_parts = BKCDropdown("Aim at Body Parts", L"Head", { L"Head", L"Body", L"Foot" });
static BKCSliderInt __aim_bot_smoothness = BKCSliderInt("Aim Bot Smoothness", 10, 0, 100, "Set to 0 for instant aim.");
static BKCSliderInt __aim_bot_target_size = BKCSliderInt("Target Marker Size", 5, 1, 20);
static BKCCheckbox __aim_bot_target_marker = BKCCheckbox("Target Marker", true, "Highlight the current target. Only works with silent aim.");
static BKCCheckbox __aim_bot_silent_aim = BKCCheckbox("Silent Aim", false, "Aims without moving your crosshair.");
static BKCSliderInt __aim_bot_radius = BKCSliderInt("Aim Radius", 300, 0, 1000, "Set to 0 to aim everywhere.");
static BKCCheckbox __aim_bot_through_walls = BKCCheckbox("Through Walls", false);
static BKCModule __aim_bot = { "Aim Bot", "Aims at enemies.", COMBAT, 0x0, true, {&__aim_bot_body_parts, &__aim_bot_smoothness, &__aim_bot_target_marker,&__aim_bot_target_size,&__aim_bot_silent_aim,&__aim_bot_radius,&__aim_bot_through_walls}};

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

static bool is_within_silent_aim(Unity::Vector3 pos, float rad)
{
    const float center_x = (window_size_aim.right + window_size_aim.left) / 2.0f;
    const float center_y = (window_size_aim.bottom + window_size_aim.top) / 2.0f;
    float distance = std::sqrt((pos.x - center_x) * (pos.x - center_x) + (pos.y - center_y) * (pos.y - center_y));
    return distance <= rad && pos.z > 0.01f;
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
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x
    };
}

static float to_rad(float angle)
{
    return angle * (float)(M_PI / 180);
}

static float to_deg(float angle)
{
    return angle * (float)(180 / M_PI);
}

static float lerp_angle(float theta1, float theta2, float ratio)
{
    float max = M_PI * 2;
    float da = fmod(theta2 - theta1, max);
    float dist = fmod(2 * da, max) - da;
    return theta1 + dist * ratio;
    
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
    bool is_using_silent_aim = __aim_bot_silent_aim.enabled;
    void* last_aim = nullptr;
    long time = 0;
    
    ModuleAimBot() : ModuleBase(&__aim_bot) {}
    
    void do_module(void* arg) override
    {
        is_using_silent_aim = __aim_bot_silent_aim.enabled;

        if (ClientUtil::tick % 60 == 0)
        {
            GetWindowRect(GetActiveWindow(), &window_size_aim);
        }
        
        const int height = window_size_aim.bottom - window_size_aim.top;

        if (is_using_silent_aim)
        {
            if (arg == nullptr) return;
            set_float(arg, Offsets::bulletDelay, 0.000001f); // bulletDelay
        }

        Unity::Vector3 prediction;
        void* target = nullptr;
        float distance = 9999999;
        Unity::Vector3 aim_at;
        void* camera = Hooks::main_camera;
        if (camera == nullptr) return;
        for (void* player : Hooks::player_list)
        {
            if (player == nullptr || Hooks::our_player == nullptr) continue;
            // Only Enemies
            if (!Hooks::is_player_enemy(player)) continue;

            void* transform = Hooks::get_player_transform(player);
            Unity::Vector3 world;
            Functions::TransformGetPosition(transform, &world);
            // Unity::Vector3 world = transform->GetPosition();

            Unity::Vector3 screen;
            Functions::CameraWorldToScreen(camera, &world, &screen);

            if (__aim_bot_radius.value > 0)
            {
                if (!is_within_silent_aim(screen, (float)__aim_bot_radius.value)) continue;
            }
            else
            {
                if (!is_on_screen_aim(screen)) continue;
            }

            if (screen.z <= 0) continue;

            Unity::Vector3 velocity = { 0, 0, 0 };

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

            Unity::Vector3 head_pos
            {
            world.x + (velocity.Normalize().x / 10),
            world.y + (velocity.Normalize().y / 10) + 0.75f,
            world.z + (velocity.Normalize().z / 10)
            };

            const float distance_r = vec3_distance(world, camera_pos);
            if (distance_r > 800) continue;

            Unity::Vector3 forward = Unity::Vector3{ 0, 0, 1 };
            Unity::Quaternion camera_rotation;
            Functions::TransformGetRotation(camera_transform, &camera_rotation);

            Unity::Vector3 aimDirection = quaternation_mul(camera_rotation, forward);
            Unity::Vector3 v = { head_pos.x - camera_pos.x, head_pos.y - camera_pos.x, head_pos.z - camera_pos.x };
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
            void* target_t = Hooks::get_player_transform(target);
            Unity::Vector3 target_p;
            Functions::TransformGetPosition(target_t, &target_p);

            /*
            if (__aim_bot_smoothness.value > 0)
            {
                Unity::Vector3* aim_vec = new Unity::Vector3 {
                    (head_pos.x - closest_point.x) / (float)__aim_bot_smoothness.value,
                    (head_pos.y - closest_point.y) / (float)__aim_bot_smoothness.value,
                    (head_pos.z - closest_point.z) / (float)__aim_bot_smoothness.value
                };
                // Logger::log_debug(std::to_string(aim_vec->x) + " | " + std::to_string(aim_vec->y) + " | " + std::to_string(aim_vec->z));
                target_p = { closest_point.x + aim_vec->x, closest_point.y + aim_vec->y, closest_point.z + aim_vec->z };
            }
            */

            if (__aim_bot.enabled && __aim_bot_body_parts.current_value == L"Head")
                aim_at = { target_p.x + prediction.x, target_p.y + prediction.y + 0.75f, target_p.z + prediction.z };
            if (__aim_bot.enabled && __aim_bot_body_parts.current_value == L"Body")
                aim_at = { target_p.x + prediction.x, target_p.y + prediction.y + 0.0f, target_p.z + prediction.z };
            if (__aim_bot.enabled && __aim_bot_body_parts.current_value == L"Foot")
                aim_at = { target_p.x + prediction.x, target_p.y + prediction.y - 0.75f, target_p.z + prediction.z };
        }

        if (target != nullptr && Hooks::main_camera != nullptr)
        {
            void* t = Functions::ComponentGetTransform(Hooks::main_camera);
            if (camera == nullptr) return;
            Hooks::fov_changer_module->run(nullptr);
            Unity::Vector3 screen;
            Functions::CameraWorldToScreen(camera, &aim_at, &screen);
            to_draw_aim.push_back({ {screen.x, height - screen.y, screen.z} });
            Unity::Vector3 up = { 0, 1, 0 };
            Unity::Quaternion camera_rotation;
            Functions::TransformGetRotation(t, &camera_rotation);
            Functions::TransformLookAt(t, &aim_at, &up);
            Unity::Quaternion camera_rotation2;
            Functions::TransformGetRotation(t, &camera_rotation2);
            
            
            if (target != last_aim && target != nullptr)
            {
                time = 0;
            }
            last_aim = target;
            time++;
            if (__aim_bot_smoothness.value > 0 && !__aim_bot_silent_aim.enabled)
            {
                Unity::Vector3 euler1 = camera_rotation.ToEuler();
                Unity::Vector3 euler2 = camera_rotation2.ToEuler();
                int smoothing = __aim_bot_smoothness.value * 10;
                if (time > smoothing) time = smoothing;
                Unity::Vector3 real_rot_euler = {
                    to_deg(lerp_angle(to_rad(euler1.x), to_rad(euler2.x), (float)time / smoothing)),
                    to_deg(lerp_angle(to_rad(euler1.y), to_rad(euler2.y), (float)time / smoothing)),
                    to_deg(lerp_angle(to_rad(euler1.z), to_rad(euler2.z), (float)time / smoothing)),
                };
                Unity::Quaternion real_rot = {0, 0, 0, 0};
                real_rot = real_rot.Euler(real_rot_euler);
                Functions::TransformSetRotation(t, &real_rot);
            }
            
            Hooks::aimed_pos = &aim_at;
        }
        else
        {
            Unity::Vector3 zero = { 0, 0, 0 };
            Hooks::aimed_pos = &zero;
            time = 0;
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
            if (__aim_bot_radius.value > 0) draw_silent_aim_circle();
        }
        to_draw_aim.clear();
    }

    void draw_silent_aim_circle()
    {
        if (__aim_bot_radius.value > 0 && is_enabled() && !Hooks::player_list.empty())
        {
            ImGui::GetBackgroundDrawList()->AddCircle({ (window_size_aim.right + window_size_aim.left) / 2.0f - 1.1f, (window_size_aim.bottom + window_size_aim.top) / 2.0f - 1.1f}, (float)__aim_bot_radius.value, color_marker, 64, 2.0f);
        }
    }

    void draw_aim(Unity::Vector3 screen_pos)
    {
        if (is_enabled() && __aim_bot_target_marker.enabled && __aim_bot_silent_aim.enabled)
        {
            ImGui::GetBackgroundDrawList()->AddCircleFilled({screen_pos.x, screen_pos.y}, (float)__aim_bot_target_size.value + 0.5f, color_border);
            ImGui::GetBackgroundDrawList()->AddCircleFilled({screen_pos.x, screen_pos.y}, (float)__aim_bot_target_size.value, color_marker);
        }
    }
};
