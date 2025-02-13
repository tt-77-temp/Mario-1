#pragma once

#include "res/textures.hpp"

namespace texture_groups{
  inline const auto coin = TextureGroup({
    &textures::coin[0],
    &textures::coin[1],
    &textures::coin[2]
  });

  inline const auto king_goomba = TextureGroup({
    &textures::king_goomba_walk[0],
    &textures::king_goomba_walk[1]
  });

  inline const auto goomba = TextureGroup({
    &textures::goomba_dead,
    &textures::goomba_walk[0],
    &textures::goomba_walk[1]
  });

  inline const auto red_goomba = TextureGroup({
    &textures::red_goomba_dead,
    &textures::red_goomba_walk[0],
    &textures::red_goomba_walk[1]
  });

  inline const auto yellow_goomba = TextureGroup({
    &textures::yellow_goomba_dead,
    &textures::yellow_goomba_walk[0],
    &textures::yellow_goomba_walk[1]
  });

  inline const auto green_koopa = TextureGroup({
    &textures::green_koopa_dead,
    &textures::green_koopa_walk[0],
    &textures::green_koopa_walk[1]
  });

  inline const auto purple_koopa = TextureGroup({
    &textures::purple_koopa_dead,
    &textures::purple_koopa_walk[0],
    &textures::purple_koopa_walk[1]
  });

  inline const auto purple_flying_koopa = purple_koopa.with({
    &textures::purple_flying_koopa_walk[0],
    &textures::purple_flying_koopa_walk[1]
  });

  inline const auto green_flying_koopa = green_koopa.with({
    &textures::green_flying_koopa_walk[0],
    &textures::green_flying_koopa_walk[1]
  });

  inline const auto red_koopa = TextureGroup({
    &textures::red_koopa_dead,
    &textures::red_koopa_walk[0],
    &textures::red_koopa_walk[1]
  });

  inline const auto red_flying_koopa = red_koopa.with({
    &textures::red_flying_koopa_walk[0],
    &textures::red_flying_koopa_walk[1]
  });

  inline const auto beetle = TextureGroup({
    &textures::beetle_dead,
    &textures::beetle_walk[0],
    &textures::beetle_walk[1]
  });

  inline const auto spike = TextureGroup({
    &textures::spike_walk[0],
    &textures::spike_walk[1]
  });

  inline const auto plant = TextureGroup({
    &textures::plant[0],
    &textures::plant[1]
  });

  inline const auto red_plant = TextureGroup({
    &textures::red_plant[0],
    &textures::red_plant[1]
  });

  inline const auto q_block = TextureGroup({
    &textures::q_block[0],
    &textures::q_block[1],
    &textures::q_block[2],
    &textures::null_block
  });

  inline const auto mushroom = q_block.with({
    &textures::mushroom
  });

  inline const auto green_mushroom = q_block.with({
    &textures::green_mushroom
  });

  inline const auto spinning_coin = q_block.with({
    &textures::spinning_coin[0],
    &textures::spinning_coin[1],
    &textures::spinning_coin[2],
    &textures::spinning_coin[3]
  });

  inline const auto fire_flower = q_block.with({
    &textures::fire_flower[0],
    &textures::fire_flower[1],
    &textures::fire_flower[2],
    &textures::fire_flower[3]
  });

  inline const auto bricks = TextureGroup({
    &textures::bricks,
    &textures::bricks_particles_normal
  });

  inline const auto mario = TextureGroup({
    &textures::small_mario_walk[0],
    &textures::small_mario_walk[1],
    &textures::small_mario_walk[2],
    &textures::small_mario,
    &textures::small_mario_dead,
    &textures::small_mario_jumping,
    &textures::small_mario_turning,
    &textures::big_mario_walk[0],
    &textures::big_mario_walk[1],
    &textures::big_mario_walk[2],
    &textures::big_mario,
    &textures::big_mario_jumping,
    &textures::big_mario_turning,
    &textures::big_mario_squating,
    &textures::fire_big_mario_walk[0],
    &textures::fire_big_mario_walk[1],
    &textures::fire_big_mario_walk[2],
    &textures::fire_big_mario,
    &textures::fire_big_mario_jumping,
    &textures::fire_big_mario_turning,
    &textures::fire_big_mario_squating,
    &textures::black_big_mario_walk[0],
    &textures::black_big_mario_walk[1],
    &textures::black_big_mario_walk[2],
    &textures::black_big_mario,
    &textures::black_big_mario_jumping,
    &textures::black_big_mario_turning,
    &textures::black_big_mario_squating,
    &textures::medium_mario,
    &textures::fireball[0],
    &textures::fireball[1],
    &textures::fireball[2],
    &textures::fireball[3],
    &textures::explosion[0],
    &textures::explosion[1],
    &textures::explosion[2]
  });

  inline const auto hammerbro = TextureGroup({
    &textures::hammerbro_walk[0],
    &textures::hammerbro_walk[1],
    &textures::hammerbro_walk_with_hammer[0],
    &textures::hammerbro_walk_with_hammer[1],
    &textures::hammer[0],
    &textures::hammer[1],
    &textures::hammer[2],
    &textures::hammer[3]
  });

  inline const auto red_cloud = TextureGroup({
    &textures::red_cloud_top_left,
    &textures::red_cloud_top_right,
    &textures::red_cloud_top_center,
    &textures::red_cloud_bottom_left,
    &textures::red_cloud_bottom_right,
    &textures::red_cloud_bottom_center
  });

  inline const auto blue_cloud = TextureGroup({
    &textures::blue_cloud_top_left,
    &textures::blue_cloud_top_right,
    &textures::blue_cloud_top_center,
    &textures::blue_cloud_bottom_left,
    &textures::blue_cloud_bottom_right,
    &textures::blue_cloud_bottom_center
  });

  inline const auto red_bush = TextureGroup({
    &textures::red_bush_left,
    &textures::red_bush_right,
    &textures::red_bush_center
  });

  inline const auto red_hill = TextureGroup({
    &textures::red_hill_top,
    &textures::red_hill_left,
    &textures::red_hill_right,
    &textures::red_hill_center,
    &textures::red_hill_center_dot
  });

  inline const auto red_pipe = TextureGroup({
    &textures::red_pipe_top_left,
    &textures::red_pipe_top_right,
    &textures::red_pipe_bottom_left,
    &textures::red_pipe_bottom_right
  });
}
