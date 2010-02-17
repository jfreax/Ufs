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

#include "script/script.hpp"

#include "action.hpp"
#include "resource.hpp"
#include "animation.hpp"
#include "game.hpp"

#include "util/util.hpp"
#include "settings/settings.hpp"

#include "engine/ui/window.hpp"
#include "engine/ui/manager.hpp"
#include "engine/ui/button.hpp"

#include "gui/window/start.hpp"
#include "gui/window/header_menu.hpp"
#include "gui/window/load.hpp"
#include "gui/window/quit.hpp"
#include "gui/window/error.hpp"
#include "gui/window/select.hpp"
#include "gui/window/terminal.hpp"

#include "gui/button/titlebar.hpp"



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

	settings::SetLang ( "de" );
	settings::SetPath ( "./data/" );
	
	settings::SetSelect ( false );

	/* Arguments */

	for ( arg_ = 1; arg_ != argc_; ++arg_ ) {
		const std::string val ( argv_[arg_] );

		if ( val.empty() ) {
			continue;
		} else if ( val == "-fps" | val == "--fps" ) {
			settings::SetShowFps ( true );
		} else if ( val == "-fs" || val == "--fullscreen" ) {
			settings::SetFullscreen ( true );
		} else if ( val == "-h" || val == "--height" || val == "-y" ) {
			if ( arg_ + 1 != argc_ ) {
				++arg_;
				settings::SetHeight ( util::lCast<int> ( argv_[arg_], -1 ) );
			}
		} else if ( val == "-w" || val == "--width" || val == "-x" ) {
			if ( arg_ + 1 != argc_ ) {
				++arg_;
				settings::SetWidth ( util::lCast<int> ( argv_[arg_], -1 ) );
			}
		} else if ( val == "-bpp" || val == "--bitsperpixel" ) {
			if ( arg_ + 1 != argc ) {
				++arg_;
				settings::SetBpp ( util::lCast<int> ( argv_[arg_], -1 ) );
			}
		} else if ( val == "--theme" ) {
			if ( arg_ + 1 != argc_ ) {
				++arg_;
				settings::SetTheme ( argv_[arg_] );
			}
		} else if ( val == "--path" || val == "--data" ) {
			if ( arg_ + 1 != argc_ ) {
				++arg_;
				settings::SetPath ( argv_[arg_] );
			}
		} else if ( val == "--lang" || val == "-l" ) {
			if ( arg_ + 1 != argc_ ) {
				++arg_;
				settings::SetLang ( argv_[arg_] );
			}
		} else if ( val[0] == '-' || val[0] == '-' && val[1] == '-' ) {
			std::cerr << "unknown option: " << val << std::endl;
			// throw config::error( "unknown option" );
		}
	}
}

CGame::~CGame()
{
// 	lua_close ( script::GetLua() );
	
	// TODO !!!
	
}



bool CGame::Initialize()
{
	if ( !settings::GetVideo().IsValid() ) {
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
	} else {
		app_.Create ( settings::GetVideo(), "Game" , settings::GetStyle(), settings::GetWindowSettings() );
	}

	/* Set sfml properties */
	app_.UseVerticalSync ( true );
	app_.EnableKeyRepeat ( true );
	app_.ShowMouseCursor ( false );

	/* FPS font */
	fpsStr_.SetSize ( 12 );


	// TODO nur Beispiele!
	cursor_[NONE] = new CAnimation ( GetImgResource()->Get ( settings::GetThemePath() + "arrow.png" ) , 36, 0.1f );
	cursor_[NONE]->Scale ( 0.6, 0.6 );
	cursor_[WINDOW] = new CAnimation ( GetImgResource()->Get ( settings::GetThemePath() + "hand.png" ) , 41, 0.08f );
	cursor_[WINDOW]->Scale ( 0.6, 0.6 );
	cursor_[WINDOW]->SetStartAt ( 6 );

	this->SetGameType ( SINGLEPLAYER );
	
	/* Log start time */
	time_t rawtime;
	time ( &rawtime );
	log_.push_back ( ctime (&rawtime) + (std::string)"   Game started!" );

	return true;
}


