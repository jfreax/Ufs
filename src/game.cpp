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
#include "animation.hpp"
#include "game.hpp"

#include "gui/window.hpp"
#include "gui/manager.hpp"
#include "gui/button.hpp"


CGame* game = NULL;

CGame* GetGameClass ( void )
{
  return game;
}


CGame::CGame ( int argc, char **argv ) :
    argc_ ( argc ),
    arg_ ( 1 ),
    argv_ ( argv ),
    run ( true ),
    app ( sf::VideoMode::GetDesktopMode(), "Game" , sf::Style::Close )
{
  game = this;

  // Standardwerte
  settings::SetWidth ( sf::VideoMode::GetDesktopMode().Width );
  settings::SetHeight ( sf::VideoMode::GetDesktopMode().Height );
  settings::SetBpp ( sf::VideoMode::GetDesktopMode().BitsPerPixel );

  // Programmargumente

  for ( arg_ = 1; arg_ != argc_; ++arg_ )
  {
    const std::string val ( argv_[arg_] );

    if ( val.empty() )
    {
      continue;
    }
    else if ( val == "-fps" | val == "--fps" )
    {
      settings::SetShowFps ( true );
    }
    else if ( val == "-fs" || val == "--fullscreen" )
    {
      settings::SetFullscreen ( true );
    }
    else if ( val == "-h" || val == "--height" || val == "-y" )
    {
      if ( arg_ + 1 != argc_ )
      {
        ++arg_;
        settings::SetHeight ( lexical_cast_default<int> ( argv_[arg_], -1 ) );
      }
    }
    else if ( val == "-w" || val == "--width" || val == "-x" )
    {
      if ( arg_ + 1 != argc_ )
      {
        ++arg_;
        settings::SetWidth ( lexical_cast_default<int> ( argv_[arg_], -1 ) );
      }
    }
    else if ( val == "-bpp" || val == "--bitsperpixel" )
    {
      if ( arg_ + 1 != argc_ )
      {
        ++arg_;
        settings::SetBpp ( lexical_cast_default<int> ( argv_[arg_], -1 ) );
      }
    }
    else if ( val == "--theme" )
    {
      if ( arg_ + 1 != argc_ )
      {
        ++arg_;
        settings::SetTheme ( argv_[arg_] );
      }
    }
    else if ( val == "--path" || val == "--data" )
    {
      if ( arg_ + 1 != argc_ )
      {
        ++arg_;
        settings::SetPath ( argv_[arg_] );
      }
    }
    else if ( val[0] == '-' || val[0] == '-' && val[1] == '-' )
    {
      std::cerr << "unknown option: " << val << std::endl;
      // throw config::error( "unknown option" );
    }
  }
}


bool CGame::Initialize ( void )
{
  if ( !settings::GetVideo().IsValid() )
  {
    std::cerr
      << "VideoMode ("
      << settings::GetVideo().Width
      << "x"
      << settings::GetVideo().Height
      << "x"
      << settings::GetVideo().BitsPerPixel
      << ") is not supported!"
      << std::endl;
    return false;
  }
  else
  {
    app.Create ( settings::GetVideo(), "Game" , settings::GetStyle(), settings::GetWindowSettings() );
  }

  app.UseVerticalSync ( true );

  app.EnableKeyRepeat ( true );

  fpsStr.SetSize ( 12 );

  app.ShowMouseCursor ( false );
  cursor[NONE] = new CAnimation ( GetImgResource()->Get ( settings::GetThemePath() + "arrow.png" ) , 36, 0.1f );
  cursor[NONE]->Scale ( 0.6, 0.6 );
  cursor[WINDOW] = new CAnimation ( GetImgResource()->Get ( settings::GetThemePath() + "hand.png" ) , 41, 0.08f );
  cursor[WINDOW]->Scale ( 0.6, 0.6 );
  cursor[WINDOW]->SetStartAt ( 6 );
//   cursor = cursors[0];
//   cursor->Scale( 0.5f, 0.5f );

  return true;
}


bool CGame::IsVideoModeValid ( void ) // TODO in CGame::initialize und settings::setHeight() !
{


}


void CGame::Start ( void )
{
  gui::CWindow* newWin = guiManager.NewWindow();
  guiManager.NewWindow();/**/
// guiManager.newWindow();
// guiManager.newWindow();/**/
// guiManager.newWindow();
// guiManager.newWindow();
  newWin->SetSizeInPercent ( sf::Vector2f ( 90, 90 ) );
  newWin->SetPosition ( sf::Vector2f ( 20, 100 ) );
//
//   gui::CWindow* newWin2 = guiManager.newWindow();
//   newWin2->setSizeInPercent ( sf::Vector2f ( 20, 20 ) );
//   newWin2->setTitlebar ( 10 );
//   newWin2->setPosition ( sf::Vector2f ( 0, 50 ) );

  gui::CButton* newButton = new gui::CButton ( newWin );
  newButton->SetName ( "TEST" );
  newButton->SetSize ( sf::Vector2f ( 80, 20 ) );



  while ( run )
  {
    input.Events();

    this->Render();
  }
}


void CGame::Stop ( void )
{
  run = false;
}


void CGame::Render ( void )
{
  // Bildschirm säubern
  app.Clear();

  // Gamegraphic
  app.SetView ( viewPoint );

  // GUI
  app.SetView ( app.GetDefaultView() );
  guiManager.Render();

  if ( settings::GetShowFps() )
  {
    this->CalcFPS();
  }

  // Mousecursor
  cursor[settings::GetMouseScope() ]->SetPosition ( app.GetInput().GetMouseX(), app.GetInput().GetMouseY() );
  cursor[settings::GetMouseScope() ]->Update();
  app.Draw ( *cursor[settings::GetMouseScope() ] );

  app.Display();
}


sf::RenderWindow* CGame::GetApp ( void )
{
  return &app;
}


gui::CManager* CGame::GetGuiManager ( void )
{
  return &guiManager;
}


CResource< sf::Image >* CGame::GetImgResource ( void )
{
  return &imgResource;
}


void CGame::CalcFPS ( void )
{
  static int frame = -1;
  static unsigned int fps = 0;

  ++frame;

  if ( frame >= fps )
  {

    fpsStr.SetText ( "FPS: " + lexical_cast_default<std::string> ( fps ) );
    fps = 1.f / app.GetFrameTime();
    frame = 0;
  }

  app.Draw ( fpsStr );
}


std::map< MOUSESCOPE, CAnimation*> CGame::GetCursor ( void )
{
  return cursor;
}


sf::String* CGame::GetFpsStr ( void )
{
  return &fpsStr;
}


