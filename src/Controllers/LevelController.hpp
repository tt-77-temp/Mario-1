#pragma once

#include "States/LevelState.hpp"
#include "States/LoopedCounter.hpp"
#include "States/AppState.hpp"

#include "Controllers/CollisionController.hpp"
#include "Controllers/PlayerController.hpp"
#include "Controllers/BlinkController.hpp"
#include "Controllers/CoinController.hpp"
#include "Controllers/BounceController.hpp"
#include "Controllers/QBlockController.hpp"
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
#include "Controllers/PlatformController.hpp"

#include "Controllers/KingGoombaController.hpp"
#include "Controllers/KingKoopaController.hpp"

#include "Util/Util.hpp"
#include "Window.hpp"
#include "config.hpp"
#include "res/textures.hpp"

static auto level_background_controller(LevelState& level){
  level.cloud_offset += window::delta_time;

  static constexpr auto CloudMaxOffset = 18.f;
  if (level.cloud_offset > CloudMaxOffset) level.cloud_offset -= CloudMaxOffset;
}

static auto level_finish(LevelState& level, AppState& app){
  const auto finish = level.finish_position; 
  auto& player = level.player;

  if (level.is_finished){
    level.score_adding_after_finish_delay -= window::delta_time;

    if (level.player.position.y / BlockBase::Size < finish.y + 1){
      level.player.position.y += window::delta_time * BlockBase::Size;
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

  if (player.position.y / BlockBase::Size - finish.y != util::in_range(-1, 0)) return;
  if (player.position.x / BlockBase::Size - finish.x != util::in_range(0.25, 0.75)) return;

  if (window::is_key_pressed(GLFW_KEY_DOWN)) level.is_finished = true;
}

static auto get_worlds_first_level(AppState::Frame level){
  static constexpr auto LevelsInWorld = 6;
  const auto level_number = level | util::as<int>;

  return ((level_number / LevelsInWorld) * LevelsInWorld) | util::as<AppState::Frame>;
}

static auto level_restart_when_player_fell_out(AppState& app){
  auto& level = app.current_level;
  auto& player = level.player;

  if (player.position.y > (level.max_size().y + 1) * BlockBase::Size) {
    player.can_move = false;
    //set speed to 0
    player.set_direction(EntityState::DirectionLeft, 0);
  }

  const auto position_required_to_restart_level 
    = LevelState::PlayerYToRestartLevel 
    + level.max_size().y 
    * BlockBase::Size;

  if (player.position.y > position_required_to_restart_level){
    app.should_restart_current_frame = true;
    level.stats.hp--;
    player.form = PlayerState::Form::Normal;
    player.growth = PlayerState::Growth::Small;

    if (level.stats.hp == 0){
      level.stats = StatsState{};
      app.current_frame = get_worlds_first_level(app.current_frame);
      level.current_checkpoint = LevelState::CheckpointNotSet;
    }
  }
}

static auto level_camera(LevelState& level){
  auto& player = level.player;
  auto player_y = player.position.y - BlockBase::Size + player.size.y;
  
  //Vertical level scroll
  if (level.type == LevelState::Type::Vertical){
    if (player_y - level.camera_offset.y < LevelState::MinPlayerRelativeY){
      level.camera_offset.y = player_y - LevelState::MinPlayerRelativeY;
    }

    if (player_y - level.camera_offset.y > LevelState::MaxPlayerRelativeY){
      level.camera_offset.y = player_y - LevelState::MaxPlayerRelativeY;
    }

    const auto max_scroll_y = level.max_size().y * BlockBase::Size - config::FrameBufferSize.y;
    const auto min_scroll_y = std::max<float>((level.finish_position.y - 5) * BlockBase::Size, 0);
    if (level.type == LevelState::Type::Vertical){
      level.camera_offset.y = std::clamp(level.camera_offset.y, min_scroll_y, max_scroll_y);
    }
  }
  //Horizontal level scroll
  else if (level.type == LevelState::Type::Horizontal){
    static constexpr auto HorizontalLevelWidth = LevelState::HorizontalLevelSize.x;
    static constexpr auto PlayerPositionToScroll = LevelState::PlayerPositionToStartLevelScrolling;

    const auto player_position_to_stop_scrolling 
      = HorizontalLevelWidth * BlockBase::Size 
      - (PlayerPositionToScroll.x + BlockBase::Size) * 2;

    if (player.position.x >= PlayerPositionToScroll.x && level.type == LevelState::Type::Horizontal){
      level.camera_offset.x = std::min(
        player.position.x - PlayerPositionToScroll.x,
        player_position_to_stop_scrolling
      );
    }
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

static auto level_controller(AppState& app){
  auto& level = app.current_level;

  //Level loading
  if (level.load_delay > 0.f) {
    level.load_delay -= window::delta_time;
    return;
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

  if (player.is_growing_up || player.is_shrinking || player.is_changing_to_fire) return;
  //Game objects
  level_background_controller(level);
  level.game_objects.run_controllers<Controller>(level);

  level_finish(level, app);

  //Counting coins
  static constexpr auto CoinsToGetHP = 100;
  if (level.stats.coins >= CoinsToGetHP) {
    level.stats.coins -= CoinsToGetHP;
    level.stats.hp++;
  }

  //Camera
  level_camera(level);

  //Timers
  LevelState::timer += window::delta_time;
  level.purple_flying_koopa_timer += window::delta_time;
}
