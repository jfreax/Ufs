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
#include "util/util.hpp"
#include "settings/settings.hpp"
#include "resource.hpp"
#include "animation.hpp"
#include "game.hpp"

#include "engine/ui/window.hpp"
#include "engine/ui/manager.hpp"
#include "engine/ui/button.hpp"

#include "gui/button/titlebar.hpp"
#include "gui/window/start.hpp"
#include "gui/window/header_menu.hpp"
// #include "particle.hpp"


CGame* game = NULL;

CGame* GetGameClass()
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

        /* Standardwerte */
	settings::SetWidth ( sf::VideoMode::GetDesktopMode().Width );
	settings::SetHeight ( sf::VideoMode::GetDesktopMode().Height );
	settings::SetBpp ( sf::VideoMode::GetDesktopMode().BitsPerPixel );
        settings::SetPath ( "./data/" );

        /* Arguments */
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
			if ( arg_ + 1 != argc )
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

CGame::~CGame()
{
	// TODO !!!
}



bool CGame::Initialize()
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


bool CGame::IsVideoModeValid() /* TODO in CGame::initialize und settings::setHeight() ! */
{


}


bool CGame::Start()
{
	// FIXME Nur BEISPIELE!
//  gui::CWindow* newWin = guiManager_.NewWindow();
//  guiManager_.NewWindow();
//  guiManager_.NewWindow();
//
//  newWin->SetSizeInPercent ( sf::Vector2f ( 90, 90 ) );
//  newWin->SetPosition ( sf::Vector2f ( 20, 130 ) );


//
// //  win->SetPosition( sf::Vector2f ( 0, 0 ));
// //  win->SetSize ( sf::Vector2f ( 200, 200 ));
// //





//  gui::CWindow* win3 = guiManager_.AddWindow ( new gui::CWindow );
//
//  win3->SetPosition( sf::Vector2f ( 250, 100 ));
//  win3->SetSize ( sf::Vector2f ( 300, 200 ));
//
//  gui::CWindow* win4 = guiManager_.AddWindow ( new gui::CWindow );
//
//  win4->SetPosition( sf::Vector2f ( 150, 300 ));
//  win4->SetSize ( sf::Vector2f ( 200, 200 ));
//
//   gui::CWindow* newWin2 = guiManager.newWindow();
//   newWin2->setSizeInPercent ( sf::Vector2f ( 20, 20 ) );
//   newWin2->setTitlebar ( 10 );
//   newWin2->setPosition ( sf::Vector2f ( 0, 50 ) );

//  gui::CButton* newButton = new gui::CButton ( newWin );
//  newButton->SetName ( "TEST" );
//  newButton->SetSize ( sf::Vector2f ( 80, 25 ) );
//  newButton->SetPosition ( sf::Vector2f ( 30, 20 ) );
//  newButton->SetBackgroundColor ( sf::Color ( 0, 0, 0, 0 ) );
//
//  util::DataHolder* buttonSettings = newButton->AddMouseEvent ( "fadeInking", MOUSE::HOVER, 0.01f );
//  buttonSettings->sprite_one = newButton->GetBackground();
//  buttonSettings->color_one = sf::Color ( 20, 1, 1, 20 );
//  buttonSettings->color_two = sf::Color ( 10, 0, 0, 10 );
//  buttonSettings->b = false;
//
//  util::DataHolder* buttonSettings2 = newButton->AddMouseEvent ( "fake", MOUSE::UNHOVER, 0.01f );
//  buttonSettings2->sprite_one = newButton->GetBackground();
//  buttonSettings2->color_one = sf::Color ( 245, 255, 255, 245 );
//  buttonSettings2->color_two = sf::Color ( 10, 0, 0, 10 );
//  buttonSettings2->b = true;

// 	testAnim = new CAnimation ( GetImgResource()->Get ( "images/sun/sun.png" ), 250, 0.1f, true );
// 	testAnim->SetFrameWidth ( 250 );
// 	testAnim->SetScale ( 0.3f, 0.3f );
// 
// 	testAnim2 = new CAnimation ( GetImgResource()->Get ( "images/sun/sun.png" ), 250, 0.39f, true );
// 	testAnim2->SetFrameWidth ( 250 );
// 	testAnim2->SetScale ( 0.3f, 0.3f );
// 	testAnim2->SetFrameDiff ( -1 );
// testAnim->SetPosition(100, 200);


	/* Tastatureinstellungen laden */
	input_.LoadKeys( "config/keyboard.ini" );
	
	/* Gui-Manager laden */
	guiManager_.Initialize();

	/* Leere Karte initialisieren */
	mapManager_.Initialize();
	
	
	 gui::CWindow* win2 = guiManager_.AddWindow ( new gui::CWindow );
	 gui::CWindow* win = guiManager_.AddWindow ( new gui::CHeaderWindow );
// 	 gui::CWindow* win = guiManager_.AddWindow ( new gui::CStartWindow );
//
 win2->SetPosition( sf::Vector2f ( 100, 100 ));
 win2->SetSize ( sf::Vector2f ( 200, 200 ));
//


	/* Spielschleife */
	while ( run_ )
	{
		/* Tasten- und Mauseingaben bearbeiten */
		input_.Events();
		
		/* Berechnenung des Spielablaufs */
		

		/* Zeichnen der GUI und Spielinhalte */
		this->Render();
	}

	return true;
}


