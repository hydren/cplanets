package model.physics;

import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

public class Universe2D implements Serializable
{
	private static final long serialVersionUID = -2775556315146608059L;
	public Set<Body2D> bodies = new HashSet<Body2D>();
	public double gravity = 1;
	
	/** Creates a deep copy of the universe, including bodies' ID. */
	public Universe2D(Universe2D u)
	{
		gravity = u.gravity;
		for(Body2D b : bodies)
			u.bodies.add(new Body2D(b));
	}
	
	public Universe2D() {}
}
