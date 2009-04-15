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

#include <typeinfo>

#include "theme.hpp"
#include "../settings.hpp"

namespace gui
{


CTheme::CTheme()
{
  window.minSize = sf::Vector2f ( 10.f, 10.f );
  window.maxSize = sf::Vector2f ( 5000.f, 5000.f );
  window.backgroundColor = window.borderColor = sf::Color ( 100, 100, 100 );
  window.titlebarColor = sf::Color ( 0, 0, 100 );
  window.border = 1;
  window.titlebar = 10;

  button.minSize = sf::Vector2f ( 5.f, 2.f );
  button.maxSize = sf::Vector2f ( 5000.f, 5000.f );
  button.backgroundColor = window.borderColor = sf::Color ( 200, 200, 200 );
  button.border = 0;


  headerList.insert ( std::make_pair< std::string, ThemeHolder* > ( "window", &window ) );
  headerList.insert ( std::make_pair< std::string, ThemeHolder* > ( "button", &button ) );

  std::map< std::string, ThemeHolder* >::iterator iter = headerList.begin();
  std::map< std::string, ThemeHolder* >::iterator iterEnd = headerList.end();
  for ( ; iter != iterEnd; iter++ ) {
    paramList.push_back ( Parameter ( "minsize", &iter->second->ThemeHolder::minSize ) );
    paramList.push_back ( Parameter ( "maxsize", &iter->second->ThemeHolder::maxSize ) );
    paramList.push_back ( Parameter ( "backgroundimage", &iter->second->ThemeHolder::background ) );
    paramList.push_back ( Parameter ( "backgroundcolor", &iter->second->ThemeHolder::backgroundColor ) );
    paramList.push_back ( Parameter ( "titlebarcolor", &iter->second->ThemeHolder::titlebarColor ) );
    paramList.push_back ( Parameter ( "border", &iter->second->ThemeHolder::border ) );
    paramList.push_back ( Parameter ( "titlebar", &iter->second->ThemeHolder::titlebar ) );
  }

}


void CTheme::open ( std::string filename )
{
  themeFile.open ( filename, false, true );

  std::stringstream stream;
  std::string strTemp;

  std::map< std::string, ThemeHolder* >::iterator iter = headerList.begin();
  std::map< std::string, ThemeHolder* >::iterator iterEnd = headerList.end();

  for ( ; iter != iterEnd; iter++ ) {
    std::string headerName = iter->first;
    ThemeHolder* headerTheme = iter->second;

    std::vector< Parameter >::iterator paramIter = paramList.begin();
    std::vector< Parameter >::iterator paramIterEnd = paramList.end();

    for ( ; paramIter != paramIterEnd; paramIter++ ) {
      stream.str ( "" );
      stream.clear();

      strTemp = themeFile.getValue ( headerName, paramIter->name );

      if ( !strTemp.empty() ) {
        switch ( paramIter->type ) {
          case INTEGER:
            stream << strTemp && stream >> *paramIter->i;
            break;
          case FLOAT:
            stream << strTemp && stream >> *paramIter->f;
            break;
          case STRING:
           *paramIter->s = strTemp;
            break;
          case COLOR:
            stream << strTemp && stream >> paramIter->c->r >> paramIter->c->g >> paramIter->c->b >> paramIter->c->a;
            break;
          case IMAGE:
            if ( !paramIter->img->LoadFromFile ( settings::getPath() + "/themes/" + settings::getTheme() + "/" + strTemp ) ) {
              // TODO Resourcenmanager
              std::cerr << "Error loading backgroundimage: " << strTemp << std::endl;
            }
            break;
          case VECTOR_INTEGER:
            stream << strTemp && stream >> paramIter->vi->x >> paramIter->vi->y;
            break;
          case VECTOR_FLOAT:
            stream << strTemp && stream >> paramIter->vf->x >> paramIter->vf->y;
            break;

          default:
            *paramIter->s = strTemp;
            break;
        }
      }
    }
  }
}


} // namespace gui
