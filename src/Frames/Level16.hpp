#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "States/BossState.hpp"
#include "res/textureGroups.hpp"
#include "Controllers/BossController.hpp"

#include <memory>

static auto run_frame_level16(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level16;
  level.type = LevelState::Type::Boss;
  level.number = { 1, 6 };
  level.extra_textures = {
    texture_groups::red_cloud,
    texture_groups::q_block,
    texture_groups::king_goomba,
    texture_groups::goomba,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;

    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level16_1.csv");
    level_generator::generate_level(level, "level16_2.csv");

    auto& objects = level.game_objects; 
    auto& boss = objects.push(KingGoombaState());
    boss.position = glm::vec2(16, 8) * BlockBase::Size;

    level.stats.boss_hp = &boss.hp;
    level.stats.max_boss_hp = KingGoombaState::MaxHp;

    level.player.growth = PlayerState::Growth::Big;
    level.player.form = PlayerState::Form::Fire;
    level.player.position.x = 2 * BlockBase::Size;
  });
}
