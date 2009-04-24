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
#include "ui/manager.hpp"

#include "script.hpp"
#include "input.hpp"
#include "resource.hpp"
#include "animation.hpp"


class CGame
{
	public:
		CGame ( int argc, char **argv );
		~CGame ( void );

		bool Initialize ( void );

		bool IsVideoModeValid ( void );

		bool Start ( void );
		bool Stop ( void );

		void Render ( void );
		
		sf::RenderWindow* GetApp ( void );
		gui::CManager* GetGuiManager ( void );
		CResource< sf::Image >* GetImgResource ( void );

		void CalcFPS();

		std::map< MOUSESCOPE, CAnimation*> GetCursor ( void );
		sf::String* GetFpsStr ( void );

	private:
		sf::RenderWindow app_;

		const int argc_;
		int arg_;
		const char* const * const argv_;
		bool run_;

		sf::View viewPoint_;

		gui::CManager guiManager_;
		CResource < sf::Image > imgResource_;
		CInput input_;

		std::map< MOUSESCOPE, CAnimation*> cursor_;

		sf::String fpsStr_;

};

CGame* GetGameClass ( void );

#endif // GAME_HPP
