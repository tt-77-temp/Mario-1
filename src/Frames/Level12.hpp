#pragma once

#include "States/AppState.hpp"
#include "Views/Level.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

static auto run_frame_level12(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level12;
  level.type = LevelState::Type::Horizontal;
  level.number = { 1, 2 };
  level.extra_textures = {
    texture_groups::red_cloud,
    &textures::mushroom_bg
  };

  run_frame_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;

    level_generator::generate_horizontal_level_clouds(level, 10);
    level_generator::generate_level(level, "level12_1.csv");
    level_generator::generate_level(level, "level12_2.csv");

    level_generator::put_q_block_with_coins(level, { 84, 2 }, 5);

    level_generator::put_q_block_with_coins(level, { 178, 3 }, 5);
    level_generator::put_q_block_with_coins(level, { 180, 3 }, 5);
    level_generator::put_q_block_with_coins(level, { 182, 3 }, 5);
  });
}
