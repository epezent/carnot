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
#include <SFVG/Animation.hpp>
#include <SFVG/Graphics.hpp>

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
using Color         = sf::Color;
using View          = sf::View;
using RenderStates  = sf::RenderStates;
using RenderTarget  = sf::RenderTarget;
using String        = sf::String;

//==============================================================================
// SFVG Imports
//=============================================================================

using Shape       = sfvg::Shape;
using SquareShape = sfvg::SquareShape;
using Stroke      = sfvg::Stroke;
using Gradient    = sfvg::Gradient;

namespace Pinks    = sfvg::Pinks;
namespace Reds     = sfvg::Reds;
namespace Oranges  = sfvg::Oranges;
namespace Yellows  = sfvg::Yellows;
namespace Browns   = sfvg::Browns;
namespace Greens   = sfvg::Greens;
namespace Cyans    = sfvg::Cyans;
namespace Blues    = sfvg::Blues;
namespace Purples  = sfvg::Purples;
namespace Whites   = sfvg::Whites;
namespace Grays    = sfvg::Grays;

namespace Tween    = sfvg::Tween;
