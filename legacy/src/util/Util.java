package util;

import java.awt.Color;
import java.util.Collection;
import java.util.Random;

import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.ListModel;

public class Util 
{
	public static Color generateRandomColor()
	{
		return new Color( new Random().nextInt(256), new Random().nextInt(256), new Random().nextInt(256));
	}

	public static void setSelectedValues(JList list, Collection<? extends Object> values) {
	    list.clearSelection();
	    for (Object value : values) {
	        int index = getIndex(list.getModel(), value);
	        if (index >=0) {
	            list.addSelectionInterval(index, index);
	        }
	    }
	    list.ensureIndexIsVisible(list.getSelectedIndex());
	}

	public static int getIndex(ListModel model, Object value) {
	    if (value == null) return -1;
	    if (model instanceof DefaultListModel) {
	        return ((DefaultListModel) model).indexOf(value);
	    }
	    for (int i = 0; i < model.getSize(); i++) {
	        if (value.equals(model.getElementAt(i))) return i;
	    }
	    return -1;
	}
}
