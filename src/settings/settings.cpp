/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
// #include <SFML/Window.hpp>
// #include <SFML/Graphics.hpp>

#include "config.hpp"

#include "../game.hpp"
#include "../util/util.hpp"
#include "settings.hpp"

namespace
{
Config config;

} // namespace


namespace settings
{

sf::VideoMode GetVideo ( void )
{
  return config.video;
}


sf::WindowSettings GetWindowSettings ( void )
{
  return config.windowSettings;
}


int GetStyle ( void )
{
  int style = sf::Style::Close;

  if ( GetFullscreen() )
    style |= sf::Style::Fullscreen;

  return style;
}


std::string GetPath ( void )
{
  return config.path;
}


std::string GetThemePath ( void )
{
  return "/themes/" + config.theme + "/";
}


void SetPath ( std::string path_ )
{
  config.path = path_;
}


bool GetFullscreen ( void )
{
  return config.fullscreen;
}


void SetFullscreen ( bool ison )
{
  config.fullscreen = ison;
  GetGameClass()->Initialize();
}


void ToggleFullscreen ( void )
{
  config.fullscreen = !config.fullscreen;
  GetGameClass()->Initialize();
}


bool GetShowFps ( void )
{
  return config.fps;
}


void SetShowFps ( bool ison )
{
  config.fps = ison;
}


int GetHeight ( void )
{
  return config.video.Height;
}


void SetHeight ( int height_ )
{
  config.video.Height = height_;
}


int GetWidth ( void )
{
  return config.video.Width;
}


void SetWidth ( int width_ )
{
  config.video.Width = width_;
  GetGameClass()->GetFpsStr()->SetPosition ( settings::GetWidth() - 70, 10 );
}


int GetBpp ( void )
{
  return config.video.BitsPerPixel;
}


void SetBpp ( int bpp_ )
{
  config.video.BitsPerPixel = bpp_;
}


std::string GetTheme ( void )
{
  return config.theme;
}


void SetTheme ( std::string theme_ )
{
  if ( util::isFile ( "themes/" + theme_ + ".ini" ) )
    config.theme = theme_;
  else
    std::cerr << "Theme-File not found: " << theme_ << std::endl;
}


MOUSESCOPE GetMouseScope ( void )
{
  return config.mousescope;
}


void SetMouseScope ( MOUSESCOPE mousescope_ )
{
  if ( config.mousescope != mousescope_ )
  {
    GetGameClass()->GetCursor() [mousescope_]->Start();
    config.mousescope = mousescope_;
  }
}



} // namespace settings
