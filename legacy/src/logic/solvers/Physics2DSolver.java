package logic.solvers;

import model.geometry.Vector2D;
import model.physics.Body2D;
import model.physics.Universe2D;

public abstract 
class Physics2DSolver 
{
	public
	String displayName;
	
	public 
	double timestep;
	
	public
	double timeElapsed=0;
	
	public
	Universe2D universe;
	
	protected 
	Physics2DSolver() {}
	
	protected 
	Physics2DSolver(Universe2D u)
	{
		universe = u;
	}
	
	public abstract
	void step();
	
	/** Helper method to create an instance of this solver. */
	public abstract
	Physics2DSolver getNewInstance(Universe2D u);
	
	protected
	void computeAllBodiesAccelerations()
	{
		for(Body2D b1 : universe.bodies)
		{
			if(b1.acceleration == null)
				b1.acceleration = new Vector2D();
			else
				b1.acceleration.scale(0);
			
			for(Body2D b2 : universe.bodies)
			{
				if(b1 != b2)
				{
					final double force = -universe.gravity*b1.mass*b2.mass/Math.pow(b1.position.distance(b2.position), 2);
					b1.acceleration.add(b1.position.difference(b2.position).normalize().scale(force/b1.mass));
				}
			}
		}
	}
	
	protected
	Vector2D calculateAccelerationDueToNeighborhood(Vector2D position, Body2D body)
	{
		Vector2D acc = new Vector2D();
		for(Body2D b2 : universe.bodies)
		{
			if(b2 != body)
			{
				final double force = -universe.gravity*body.mass*b2.mass/Math.pow(position.distance(b2.position), 2);
				acc.add(position.difference(b2.position).normalize().scale(force/body.mass));
			}
		}
		return acc;
	}
	
	@Override public
	String toString()
	{
		return displayName;
	}
}
