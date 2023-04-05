#pragma once

#include "Controllers/KingGoombaController.hpp"
#include "States/LevelState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/AppState.hpp"

#include "Controllers/CollisionController.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/BlinkController.hpp"
#include "Controllers/CoinController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/QBlockController.hpp"
#include "Controllers/SpinningCoinController.hpp"
#include "Controllers/StatsController.hpp"
#include "Controllers/PointsParticlesController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/GoombaController.hpp"
#include "Controllers/MushroomController.hpp"
#include "Controllers/BricksController.hpp"
#include "Controllers/FireFlowerController.hpp"
#include "Controllers/ShellMonsterController.hpp"
#include "Controllers/SpikeController.hpp"
#include "Controllers/PlantController.hpp"
#include "Controllers/JumpingKoopaController.hpp"
#include "Controllers/FlyingKoopaController.hpp"
#include "Controllers/BeetleController.hpp"
#include "Controllers/KoopaController.hpp"
#include "Controllers/FireBarController.hpp"
#include "Controllers/HammerBroController.hpp"

#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_blocks_controller(LevelState& level){
  auto& player = level.player;
  auto& blocks = level.blocks;

  for (auto& block : blocks.q_blocks){
    q_block_controller(block, level);
  }

  for (auto& block : blocks.bricks){
    bricks_controller(block, level);
  }

  for (auto& block : blocks.spinning_coins){
    spinning_coin_controller(block, level);
  }

  for (auto& block : blocks.coins){
    coin_controller(block, level);
  }
}

static auto level_entities_controller(LevelState& level){
  auto& entities = level.entities;
  auto& player = level.player;

  for (auto& goomba : level.entities.goombas){
    normal_goomba_controller(goomba, level);
  }

  for (auto& goomba : level.entities.red_goombas){
    red_goomba_controller(goomba, level);
  }

  for (auto& goomba : level.entities.yellow_goombas){
    yellow_goomba_controller(goomba, level);
  }

  for (auto& spike : level.entities.spikes){
    spike_controller(spike, level);
  }

  for (auto& plant : level.entities.plants){
    green_plant_controller(plant, level);
  }

  for (auto& plant : level.entities.red_plants){
    red_plant_controller(plant, level);
  }

  for (auto& koopa : level.entities.green_koopas){
    green_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.purple_koopas){
    purple_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.green_jumping_koopas){
    green_jumping_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.green_flying_koopas){
    green_flying_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.red_koopas){
    red_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.red_jumping_koopas){
    red_jumping_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.red_flying_koopas){
    red_flying_koopa_controller(koopa, level);
  }

  for (auto& koopa : level.entities.beetles){
    beetle_controller(koopa, level);
  }

  for (auto& bro : level.entities.hammerbros){
    hammerbro_controller(bro, level);
  }
}

static auto level_mushrooms_controller(LevelState& level){
  auto& player = level.player;

  for (auto& mushroom : level.entities.mushrooms){
    red_mushroom_controller(mushroom, level);
  }

  for (auto& mushroom : level.entities.green_mushrooms){
    green_mushroom_controller(mushroom, level);
  }
}

static auto level_finish(LevelState& level, AppState& app){
  const auto finish = level.get_finishing_pipe_position();
  auto& player = level.player;

  if (level.is_finished){
    level.score_adding_after_finish_delay -= window::delta_time;

    if (level.player.position.y / config::BlockSize < finish.y + 1){
      level.player.position.y += window::delta_time * config::BlockSize;
    }

    if (level.stats.time > 0) {
      if (level.score_adding_after_finish_delay <= 0.f){
        const auto multiplier = std::min(2, level.stats.time);

        level.score_adding_after_finish_delay = 1.f / 60.f;
        level.stats.time -= multiplier;
        static constexpr auto PointsForEachTimeUnit = 10;
        level.stats.score += PointsForEachTimeUnit * multiplier;
      }
    }
    else{
      level.finish_delay -= window::delta_time;

      if (level.finish_delay <= 0.f){
        app.current_frame = (app.current_frame | util::as<int>) + 1 | util::as<AppState::Frame>;
        app.current_level.current_checkpoint = LevelState::CheckpointNotSet;
      }
    }
  }

  if (player.position.y / config::BlockSize - finish.y != util::in_range(-1, 0)) return;
  if (player.position.x / config::BlockSize - finish.x != util::in_range(0.25, 0.75)) return;

  if (window::is_key_pressed(GLFW_KEY_DOWN)) level.is_finished = true;
}

