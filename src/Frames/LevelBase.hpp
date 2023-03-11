#pragma once
#include "States/AppState.hpp"
#include "LevelGenerator.hpp"
#include "res/textureGroups.hpp"
#include "res/textures.hpp"

struct LevelFrameSharedData{
  LevelState::Type type;
  AppState::Frame frame;
  std::vector<TextureGroup> extra_textures;

  struct LevelNumber{
    int major = 0;
    int minor = 0;
  } number;
};

template<typename Function>
static auto run_frame_levelbase(
    AppState& app, 
    const LevelFrameSharedData& level_data,
    const Function& extra_setup
){
  auto textures = std::vector<TextureGroup>{
    texture_groups::mario,
    &textures::mini_coin
  };

  const auto setup = [&](auto& app){
    fonts::normal.allocate(16);
    LevelState::timer = 0.f;

    auto& level = app.current_level;

    level.type = level_data.type;
    level.generate_hitbox_grid();

    auto& player = level.player;

    if (level.type == LevelState::Type::Horizontal) {
      level.camera_offset_y = 0.f;
    }

    player.position = { config::BlockSize, (level.get_size().y - 3.f) * config::BlockSize };

    level.stats.level_major = level_data.number.major;
    level.stats.level_minor = level_data.number.minor;

    level.stats.time = level.type == LevelState::Type::Horizontal ? 400 : 600;

    extra_setup(app);

    for (const auto texture_id : level_generator::allocated_textures){
      textures.push_back(level_generator::index_to_texture[texture_id]);
    }

    for (const auto& texture : level_data.extra_textures){
      textures.push_back(texture);
    }
  };

  const auto loop = [](auto& app){
    level_controller(app, app.current_level);
    render_level(app.current_level);
  };

  run_frame(app, level_data.frame, textures, setup, loop);
}
