package logic.solvers;

import model.physics.Body2D;
import model.physics.Universe2D;

public 
class EulerCromerSolver
extends Physics2DSolver
{
	public 
	EulerCromerSolver(Universe2D u) 
	{
		super(u);
		timestep = 0.1;
		displayName = "Semi-implicit Euler (Euler-Cromer)";
	}
	
	@Override
	public Physics2DSolver getNewInstance(Universe2D u) {
		return new EulerCromerSolver(u);
	}

	@Override public 
	void step() 
	{
		computeAllBodiesAccelerations();
		
		//velocity & position loop
		for(Body2D b1 : universe.bodies)
		{
			b1.velocity.add(b1.acceleration.times(timestep));
			b1.position.add(b1.velocity.times(timestep));
		}
		
		timeElapsed += timestep;
	}
}
