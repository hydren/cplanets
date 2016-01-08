package model;

import java.awt.Dimension;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.Properties;

public class Settings implements Serializable
{
	/** Default generated value */
	private static final long serialVersionUID = 5882287971388427140L;
	
	// OVERALL APPLICATION RELATED

	/** The appearance applied on the application */
	public String theme="Metal";
	
	/** The window's last dimension used. */
	public Dimension lastWindowDimension=new Dimension(800,600);
	
	/** Controls whether use anti-aliasing when rendering the PlanetariumPane */
	public boolean usingAntiAliasing=false;
	
	/** Controls whether the movement speed of the camera is scaled together with all distances. In other words, if set true, zooming would also affects the movement speed of the camera. */
	public boolean viewMovementSpeedRelatedToScale=true;
	
	/** The view's movement speed when using arrows to pan. It may be relative to scale, depending on the variable viewMovementSpeedRelatedToScale. */
	public double viewMovementSpeed=15;
	
	public String solver="default";
	
	// BODY CREATION RELATED
	
	/** Controls whether all bodies created have same density or size-independent mass */
	public boolean fixedDensity=true;
	
	/** The default diameter of bodies, proportional to the view size. Zooming affects the diameter of newly created bodies. */
	public double newBodyDiameterRatio=0.3;
	
	/** The default density of newly created bodies. Thus, zooming affects the mass of newly created bodies. */
	public double newBodyDensity=0.1;
	
	public Settings(){}
	
	public Settings(File settingsFile) throws FileNotFoundException, IOException
	{
		Properties p = new Properties();
		p.load(new FileInputStream(settingsFile));
		
		if(p.containsKey("theme"))
		{
			theme = p.getProperty("theme");
		}
		
		if(p.containsKey("lastWindowDimension"))
		{
			String[] args = p.getProperty("lastWindowDimension").split(",");
			if(args.length > 1) try
			{
				lastWindowDimension = new Dimension(Integer.parseInt(args[0].trim()), Integer.parseInt(args[1].trim()));
			}
			catch(NumberFormatException e)
			{
				e.printStackTrace();
			}
		}
		
		if(p.containsKey("usingAntiAliasing"))
		{
			usingAntiAliasing = Boolean.parseBoolean(p.getProperty("usingAntiAliasing"));
		}
		
		if(p.containsKey("viewMovementSpeedRelatedToScale"))
		{
			viewMovementSpeedRelatedToScale = Boolean.parseBoolean(p.getProperty("viewMovementSpeedRelatedToScale"));
		}
		
		if(p.containsKey("viewMovementSpeed"))
		{
			try{
				viewMovementSpeed = Double.parseDouble(p.getProperty("viewMovementSpeed"));				
			}
			catch(NumberFormatException e)
			{
				e.printStackTrace();
			}
		}
		
		if(p.containsKey("fixedDensity"))
		{
			fixedDensity = Boolean.parseBoolean(p.getProperty("fixedDensity"));
		}
		
		if(p.containsKey("newBodyDiameterRatio"))
		{
			try{
				newBodyDiameterRatio = Double.parseDouble(p.getProperty("newBodyDiameterRatio"));				
			}
			catch(NumberFormatException e)
			{
				e.printStackTrace();
			}
		}
		
		if(p.containsKey("newBodyDensity"))
		{
			try{
				newBodyDensity = Double.parseDouble(p.getProperty("newBodyDensity"));				
			}
			catch(NumberFormatException e)
			{
				e.printStackTrace();
			}
		}
		
		if(p.containsKey("solver"))
		{
			solver = p.getProperty("solver");
		}
	}
	
	public void storeToFile(File file) throws FileNotFoundException, IOException
	{
		Properties p = new Properties();
		
		p.setProperty("theme", theme);
		p.setProperty("lastWindowDimension", lastWindowDimension.width+", "+lastWindowDimension.height);
		p.setProperty("usingAntiAliasing", ""+usingAntiAliasing);
		p.setProperty("viewMovementSpeedRelatedToScale", ""+viewMovementSpeedRelatedToScale);
		p.setProperty("viewMovementSpeed", ""+viewMovementSpeed);
		p.setProperty("fixedDensity", ""+fixedDensity);
		p.setProperty("newBodyDiameterRatio", ""+newBodyDiameterRatio);
		p.setProperty("newBodyDensity", ""+newBodyDensity);
		p.setProperty("solver", solver);
		
		p.store(new FileOutputStream(file), "japla serialVersionUID="+serialVersionUID);
	}
}
