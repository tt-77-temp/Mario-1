#pragma once

#include "States/EntityState.hpp"
#include "States/BlockState.hpp"

#include "Controllers/PlayerController.hpp"
#include "Controllers/PointsParticlesController.hpp"

#include "Window.hpp"

static auto fire_flower_controller(FireFlowerState& flower, LevelState& level){
  for (auto& p : flower.points_generator.items){
    points_particles_controller(p);
  }

  //Interaction with player
  auto& player = level.player;
  if (collision::is_hovering(player, flower) && flower.is_visible){
    flower.points_generator.item().set_active(
      FireFlowerState::RewardForEating,
      flower.position
    );
    level.stats.score += FireFlowerState::RewardForEating;
  
    flower.is_visible = false;
    flower.position.y = util::BigValue;

    const auto is_player_big = player.growth == PlayerState::Growth::Big;
    if (is_player_big){
      player.is_changing_to_fire = true;
    }
    else{
      player.is_growing_up = true;
    }
  }
}
