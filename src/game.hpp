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
#include "gui/manager.hpp"

#include "input.hpp"
#include "resource.hpp"
#include "animation.hpp"

class CGame
{

  public:
    CGame ( int argc, char **argv );
    bool Initialize ( void );

    bool IsVideoModeValid ( void );

    void Start ( void );
    void Stop ( void );

    void Render ( void );

    sf::RenderWindow* GetApp ( void );
    gui::CManager* GetGuiManager ( void );
    CResource< sf::Image >* GetImgResource ( void );

    void CalcFPS();

    std::map< MOUSESCOPE, CAnimation*> GetCursor ( void );
    sf::String* GetFpsStr ( void );

  private:
    sf::RenderWindow app;

    const int argc_;
    int arg_;
    const char* const * const argv_;
    bool run;

    sf::View viewPoint;

    gui::CManager guiManager;
    CResource < sf::Image > imgResource;
    CInput input;

    std::map< MOUSESCOPE, CAnimation*> cursor;

    sf::String fpsStr;

};

CGame* GetGameClass ( void );

#endif // GAME_HPP
