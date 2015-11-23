package view;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.geom.Line2D;
import java.awt.geom.QuadCurve2D;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.JPanel;

import logic.Physics2D;
import model.geometry.Vector2D;
import model.physics.Body2D;
import util.Util;
import controller.Main;

public class PlanetariumPane extends JPanel implements FocusListener
{
	private static final long serialVersionUID = -1308255186400979996L;
	
	public enum OrbitTracerType { POINT, LINEAR, SPLINE }
	
	private Physics2D physics;
	private Vector2D viewportPosition;
	private boolean running = false;
	private long sleepingTime = 25;
	
	private Vector2D newBodyPosition, newBodyVelocity;
	private Vector2D selectionInitialPosition, selectionFinalPosition;
	
	private double zoom = 1, minimumSize = 3;
	private boolean focused=false, usingAA=false;
	
	private Set<Body2D> focusedBodies = new HashSet<Body2D>();
	
	private BasicStroke normalStroke = new BasicStroke(1), focusedStroke = new BasicStroke(2);

	//possibly inefficient aproach
	private static class OrbitTracer
	{
		OrbitTracerType type = OrbitTracerType.POINT;
		
		boolean active=false;
		int traceLength = 20;		
		private Map<Body2D, ArrayDeque<Vector2D>> orbitTrace = new HashMap<Body2D, ArrayDeque<Vector2D>>();
		
		public void registerPosition(Body2D b, Vector2D pos)
		{
			ArrayDeque<Vector2D> fifo = orbitTrace.get(b);
			
			if(fifo == null)
			{
				fifo = new ArrayDeque<Vector2D>();
				orbitTrace.put(b, fifo);
			}
			
			fifo.addFirst(new Vector2D(pos.x, pos.y));
			
			while(fifo.size() > traceLength)
				fifo.removeLast();
		}
		
		public ArrayDeque<Vector2D> getTrace(Body2D b)
		{
			ArrayDeque<Vector2D> fifo = orbitTrace.get(b);
			if(fifo == null)
				return new ArrayDeque<Vector2D>();
			else
				return fifo;
		}
	}
	private OrbitTracer tracer = new OrbitTracer();
	
	public PlanetariumPane()
	{
		super();
		physics = new Physics2D();
		viewportPosition = new Vector2D();
		setBackground(Color.black);
		addFocusListener(this);
	}
	
	public PlanetariumPane(Physics2D physics)
	{
		this();
		this.physics = physics;
	}
	
