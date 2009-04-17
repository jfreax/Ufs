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

CManager::CManager ( void )
{
  theme.Open ( settings::GetThemePath() + settings::GetTheme() + ".ini" );

  previousMouseScope = NONE;
  previousMousePos.x = previousMousePos.y = 0;
}


bool CManager::Render ( void )
{
  std::vector<gui::CWindow*>::iterator iter = windowList.begin();
  std::vector<gui::CWindow*>::iterator iterEnd = windowList.end();

  for ( int i = 0; iter != iterEnd; ++iter )
  {
    ( *iter )->Render();
  }

  return true;
}


bool CManager::MouseLClick ( const sf::Input& input )
{
  CGame* game = GetGameClass();

  int x = input.GetMouseX();
  int y = input.GetMouseY();
  sf::Vector2f mouse ( x, y );
  sf::Rect< float > mouseRect ( x, y, x + 1, y + 1 );

  MOUSESCOPE highestMouseScope = NONE;


  std::vector< CWindow* >::iterator iter = --windowList.end();
  std::vector< CWindow* >::iterator iterBegin = windowList.begin();

  for ( ; iter + 1 != iterBegin; --iter )
  {

    // Auf Fenstertitelleiste geklickt?
    if ( isCollision ( ( *iter )->GetTitlebarDimension(), mouseRect )
         && previousMouseScope == NONE || previousMouseScope == TITLE )
    {
      previousMouseScope = TITLE;

      if ( iter == --windowList.end() )   // nur letztes (im Vordergrundstehendes) Fenster
      {
        sf::Vector2f winPos = ( *iter )->GetPosition();

        if ( !previousMousePos.x && !previousMousePos.y )   // Maus wurde zuvor hier nicht geklickt
        {
          previousMousePos.x = x - winPos.x;
          previousMousePos.y = y - winPos.y;
        }

        ( *iter )->SetPosition ( sf::Vector2f ( x - ( previousMousePos.x ), y - ( previousMousePos.y ) ) ); // verschieben
      }
      else if ( iter != windowList.end() )   // ansonsten das Fenster in den Vordergrund holen
      {
        this->BringToFront ( iter );
        return true;
      }

      return true;
    }

    // Fenster vergrößern/verkleinern?
    else if ( isCollision ( ( *iter )->GetResizeArea(), mouseRect )
              && previousMouseScope == NONE || previousMouseScope == RESIZE )
    {
      previousMouseScope = RESIZE;

      if ( iter == --windowList.end() )   // nur letztes (im Vordergrundstehendes) Fenster
      {
        sf::Vector2f winPos = ( *iter )->GetPosition();

        if ( !previousMousePos.x && !previousMousePos.y )
        {
          previousMousePos.x = x - winPos.x;
          previousMousePos.y = y - winPos.y;
        }

        ( *iter )->SetSize ( mouse - ( *iter )->GetPosition() );
      }
      else if ( iter != windowList.end() )   // ansonsten das Fenster in den Vordergrund holen
      {
        this->BringToFront ( iter );
        return true;
      }

      return true;
    }

    // Fenster geklickt?
    else if ( isCollision ( ( *iter )->GetWindowDimension(), mouseRect )
              && previousMouseScope == NONE || previousMouseScope == WINDOW )
    {
      previousMouseScope = WINDOW;

      if ( iter != windowList.end() )   // Fenster in den Vordergrund holen
      {
        this->BringToFront ( iter );
        return true;
      }

      // TODO Prüfen ob Widgets geklickt wurden
    }
  }

  settings::SetMouseScope ( highestMouseScope );

  return false;
}


bool CManager::MouseHover ( const sf::Input& input )
{

//  else {
//       mouseScope = NONE;
//       mousePos.x = mousePos.y = 0;
//
//       if ( isCollision ( ( *iter )->GetTitlebarDimension(), mouseRect ) ) {  // Titelbar
//         if ( isCollision ( ( *iter )->GetIconCloseCoord(), mouseRect ) ) { // schließen
// //           highestMouseScope = BUTTON > highestMouseScope ? BUTTON : highestMouseScope;
//         }
//       } else if ( isCollision ( ( *iter )->GetWindowDimension(), mouseRect ) ) { // Maus über Fenster?
//         highestMouseScope = WINDOW > highestMouseScope ? WINDOW : highestMouseScope;
//
//       } else {
//
//
//       }
//     }
}


gui::CWindow* CManager::NewWindow ( sf::Vector2f position_, sf::Vector2f size_ )
{
  gui::CWindow* win = new gui::CWindow ( &theme );
  windowList.push_back ( win );

  return win;
}


bool CManager::CloseWindow ( gui::CWindow* window_ )
{
  if ( !window_ && windowList.size() )
  {
    delete this->GetActiveWindow();

    windowList.pop_back();
  }
  else
  {
    std::vector<gui::CWindow*>::iterator iter = windowList.begin();

    for ( ; iter != windowList.end(); ++iter )
    {
      if ( ( *iter )->GetId() == window_->GetId() )
      {
        windowList.erase ( iter );
        delete ( *iter );

        iter = windowList.end() - 1;
      }
    }
  }
}


bool CManager::BringToFront ( std::vector< CWindow* >::iterator iter_ )
{
  CWindow* tmp = *iter_;

  windowList.erase ( iter_ );
  windowList.push_back ( ( tmp ) );
}


CTheme* CManager::GetTheme ( void )
{
  return &theme;
}


gui::CWindow* CManager::GetActiveWindow ( void )
{
  return windowList[windowList.size()-1];
}



} // namespace gui

