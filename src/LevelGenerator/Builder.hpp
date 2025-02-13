#pragma once

#include "LevelGenerator/Textures.hpp"
#include "States/BlockState.hpp"
#include "States/FireFlowerPusherState.hpp"
#include "States/FireFlowerState.hpp"
#include "States/JumpingKoopaState.hpp"
#include "States/LevelState.hpp"
#include <glm/glm.hpp>

namespace level_generator{

static auto put_solid(LevelState& level, const glm::vec2& position, const Texture& texture){
  level.hitbox_grid_element(position) = 1;
  return level.game_objects.push(BlockState(position, &texture));
}

static auto put_hitbox_block(LevelState& level, const glm::vec2& position){
  put_solid(level, position, textures::dirt);
  level.game_objects.back<BlockState>().is_visible = false;
}

static auto put_nonsolid(LevelState& level, const glm::vec2& position, const Texture& texture){
  auto& block = level.game_objects.push(BlockState(position, &texture));
  block.is_solid = false;
}

static auto put_bricks(LevelState& level, const glm::vec2& position){
  level.hitbox_grid_element(position) = 1;

  put_solid(level, position, textures::dirt);
  level.game_objects.back<BlockState>().is_visible = false;
  level.game_objects.push(BricksBlockState(position));
  level.game_objects.back<BricksBlockState>().hitbox_block_index = level.game_objects.size<BlockState>() - 1;
};

static auto put_coin(LevelState& level, const glm::vec2& position){
  level.game_objects.push(CoinBlockState(position));
}

static auto put_q_block_with_coins(LevelState& level, const glm::vec2& position, int coins = 1){
  put_hitbox_block(level, position);
  auto& block = level.game_objects.push(QBlockState<CoinPusherState>(position));
  block.bounce_state.bounces_count = coins;
  block.pusher.coins.resize(coins, SpinningCoinState(position));

  auto& points = block.pusher.points_generator.items;
  points.reserve(coins);

  for (int i = 0; i < coins; ++i){
    auto particle = PointsParticlesState(QBlockReward, position * BlockBase::Size);

    points.push_back(particle);
  }
}

static auto put_q_block_with_flower(LevelState& level, const glm::vec2& position){
  put_hitbox_block(level, position);

  auto& block = level.game_objects.push(QBlockState<FireFlowerPusherState>(position));
  block.pusher.fire_flower = FireFlowerState(position);
}

using Direction = EntityState::Direction;
static constexpr auto DirectionLeft = EntityState::DirectionLeft;

static auto put_q_block_with_mushroom(
    LevelState& level, 
    const glm::vec2& position,
    MushroomState::Type mushroom_type
){
  put_hitbox_block(level, position);
  auto& block = level.game_objects.push(QBlockState<MushroomPusherState>(position));
  auto& mushroom = block.pusher.entity;

  mushroom.type = mushroom_type;

  using MushroomType = MushroomState::Type;
  mushroom = [&]{
    switch(mushroom.type){
      case MushroomType::Red: return MushroomState::make_red(position);
      case MushroomType::Green: return MushroomState::make_green(position);
    }
  }();
}

static auto put_checkpoint(LevelState& level, const glm::vec2& position){
  level.checkpoints.push_back(position * BlockBase::Size);
}

static auto put_hill(LevelState& level, const glm::vec2& position, Tile tile){
  level.game_objects.push(BackgroundHillState(position, &id_to_texture[tile].front()));
}

static auto put_bush(LevelState& level, const glm::vec2& position, Tile tile){
  level.game_objects.push(BackgroundBushState(position, &id_to_texture[tile].front()));
}

//Entities:

static auto put_goomba(LevelState& level, const glm::vec2& position){
  level.game_objects.push(GoombaState::make_normal(position));
}

static auto put_red_goomba(LevelState& level, const glm::vec2& position){
  level.game_objects.push(GoombaState::make_red(position));
}

static auto put_yellow_goomba(LevelState& level, const glm::vec2& position){
  level.game_objects.push(GoombaState::make_yellow(position));
}

static auto put_green_koopa(LevelState& level, const glm::vec2& position){
  level.game_objects.push(KoopaState::make_green(position - glm::vec2(0, 0.5)));
}

static auto put_red_koopa(LevelState& level, const glm::vec2& position){
  level.game_objects.push(KoopaState::make_red(position - glm::vec2(0, 0.5)));
}

static auto put_purple_koopa(LevelState& level, const glm::vec2& position){
  level.game_objects.push(KoopaState::make_purple(position - glm::vec2(0, 0.5)));
}

static auto put_green_koopa_with_wings(LevelState& level, const glm::vec2& position){
  level.game_objects.push(JumpingKoopaState::make_green(position - glm::vec2(0, 0.5)));
}

static auto put_red_koopa_with_wings(LevelState& level, const glm::vec2& position){
  level.game_objects.push(JumpingKoopaState::make_red(position - glm::vec2(0, 0.5)));
}

static auto put_purple_koopa_with_wings(LevelState& level, const glm::vec2& position){
  level.game_objects.push(JumpingKoopaState::make_purple(position - glm::vec2(0, 0.5)));
}

static auto put_plant(LevelState& level, const glm::vec2& position){
  level.game_objects.push(PlantState::make_green(position + glm::vec2(0.5, 1.f)));
}

static auto put_red_plant(LevelState& level, const glm::vec2& position){
  level.game_objects.push(PlantState::make_red(position + glm::vec2(0.5, 1.f)));
}

static auto put_hammerbro(LevelState& level, const glm::vec2& position){
  level.game_objects.push(HammerBroState::make(position - glm::vec2(0, 1)));
}

static auto put_beetle(LevelState& level, const glm::vec2& position){
  level.game_objects.push(BeetleState::make(position - glm::vec2(0, 0.5)));
}

template<typename Entity>
static auto put_q_block_with_entity(
    LevelState& level, 
    std::vector<QBlockState<EntityPusherState<Entity>>>& q_blocks_vec,
    const Entity& entity
){
  const auto position = entity.position / BlockBase::Size;

  level_generator::put_hitbox_block(level, position);

  auto& block = q_blocks_vec.emplace_back(position);
  auto& pusher_entity = block.pusher.entity;

  pusher_entity = entity;
  pusher_entity.position.y -= entity.size.y - BlockBase::Size;
  pusher_entity.is_in_q_block = true;
  pusher_entity.is_visible = false;
}

} //namespace level_generator