bool CGame::Stop()
{
	run_ = false;
	return true;
}


void CGame::Render()
{
	// Bildschirm säubern
	app_.Clear();

	// Gamegraphic
	app_.SetView ( viewPoint_[0] );
	mapManager_.Update();
	mapManager_.Render();

	// GUI
	app_.SetView ( app_.GetDefaultView() );
	guiManager_.Render();

// 	sf::Shape testUmrandung;
// 	testUmrandung.AddPoint( sf::Vector2f ( 100, 100 ), sf::Color ( 150, 150, 150, 160 ) );
// 	testUmrandung.AddPoint( sf::Vector2f ( 280, 100 ), sf::Color ( 150, 150, 150, 160 ) );
// 	testUmrandung.AddPoint( sf::Vector2f ( 300, 120 ), sf::Color ( 150, 150, 150, 160 ) );
// 	testUmrandung.AddPoint( sf::Vector2f ( 300, 300 ), sf::Color ( 150, 150, 150, 160 ) );
// 	testUmrandung.AddPoint( sf::Vector2f ( 120, 300 ), sf::Color ( 150, 150, 150, 160 ) );
// 	testUmrandung.AddPoint( sf::Vector2f ( 100, 280 ), sf::Color ( 150, 150, 150, 160 ) );
// 	app_.Draw( testUmrandung );
// 
// 	sf::Shape test;
// 	test.AddPoint( sf::Vector2f ( 105, 105 ), sf::Color::Black );
// 	test.AddPoint( sf::Vector2f ( 280, 105 ), sf::Color::Black );
// 	test.AddPoint( sf::Vector2f ( 295, 120 ), sf::Color::Black );
// 	test.AddPoint( sf::Vector2f ( 295, 280 ), sf::Color ( 0, 0, 0, 220 ) );
// 	test.AddPoint( sf::Vector2f ( 295, 295 ), sf::Color ( 0, 0, 0, 220 ) );
// 	test.AddPoint( sf::Vector2f ( 120, 295 ), sf::Color ( 0, 0, 0, 220 ) );
// 	test.AddPoint( sf::Vector2f ( 105, 280 ), sf::Color ( 0, 0, 0, 220 ) );
// 	app_.Draw( test );

	
// 	CParticleManager particle;
// 	particle.set_Material ( GetImgResource()->Get ( "images/sun/fire.png"  ) );
// // 	particle.set_Dimension ( sf::Vector2i ( 500, 500 ) );
// 	CEmitter nEmit ( 500, 1090.2f, sf::Vector3f ( 100, 100, 0.f ) );
// 	nEmit.set_Position( sf::Vector3f ( 50,50,1) );
// 	nEmit.set_Velocity ( 20.f, 22.f );
// 	nEmit.set_Direction ( 90, 50.f );
// 	particle.AddEmitter ( nEmit );
// 	particle.Update();
// 	particle.Draw();

	/* Ggf. FPS Anzeigen */
	this->CalcFPS();

	/* Mousecursor TODO extra klasse oder funktion zur verwaltung! */
	cursor_[settings::GetMouseScope() ]->SetPosition ( app_.GetInput().GetMouseX(), app_.GetInput().GetMouseY() );
	cursor_[settings::GetMouseScope() ]->Update();
	app_.Draw ( *cursor_[settings::GetMouseScope() ] );

	/* Zeichnen! */
	app_.Display();
}


sf::RenderWindow* CGame::GetApp()
{
	return &app_;
}


gui::CManager* CGame::GetGuiManager()
{
	return &guiManager_;
}


CMapManager* CGame::GetMapManager()
{
	return &mapManager_;
}


CImageResource* CGame::GetImgResource()
{
	return &imgResource_;
}


CFontResource* CGame::GetFontResource()
{
	return &fontResource_;
}


void CGame::CalcFPS()
{
	static int frame = -1;
	static unsigned int fps = 0;
	
	if ( !settings::GetShowFps() )
	{
		return;
	}

	++frame;

	if ( frame >= fps )
	{

		fpsStr_.SetText ( "FPS: " + util::lCast<std::string> ( fps ) );
		fps = 1.f / app_.GetFrameTime();
		frame = 0;
	}

	app_.Draw ( fpsStr_ );
}


std::map< MOUSESCOPE, CAnimation*> CGame::GetCursor()
{
	return cursor_;
}


sf::String* CGame::GetFpsStr()
{
	return &fpsStr_;
}


sf::View* CGame::GetViewPoint ( int i )
{
	return &viewPoint_[i];
}