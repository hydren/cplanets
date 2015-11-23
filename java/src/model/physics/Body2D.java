package model.physics;

import java.awt.Color;

import model.geometry.Vector2D;

public class Body2D extends Body
{
	private static final long serialVersionUID = 6135122071836811390L;
	public Vector2D position, velocity, acceleration;
	
	public Body2D(double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
	{
		super(mass, diameter);
		this.position = position;
		this.velocity = velocity;
		this.acceleration = acceleration;
	}
	
	public Body2D(String id, Color c, double mass, double diameter, Vector2D position, Vector2D velocity, Vector2D acceleration)
	{
		super(id, mass, diameter, c, null);
		this.position = position;
		this.velocity = velocity;
		this.acceleration = acceleration;
	}
	
	public Body2D()
	{
		super(1, 1);
		position = new Vector2D();
		velocity = new Vector2D();
		acceleration = new Vector2D();
	}
	
	/** Copy constructor (deep). Creates a new Body2D instance with the same properties (including ID). */
	public Body2D(Body2D b)
	{
		super(b);
		position = new Vector2D(b.position);
		velocity = new Vector2D(b.velocity);
		acceleration = new Vector2D(b.acceleration);
	}
}
