/*
 * state_manager.hxx
 *
 *  Created on: 28 de set de 2016
 *      Author: carlosfaruolo
 */

struct Planetarium::StateManager
{
	enum ChangeType { ADDITION, REMOVAL, MERGE };

	struct Change
	{
		vector<Body2DClone> backup;
		vector<Body2DClone> diff;
		ChangeType type;

		Change (const Universe2D& u, const vector<Body2D*>& diff, ChangeType type)
		: backup(), diff(), type(type)
		{
			if(type == MERGE) throw_exception("Invalid StateChange type for constructor! MERGE cannot be used with this constructor!");

			const_foreach(Body2D*, body, vector<Body2D*>, u.bodies)
				backup.push_back(Body2DClone(body));

			const_foreach(Body2D*, body, vector<Body2D*>, diff)
				this->diff.push_back(Body2DClone(body));
		}

		Change (const Universe2D& u, Body2D* diff, ChangeType type)
		: backup(), diff(), type(type)
		{
			if(type == MERGE) throw_exception("Invalid StateChange type for constructor! MERGE cannot be used with this constructor!");

			const_foreach(Body2D*, body, vector<Body2D*>, u.bodies)
				backup.push_back(Body2DClone(body));

			this->diff.push_back(Body2DClone(diff));
		}

		Change (const vector<Body2D*>& merged, Body2D* merger)
		: backup(), diff(), type(MERGE)
		{
			diff.push_back(Body2DClone(merger));
			const_foreach(Body2D*, body, vector<Body2D*>, merged)
				diff.push_back(Body2DClone(body));
		}
	};

	Planetarium* planetarium;
	deque<Change> changes;
	unsigned nonMergeChangesCount;

	StateManager(Planetarium* planetarium) : planetarium(planetarium), changes(), nonMergeChangesCount(0) {}

