package controller;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.swing.JOptionPane;

import model.Settings;
import util.RepeatingReleasedEventsFixer;
import util.UIManager2;
import view.MainWindow;

public class Main
{
	/**Application version*/
	public static final 
	String version = "0.8.19 alpha";
	
	public static 
	Settings settings = new Settings();
	
	public final static 
	String licence = 
		"Japla, a interactive application to play with gravitation. \n" +
		"Copyright (C) 2014 Carlos Faruolo \nE-mail: 5carlosfelipe5@gmail.com\n\n" +
		"This program is free software: you can redistribute it and/or modify\n" +
		"it under the terms of the GNU General Public License as published by\n" +
		"the Free Software Foundation, either version 3 of the License, or\n" +
		"(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\n" +
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n" +
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" +
		"GNU General Public License for more details.\n\n" +
		"You should have received a copy of the GNU General Public License\n" +
		"along with this program.  If not, see <http://www.gnu.org/licenses/>.";
	
	public final static 
	String settingsFileName = "japla.properties";
	
	/**
	 * @param args
	 */
	public static 
	void main(String[] args) 
	{	
		try 
		{
			settings = new Settings(new File(settingsFileName));
		} 
		catch (FileNotFoundException e1) 
		{
			JOptionPane.showMessageDialog(null, "\""+settingsFileName+"\" not found. Using default settings.", "Settings", JOptionPane.WARNING_MESSAGE);
		} 
		catch (IOException e1) 
		{
			JOptionPane.showMessageDialog(null, "Error while reading \""+settingsFileName+"\":"+e1.getLocalizedMessage()+"\n Using default settings.", "Settings", JOptionPane.ERROR_MESSAGE);
		}
		try
		{
			UIManager2.setLookAndFeelByName(settings.theme);
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(null, "Appearance \""+settings.theme+"\" not recognized. Using default appearance.", "Warning", JOptionPane.WARNING_MESSAGE);
		}
		
		RepeatingReleasedEventsFixer.install(); //work around input bug

		new MainWindow(settings.lastWindowDimension);
	}

}
