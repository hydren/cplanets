/*
 * physics2d_events_manager.hxx
 *
 *  Created on: 28 de set de 2016
 *      Author: carlosfaruolo
 */

// helper struct to buffer collision events
struct Planetarium::Physics2DEventsManager
{
	queue<CollisionEvent*> collisionEvents;
	SDL_mutex* mutex;
	Physics2DEventsManager() : collisionEvents(), mutex(SDL_CreateMutex()) {}
	~Physics2DEventsManager() { SDL_DestroyMutex(mutex); }
};
