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

#ifndef GAME_HPP
#define GAME_HPP


#include <SFML/Graphics.hpp>
#include "engine/ui/manager.hpp"
#include "engine/ui/window.hpp"

#include "script/script.hpp"

#include "input.hpp"
#include "resource.hpp"

#include "gfx/animation.hpp"
#include "map/manager.hpp"
#include "settings/locale.hpp"


enum GAMETYPE {
	LOADING,
	START,
	ERROR,
	QUIT,
	PAUSED,
	MAINMENU,
	SINGLEPLAYER,
	MULTIPLAYER
};

class CGame
{
	public:
		CGame ( int argc, char **argv );
		~CGame();
		
		bool Initialize();

		bool Start();
		bool Stop();
		
		void Error ( std::string text, std::string function = "", std::string file = "", int line = 0);
		std::vector< std::string >* GetLog();

		GAMETYPE GetGameType();
		void SetGameType ( GAMETYPE gametype );

		sf::RenderWindow* GetApp();
		gui::CManager* GetGuiManager();
		CMapManager* GetMapManager();
		
		CLocale* GetLocale();

		CImageResource* GetImgResource();
		CFontResource* GetFontResource();

		void CalcFPS();

		std::map< MOUSESCOPE, CAnimation*> GetCursor();
		sf::String* GetFpsStr();
		
		sf::View* GetViewPoint();
// 		gui::CWindow* GetSpecialWindow ( std::string windowName );
		
	private:
		bool IsVideoModeValid();
		
		void Render();
		
		void Calc();
		void CalcSpecialWindow();

	private:
		sf::RenderWindow app_;

		const int argc_;
		int arg_;
		const char* const * const argv_;
		
		bool run_;
		GAMETYPE gametype_;
		gui::CWindow* blackWindow;

		sf::View* viewPoint_;
		std::map < std::string, gui::CWindow* > specialWindow_;

		CImageResource imgResource_;
		CFontResource fontResource_;

		gui::CManager guiManager_;
		CMapManager mapManager_;

		CInput input_;
		CLocale* locale_;

		std::map< MOUSESCOPE, CAnimation*> cursor_;

		sf::String fpsStr_;
		
		std::vector< std::string > log_;

};

CGame* GetGameClass ( void );

#endif // GAME_HPP
