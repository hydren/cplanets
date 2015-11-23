package logic.solvers;

import java.util.ArrayList;
import java.util.List;

public class AvailableSolvers 
{
	public static
	List<Physics2DSolver> list = new ArrayList<Physics2DSolver>();
	static
	{
		list.add(new EulerSolver(null));
		list.add(new LeapfrogSolver(null));
		list.add(new SemiImplicitEulerSolver(null));
		list.add(new EulerCromerSolver(null));
		list.add(new HeunSolver(null));
		list.add(new RungeKuttaOrder4Solver(null));
	}
	
	/** Returns a solver by its class name. 
	 *  The solver returned by this method serves as helpers to access the {@link Physics2DSolver#getNewInstance(Universe2D) getNewInstance} method. 
	 *  <b> It's strongly unadvised to use these instances directly. </b> */
	public static
	Physics2DSolver getSolverByClassName(String className)
	{
		if(className.equalsIgnoreCase("default"))
			className = LeapfrogSolver.class.getName();
		
		for(Physics2DSolver s : list)
			if(s.getClass().getName().equalsIgnoreCase(className))
				return s;
		
		return null;
	}
}
