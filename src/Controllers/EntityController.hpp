#pragma once

#include "Controllers/CollisionController.hpp"
#include "States/EntityState.hpp"
#include "States/LevelState.hpp"
#include "Util.hpp"
#include "Window.hpp"

static auto detect_entity_collision_with_level = [](EntityState& entity, LevelState& level, auto callable){
  if (!entity.should_collide){
    return;
  }

  for (const auto& block : level.blocks.normal){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }
};

static auto detect_turning_entity_collision_with_level = [](EntityState& entity, LevelState& level, auto callable){
  detect_entity_collision_with_level(entity, level, callable);

  for (const auto& block : level.blocks.entity_hitbox_blocks){
    if (!block.is_solid) continue;

    const auto collision_state = collision_controller(util::Rect(entity), util::Rect(block));
    callable(collision_state);
  }
};

template<typename F>
static auto entity_movement_helper(EntityState& entity, LevelState& level, const F& detect_collisions){
  if (entity.is_dead || !entity.is_active) return;

  auto left_boost = window::delta_time * entity.acceleration.left * 100;
  auto right_boost = window::delta_time * entity.acceleration.right * 100;

  detect_collisions(entity, level, [&](const auto& collision_state){
    if (collision_state.distance_left < left_boost){
      left_boost = collision_state.distance_left;
      entity.acceleration.left = 0.f;
    }
    if (collision_state.distance_right < right_boost){
      right_boost = collision_state.distance_right;
      entity.acceleration.right = 0.f;
    }
  });

  entity.position.x += right_boost;
  entity.position.x -= left_boost;
}

static auto entity_movement(EntityState& entity, LevelState& level){
  entity_movement_helper(entity, level, detect_entity_collision_with_level);
}

//This is for entities that can turn around when reaching the edge of a block
static auto entity_movement_with_turning(EntityState& entity, LevelState& level){
  entity_movement_helper(entity, level, detect_turning_entity_collision_with_level);
}

static auto entity_gravity(EntityState& entity, LevelState& level, int gravity_boost = 1){
  if (entity.is_dead && entity.death_delay > 0.f) return;
  if (!entity.is_active) return;

  if (entity.is_on_ground && !entity.is_dead && entity.should_collide) entity.gravity = 0.f;
  entity.is_on_ground = false;

  entity.gravity += window::delta_time * 50 * gravity_boost;

  detect_entity_collision_with_level(entity, level, [&](const auto& collision_state){
    if (entity.death_delay <= 0.f) return;

    if (collision_state.distance_below < entity.gravity && collision_state.distance_below > -CollisionOffset){
      entity.is_on_ground = true;
      entity.gravity = collision_state.distance_below;
    }
  });

  entity.position.y += entity.gravity * window::delta_time * 60;
}

static auto entity_turn_around(EntityState& entity, int speed){
  if (entity.acceleration.left == 0 && entity.acceleration.right == 0){
    entity.direction = -entity.direction;

    if (entity.direction == EntityState::DirectionLeft){
      entity.acceleration.left = speed;
    }

    if (entity.direction == EntityState::DirectionRight){
      entity.acceleration.right = speed;
    }
  }
}