	@Override
	protected void paintComponent(Graphics graphics) 
	{
        super.paintComponent(graphics);   // paints background (or should)
        Graphics2D graphics2d = (Graphics2D) graphics;
        
        //set AA hint
        if(usingAA) graphics2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
        
        //draw orbits
        if(tracer.active){ synchronized (physics) { 
        for(Body2D body : physics.universe.bodies)
        {
        	graphics2d.setColor( body.color );
        	
        	switch(tracer.type)
        	{
        		case POINT:
        		{
        			for(Vector2D p : tracer.getTrace(body))
            		{
            			final Vector2D u = getTransposed(p);
                    	final int ux = (int) Math.round(u.x);
                    	final int uy = (int) Math.round(u.y);
                    	
   						graphics2d.drawLine(ux, uy, ux, uy);
            		}
        			break;
        		}
        		case LINEAR:
        		{
        			final Vector2D v = getTransposed2(body.position);
                	int previous_ux = (int) Math.round(v.x);
                	int previous_uy = (int) Math.round(v.y);
            		for(Vector2D p : tracer.getTrace(body))
            		{
            			final Vector2D u = getTransposed(p);
                    	final int ux = (int) Math.round(u.x);
                    	final int uy = (int) Math.round(u.y);
                    	
						graphics2d.drawLine(previous_ux, previous_uy, ux, uy);
						previous_ux = ux;
						previous_uy = uy;
            		}
        			break;
        		}
        		case SPLINE:
        		{
        			final Vector2D v = getTransposed(body.position);
        			double ux1 = Math.round(v.x);
        			double uy1 = Math.round(v.y);
        			double ux0=0, uy0=0;
        			boolean ready = false;
        			
        			for(Vector2D p : tracer.getTrace(body))
            		{
            			final Vector2D u = getTransposed(p);
                    	final double ux2 = u.x;
                    	final double uy2 = u.y;
                    	
                    	if(ready)
                    	{
                    		QuadCurve2D curve = new QuadCurve2D.Double();
                    		curve.setCurve(ux0, uy0, ux1, uy1, ux2, uy2);
                    		graphics2d.draw(curve);
                    	}
                    	else
                    	{
                    		Line2D line = new Line2D.Double();
                    		line.setLine(ux1, uy1, ux2, uy2);
                    		graphics2d.draw(line);                    		
                    	}
                    	
						ux0 = ux1; uy0 = uy1;
						ux1 = ux2; uy1 = uy2;
            		}
        			
        			break;
        		}
        		
        		default: break;
        	}
        }}}
        
        //draw bodies
        synchronized (physics) { 
        for(Body2D body : physics.universe.bodies)
        {
        	double size = zoom*body.diameter;
        	if(size < minimumSize) size = minimumSize;
        	
        	final Vector2D v = getTransposed2(body.position);
        	final int x = (int) Math.round(v.x - size/2 );
        	final int y = (int) Math.round(v.y - size/2 );
 	
        	graphics2d.setColor( body.color );
        	graphics2d.fillOval(x, y, (int) Math.round( size ), (int) Math.round ( size ));
        	
        	if(focusedBodies.contains(body))
        	{
        		graphics2d.setStroke(focusedStroke);
        		graphics2d.setColor( Color.ORANGE );        		
        	}
        	else
        		graphics2d.setColor( Color.WHITE );
        	
        	graphics2d.drawOval(x, y, (int) Math.round( size ), (int) Math.round ( size ));
    		graphics2d.setStroke(normalStroke);
    		
    		if(running)
        		tracer.registerPosition(body, getTransposed3(body.position));
        }}
        
        //if there is a new body being created
        if(newBodyPosition != null && newBodyVelocity != null)
        {
        	double size = Main.settings.newBodyDiameterRatio*100;
        	if(size < minimumSize) size = minimumSize;
        	
        	final Vector2D d = getTransposed2(newBodyPosition);
        	final int x = (int) Math.round(d.x - size/2 );
        	final int y = (int) Math.round(d.y - size/2 );
        	
        	graphics2d.setColor(Color.WHITE);
        	graphics2d.drawOval(x, y, (int) Math.round( size ), (int) Math.round( size ));
        	graphics2d.drawLine((int)d.x, (int)d.y, (int) Math.round(d.x + newBodyVelocity.x*zoom), (int) Math.round(d.y + newBodyVelocity.y*zoom));
        }
        
        if(selectionInitialPosition != null && selectionFinalPosition != null)
        {
        	graphics2d.setColor(Color.WHITE);
        	int x, y, w, h;
        	
        	if(selectionInitialPosition.x <= selectionFinalPosition.x)
        		x = (int) getTransposed2(selectionInitialPosition).x;
        	else
        		x = (int) getTransposed2(selectionFinalPosition).x;
        	
        	if(selectionInitialPosition.y <= selectionFinalPosition.y)
        		y = (int) getTransposed2(selectionInitialPosition).y;
        	else
        		y = (int) getTransposed2(selectionFinalPosition).y;

        	w = Math.abs((int)((selectionInitialPosition.x - selectionFinalPosition.x)*zoom));
        	h = Math.abs((int)((selectionInitialPosition.y - selectionFinalPosition.y)*zoom));
        	
        	graphics2d.drawRect(x, y, w, h);
        }
        
        //draw focus border
        if(focused)
        {
        	graphics2d.setColor(Color.ORANGE);
        	graphics2d.drawRect(0, 0, this.getWidth()-1, this.getHeight()-1);
        	graphics2d.drawRect(1, 1, this.getWidth()-3, this.getHeight()-3);
        }
        
        System.out.println("[DEBUG] ReFr: " + physics.referenceFrame.getPosition());
        
        graphics2d.setColor(Color.BLACK); // restore default color
	}

	Thread threadPhysics = new Thread()
	{
		@Override
		public void run() {
			while(true)
			{
				if(running) synchronized (physics) 
				{
					physics.step();
				}

				sleepFor(sleepingTime);
			}
		}
	};
	
	
	public Vector2D getTransposed3(Vector2D position)
	{
		if(!physics.referenceFrame.bodies.isEmpty())
			return position.difference(physics.referenceFrame.getPosition());
		else
			return new Vector2D(position);
	}
	
	public Vector2D getTransposed2(Vector2D position)
	{
		if(!physics.referenceFrame.bodies.isEmpty())
			return position.difference(physics.referenceFrame.getPosition()).subtract(viewportPosition).scale(zoom);
		else
			return position.difference(viewportPosition).scale(zoom);
	}
	
	public Vector2D getTransposed(Vector2D position)
	{
		return position.difference(viewportPosition).scale(zoom);
	}

	/** Adds the current creating body to the universe */
	public void addCurrentCreatingBody()
	{
		Body2D body = new Body2D();
		body.position = newBodyPosition;
		body.velocity = newBodyVelocity;
		body.diameter = 100d * Main.settings.newBodyDiameterRatio/zoom;
		body.mass = (Math.PI/6d) * Main.settings.newBodyDensity * body.diameter * body.diameter * body.diameter;
		//TODO set velocity to orbit: ve = sqrt(2GM/r)
		physics.universe.bodies.add(body);
		
		//reset new body drawing
		newBodyPosition = null;
		newBodyVelocity = null;
	}
	
