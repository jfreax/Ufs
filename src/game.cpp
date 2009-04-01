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
#include "game.hpp"

#include "gui/window.hpp"
#include "gui/manager.hpp"


CGame::CGame( int argc, char **argv ) :
    argc_( argc ),
    arg_( 1 ),
    argv_( argv ),
    run( true )
{
  for ( arg_ = 1; arg_ != argc_; ++arg_ ) {
    const std::string val( argv_[arg_] );

    if ( val.empty() ) {
      continue;
    } else if ( val == "-fps" | val == "--fps" ) {
      settings::setShowFps( true );
    } else if ( val == "-fs" || val == "--fullscreen" ) {
      settings::setFullscreen( true );
    } else if ( val == "-h" || val == "--height" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setHeight( lexical_cast_default<int>( argv_[arg_], -1 ) );
      }
    } else if ( val == "-w" || val == "--width" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setWidth( lexical_cast_default<int>( argv_[arg_], -1 ) );
      }
    } else if ( val == "-bpp" || val == "--bitsperpixel" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setBpp( lexical_cast_default<int>( argv_[arg_], -1 ) );
      }
    } else if ( val == "--theme" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setTheme( argv_[arg_] );
      }
    } else if ( val == "--path" ) {
      if ( arg_ + 1 != argc_ ) {
        ++arg_;
        settings::setPath( argv_[arg_] );
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
  }

  App.Create( settings::getVideo(), "StarNight", sf::Style::Close, settings::getWindowSettings() );

  App.UseVerticalSync( false );
  App.EnableKeyRepeat( true );

  fpsStr.SetSize( 12 );
  fpsStr.SetPosition( settings::getWidth() - 70, 10 ); // TODO Bei Bildschirmauflösung auch ändern!

  return true;
}


void CGame::start()
{
  guiManager.newWindow();

  while ( run ) {
    input.events( this );

    this->draw();

    if ( settings::showFps() )
      this->calcFPS();

  }
}


void CGame::stop()
{
  run = false;
}


sf::RenderWindow* CGame::getApp()
{
  return &App;
}


gui::CManager*  CGame::getGuiManager()
{
  return &guiManager;
}


void CGame::draw()
{
  App.SetView( viewPoint );

  App.SetView( App.GetDefaultView() );
  guiManager.draw( this );

  App.Display();
  App.Clear();
}


void CGame::calcFPS()
{
  static int frame = -1;
  static unsigned int fps = 0;

  ++frame;

  if ( frame >= fps ) {
    fpsStr.SetText( "FPS: " + lexical_cast_default<std::string> ( fps ) );
    fps = 1.f / App.GetFrameTime();
    frame = 0;
  }

  App.Draw( fpsStr );
}