bool CGame::Start()
{
	/* Load keyboardsettings */
	input_.LoadKeys( "config/keyboard.ini" );

	/* Gui-Manager laden */
	guiManager_.Initialize();
	
	/* Leere Karte initialisieren */
	mapManager_.Initialize();
	
	/* Load for errorsystem */
	blackWindow = guiManager_.AddWindow ( new gui::CWindow () );
	blackWindow->SetCloseAble ( false );
	blackWindow->SetTitlebar ( 0 );
	blackWindow->SetPosition ( sf::Vector2f ( 0, 0 ) );
	blackWindow->SetSize ( sf::Vector2f ( settings::GetWidth(), settings::GetHeight() ) );
	blackWindow->SetColor ( sf::Color ( 0, 0, 0 ) );
	blackWindow->SetShow ( false );
	
	/* Language file */
	locale_ = new CLocale ( settings::GetLang() );

	/* Load standard windows */	
	guiManager_.AddWindow ( specialWindow_["LOADING"] = new gui::CLoadWindow );
	guiManager_.AddWindow ( specialWindow_["QUIT"] = new gui::CQuitWindow );
	guiManager_.AddWindow ( specialWindow_["TERMINAL"] = new gui::CTerminalWindow );
	guiManager_.AddWindow ( new gui::CHeaderWindow );
	guiManager_.AddWindow ( new gui::CSelectWindow );

	/* Set view point */
	viewPoint_ = new sf::View( sf::FloatRect ( 0, 0, settings::GetWidth(), settings::GetHeight() ) );

	/* Initialize lua (in a new thread!) */
	sf::Thread luaThread ( &script::Initialize );
	luaThread.Launch();
	
// 	dynamic_cast< gui::CTerminalWindow* >(specialWindow_["TERMINAL"])->GetTerminalWidget()->Initialize();

	/* Start loading screen */
	this->SetGameType ( LOADING );
	
	/* Start game loop */
	while ( run_ ) {
		
		/* Keyboard and mouse input */
		input_.Events();
		
		/* Calculate error window, pause window, ... */
		this->CalcSpecialWindow();

		/* Berechnenung des Spielablaufs */
		if ( this->GetGameType() == SINGLEPLAYER || this->GetGameType() == MULTIPLAYER )
			this->Calc();

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


void CGame::Error ( std::string text, std::string function, std::string file, int line )
{
	/* Get local time */
	time_t rawtime;
	time ( &rawtime );
	
	/* Log the error */
	log_.push_back ( ctime (&rawtime) + (std::string)"   " + text );
	
	if ( !file.empty() )
		text += "\n\nFunction: " + function + "\nFile: " + file + "\nLine: " + util::lCast< std::string >( line );
	
	specialWindow_ [ "ERROR" ] = guiManager_.AddWindow ( new gui::CErrorWindow ( text ) );
	
	this->SetGameType ( ERROR );
}


std::vector< std::string >* CGame::GetLog() {
	return &log_;
}



GAMETYPE CGame::GetGameType()
{
	return gametype_;
}


void CGame::SetGameType ( GAMETYPE gametype )
{
	switch ( gametype ) {
		case LOADING:
			blackWindow->SetShow();
			blackWindow->ChangeTransparency( 230 );
			specialWindow_ [ "LOADING" ]->SetShow();
			break;
		case START:
			specialWindow_ [ "LOADING" ]->SetShow( false );
			break;
		case QUIT:
			blackWindow->SetShow();
			specialWindow_ [ "QUIT" ]->SetShow();
			break;
		case ERROR:
			blackWindow->SetShow();
			break;
	}
	
	gametype_ = gametype;
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


CLocale* CGame::GetLocale()
{
	return locale_;
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

	if ( !settings::GetShowFps() ) {
		return;
	}

	++frame;

	if ( frame >= fps ) {

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


sf::View* CGame::GetViewPoint()
{
	return viewPoint_;
}


/* --- PRIVATE --- */

bool CGame::IsVideoModeValid() /* TODO in CGame::initialize und settings::setHeight() ! */
{
	
	
}



void CGame::Render()
{
	// Bildschirm säubern
	app_.Clear();
	
	// Gamegraphic
	app_.SetView ( *viewPoint_ );
	mapManager_.Update();
	mapManager_.Render();
	
	// GUI
	app_.SetView ( app_.GetDefaultView() );
	guiManager_.Render();
	
	/* Ggf. FPS Anzeigen */
	this->CalcFPS();
	
	/* Mousecursor TODO extra klasse oder funktion zur verwaltung! */
	cursor_[settings::GetMouseScope() ]->SetPosition ( app_.GetInput().GetMouseX(), app_.GetInput().GetMouseY() );
	cursor_[settings::GetMouseScope() ]->Update();
	app_.Draw ( *cursor_[settings::GetMouseScope() ] );
	
	/* Zeichnen! */
	app_.Display();
}



void CGame::Calc()
{
	
}


void CGame::CalcSpecialWindow()
{
	static float alpha = 230;
	static sf::Clock clock;
	
	switch ( gametype_ ) {
		case LOADING:
			if ( alpha < 230 )
				alpha = clock.GetElapsedTime() * 500;
			if ( alpha >= 230 ) {
				alpha = 230;
				clock.Reset();
			}
			
			blackWindow->ChangeTransparency ( alpha );
			guiManager_.BringToFront ( blackWindow );
			guiManager_.BringToFront ( specialWindow_ [ "LOADING" ] );
			
			specialWindow_ [ "LOADING" ]->SetShow();
			return;
		case QUIT:
			if ( alpha < 230 )
				alpha = clock.GetElapsedTime() * 500;
			if ( alpha >= 230 ) {
				alpha = 230;
				clock.Reset();
			}
			
			blackWindow->ChangeTransparency ( alpha );
			guiManager_.BringToFront ( blackWindow );
			guiManager_.BringToFront ( specialWindow_ [ "QUIT" ] );
			
			specialWindow_ [ "QUIT" ]->SetShow();
			return;
		case ERROR:
			if ( alpha < 230 )
				alpha = clock.GetElapsedTime() * 500;
			if ( alpha >= 230 ) {
				alpha = 230;
				clock.Reset();
			}

			blackWindow->ChangeTransparency ( alpha );
			guiManager_.BringToFront ( blackWindow );
			guiManager_.BringToFront ( specialWindow_ [ "ERROR" ] );
			// 			specialWindow_ [ "ERROR" ]->SetShow();
			return;
		default:
			if ( alpha >= 230 ) {
				alpha = 230;
				clock.Reset();
			}
			
			if ( alpha > 0 ) {	
				alpha = 230 - clock.GetElapsedTime() * 500;
				blackWindow->ChangeTransparency ( alpha );			
			} if ( alpha <= 0 ) {
				alpha = 0;
				clock.Reset();
				blackWindow->SetShow ( false );
			}
			return;
			
	}
	
// 	clock.Reset();
// 	alpha = 0;
	
}