/*
 * collision_event.hxx
 *
 *  Created on: 28 de set de 2016
 *      Author: carlosfaruolo
 */

//helper struct to deal with collision events
struct CollisionEvent
{
	vector<Body2D> collidingBodies;
	Body2D resultingMerger;

	~CollisionEvent()
	{
		purgeUserObjects(collidingBodies);
	}
};
