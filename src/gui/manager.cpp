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

#include "theme.hpp"
#include "widget.hpp"

#include "../collision.hpp"
#include "../game.hpp"
#include "../settings.hpp"

#include "manager.hpp"

namespace gui
{

CManager::CManager()
{
  theme.open ( "themes/" + settings::getTheme() + "/" + settings::getTheme() + ".ini" );
  mousePos4WinMove = sf::Vector2f ( 0.f, 0.f );
}


bool CManager::draw ( )
{
  CGame* game = getGameClass();

  for ( std::vector<gui::CWindow*>::size_type i = windowList.size(); i; --i )
    windowList[i-1]->draw ( );

  return true;
}


bool CManager::proofMouseClick ( const sf::Input& input )
{
  bool ret = false;
  int x = input.GetMouseX();
  int y = input.GetMouseY();

  for ( std::vector<gui::CWindow*>::size_type i = windowList.size(); i; --i ) {
    // Auf Fenstertitelleiste geklickt?
    if ( isCollision ( windowList[i-1]->getTitlebarDimension(), sf::Rect<float> ( x, y, x + 1, y + 1 ) ) || ( mousePos4WinMove.x && mousePos4WinMove.y ) ) {
      if ( input.IsMouseButtonDown ( sf::Mouse::Left ) ) {
        ret = true;
        sf::Vector2f winPos = windowList[i-1]->getPosition();

        if ( !mousePos4WinMove.x && !mousePos4WinMove.y ) {
          mousePos4WinMove.x = x - winPos.x; mousePos4WinMove.y = y - winPos.y;
        }

        windowList[i-1]->setPosition ( sf::Vector2f ( x - ( mousePos4WinMove.x ), y - ( mousePos4WinMove.y ) ) );
      } else {
        mousePos4WinMove.x = mousePos4WinMove.y = 0;
      }
    }

    // Fenster geklickt?
    if ( isCollision ( windowList[i-1]->getWindowDimension(), sf::Rect<float> ( x, y, x + 1, y + 1 ) ) ) {
      ret = true;
      // Prüfen ob Widgets geklickt wurden
    }
  }

  return ret;
}


gui::CWindow* CManager::newWindow ( sf::Vector2f position_, sf::Vector2f size_ )
{
  gui::CWindow* win = new gui::CWindow ( &theme );
  windowList.push_back ( win );

  return win;
}


bool CManager::closeWindow ( gui::CWindow* window_ )
{
  if ( !window_ && windowList.size() ) {
    delete this->getActiveWindow();

    windowList.pop_back();
  } else {
    std::vector<gui::CWindow*>::iterator iter = windowList.begin();
    for( ; iter != windowList.end(); ++iter ) {
      if ( (*iter)->getId() == window_->getId() ) {
        windowList.erase ( iter );
        delete (*iter);

        iter = windowList.end()-1;
      }
    }

  }

}


gui::CWindow* CManager::getActiveWindow()
{
  return windowList[windowList.size()-1];
}



} // namespace gui

