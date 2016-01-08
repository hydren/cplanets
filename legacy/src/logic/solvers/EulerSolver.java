package logic.solvers;

import model.physics.Body2D;
import model.physics.Universe2D;

public 
class EulerSolver
extends Physics2DSolver
{
	public 
	EulerSolver(Universe2D u) 
	{
		super(u);
		timestep = 0.01;
		displayName = "Euler";
	}
	
	@Override
	public Physics2DSolver getNewInstance(Universe2D u) {
		return new EulerSolver(u);
	}

	@Override public 
	void step() 
	{
		computeAllBodiesAccelerations();
		
		//velocity & position loop
		for(Body2D b1 : universe.bodies)
		{
			b1.position.add(b1.velocity.times(timestep));
			b1.velocity.add(b1.acceleration.times(timestep));
		}
		
		timeElapsed += timestep;
	}
}
