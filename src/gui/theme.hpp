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

#ifndef CTHEME_HPP
#define CTHEME_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../util.hpp"
#include "../parser.hpp"

namespace gui
{


struct ThemeHolder {
  sf::Vector2f minSize;
  sf::Vector2f maxSize;

  sf::Color backgroundColor;
  std::string background;

  int border;
  sf::Color borderColor;

  int titlebar;
  sf::Color titlebarColor;

  int fontSize;

  sf::Vector3f iconClose;
};


class Parameter
{
  public:
    Parameter ( std::string name_, int* x ) : name ( name_ ), i ( x ) {
      type = INTEGER;
    }
    Parameter ( std::string name_, float* x ) : name ( name_ ), f ( x ) {
      type = FLOAT;
    }
    Parameter ( std::string name_, std::string* x ) : name ( name_ ), s ( x ) {
      type = STRING;
    }
    Parameter ( std::string name_, sf::Color* x ) : name ( name_ ), c ( x ) {
      type = COLOR;
    }
    Parameter ( std::string name_, sf::Image* x ) : name ( name_ ), img ( x ) {
      type = IMAGE;
    }
    Parameter ( std::string name_, sf::Vector2i* x ) : name ( name_ ), vi ( x ) {
      type = VECTOR_INTEGER;
    }
    Parameter ( std::string name_, sf::Vector2f* x ) : name ( name_ ), vf ( x ) {
      type = VECTOR_FLOAT;
    }
    Parameter ( std::string name_, sf::Vector3f* x ) : name ( name_ ), v3f ( x ) {
      type = VECTOR3_FLOAT;
    }

    TYPES type;
    std::string name;

    int* i;
    float* f;
    std::string* s;

    sf::Color* c;
    sf::Image* img;
    sf::Vector2f* vf;
    sf::Vector2i* vi;
    sf::Vector3f* v3f;
};


class CTheme
{
  public:
    CTheme ( void );
    void Open ( std::string filename );

    std::map< std::string, ThemeHolder* > headerList;
    std::multimap< std::string, Parameter > paramList;

    Parser themeFile;
    ThemeHolder window;
    ThemeHolder button;
};


} // namespace gui

#endif // CTHEME_HPP
