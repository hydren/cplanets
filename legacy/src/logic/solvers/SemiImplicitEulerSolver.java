package logic.solvers;

import model.physics.Body2D;
import model.physics.Universe2D;

public class SemiImplicitEulerSolver extends Physics2DSolver
{
	public 
	SemiImplicitEulerSolver(Universe2D u) 
	{
		super(u);
		timestep = 0.1;
		displayName = "Semi-implicit Euler";
	}
	
	@Override
	public Physics2DSolver getNewInstance(Universe2D u) {
		return new SemiImplicitEulerSolver(u);
	}
	
	@Override
	public void step()
	{
		//position loop
		for(Body2D b1 : universe.bodies)
		{
			b1.position.add(b1.velocity.times(timestep));
		}
		
		computeAllBodiesAccelerations();
		
		//velocity loop
		for(Body2D b1 : universe.bodies)
		{
			b1.velocity.add(b1.acceleration.times(timestep));
			b1.position.add(b1.velocity.times(timestep));
		}
		
		timeElapsed += timestep;
	}

}
