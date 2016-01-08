package model.physics;

import java.awt.Color;
import java.io.Serializable;
import java.util.Random;

import util.Util;

public abstract class Body implements Cloneable, Serializable
{
	private static final long serialVersionUID = 5605771961032795093L;

	public double mass=0;
	
	public double diameter=0;
	
	public Color color=null;
	
	public String id=null;
	
	public Object userObject=null;
	
	/** Creates a body with zero mass and diameter. ID and color are runtime generated. */
	public Body()
	{
		id = generateRandomName();
		color = Util.generateRandomColor();
	}
	
	/** Creates a body with zero mass and diameter, specifying ID and color. */
	public Body(String id, Color c)
	{
		this.id = id;
		this.color = c;
	}
	
	/** Creates a body specifying mass and diameter. ID and color are runtime generated. */
	public Body(double mass, double diameter) 
	{
		this();
		this.mass = mass;
		this.diameter = diameter;
	}
	
	/** Creates a body specifying all attributes. */
	public Body(String id, double mass, double diameter, Color color, Object userObject) 
	{
		this.id = id;
		this.mass = mass;
		this.diameter = diameter;
		this.color = color;
		this.userObject = userObject;
	}

	/** Copy constructor (deep). Creates a new Body instance with the same properties (including ID). */
	public Body(Body b)
	{
		mass = b.mass;
		diameter = b.diameter;
		color = b.color;
		id = b.id;
		userObject = b.userObject;
	}
	
	@Override
	public String toString()
	{
		return id + " ("+String.format("%.4g", mass)+"Kg, "+String.format("%.4g", diameter)+"Km)";
	}
	
	public static String generateRandomName()
	{
		return "B"+String.format("%04d", new Random().nextInt(10000));
	}
}
