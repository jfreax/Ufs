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

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

class CAnimation : public sf::Drawable
{
  public:
    CAnimation ( sf::Image* image_, int frames_, float fpm_ );

    void Start ( void );
    void Stop ( void );
    void Update ( void );

    virtual void Render ( sf::RenderTarget& ) const;

    void SetStartAt ( int startAt_ );
  private:
    bool run;
    sf::Clock timer;
    int startAt;

    sf::Image* image;
    sf::IntRect clipArea;

    int frameWidth;
    int frames;
    float timePerFrame;

    int frame;

};

#endif // ANIMATION_HPP
