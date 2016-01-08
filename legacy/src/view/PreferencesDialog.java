package view;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.border.EmptyBorder;

import logic.solvers.AvailableSolvers;
import logic.solvers.Physics2DSolver;
import model.Settings;
import util.UIManager2;
import controller.Main;

public class PreferencesDialog extends JDialog implements ActionListener
{
	private static final long serialVersionUID = -5025588941073414839L;
	private JButton btnApply;
	private JButton btnCancel;
	private JComboBox combo_appearance;
	private JButton btnRestoreToDefault;
	private JComboBox combo_method;
	
	public PreferencesDialog(JFrame owner)
	{
		super(owner);
		setModal(true);
		setTitle("Preferences");
		setLocationRelativeTo(owner);
		setSize(480, 320);
		
		JPanel panelbuttons = new JPanel();
		FlowLayout flowLayout = (FlowLayout) panelbuttons.getLayout();
		flowLayout.setAlignment(FlowLayout.TRAILING);
		getContentPane().add(panelbuttons, BorderLayout.SOUTH);
		
		btnRestoreToDefault = new JButton("Restore to default");
		btnRestoreToDefault.addActionListener(this);
		panelbuttons.add(btnRestoreToDefault);
		
		btnApply = new JButton("Apply");
		btnApply.addActionListener(this);
		panelbuttons.add(btnApply);
		
		btnCancel = new JButton("Cancel");
		btnCancel.addActionListener(this);
		panelbuttons.add(btnCancel);
		
		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		getContentPane().add(tabbedPane, BorderLayout.CENTER);
		
		JPanel panelGeneral = new JPanel();
		tabbedPane.addTab("General", null, panelGeneral, null);
		panelGeneral.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));
		
		JPanel panel = new JPanel();
		panelGeneral.add(panel);
		panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
		
		JLabel lblAppearance = new JLabel("Interface appearance:");
		lblAppearance.setBorder(new EmptyBorder(0, 4, 0, 2));
		lblAppearance.setHorizontalAlignment(SwingConstants.LEFT);
		panel.add(lblAppearance);
		
		combo_appearance = new JComboBox(new Vector<String>(UIManager2.getInstalledLookAndFeelsNames()));
		combo_appearance.setSelectedItem(Main.settings.theme);
		combo_appearance.setMaximumSize(new Dimension(32767, 25));
		panel.add(combo_appearance);
		
		JPanel panel_1 = new JPanel();
		panelGeneral.add(panel_1);
		
		JLabel lblIntegrationMethod = new JLabel("Integration method:");
		panel_1.add(lblIntegrationMethod);
		
		combo_method = new JComboBox(new Vector<Physics2DSolver>((AvailableSolvers.list)));
		combo_method.setSelectedItem(AvailableSolvers.getSolverByClassName(Main.settings.solver));
		panel_1.add(combo_method);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		Object source = e.getSource();
		if(source == null) return;
		
		else if(source == btnRestoreToDefault)
		{
			if(JOptionPane.NO_OPTION == JOptionPane.showConfirmDialog(this, "This will reset all settings to default. Are you sure?", "Reset settings", JOptionPane.YES_NO_OPTION))
				return;
				
			Main.settings = new Settings();
			try 
			{
				UIManager2.setLookAndFeelByName((String) combo_appearance.getSelectedItem());
				SwingUtilities.updateComponentTreeUI(this.getOwner());
				Main.settings.theme = (String) combo_appearance.getSelectedItem();
			} 
			catch (Exception exception) 
			{
				JOptionPane.showMessageDialog(null, "Unable to set appearance to \""+(String) combo_appearance.getSelectedItem()+"\".", "Error", JOptionPane.ERROR_MESSAGE);
			}
			this.dispose();
		}
		
		else if(source == btnApply)
		{
			if(Main.settings.theme.equals((String) combo_appearance.getSelectedItem()) == false) try 
			{
				UIManager2.setLookAndFeelByName((String) combo_appearance.getSelectedItem());
				SwingUtilities.updateComponentTreeUI(this.getOwner());
				Main.settings.theme = (String) combo_appearance.getSelectedItem();
			} 
			catch (Exception exception) 
			{
				JOptionPane.showMessageDialog(null, "Unable to set appearance to \""+(String) combo_appearance.getSelectedItem()+"\".", "Error", JOptionPane.ERROR_MESSAGE);
			}
			
			Main.settings.solver = combo_method.getSelectedItem().getClass().getName();
				
			this.dispose();
		}
		
		else if(source == btnCancel)
		{
			this.dispose();
		}
	}
	
}
