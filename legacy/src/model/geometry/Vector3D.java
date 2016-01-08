package model.geometry;

public class Vector3D extends Vector
{
	private static final long serialVersionUID = 5632320457981557910L;
	
	public double x, y, z;
	
	public Vector3D(double x, double y, double z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public Vector3D()
	{
		this(0,0,0);
	}
	
	@Override
	public boolean equals(Object object)
	{
		return object instanceof Vector3D? equals((Vector3D) object) : false;
	}
	
	public boolean equals(Vector3D vector)
	{
		return vector.x == x && vector.y == y && vector.z == z;
	}
	
	@Override
	public Vector3D clone()
	{
		return new Vector3D(this.x, this.y, this.z);
	}
	
	@Override
	public long getNumberOfDimensions() 
	{
		return 3;
	}

	@Override
	public Vector[] getCanonicalBase() 
	{
		return new Vector3D[]{ X_VERSOR.clone(), Y_VERSOR.clone(), Z_VERSOR.clone() };
	}

	@Override
	public double[] getCoordinates() {
		return new double[]{x, y, z};
	}
	
	@Override
	public double magnitude()
	{
		return Math.sqrt( x*x + y*y + z*z );
	}
	
	@Override
	public boolean isZero()
	{
		return x == 0 && y == 0 && z == 0;
	}

	@Override
	public Vector3D unit()
	{
		return isZero()? ZERO : new Vector3D(x/magnitude(), y/magnitude(), z/magnitude());
	}
	
	@Override
	public Vector3D normalize()
	{
		if(! isZero())
		{
			x /= magnitude();
			y /= magnitude();
			z /= magnitude();
		}
		return this;
	}
	
	@Override
	public Vector3D opposite()
	{
		return new Vector3D(-this.x, -this.y, -this.z);
	}
	
	@Override
	public Vector3D reflect()
	{
		x = -x;
		y = -y;
		z = -z;
		return this;
	}
		
	@Override
	public Vector3D sum(Vector vector)
	{
		Vector3D v3d = (Vector3D) vector;
		return new Vector3D(this.x + v3d.x, this.y + v3d.y, this.z + v3d.z);
	}
	
	@Override
	public Vector add(Vector v) 
	{
		Vector3D v3d = (Vector3D) v;
		this.x += v3d.x;
		this.y += v3d.y;
		this.z += v3d.z;
		return this;
	}

	@Override
	public Vector3D difference(Vector vector)
	{
		return sum(vector.opposite());
	}
	
	@Override
	public Vector subtract(Vector v) 
	{
		Vector3D v3d = (Vector3D) v;
		this.x -= v3d.x;
		this.y -= v3d.y;
		this.z -= v3d.z;
		return this;
	}
	
	@Override
	public Vector times(double factor) {
		return new Vector3D(this.x*factor, this.y*factor, this.z*factor);
	}

	@Override
	public Vector scale(double factor) {
		this.x *= factor;
		this.y *= factor;
		this.z *= factor;
		return this;
	}
	
	@Override
	public double innerProduct(Vector vector)
	{
		Vector3D v3d = (Vector3D) vector;
		return this.x*v3d.x + this.y*v3d.y + this.z*v3d.z;
	}
	
	@Override
	public Vector3D vectorProduct(Vector[] v_array)
	{
		Vector3D vector = (Vector3D) v_array[0];
		return new Vector3D(this.y*vector.z - this.z*vector.y, this.z*vector.x - this.x*vector.z, this.x*vector.y - this.y*vector.x);
	}
	
	@Override
	public double distance(Vector v) {
		return this.difference(v).magnitude();
	}
	
	public void reflectX()
	{
		x = -x;
	}
	
	public void reflectY()
	{
		y = -y;
	}
	
	public void reflectZ()
	{
		z = -z;
	}
		
	public static Vector3D ZERO  = new Vector3D();
	public static Vector3D X_VERSOR = new Vector3D(1, 0, 0);
	public static Vector3D Y_VERSOR = new Vector3D(0, 1, 0);
	public static Vector3D Z_VERSOR = new Vector3D(0, 0, 1);

}
