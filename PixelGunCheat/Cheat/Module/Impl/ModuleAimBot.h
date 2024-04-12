#pragma once
#include <map>

#include "../IL2CPPResolver/IL2CPP_Resolver.hpp"

#include "../ModuleBase.h"
#include "../../Hooks/Hooks.h"
#include "../../Internal/Functions.h"

static BKCCheckbox __aim_bot_through_walls = BKCCheckbox("Through Walls", false);
static BKCModule __aim_bot = { "Aim Bot", COMBAT, 0x0, false, {&__aim_bot_through_walls} };

static std::map<std::string, Unity::Vector3> player_pos_cache;
static Unity::Vector3 aimed_pos = {0, 0, 0};

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

static Unity::Vector3 quaternation_mul(Unity::Quaternion& q, Unity::Vector3& v)
{
    Unity::Vector3 ret;
    // ret.w = q.x * v.x - q.y * v.y -q.z * v.z;
    ret.x = q.w * v.x + q.y * v.z -q.z * v.y;
    ret.y = q.w * v.y - q.x * v.z + q.z * v.x;
    ret.z = q.w * v.z + q.x * v.y - q.y * v.x;
    return ret;
}

class ModuleAimBot : ModuleBase
{
public:
    ModuleAimBot() : ModuleBase(&__aim_bot) {}
    
    void do_module(void* arg) override
    {
        Unity::Vector3 prediction;
        void* target = nullptr;
        double distance = 9999999;
        Unity::CCamera* camera = (Unity::CCamera*)Hooks::main_camera;
        for (void* player : Hooks::player_list)
        {
            if (player == nullptr) continue;
            // Only Enemies
            std::cout << Hooks::is_player_enemy(player) << std::endl;

            return;
            
            if (!Hooks::is_player_enemy(player)) continue;
            
            Unity::CTransform* transform = (Unity::CTransform*)Functions::PlayerGetTransform(player);
            Unity::Vector3 world = transform->GetPosition();
            
            Unity::Vector3 screen;
            camera->WorldToScreen(world, screen);

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

            Unity::Vector3 camera_pos = camera->GetTransform()->GetPosition();
            Unity::Vector3 head_pos = {
                world.x + (velocity.Normalize().x / 10),
                world.y + (velocity.Normalize().y / 10) + 0.75f,
                world.z + (velocity.Normalize().z / 10)
            };

            float distance = vec3_distance(world, camera_pos);
            
            if (distance > 800) continue;

            Unity::Vector3 one = Unity::Vector3 {1, 1, 1};
            Unity::Quaternion camera_rotation = camera->GetTransform()->GetRotation();
            Unity::Vector3 aimDirection = quaternation_mul(camera_rotation, one);
            Unity::Vector3 v = {head_pos.x - world.x, head_pos.y - world.x, head_pos.z - world.x};
            float d = v.Dot(aimDirection);
            Unity::Vector3 closest_point = {
                world.x + aimDirection.x * d,
                world.y + aimDirection.y * d,
                world.z + aimDirection.z * d
            };

            float new_dist = vec3_distance(closest_point, head_pos);
            if (!(distance > new_dist)) continue;

            if (!__aim_bot_through_walls.enabled)
            {
                // do raycast
            }

            distance = new_dist;
            target = player;
            prediction = {
                velocity.Normalize().x / 10,
                velocity.Normalize().y / 10,
                velocity.Normalize().z / 10
            };
        }

        return;
        
        if (target == nullptr)
        {
            std::cout << "No target" << std::endl;
        }
        
        if (target != nullptr)
        {
            Unity::CTransform* t = camera->GetTransform();
            Unity::CTransform* target_t = (Unity::CTransform*)Functions::PlayerGetTransform(target);
            Unity::Vector3 target_p = target_t->GetPosition();
            Unity::Vector3 aim_at = {
                target_p.x + prediction.x,
                target_p.y + prediction.y + 0.75f,
                target_p.z + prediction.z
            };
            // t->CallMethod<void*, Unity::Vector3>(t->GetMethodPointer("LookAt"), aim_at);
            // aimed_pos = aim_at;
            std::cout << Hooks::get_player_name(target) << std::endl;
            std::cout << "x: " << aim_at.x << " y: " << aim_at.y << " z: " << aim_at.z << std::endl;
        }
        else
        {
            aimed_pos = {0, 0, 0};
        }
    }
};
