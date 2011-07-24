#include <iostream>
#include <sstream>
#include <SFML/System.hpp>
#include "player.h"
#include "objects/objects.h"
#include "game.h"
#include "utility.h"
#include "objectmanager.h"

std::map< sf::Uint16, boost::function<Object* ()> >* ObjectManager::factories = 0;

ObjectManager::ObjectManager() {
  lastId = 0;
  LastFullUpdate.Reset();
  objects_loaded = false;
}

ObjectManager::~ObjectManager() {
  ClearObjects();

  LogConsole("Object manager cleanup done.");

  delete factories;
}

void ObjectManager::AddObject(Object* o) {
  objects.push_back(o);
}

void ObjectManager::RemoveObject(Object* o) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if((*i) && (*i)->GetId() == o->GetId()) {
      delete (*i);
      objects.erase(i);
      break;
    }
  }
}

void ObjectManager::RemoveObjectById(sf::Uint16 id) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if((*i) && (*i)->GetId() == id) {
      delete (*i);
      objects.erase(i);
      break;
    }
  }
}

Object* ObjectManager::GetObjectById(sf::Uint16 id) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    if(objects[i]->GetId() == id) {
      return objects[i];
    }
  }

  std::stringstream ss;
  ss << "Couldn't find object with id " << id;
  LogConsole(ss.str());

  return 0;
}

Object* ObjectManager::CreateObject(sf::Uint16 type) {

  if( !factories ) {
    LogConsole("No factories");
    return 0;
  }

  std::map<sf::Uint16, boost::function<Object* ()> >::iterator i = factories->find(type);

  if( i == factories->end() ) {
    LogConsole("Invalid object type.");
    return 0;
  } else {
    Object* o = (i->second)();
    AddObject(o);
    return o;
  }
}

void ObjectManager::ClearObjects() {
  while( objects.size() ) {
    if( objects.back() ) {
      delete objects.back();
      objects.back() = 0;
    }
    objects.pop_back();
  }
}

void ObjectManager::SubscribeRelevant(Player* p) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    p->AddObjectToView(objects[i]);
  }
}

void ObjectManager::Tick(float time) {
  if( !objects_loaded ) {
    LoadObjects();
  }

  for( size_t i = 0; i < objects.size(); ) {
    if( objects[i]->IsDeleted() ) {
      RemoveObject(objects[i]);
      continue;
    }

    if( objects[i]->IsFresh() ) {
      Game::GetGame()->GetPlayerManager()->BroadcastNewObject(objects[i]);
      objects[i]->ClearFresh();
    }

    objects[i]->Update(time);

    i++;
  }
}

void ObjectManager::AddFactory(sf::Uint16 t, boost::function<Object* ()> factory) {
  if( !factories ) {
    factories = new std::map< sf::Uint16, boost::function<Object* ()> >;
  }

  ObjectManager::factories->insert( std::pair<sf::Uint16, boost::function<Object* ()> >(t, factory) );

  std::stringstream ss;
  ss << "Registered object factory type " << t;
  LogConsole(ss.str());
}

void ObjectManager::LoadObjects() {
  Planet* p = (Planet*)(CreateObject(PLANET));
  p->SetOrbit(5,400);
  p->SetRotationalVelocity(5);
  p->SetAnchor(100,50);

  objects_loaded = true;
}
