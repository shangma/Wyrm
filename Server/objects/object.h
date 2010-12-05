#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Object {
  public:
    Object(sf::Uint16 type_, sf::Uint16 id_, sf::String name_, sf::Vector2f pos_, sf::Vector2f vel_, float rot_, float rot_vel_);
    virtual ~Object();
    virtual void Update(float time) = 0;

    virtual void HandlePacket(sf::Packet p);

    sf::Uint16 type;

    sf::Uint16 id;
    sf::String name;

    sf::Vector2f position;
    sf::Vector2f velocity;

    float rotation;
    float rotational_velocity;
};

#endif // OBJECT_H_INCLUDED