	void commitAddition(const Universe2D& u, const vector<Body2D*>& diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, ADDITION));
		nonMergeChangesCount++;
		//cout << "commit add" << endl;
	}

	void commitAddition(const Universe2D& u, Body2D* diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, ADDITION));
		nonMergeChangesCount++;
		//cout << "commit add" << endl;
	}

	void commitRemoval(const Universe2D& u, const vector<Body2D*>& diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, REMOVAL));
		nonMergeChangesCount++;
		//cout << "commit rem" << endl;
	}

	void commitRemoval(const Universe2D& u, Body2D* diff)
	{
		shrinkToFit();
		changes.push_back(Change(u, diff, REMOVAL));
		nonMergeChangesCount++;
		//cout << "commit rem" << endl;
	}

	void commitMerge(const vector<Body2D*>& merged, Body2D* merger)
	{
		shrinkToFit();
		changes.push_back(Change(merged, merger));
		//cout << "commit merge" << endl;
	}

	/// Swaps 'oldAdress' pointers in the changes stack (history) for 'newAddress' pointers
	void fixReferences(const Body2D* oldAddress, Body2D* newAddress)
	{
		foreach(Change&, change, deque<Change>, changes)
		{
			foreach(Body2DClone&, backupBody, vector<Body2DClone>, change.backup)
				if(backupBody.original == oldAddress)
					backupBody.original = newAddress;

			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
				if(diffBody.original == oldAddress)
					diffBody.original = newAddress;
		}
	}

	private:void purgeAsUndone(Change& change)
	{
		//cout << "purging " << (change.type==MERGE? "merge":"non-merge") << " as undone" << endl;
		if(change.type == ADDITION)
		{
			//deletes previously added bodies' user objects and the bodies' themselves.
			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
			{
				planetarium->orbitTracer->clearTrace(diffBody.original);
				delete static_cast<PlanetariumUserObject*>(diffBody.original->userObject);
				delete diffBody.original;
			}
		}
		else if(change.type == MERGE)
		{
			//deletes merger's user object and the merger's itself.
			planetarium->orbitTracer->clearTrace(change.diff[0].original);
			delete static_cast<PlanetariumUserObject*>(change.diff[0].original->userObject);
			delete change.diff[0].original;
		}
	}

	private:void purgeAsForgotten(Change& change)
	{
		//cout << "purging " << (change.type==MERGE? "merge":"non-merge") << " as forgotten" << endl;
		if(change.type == REMOVAL)
		{
			//deletes previously removed bodies' user objects and the bodies' themselves.
			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
			{
				planetarium->orbitTracer->clearTrace(diffBody.original);
				delete static_cast<PlanetariumUserObject*>(diffBody.original->userObject);
				delete diffBody.original;
			}
		}
		else if(change.type == MERGE)
		{
			change.diff.erase(change.diff.begin()); //remove merger from the diff list

			//deletes merged bodies' user objects and themselves.
			foreach(Body2DClone&, diffBody, vector<Body2DClone>, change.diff)
			{
				planetarium->orbitTracer->clearTrace(diffBody.original);
				delete static_cast<PlanetariumUserObject*>(diffBody.original->userObject);
				delete diffBody.original;
			}
		}
	}

	private:void rollbackPositions(Change& state)
	{
		foreach(Body2DClone&, backupBody, vector<Body2DClone>, state.backup)
			foreach(Body2D*, body, vector<Body2D*>, planetarium->physics->universe.bodies)
				if(body == backupBody.original)
					*body = backupBody.clone; // copy attributes (position, velocities, etc...)
	}

	private:void rollbackAddition(Change& state, bool rewind=false)
	{
		rollbackPositions(state);

		foreach(Body2DClone&, addedBody, vector<Body2DClone>, state.diff)
			if (not rewind)
				remove_element(planetarium->physics->universe.bodies, addedBody.original);
			else
				*(addedBody.original) = addedBody.clone; // copy attributes (position, velocities, etc...)
	}

	private:void rollbackRemoval(Change& state, bool rewind=false)
	{
		rollbackPositions(state);
		if(not rewind)
			foreach(Body2DClone&, removedBody, vector<Body2DClone>, state.diff)
				planetarium->physics->universe.bodies.push_back(removedBody.original);
	}

	private:void rollbackMerge(Change& state)
	{
		Body2D* merger = state.diff[0].original;
		remove_element(planetarium->physics->universe.bodies, merger);

		foreach(Body2DClone&, removedBody, vector<Body2DClone>, state.diff)
			if(removedBody.original != merger) //merger is in the first index, ignore it
				planetarium->physics->universe.bodies.push_back(removedBody.original);
	}

	public:
	void rollback(bool rewind=false)
	{
		if(nonMergeChangesCount == 0) return;
		//cout << (rewind? "rewind":"rollback") << endl;
		SDL_mutex* physicsAccessMutex = planetarium->physicsAccessMutex;

		if(changes.back().type != MERGE) // no merge involved, simpler case
		{
			Change& change = changes.back();
			if(change.type == ADDITION)
			{
				synchronized(physicsAccessMutex)
				{
					rollbackAddition(change, rewind);
				}

				if(not rewind) foreach(Body2DClone&, addedBody, vector<Body2DClone>, change.diff)
				{
					//notify listeners about the body deleted
					for(unsigned i = 0; i < planetarium->listeners.size(); i++)
						planetarium->listeners[i]->onBodyDeletion(addedBody.original);
				}
			}

			else if (change.type == REMOVAL)
			{
				synchronized(physicsAccessMutex)
				{
					rollbackRemoval(change, rewind);
				}

				if(not rewind) foreach(Body2DClone&, removedBody, vector<Body2DClone>, change.diff)
				{
					for(unsigned i = 0; i < planetarium->listeners.size(); i++)
						planetarium->listeners[i]->onBodyCreation(*removedBody.original);
				}
			}

			if(not rewind)
			{
				purgeAsUndone(change);
				changes.pop_back(); //stuack pop
				if(change.type != MERGE) nonMergeChangesCount--;
			}
		}

		else //merges happened after last body addition/removal, needs to rollback more than one state
		{
			vector<Body2DClone> removedAddedBodies, addedRemovedBodies; //write down to notify or delete later
			bool needsRollback = true; //when rollbacking merge changes, keep rollbacking futher

			synchronized(physicsAccessMutex)
			{
				while(not changes.empty() and needsRollback)
				{
					Change& change = changes.back();

					if(change.type == ADDITION)
					{
						rollbackAddition(change, rewind);
						if(not rewind) removedAddedBodies.insert(removedAddedBodies.end(), change.diff.begin(), change.diff.end()); //write down to notify or delete later
						needsRollback = false;
					}

					else if (change.type == REMOVAL)
					{
						rollbackRemoval(change, rewind);
						if(not rewind) addedRemovedBodies.insert(addedRemovedBodies.end(), change.diff.begin(), change.diff.end()); //write down to notify later
						needsRollback = false;
					}

					else if (change.type == MERGE)
					{
						rollbackMerge(change);
						removedAddedBodies.push_back(change.diff[0]); //merger ("diff[0]") was removed
						addedRemovedBodies.insert(addedRemovedBodies.end(), change.diff.begin()+1, change.diff.end()); //mergeds were re-added
						needsRollback = true;
					}

					if(change.type == MERGE or not rewind)
					{
						changes.pop_back(); //stack pop
						if(change.type != MERGE) nonMergeChangesCount--;
					}
				}
			}

			//notify listeners about the body removed
			foreach(Body2DClone&, removedBody, vector<Body2DClone>, removedAddedBodies)
			{
				for(unsigned i = 0; i < planetarium->listeners.size(); i++)
					planetarium->listeners[i]->onBodyDeletion(removedBody.original);

				//delete unused manually
				planetarium->orbitTracer->clearTrace(removedBody.original);
				delete static_cast<PlanetariumUserObject*>(removedBody.original->userObject);
				delete removedBody.original;
			}

			//notify listeners about the body re-added
			foreach(Body2DClone&, addedBody, vector<Body2DClone>, addedRemovedBodies)
				for(unsigned i = 0; i < planetarium->listeners.size(); i++)
					planetarium->listeners[i]->onBodyCreation(*addedBody.original);
		}
	}

	void reset()
	{
		while(not changes.empty())
		{
			purgeAsUndone(changes.back());
			changes.pop_back(); //stack pop
		}
		nonMergeChangesCount = 0;
	}

	void shrinkToFit()
	{
		while(changes.size() > planetarium->undoStackMaxSize-1)
		{
			purgeAsForgotten(changes.front());
			if(changes.front().type != MERGE) nonMergeChangesCount--;
			changes.pop_front();
		}
	}

	/// Prints current change stack
	void debug()
	{
		cout << "# debug-stack #" << endl;
		foreach(Change&, change, deque<Change>, changes)
		{
			cout << "@change " << &change << endl;

			cout << "-->backup: ";
			foreach(Body2DClone&, backupBody, vector<Body2DClone>, change.backup)
				cout << backupBody.original << ", ";
			cout << endl;

			cout << "-->diff: ";
			for(unsigned i = 0; i < change.diff.size(); i++)
				cout << change.diff[i].original << (i==0 and change.type==MERGE? "(merger)":"") << ", ";
			cout << endl;
		}
		cout << "# " << nonMergeChangesCount << " non-merge changes #" << endl;
	}
};
