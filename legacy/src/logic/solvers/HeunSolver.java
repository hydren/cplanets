package logic.solvers;

import java.util.HashMap;

import model.geometry.Vector2D;
import model.physics.Body2D;
import model.physics.Universe2D;

public class HeunSolver extends Physics2DSolver 
{
	private 
	HashMap<Body2D, Vector2D[]> ktable = new HashMap<Body2D, Vector2D[]>();
	private Vector2D acceleration(Vector2D p, Body2D b) { return calculateAccelerationDueToNeighborhood(p, b); }
	
	public 
	HeunSolver(Universe2D u) 
	{
		super(u);
		timestep = 0.2;
		displayName = "Heun (RK2) (experimental!)";
	}
	
	@Override public 
	void step() 
	{
		//clears the k-table. TODO maybe a refined 'clear' could avoid clearing keys that would be used right after
		ktable.clear();
		
		Vector2D p1, p2, v1, v2, a1, a2;
			
		//compute Runge-Kutta terms for all bodies before updating position/velocity
		for(Body2D b : universe.bodies)
		{	
			p1 = b.position.clone();
			v1 = b.velocity.clone();
			a1 = acceleration( p1, b);
			p2 = p1.sum(v1.times(timestep));
			v2 = v1.sum(a1.times(timestep));
			a2 = acceleration( p2, b);
			
			Vector2D[] k = new Vector2D[4];
			k[0] = v1; k[1] = v2;
			k[2] = a1; k[3] = a2;
			
			ktable.put(b, k);
		}
		
		for(Body2D b : universe.bodies)
		{
			Vector2D[] k = ktable.get(b);
			v1 = k[0]; v2 = k[1];
			a1 = k[2]; a2 = k[3];
			
			b.position.add((v1.add(v2)).scale(timestep*0.5));
			b.velocity.add((a1.add(a2)).scale(timestep*0.5));
		}
		
		//just to have acceleration vectors filled
		computeAllBodiesAccelerations();
	}

	@Override public 
	Physics2DSolver getNewInstance(Universe2D u) 
	{
		return new HeunSolver(u);
	}

}
