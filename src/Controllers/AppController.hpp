#pragma once

#include "States/AppState.hpp"
#include "Controllers/LevelController.hpp"
#include "Frames/Level11.hpp"
#include "Frames/Level12.hpp"
#include "Frames/Level13.hpp"
#include "Frames/Level14.hpp"
#include "Frames/Level15.hpp"

#include "Util/Util.hpp"
#include "config.hpp"

static auto app_update_level(AppState& app){
  const auto stats = app.current_level.stats;
  const auto player_form = app.current_level.player.form;
  const auto player_growth = app.current_level.player.growth;
  const auto current_checkpoint = app.current_level.current_checkpoint;

  app.current_level = LevelState{};

  app.current_level.stats = stats;
  app.current_level.player.form = player_form;
  app.current_level.player.growth = player_growth;
  app.current_level.current_checkpoint = current_checkpoint;
}

static auto app_controller(AppState& app){
  auto& frame = app.current_frame;

  if (frame == AppState::Frame::Level11){
    run_frame_level11(app);
    app_update_level(app);
  }

  if (frame == AppState::Frame::Level12){
    run_frame_level12(app);
    app_update_level(app);
  }

  if (frame == AppState::Frame::Level13){
    run_frame_level13(app);
    app_update_level(app);
  }

  if (frame == AppState::Frame::Level14){
    run_frame_level14(app);
    app_update_level(app);
  }

  if (frame == AppState::Frame::Level15){
    run_frame_level15(app);
    app_update_level(app);
  }
}
