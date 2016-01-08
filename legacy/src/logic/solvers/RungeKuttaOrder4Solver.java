package logic.solvers;

import java.util.HashMap;

import model.geometry.Vector2D;
import model.physics.Body2D;
import model.physics.Universe2D;

public 
class RungeKuttaOrder4Solver 
extends Physics2DSolver 
{
	private 
	HashMap<Body2D, Vector2D[]> ktable = new HashMap<Body2D, Vector2D[]>();
	
	public 
	RungeKuttaOrder4Solver(Universe2D u) 
	{
		super(u);
		timestep = 0.3;
		displayName = "Runge-Kutta (4th Order) (experimental!)";
	}
	
	@Override public 
	Physics2DSolver getNewInstance(Universe2D u) 
	{
		return new RungeKuttaOrder4Solver(u);
	}

	@Override public 
	void step() 
	{
		//clears the k-table. TODO maybe a refined 'clear' could avoid clearing keys that would be used right after
		ktable.clear();
			
		//compute Runge-Kutta terms for all bodies before updating position/velocity
		for(Body2D b : universe.bodies)
		{	
			Vector2D[] k = new Vector2D[8];
			
			k[0] = calculateAccelerationDueToNeighborhood(b.position, b).scale(timestep);
			k[1] = new Vector2D(b.velocity).scale(timestep);
			k[2] = calculateAccelerationDueToNeighborhood(k[1].times(0.5).add(b.position), b).scale(timestep);
			k[3] = k[0].times(0.5).add(b.velocity).scale(timestep);
			k[4] = calculateAccelerationDueToNeighborhood(k[3].times(0.5).add(b.position), b).scale(timestep);
			k[5] = k[2].times(0.5).add(b.velocity).scale(timestep);
			k[6] = calculateAccelerationDueToNeighborhood(k[5].sum(b.position), b).scale(timestep);
			k[7] = k[4].sum(b.velocity).scale(timestep);
			
			ktable.put(b, k);
		}
		
		for(Body2D b : universe.bodies)
		{
			Vector2D[] k = ktable.get(b);
			b.velocity.add( k[0] .add(k[2].scale(2d)) .add(k[4].scale(2d)) .add(k[6]) .scale(1/6d) );
			b.position.add( k[1] .add(k[3].scale(2d)) .add(k[5].scale(2d)) .add(k[7]) .scale(1/6d) );
		}
		
		//just to have acceleration vectors filled
		computeAllBodiesAccelerations();
	}
	
	//	private Vector2D acceleration(Vector2D p, Body2D b) { return calculateAccelerationDueToNeighborhood(p, b); }
	
	/* Old version
	 //compute Runge-Kutta terms for all bodies before updating position/velocity
		Vector2D p1, p2, p3, p4, v1, v2, v3, v4, a1, a2, a3, a4;
		for(Body2D b : universe.bodies)
		{	
			p1 = b.position.clone();
			v1 = b.velocity.clone();
			a1 = acceleration( p1, b);
			p2 = p1.sum(v1.times(timestep*0.5));
			v2 = v1.sum(a1.times(timestep*0.5));
			a2 = acceleration( p2, b);
			p3 = p1.sum(v2.times(timestep*0.5));
			v3 = v1.sum(a2.times(timestep*0.5));
			a3 = acceleration( p3, b);
			p4 = p1.sum(v3.times(timestep));
			v4 = v1.sum(a3.times(timestep));
			a4 = acceleration( p4, b);
			
			Vector2D[] k = new Vector2D[8];
			k[0] = v1; k[1] = v2; k[2] = v3; k[3] = v4;
			k[4] = a1; k[5] = a2; k[6] = a3; k[7] = a4;
			ktable.put(b, k);
		}
		
		for(Body2D b : universe.bodies)
		{
			Vector2D[] k = ktable.get(b);
			v1 = k[0]; v2 = k[1]; v3 = k[2]; v4 = k[3];
			a1 = k[4]; a2 = k[5]; a3 = k[6]; a4 = k[7];
			
			b.position.add ((v1 .add(v2.scale(2.0)) .add(v3.scale(2.0)) .add(v4)).scale(timestep / 6.0));
			b.velocity.add ((a1 .add(a2.scale(2.0)) .add(a3.scale(2.0)) .add(a4)).scale(timestep / 6.0));
		} 
	 */

}
