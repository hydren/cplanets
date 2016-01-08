package model.geometry;

public class Vector2D extends Vector
{
	private static final long serialVersionUID = 4465655714316563553L;

	/** The coordinates */
	public double x, y;
	
	/** Creates a Vector2D with the given coordinates */
	public Vector2D(double x, double y)
	{
		this.x = x;
		this.y = y;
	}
	
	/** Creates a Vector2D with the (0, 0) coordinates */	
	public Vector2D()
	{
		this.x = 0;
		this.y = 0;
	}
	
	/** Copy constructor */
	public Vector2D(Vector2D v)
	{
		this.x = v.x;
		this.y = v.y;
	}
	
	/** Returns true if the given object is a instance of Vector2D and its coordinates equals the ones from this */
	@Override
	public boolean equals(Object object)
	{
		return object instanceof Vector2D? equals((Vector2D) object) : false;
	}
	
	/** Returns true if the given vector coordinates equals the ones from this */
	public boolean equals(Vector2D vector)
	{
		return vector.x == x && vector.y == y;
	}
	
	@Override
	public String toString()
	{
		return "(" + x + ", " + y + ")";
	}
	
	@Override
	public long getNumberOfDimensions() 
	{
		return 2;
	}

	@Override
	public Vector[] getCanonicalBase() 
	{
		return new Vector[]{ X_VERSOR.clone(), Y_VERSOR.clone() };
	}

	@Override
	public double[] getCoordinates()
	{
		return new double[]{x, y};
	}
	
	@Override
	public Vector2D clone()
	{
		return new Vector2D(x, y);
	}
	
	@Override
	public double magnitude()
	{
		return Math.sqrt( x*x + y*y );
	}
		
	@Override
	public boolean isZero()
	{
		return x == 0 && y == 0;
	}	

	@Override
	public Vector2D unit()
	{
		return isZero()? ZERO : new Vector2D(x/magnitude(), y/magnitude());
	}
	
	@Override
	public Vector2D normalize()
	{
		if(! isZero())
		{
			final double length = magnitude();
			x /= length;
			y /= length;
		}
		return this;
	}
	
	/** {@inheritDoc} In this case, returns a vector with coordinates (-this.x, -this.y). */
	@Override
	public Vector2D opposite()
	{
		return new Vector2D(-this.x, -this.y);
	}
	
	@Override
	public Vector2D reflect()
	{
		x = -x;
		y = -y;
		return this;
	}

	/** Changes the signal of this vector's X coordinate, effectively reflecting it relative to the Y axis. 
		The returned object is the vector instance itself after reflection. */
	public Vector2D reflectX()
	{
		x = -x;
		return this;
	}

	/** Changes the signal of this vector's Y coordinate, effectively reflecting it relative to the X axis. 
	The returned object is the vector instance itself after reflection. */
	public Vector2D reflectY()
	{
		y = -y;
		return this;
	}
		
	@Override
	public Vector2D sum(Vector v)
	{
		Vector2D v2d = (Vector2D) v;
		return new Vector2D(this.x + v2d.x, this.y + v2d.y);
	}
	
	@Override
	public Vector2D add(Vector v)
	{
		Vector2D v2d = (Vector2D) v;
		this.x += v2d.x;
		this.y += v2d.y;
		return this;
	}

	@Override
	public Vector2D difference(Vector v)
	{
		return  ((Vector2D) v).opposite().add(this);
	}
	
	@Override
	public Vector2D subtract(Vector v)
	{
		Vector2D v2d = (Vector2D) v;
		this.x -= v2d.x;
		this.y -= v2d.y;
		return this;
	}
	
	@Override
	public Vector2D times(double factor){
		return new Vector2D(this.x * factor, this.y * factor);
	}
	
	@Override
	public Vector2D scale(double factor)
	{
		x *= factor;
		y *= factor;
		return this;
	}
	
	@Override
	public double distance(Vector vector)
	{
		return this.difference(vector).magnitude();
	}
	
	@Override
	public double innerProduct(Vector v)
	{
		Vector2D v2d = (Vector2D) v;
		return this.x*v2d.x + this.y*v2d.y;
	}
	
	public double innerProduct(Vector2D v)
	{
		return this.x*v.x + this.y*v.y;
	}
	
	@Override
	public Vector2D vectorProduct(Vector[] vectors) 
	{
		return new Vector2D(-this.x, this.y);
	}
	
	
	public Vector2D reflection()
	{
		//TODO
		return null;
	}
	
	public Vector2D projection(Vector2D v)
	{
		return v.unit().scale(this.innerProduct(v)/(v.innerProduct(v)));
	}
	
	public Vector2D rejection()
	{
		//TODO
		return null;
	}
	
	public Vector2D rotation()
	{
		//TODO
		return null;
	}
	
	public Vector2D rotate()
	{
		//TODO
		return null;
	}
	
	public Vector2D perpendicular()
	{
		//TODO
		return null;
	}
	
	/** Represents the null/zero vector. It has coordinates (0, 0). */
	public static final Vector2D ZERO  = new Vector2D(), NULL_VECTOR=ZERO;
	
	/** A vector codirectional with the X axis, with length 1. */
	public static final Vector2D X_VERSOR = new Vector2D(1, 0);

	/** A vector codirectional with the Y axis, with length 1. */
	public static final Vector2D Y_VERSOR = new Vector2D(0, 1);
}
