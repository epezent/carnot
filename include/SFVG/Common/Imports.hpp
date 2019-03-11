#pragma once

// SFML
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
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

namespace sfvg {

//==============================================================================
// SFML Imports
//==============================================================================

using sf::Event;
using sf::Drawable;
using sf::Transformable;
using sf::NonCopyable;
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;
using sf::FloatRect;
using sf::IntRect;
using sf::Font;
using sf::RenderWindow;
using sf::Text;
using sf::Time;
using sf::Clock;
using sf::Color;
using sf::View;
using sf::RenderStates;
using sf::RenderTarget;
using sf::Uint8;
using sf::Packet;
using sf::Socket;
using sf::SocketSelector;
using sf::UdpSocket;
using sf::TcpSocket;
using sf::TcpListener;
using sf::IpAddress;
using sf::VertexArray;
using sf::Sprite;
using sf::Texture;
using sf::Image;
using sf::SoundBuffer;
using sf::Sound;
using sf::Music;
using sf::Shader;
using sf::Vertex;

using Key         = sf::Keyboard::Key;
using MouseButton = sf::Mouse::Button;
using Matrix3x3   = sf::Transform;

using sf::seconds;
using sf::milliseconds;
using sf::microseconds;

namespace WindowStyle = sf::Style;

typedef sf::Vector2<double> Vector2d;

} // namespace sfvg
