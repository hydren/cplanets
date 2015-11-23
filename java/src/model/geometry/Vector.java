package model.geometry;

import java.io.Serializable;

/** A class that represents a Euclidean vector */
public abstract class Vector implements Cloneable, Serializable
{	
	private static final long serialVersionUID = -7946854556131024523L;

	/** Returns the number of dimensions of this Vector 
	@return the number of dimensions */
	public abstract long getNumberOfDimensions();
	
	/** Creates an array of unit vectors corresponding to the canonical base from which this Vector is based.
	@return An array of versors in the order from which this Vector coordinates are represented. */
	public abstract Vector[] getCanonicalBase();
	
	/** Creates an array with this Vectors coordinates.
	@return An array with this Vectors coordinates in correct order.  */
	public abstract double[] getCoordinates();
	
	/** Returns the length/magnitude of this vector. */
	public abstract double magnitude();
	
	/** Returns the length/magnitude of this vector. */
	public double length() { return magnitude(); }
	
	/** Returns true if this vector is a null/zero vector. 
	@return true if this vector is a null/zero vector; false otherwise */
	public abstract boolean isZero();
	
	/** Creates a vector with length 1 and same direction as this vector. In other words, a new vector that is a normalized version of this vector. Note that <b>the original vector remains unchanged</b>.
	@return a <b>new</b> Vector instance with length 1 and same direction as <b>this</b> vector. */
	public abstract Vector unit();
	
	/** Divides this vector's coordinates by its length/magnitude, normalizing it.
	<br> The returned object is <b>the vector instance itself</b> after normalization. 
	@return <b>this</b> vector normalized. */
	public abstract Vector normalize();
	
	/** Creates the opposite of this vector. In other words, returns a vector with same coordinates as this, but with changed signal. Note that <b>the original vector remains unchanged</b>. 
	@return a <b>new</b> Vector instance with opposite direction.*/
	public abstract Vector opposite();
	
	/** Changes the signal of this vector coordinates, effectively reflecting it. 
	<br> The returned object is <b>the vector instance itself</b> after reflection. 
	@return <b>this</b> vector after reflection. */
	public abstract Vector reflect();
	
	/** Creates a vector that represents the sum of this vector and the given vector. Note that <b>the original vector remains unchanged</b>.
	@param v the summand vector instance
	@return a <b>new</b> Vector instance that is the sum of this vector and the given vector, when applicable.*/
	public abstract Vector sum(Vector v);
	
	/** Adds to this vector the given vector. In other words, it performs an addition to this vector coordinates.
	 <br> The returned object is <b>the vector instance itself</b> after summation. 
	@param v the summand vector instance 
	@return <b>this</b> vector after summation with the given vector. */
	public abstract Vector add(Vector v);
	
	/** Creates a vector that represents the difference/displacement of this vector and the given vector, in this order. It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a. 
	<br> Note that <b>the original vector remains unchanged</b>. 
	@param v the subtrahend vector instance 
	@return a <b>new</b> Vector instance that is the difference between this vector and the given vector*/
	public abstract Vector difference(Vector v);
	
	/** Subtracts from this vector the given vector. In other words, it performs an subtraction to this vector coordinates. 
	It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
	<br> The returned object is the <b>the vector instance itself</b> after subtraction.
	@param v the subtrahend vector instance  
	@return <b>this</b> vector after subtraction*/
	public abstract Vector subtract(Vector v); 
	
	/** Creates a vector that represents the scalar multiplication of this vector by the given factor. Note that <b>the original vector remains unchanged</b>.
	@param factor the factor from which multiply by 
	@return a <b>new</b> Vector instance that is this vector multiplied by the given factor */
	public abstract Vector times(double factor);
	
	/** Multiply this vectors coordinates by the given factor. The returned object is <b>the vector instance itself</b> after multiplication.
	@param factor the factor from which multiply by 
	@return <b>this</b> vector after multiplication*/
	public abstract Vector scale(double factor);
	
	/** Compute the distance between this vector and the given vector. In other words, returns the length/magnitude of the displacement between this and the given vector.
	@param v a vector from which to compute the distance.
	@return the distance between this vector and the given vector*/
	public abstract double distance(Vector v);
	
	/** Compute the inner/dot product between this and the given vector. 
	@param v a vector from which to compute the inner product
	@return the inner/dot product of this and the given vector*/
	public abstract double innerProduct(Vector v);
	
	/** Compute the vector/cross product between this and the given vectors.
	The given array should have <i>n - 2</i> elements, where <i>n</i> is the number of dimensions of the vectors, 
	as the cross product is only defined as a <i>(n - 1)</i>-ary operation on <i>n</i>-dimensional vectors,
	and this vector itself is the first element of the product p(this, v[0], v[1], ...).
	Note that in the case of 3-dimensional vectors, the operation is the ordinary 3-dimensional cross product, and the given array must have <b>only one</b> element. 
	@param vectors the array of vectors to perform the product, together with this vector, as first argument 
	@return a <b>new</b> vector instance that is perpendicular to this vector and all given ones. */
	public abstract Vector vectorProduct(Vector[] vectors);
}