static auto level_restart_when_player_fell_out(AppState& app){
  auto& level = app.current_level;
  auto& player = level.player;

  if (player.position.y > (level.get_size().y + 1) * config::BlockSize) {
    player.can_move = false;
    //set speed to 0
    player.set_direction(EntityState::DirectionLeft, 0);
  }

  const auto position_required_to_restart_level 
    = config::PlayerPositionToRestartLevel 
    + level.get_size().y 
    * config::BlockSize;

  if (player.position.y > position_required_to_restart_level){
    app.should_restart_current_frame = true;
    level.stats.hp--;
    player.form = PlayerState::Form::Normal;
    player.growth = PlayerState::Growth::Small;

    if (level.stats.hp == 0){
      level.stats = StatsState{};
      app.current_frame = AppState::Frame::Level11;
      level.current_checkpoint = LevelState::CheckpointNotSet;
    }
  }
}

static auto level_handle_vertical_scroll(LevelState& level){
  auto& player = level.player;
  auto player_y = player.position.y - config::BlockSize + player.size.y;
  
  if (player_y - level.camera_offset_y < LevelState::MinPlayerRelativeY && player.gravity < 0.f){
    level.camera_offset_y = player_y - LevelState::MinPlayerRelativeY;
  }

  if (player_y - level.camera_offset_y > LevelState::MaxPlayerRelativeY && player.gravity > 0.f){
    level.camera_offset_y = player_y - LevelState::MaxPlayerRelativeY;
  }
}

static auto level_checkpoints_controller(LevelState& level){
  auto& player = level.player;

  for (const auto& checkpoint : level.checkpoints){
    if (level.type == LevelState::Type::Horizontal && player.position.x >= checkpoint.x){
      level.current_checkpoint = checkpoint;
    }

    if (level.type == LevelState::Type::Vertical && player.position.y <= checkpoint.y){
      level.current_checkpoint = checkpoint;
    }
  }
}

static auto level_bosses(AppState& app){
  auto& level = app.current_level;

  switch(app.current_frame){
    case AppState::Frame::Level16: king_goomba_controller(*level.bosses.king_goomba, level); break;
    default: break;
  }
}

static auto level_controller(AppState& app){
  auto& level = app.current_level;

  //Level loading
  if (level.load_delay > 0.f) {
    level.load_delay -= window::delta_time;
    return;
  }
  
  if (level.type == LevelState::Type::Vertical){
    level_handle_vertical_scroll(level);
  }

  level_checkpoints_controller(level);
  level_restart_when_player_fell_out(app);

  //Blinking and counters
  level.blink_state = blink_controller();
  level.coin_spin_counter.run();
  level.fire_flower_blink_counter.run();

  level.purple_koopa_counter.run();
  level.fireball_counter.run();
  level.hammer_counter.run();

  auto& player = level.player;

  if (!level.is_finished) {
    stats_controller(level.stats);
    player_controller(player, level);

    if (level.stats.time <= 0){
      player.is_dead = true;
    }
  }

  level_mushrooms_controller(level);

  for (auto& block : level.blocks.fire_flowers){
    fire_flower_controller(block, level);
  }

  for (auto& bar : level.fire_bars){
    fire_bar_controller(bar, level);
  }

  if (player.is_growing_up || player.is_shrinking || player.is_changing_to_fire) return;

  level_finish(level, app);

  level_blocks_controller(level);
  level_entities_controller(level);

  //Counting coins
  static constexpr auto CoinsAmountToGetHP = 100;
  if (level.stats.coins >= CoinsAmountToGetHP) {
    level.stats.coins -= CoinsAmountToGetHP;
    level.stats.hp++;
  }

  //Bosses
  if (level.type == LevelState::Type::Boss){
    level_bosses(app);
  }

  LevelState::timer += window::delta_time;
}