	/** Assign a new random color to every body on the current universe */
	public void recolorAllBodies()
	{
		for(Body2D body : physics.universe.bodies)
			body.color = Util.generateRandomColor();
	}
	
	/** Set the bodies inside the selection as focused */
	public void focusBodiesFromSelectionRect()
	{
		if(selectionInitialPosition == null || selectionFinalPosition == null)
			return; //safety
		
		for(Body2D body : physics.universe.bodies)
		{
			if( selectionInitialPosition.x < body.position.x && body.position.x < selectionFinalPosition.x 
			&&  selectionInitialPosition.y < body.position.y && body.position.y < selectionFinalPosition.y)
			{
				focusedBodies.add(body);
			}
			else focusedBodies.remove(body);
		}
	}
	
	public void attemptSelectionFromClick(int mouseX, int mouseY)
	{
		focusedBodies.clear();
		Vector2D point = new Vector2D(mouseX, mouseY);
		point.scale(1d/zoom);
		point.add(viewportPosition);
		
		for(Body2D body : physics.universe.bodies)
		{
			if( point.distance(body.position) < body.diameter/2d )
			{
				focusedBodies.add(body);
				break;
			}
		}
	}
	
	public void sleepFor(long ms)
	{
		try {
			Thread.sleep(ms);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	///// Listeners
	
	@Override
	public void focusGained(FocusEvent arg0) {
		focused=true;
		repaint();
	}

	@Override
	public void focusLost(FocusEvent arg0) {
		focused=false;
		repaint();
	}

	//############################################ GETTERS & SETTERS ##################################################
	
	public Vector2D getViewportPosition() {
		return viewportPosition;
	}

	public void setViewportPosition(Vector2D vector) {
		this.viewportPosition = vector;
	}

	public double getZoom() {
		return zoom;
	}

	public void setZoom(double zoom) {
		this.zoom = zoom;
	}
	
	public Physics2D getPhysics() {
		return physics;
	}

	public void setPhysics(Physics2D physics) {
		this.physics = physics;
	}
	
	public boolean isFocused() {
		return focused;
	}

	public void setFocused(boolean focused) {
		this.focused = focused;
	}

	public double getMinimumRadius() {
		return minimumSize;
	}

	public void setMinimumRadius(double minimumRadius) {
		this.minimumSize = minimumRadius;
	}

	public boolean isUsingAA() {
		return usingAA;
	}

	public void setUsingAA(boolean usingAA) {
		this.usingAA = usingAA;
	}

	public Vector2D getNewBodyPosition() {
		return newBodyPosition;
	}

	public void setNewBodyPosition(Vector2D newBodyPosition) {
		this.newBodyPosition = newBodyPosition;
	}

	public Vector2D getNewBodyVelocity() {
		return newBodyVelocity;
	}

	public void setNewBodyVelocity(Vector2D newBodyVelocity) {
		this.newBodyVelocity = newBodyVelocity;
	}

	public int getTraceLength() {
		return tracer.traceLength;
	}

	public void setTraceLength(int traceLength) {
		this.tracer.traceLength = traceLength;
	}

	public boolean isTracing() {
		return tracer.active;
	}

	public void setTracing(boolean tracing) {
		this.tracer.active = tracing;
	}
	
	public void setTracerType(OrbitTracerType type)
	{
		this.tracer.type = type;
	}
	
	public OrbitTracerType getTracerType()
	{
		return tracer.type;
	}
	
	public boolean isRunning() {
		return running;
	}

	public void setRunning(boolean running) {
		if(running == true && threadPhysics.isAlive()==false)
			threadPhysics.start();
		
		this.running = running;
	}
	
	public void toogleRunning()
	{
		running = ! running;
		
		if(running == true && threadPhysics.isAlive()==false)
			threadPhysics.start();
	}

	public long getSleepingTime() {
		return sleepingTime;
	}

	public void setSleepingTime(long sleepingTime) {
		this.sleepingTime = sleepingTime;
	}
	
	public Vector2D getSelectionInitialPosition() {
		return selectionInitialPosition;
	}

	public void setSelectionInitialPosition(Vector2D selectionInitialPosition) {
		this.selectionInitialPosition = selectionInitialPosition;
	}

	public Vector2D getSelectionFinalPosition() {
		return selectionFinalPosition;
	}

	public void setSelectionFinalPosition(Vector2D selectionFinalPosition) {
		this.selectionFinalPosition = selectionFinalPosition;
	}

	public Set<Body2D> getFocusedBodies() {
		return focusedBodies;
	}

	public void setFocusedBodies(Set<Body2D> focusedBodies) {
		this.focusedBodies = focusedBodies;
	}
}
