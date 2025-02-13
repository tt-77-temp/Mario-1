#pragma once

#include "Renderer/Drawable.hpp"
#include "Renderer/Font.hpp"

#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include <vector>

class Text :public Drawable{
private:
  struct Glyph : Drawable{
    glm::vec4 color;
    glm::vec2 bearing;
    int advance;
  };

  glm::vec4 color;
  Font* font;

  std::vector<Glyph> glyphs;
  std::string text;
  
  float largest_glyph_bearing_y = 0.f;
  float font_size = 0.f;

  auto set_position_for_every_glyph_(const glm::vec2& position) -> void;
  auto get_text_width_() -> float;
  auto create_glyph_from_char_(char character) const -> Glyph;
  auto setup_text_height_() -> void;

public:
  float font_scale = 0.f;

  Text();
  Text(Font* font, const std::string& label, float scale = 1.f);

  auto set_position(const glm::vec2& position) -> void;
  auto set_text(const std::string& text) -> void;
  auto set_color(const glm::vec4& color) -> void;
  auto set_font(Font* font) -> void;

  auto refresh() -> void;

  auto get_color() const -> const glm::vec4&;
  auto get_size() const -> const glm::vec2&;
  auto get_position() const -> const glm::vec2;
  auto get_text() const -> const std::string&;
  auto get_font() const -> Font*;

  auto get_glyph(int index) const -> const Glyph&;
  auto get_length() const -> int;

  auto get_all_lines_height() const -> float;
};
