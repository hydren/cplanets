package logic;

import java.awt.Color;
import java.util.HashSet;
import java.util.Set;

import logic.solvers.AvailableSolvers;
import logic.solvers.Physics2DSolver;
import model.geometry.Vector2D;
import model.physics.Body2D;
import model.physics.Universe2D;
import controller.Main;

public class Physics2D 
{
	/** The universe instance: all bodies and gravity. 
	 * <b>This instance shouldn't be exchanged directly.</b> 
	 * Use {@link #setUniverse(Universe2D) setUniverse} instead. */
	public Universe2D universe = new Universe2D();
	
	public Physics2D() 
	{
		setSolver(AvailableSolvers.getSolverByClassName(Main.settings.solver).getNewInstance(universe));
		if(solver == null)
			setSolver(AvailableSolvers.getSolverByClassName("default").getNewInstance(universe));
	}
	
	public class ReferenceFrame
	{
		public Set<Body2D> bodies = new HashSet<Body2D>();
		
		public Vector2D getPosition()
		{
			Vector2D centerOfMass = new Vector2D();
			double totalMass = 0;
			for(Body2D b : bodies)
			{
				centerOfMass.add(b.position.times(b.mass));
				totalMass += b.mass;
			}
			
			centerOfMass.scale(1.0d/totalMass);
			
			return centerOfMass;
		}
		
		public Vector2D getVelocity()
		{
			Vector2D totalVelocity = new Vector2D();
			
			double totalMass = 0;
			for(Body2D b : bodies)
			{
				totalVelocity.add(b.velocity.times(b.mass));
				totalMass += b.mass;
			}
			
			totalVelocity.scale(1.0d/totalMass);
			
			System.out.println("refential velocity: " + totalVelocity.x + " " + totalVelocity.y);
			System.out.println("total mass of referential: "+totalMass);
			
			return totalVelocity;
		}
	}
	public ReferenceFrame referenceFrame = new ReferenceFrame();
	
	
	/** The current physics simulation class. 
	 * <b>This instance shouldn't be exchanged directly.</b> 
	 * Use {@link #setSolver(Physics2DSolver) setSolver} instead. */
	public Physics2DSolver solver;
	
	@Override
	public Physics2D clone()
	{
		Physics2D p = new Physics2D();
		synchronized(this)
		{
			p.solver.timestep = solver.timestep;
			p.universe.gravity = universe.gravity;
			for(Body2D b : universe.bodies)
			{
				p.universe.bodies.add(b);
			}
		}
		return p;
	}
	
	private HashSet<HashSet<Body2D>> collisions = new HashSet<HashSet<Body2D>>();
	private void resolveCollisions()
	{
		//detect collisions
		collisions.clear();
		for(Body2D a : universe.bodies)	for(Body2D b : universe.bodies)
		{
			if(a == b) continue;
				
			if(a.position.distance(b.position) < a.diameter/2 + b.diameter/2)
			{
				boolean bothAdded=false;
				for(Set<Body2D> set : collisions)
				{
					if(set.contains(a) && set.contains(b)) //probably a duplicate lookup
					{
						bothAdded = true;
						break;
					}
					else if(set.contains(a) && !set.contains(b)) //append colliding b
					{
						set.add(b);
						bothAdded = true;
						break;
					}
					else if(!set.contains(a) && set.contains(b)) //append colliding a
					{
						set.add(a);
						bothAdded = true;
						break;
					}
				}
				if(!bothAdded) //new colliding pair
				{
					HashSet<Body2D> newSet = new HashSet<Body2D>();
					newSet.add(a);
					newSet.add(b);
					collisions.add(newSet);
				}
			}
		}
		
		if(collisions.isEmpty() == false)
			referenceFrame.bodies.clear();
		
		//resolve collisions
		for(HashSet<Body2D> collisionSet : collisions)
		{
			Body2D merger = new Body2D(0, 0, new Vector2D(), new Vector2D(), new Vector2D());
			for(Body2D body : collisionSet)
			{
				merger.position.x += body.position.x;
				merger.position.y += body.position.y;

				merger.velocity.x = (merger.velocity.x*merger.mass + body.velocity.x*body.mass)/(merger.mass + body.mass);
				merger.velocity.y = (merger.velocity.y*merger.mass + body.velocity.y*body.mass)/(merger.mass + body.mass);
				
				merger.diameter = Math.sqrt(merger.diameter*merger.diameter + body.diameter*body.diameter);
				merger.mass += body.mass;

				merger.color = new Color((merger.color.getRed() + body.color.getRed())/2, (merger.color.getGreen() + body.color.getGreen())/2, (merger.color.getBlue() + body.color.getBlue())/2);
				
				universe.bodies.remove(body);
			}
			
			if(collisionSet.size() == 0) continue;
			
			merger.position.x /= collisionSet.size();
			merger.position.y /= collisionSet.size();
			
			universe.bodies.add(merger);

			//notify listeners about the collision
			for(BodyCollisionListener listener : registeredBodyCollisionListeners)
				listener.onBodyCollision(collisionSet, merger);
		}
	}
	
	public void step()
	{
		//update positions
		solver.step();
		
		//resolve collisions
		resolveCollisions();
	}
	
	
	public void changeReferenceFrameTo(Set<Body2D> reference)
	{
		//compute center of mass of 'reference'
		//compute total momentum for 'reference'
		//subtract from all bodies the position and speed
	}
	
	/** Safe way to change the universe instance. */
	public void setUniverse(Universe2D u)
	{
		universe = u;
		solver.universe = u;
	}
	
	/** Safe way to change solver instance. The solver's Universe2D instance will be replaced by this instance's Univers2D */
	public void setSolver(Physics2DSolver s)
	{
		if(solver != null)
		{
			s.timeElapsed = solver.timeElapsed;
			s.timestep = solver.timestep;
		}
		s.universe = universe;
		solver = s;
	}
	
	
	//collision listeners and class
	public static interface BodyCollisionListener {
		public void onBodyCollision(HashSet<Body2D> collidingSet, Body2D resultingMerger);
	}
	
	private Set<BodyCollisionListener> registeredBodyCollisionListeners = new HashSet<BodyCollisionListener>();
	
	public void addBodyCollisionListener(BodyCollisionListener listener) {
		registeredBodyCollisionListeners.add(listener);
	}
	
	public void removeBodyCollisionListener(BodyCollisionListener listener){
		registeredBodyCollisionListeners.remove(listener);
	}
}
