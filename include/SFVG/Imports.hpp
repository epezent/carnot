#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderStates.hpp>

//==============================================================================
// SFML Imports
//=============================================================================

using Key           = sf::Keyboard::Key;
using MouseButton   = sf::Mouse::Button;
using Event         = sf::Event;
using Drawable      = sf::Drawable;
using Transformable = sf::Transformable;
using Transform     = sf::Transform;
using NonCopyable   = sf::NonCopyable;
using Vector2f      = sf::Vector2f;
using Vector2i      = sf::Vector2i;
using Vector2u      = sf::Vector2u;
using FloatRect     = sf::FloatRect;
using Font          = sf::Font;
using RenderWindow  = sf::RenderWindow;
using Text          = sf::Text;
using Time          = sf::Time;
using Clock         = sf::Clock;
using Color         = sf::Color;
using View          = sf::View;
using RenderStates  = sf::RenderStates;
using RenderTarget  = sf::RenderTarget;
using String        = sf::String;

using sf::seconds;
using sf::milliseconds;
using sf::microseconds;

