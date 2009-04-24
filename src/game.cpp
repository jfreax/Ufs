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

#include "action.hpp"
#include "util.hpp"
#include "settings.hpp"
#include "resource.hpp"
#include "animation.hpp"
#include "game.hpp"

#include "ui/window.hpp"
#include "ui/manager.hpp"
#include "ui/button.hpp"

#include "gui/button/close.hpp"


CGame* game = NULL;

CGame* GetGameClass ( void )
{
	return game;
}


CGame::CGame ( int argc, char **argv ) :
		argc_ ( argc ),
		arg_ ( 1 ),
		argv_ ( argv ),
		run_ ( true ),
		app_ ( sf::VideoMode::GetDesktopMode(), "Game" , sf::Style::Close )
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
				settings::SetHeight ( util::lCast<int> ( argv_[arg_], -1 ) );
			}
		}
		else if ( val == "-w" || val == "--width" || val == "-x" )
		{
			if ( arg_ + 1 != argc_ )
			{
				++arg_;
				settings::SetWidth ( util::lCast<int> ( argv_[arg_], -1 ) );
			}
		}
		else if ( val == "-bpp" || val == "--bitsperpixel" )
		{
			if ( arg_ + 1 != argc_ )
			{
				++arg_;
				settings::SetBpp ( util::lCast<int> ( argv_[arg_], -1 ) );
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

CGame::~CGame ( void )
{
	// TODO !!!
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
		app_.Create ( settings::GetVideo(), "Game" , settings::GetStyle(), settings::GetWindowSettings() );
	}

	app_.UseVerticalSync ( true );
	app_.EnableKeyRepeat ( true );
	app_.ShowMouseCursor ( false );

	fpsStr_.SetSize ( 12 );

	// TODO nur Beispiele!
	cursor_[NONE] = new CAnimation ( GetImgResource()->Get ( settings::GetThemePath() + "arrow.png" ) , 36, 0.1f );
	cursor_[NONE]->Scale ( 0.6, 0.6 );
	cursor_[WINDOW] = new CAnimation ( GetImgResource()->Get ( settings::GetThemePath() + "hand.png" ) , 41, 0.08f );
	cursor_[WINDOW]->Scale ( 0.6, 0.6 );
	cursor_[WINDOW]->SetStartAt ( 6 );

	return true;
}


bool CGame::IsVideoModeValid ( void ) /* TODO in CGame::initialize und settings::setHeight() ! */
{


}


bool CGame::Start ( void )
{
	// FIXME Nur BEISPIELE!
	gui::CWindow* newWin = guiManager_.NewWindow();
	guiManager_.NewWindow();
	guiManager_.NewWindow();

	newWin->SetSizeInPercent ( sf::Vector2f ( 90, 90 ) );
	newWin->SetPosition ( sf::Vector2f ( 20, 130 ) );
	
	
//
//   gui::CWindow* newWin2 = guiManager.newWindow();
//   newWin2->setSizeInPercent ( sf::Vector2f ( 20, 20 ) );
//   newWin2->setTitlebar ( 10 );
//   newWin2->setPosition ( sf::Vector2f ( 0, 50 ) );

// 	gui::CButton* newButton = new gui::CButton ( newWin );
// 	newButton->SetName ( "TEST" );
// 	newButton->SetSize ( sf::Vector2f ( 80, 25 ) );
// 	newButton->SetPosition ( sf::Vector2f ( 30, 20 ) );
// 	newButton->SetBackgroundColor ( sf::Color ( 0, 0, 0, 0 ) );
// 
// 	util::DataHolder* buttonSettings = newButton->AddMouseEvent ( "fadeInking", MOUSE::HOVER, 0.01f );
// 	buttonSettings->sprite_one = newButton->GetBackground();
// 	buttonSettings->color_one = sf::Color ( 20, 1, 1, 20 );
// 	buttonSettings->color_two = sf::Color ( 10, 0, 0, 10 );
// 	buttonSettings->b = false;
// 
// 	util::DataHolder* buttonSettings2 = newButton->AddMouseEvent ( "fake", MOUSE::UNHOVER, 0.01f );
// 	buttonSettings2->sprite_one = newButton->GetBackground();
// 	buttonSettings2->color_one = sf::Color ( 245, 255, 255, 245 );
// 	buttonSettings2->color_two = sf::Color ( 10, 0, 0, 10 );
// 	buttonSettings2->b = true;

	while ( run_ )
	{
		input_.Events();

		this->Render();
	}

	return true;
}


bool CGame::Stop ( void )
{
	run_ = false;
	return true;
}


void CGame::Render ( void )
{
	// Bildschirm säubern
	app_.Clear();

	// Gamegraphic
	app_.SetView ( viewPoint_ );

	// GUI
	app_.SetView ( app_.GetDefaultView() );
	guiManager_.Render();

	if ( settings::GetShowFps() )
	{
		this->CalcFPS();
	}

	// Mousecursor
	cursor_[settings::GetMouseScope() ]->SetPosition ( app_.GetInput().GetMouseX(), app_.GetInput().GetMouseY() );
	cursor_[settings::GetMouseScope() ]->Update();
	app_.Draw ( *cursor_[settings::GetMouseScope() ] );

	app_.Display();
}


sf::RenderWindow* CGame::GetApp ( void )
{
	return &app_;
}


gui::CManager* CGame::GetGuiManager ( void )
{
	return &guiManager_;
}


CResource< sf::Image >* CGame::GetImgResource ( void )
{
	return &imgResource_;
}


void CGame::CalcFPS ( void )
{
	static int frame = -1;
	static unsigned int fps = 0;

	++frame;

	if ( frame >= fps )
	{

		fpsStr_.SetText ( "FPS: " + util::lCast<std::string> ( fps ) );
		fps = 1.f / app_.GetFrameTime();
		frame = 0;
	}

	app_.Draw ( fpsStr_ );
}


std::map< MOUSESCOPE, CAnimation*> CGame::GetCursor ( void )
{
	return cursor_;
}


sf::String* CGame::GetFpsStr ( void )
{
	return &fpsStr_;
}


