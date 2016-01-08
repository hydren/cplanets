package util;

import java.util.ArrayList;
import java.util.List;

import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public abstract class UIManager2 extends UIManager
{
	private static final long serialVersionUID = 6294802942431000411L;

	/** Attempts to set the look and feel corresponding to the given name.*/
	public static void setLookAndFeelByName(String lookAndFeelName) throws ClassNotFoundException, InstantiationException, IllegalAccessException, UnsupportedLookAndFeelException
	{
		if(lookAndFeelName == null)
			return;
		
		if(lookAndFeelName.equalsIgnoreCase("System"))
		{
			String sysThemeClassName = UIManager.getSystemLookAndFeelClassName();

			//special case for gtk+ interfaced linux
			String os_name = System.getProperty("os.name");
			if(os_name != null && os_name.equalsIgnoreCase("Linux"))
				for(LookAndFeelInfo lfi : UIManager.getInstalledLookAndFeels())
					if(lfi.getName().equalsIgnoreCase("GTK+"))
					{
						sysThemeClassName = lfi.getClassName();
						break;
					}
			
			UIManager.setLookAndFeel(sysThemeClassName);
		}
		
		else
		{
			for(LookAndFeelInfo info : UIManager.getInstalledLookAndFeels())
			if(info.getName().equals(lookAndFeelName))
			{
				UIManager.setLookAndFeel(info.getClassName());
				return;
			}
			UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
			throw new UnsupportedLookAndFeelException("\""+lookAndFeelName + "\" does not match any available theme. Using default theme instead.");
		}
	}
	
	/** Returns a list containing all names of the corresponding installed L&Fs.  */
	public static List<String> getInstalledLookAndFeelsNames()
	{
		List<String> list = new ArrayList<String>();
		list.add("System");
		for(LookAndFeelInfo info : UIManager.getInstalledLookAndFeels())
			list.add(info.getName());
		
		return list;
	}
	
	/** Returns the default look and feel's name. */
	public static String getDefaultLookAndFeelName()
	{
		for(LookAndFeelInfo info : UIManager.getInstalledLookAndFeels())
			if(info.getClassName().equals(UIManager.getCrossPlatformLookAndFeelClassName()))
				return info.getName();
		
		return null;
	}
	
	/** Returns a LookAndFeelInfo instance corresponding to the look and feel with the given name. If lookAndFeel is null or doesn't match any installed Look And Feel name, null is returned.
	 * 
	 * @param lookAndFeelName The look and feel name.
	 * 
	 * @return A LookAndFeelInfo which name matches the given name.
	 *  */
	public static LookAndFeelInfo getLookAndFeelInfoByName(String lookAndFeelName)
	{
		if(lookAndFeelName == null)
			return null;
		
		for(LookAndFeelInfo info : UIManager.getInstalledLookAndFeels())
			if(info.getName().equals(lookAndFeelName))
				return info;
		
		return null;
	}

}
