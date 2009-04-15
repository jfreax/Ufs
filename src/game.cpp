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
#include <SFML/Graphics.hpp>

#include "util.hpp"
#include "settings.hpp"
#include "resource.hpp"
#include "game.hpp"

#include "gui/window.hpp"
#include "gui/manager.hpp"
#include "gui/button.hpp"


CGame* game = NULL;

CGame* getGameClass()
{
  return game;
}


CGame::CGame ( int argc, char **argv ) :
    argc_ ( argc ),
    arg_ ( 1 ),
    argv_ ( argv ),
    run ( true ),
    App ( sf::VideoMode::GetDesktopMode(), "Game" , sf::Style::Close )
{
  game = this;

  // Standardwerte
  settings::setWidth ( sf::VideoMode::GetDesktopMode().Width );
  settings::setHeight ( sf::VideoMode::GetDesktopMode().Height );
  settings::setBpp ( sf::VideoMode::GetDesktopMode().BitsPerPixel );

  // Programmargumente
  for ( arg_ = 1; arg_ != argc_; ++arg_ ) {
    const std::string val ( argv_[arg_] );

    if ( val.empty() ) {
      continue;
    } else if ( val == "-fps" | val == "--fps" ) {
      settings::setShowFps ( true );
    } else if ( val == "-fs" || val == "--fullscreen" ) {
      settings::setFullscreen ( true );
    } else if ( val == "-h" || val == "--height" || val == "-y" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setHeight ( lexical_cast_default<int> ( argv_[arg_], -1 ) );
      }
    } else if ( val == "-w" || val == "--width" || val == "-x" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setWidth ( lexical_cast_default<int> ( argv_[arg_], -1 ) );
      }
    } else if ( val == "-bpp" || val == "--bitsperpixel" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setBpp ( lexical_cast_default<int> ( argv_[arg_], -1 ) );
      }
    } else if ( val == "--theme" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setTheme ( argv_[arg_] );
      }
    } else if ( val == "--path" || val == "--data" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setPath ( argv_[arg_] );
      }
    } else if ( val[0] == '-' || val[0] == '-' && val[1] == '-' ) {
      std::cerr << "unknown option: " << val << std::endl;
      // throw config::error( "unknown option" );
    }
  }
}


bool CGame::initialize()
{
  if ( !settings::getVideo().IsValid() ) {
    std::cerr
      << "VideoMode ("
      << settings::getVideo().Width
      << "x"
      << settings::getVideo().Height
      << "x"
      << settings::getVideo().BitsPerPixel
      << ") is not supported!"
      << std::endl;
    return false;
  } else {
    App.Create ( settings::getVideo(), "Game" , settings::getStyle(), settings::getWindowSettings() );
  }

  App.UseVerticalSync ( true );
  App.EnableKeyRepeat ( true );

  fpsStr.SetSize ( 12 );

  return true;
}


bool CGame::IsVideoModeValid() // TODO in CGame::initialize und settings::setHeight() !
{


}


void CGame::start()
{
  gui::CWindow* newWin = guiManager.newWindow();
// guiManager.newWindow();/**/
// guiManager.newWindow();
// guiManager.newWindow();/**/
guiManager.newWindow();
guiManager.newWindow();
  newWin->setSizeInPercent ( sf::Vector2f ( 90, 90 ) );
  newWin->setTitlebar ( 10 );
  newWin->setPosition ( sf::Vector2f ( 20, 100 ) );

  gui::CWindow* newWin2 = guiManager.newWindow();
  newWin2->setSizeInPercent ( sf::Vector2f ( 20, 20 ) );
  newWin2->setTitlebar ( 10 );
  newWin2->setPosition ( sf::Vector2f ( 0, 50 ) );

  gui::CButton* newButton = new gui::CButton ( newWin );


  while ( run ) {
    input.events();

    this->draw();


  }
}


void CGame::stop()
{
  run = false;
}


void CGame::draw()
{
  App.Clear();

  // Gamegraphic
  App.SetView ( viewPoint );

  // GUI
  App.SetView ( App.GetDefaultView() );
  guiManager.draw ( );

  if ( settings::showFps() )
    this->calcFPS();

  App.Display();
}


sf::RenderWindow* CGame::getApp ()
{
  return &App;
}


gui::CManager* CGame::getGuiManager ()
{
  return &guiManager;
}


CResource< sf::Image >* CGame::getImgResource()
{
  return &imgResource;
}


void CGame::calcFPS()
{
  static int frame = -1;
  static unsigned int fps = 0;

  ++frame;

  if ( frame >= fps ) {

    fpsStr.SetText ( "FPS: " + lexical_cast_default<std::string> ( fps ) );
    fps = 1.f / App.GetFrameTime();
    frame = 0;
  }

  App.Draw ( fpsStr );
}


sf::String* CGame::getFpsStr()
{
  return &fpsStr;
}


