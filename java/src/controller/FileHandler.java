package controller;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;

import javax.swing.filechooser.FileFilter;

import model.physics.Universe2D;

public class FileHandler 
{
	public static void save_sjo(File file, Universe2D universe) throws FileNotFoundException, IOException
	{
		ObjectOutput output = new ObjectOutputStream(new FileOutputStream(file));
		try{ output.writeObject(universe); }
		catch(FileNotFoundException e)	{ throw e;	}
		catch(IOException e) 			{ throw e; }
		finally {	output.close();  }
	}
	
	public static Universe2D load_sjo(File file) throws FileNotFoundException, IOException, ClassNotFoundException
	{
		ObjectInput input = new ObjectInputStream(new FileInputStream(file));
		Object obj;
		try { obj = input.readObject();	}
		catch(FileNotFoundException e)	{ throw e;	}
		catch(IOException e) 			{ throw e; }
		catch(ClassNotFoundException e)	{ throw e;	}
		finally {	input.close();  }
		return (Universe2D) obj;
	}
	
	public static FileFilter 
	fileFilterForSerializedJavaObject=new FileFilter() {
		@Override
		public String getDescription() {
			return "Serialized Java Object (*.sjo)";
		}
		
		@Override
		public boolean accept(File file) {
			if(file.isDirectory() ) return true;
			else if(file.getName().toLowerCase().endsWith(".sjo")) return true;
			else return false;
		}
	}
	/*
	,fileFilterForCommaSeparatedValues=new FileFilter() {
		@Override
		public String getDescription() {
			return "Comma-separated values (*.csv)";
		}
		
		@Override
		public boolean accept(File arg0) {
			if(arg0.getName().toLowerCase().endsWith(".csv")) return true;
			else return false;
		}
	}
	
	,fileFilterForXMLFiles=new FileFilter() {
		@Override
		public String getDescription() {
			return "XML tags (*.xml)";
		}
		
		@Override
		public boolean accept(File arg0) {
			if(arg0.getName().toLowerCase().endsWith(".xml")) return true;
			else return false;
		}
	}
	
	,fileFilterForODSFiles=new FileFilter() {
		@Override
		public String getDescription() {
			return "OpenDocument Spreadsheet (*.ods)";
		}
		
		@Override
		public boolean accept(File arg0) {
			if(arg0.getName().toLowerCase().endsWith(".ods")) return true;
			else return false;
		}
	}
	*/
	;

}
