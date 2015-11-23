package logic.solvers;

import model.physics.Body2D;
import model.physics.Universe2D;

public 
class LeapfrogSolver 
extends Physics2DSolver
{
	public 
	LeapfrogSolver(Universe2D u) 
	{
		super(u);
		timestep = 0.1;
		displayName = "Leapfrog";
	}
	
	@Override
	public Physics2DSolver getNewInstance(Universe2D u) {
		return new LeapfrogSolver(u);
	}

	@Override
	public void step() {
		computeAllBodiesAccelerations();

		for(Body2D b1 : universe.bodies)
		{
			if(timeElapsed == 0)
				b1.velocity.add(b1.acceleration.times(timestep/2));
			else
				b1.velocity.add(b1.acceleration.times(timestep));
			
			b1.position.add(b1.velocity.times(timestep));
		}
		
		timeElapsed += timestep;
	}

}
