package view;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.IOException;
import java.util.HashSet;
import java.util.Vector;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JSpinner;
import javax.swing.JSplitPane;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.SpinnerNumberModel;
import javax.swing.SwingConstants;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import logic.Physics2D;
import logic.Physics2D.BodyCollisionListener;
import logic.solvers.AvailableSolvers;
import model.geometry.Vector2D;
import model.physics.Body2D;
import model.physics.Universe2D;
import util.Util;
import controller.FileHandler;
import controller.Main;

public class MainWindow extends JFrame implements  ActionListener, ChangeListener, KeyListener, MouseListener, MouseMotionListener, WindowListener, BodyCollisionListener, ListSelectionListener
{
	private static final long serialVersionUID = 9213865282601529100L;

	//simulation control variables
	private volatile Physics2D physics = new Physics2D();
	
	private double zoomingSpeed = 0.1, viewMovementSpeed = Main.settings.viewMovementSpeed;
	private Double zoomChange = null;
	private Vector2D viewChange = null;
	//simulation control variables
	
	boolean waitForClick=false, waitForSecondClick=false;
	
	private PlanetariumPane planetarium;
	
	private JButton btnRun;
	private JButton btnPause;
	private JToggleButton btnAdd;
	private JList jlist_bodies;
	private JMenuItem mntmAbout;
	private JMenuItem mntmSaveAs;
	private JMenuItem mntmOpen;
	private JMenuItem mntmNewUniverse;
	private JMenuItem mntmExit;
	private JToggleButton tglbtnSmooth;
	private JMenuItem mntmPreferences;
	private JSpinner spinner_setpsize;
	private JSpinner spinner_dispPeriod;
	private JSpinner spinner_newbodydensity;
	private JSpinner spinner_newbodysize;
	private JSpinner spinner_gravity;
	private JButton btnRecolorAll;
	private JToggleButton tglbtnTraceOrbit;
	private JSpinner spinner_tracelength;
	private JRadioButton rdbtnPoint;
	private JRadioButton rdbtnLinear;
	private JRadioButton rdbtnSpline;
	private JButton btnFocus;
	private JButton btnFollowSelection;

	public MainWindow(Dimension windowSize)
	{
		super("Japla");
		this.addWindowListener(this);
		this.setIconImage(new ImageIcon("data/japla.png").getImage());
		this.setSize(800, 600);
		
		if(windowSize != null)
			this.setSize(windowSize);
			
		
		//finishing statements
		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.setLocationRelativeTo(null);
		
		JMenuBar menuBar = new JMenuBar();
		setJMenuBar(menuBar);
		
		JMenu mnFile = new JMenu("File");
		menuBar.add(mnFile);
		
		mntmNewUniverse = new JMenuItem("New universe");
		mntmNewUniverse.addActionListener(this);
		mnFile.add(mntmNewUniverse);
		
		mntmOpen = new JMenuItem("Open");
		mntmOpen.addActionListener(this);
		mnFile.add(mntmOpen);
		
		JSeparator separator = new JSeparator();
		mnFile.add(separator);
		
		mntmSaveAs = new JMenuItem("Save as");
		mntmSaveAs.addActionListener(this);
		mnFile.add(mntmSaveAs);
		
		JSeparator separator_1 = new JSeparator();
		mnFile.add(separator_1);
		
		mntmExit = new JMenuItem("Exit");
		mntmExit.addActionListener(this);
		mnFile.add(mntmExit);
		
		JMenu mnEdit = new JMenu("Edit");
		menuBar.add(mnEdit);
		
		JMenuItem mntmUndo = new JMenuItem("Undo");
		mntmUndo.setEnabled(false);
		mnEdit.add(mntmUndo);
		
		JMenuItem mntmRedo = new JMenuItem("Redo");
		mntmRedo.setEnabled(false);
		mnEdit.add(mntmRedo);
		
		JSeparator separator_2 = new JSeparator();
		mnEdit.add(separator_2);
		
		mntmPreferences = new JMenuItem("Preferences");
		mnEdit.add(mntmPreferences);
		mntmPreferences.addActionListener(this);
		
		JMenu mnHelp = new JMenu("Help");
		menuBar.add(mnHelp);
		
		JMenuItem mntmHelpContents = new JMenuItem("Help contents");
		mntmHelpContents.setEnabled(false);
		mnHelp.add(mntmHelpContents);
		
		mntmAbout = new JMenuItem("About");
		mntmAbout.addActionListener(this);
		mnHelp.add(mntmAbout);
		
		Component horizontalGlue_1 = Box.createHorizontalGlue();
		menuBar.add(horizontalGlue_1);
		
		JLabel lblVersion = new JLabel("v"+Main.version);
		menuBar.add(lblVersion);
		lblVersion.setForeground(Color.LIGHT_GRAY);
		
		JToolBar toolBar = new JToolBar();
		toolBar.setFloatable(false);
		toolBar.setFocusable(false);
		getContentPane().add(toolBar, BorderLayout.NORTH);
		
		btnAdd = new JToggleButton("Add");
		btnAdd.addActionListener(this);
		toolBar.add(btnAdd);
		
		btnRecolorAll = new JButton("Recolor all");
		btnRecolorAll.addActionListener(this);
		toolBar.add(btnRecolorAll);
		
		btnFollowSelection = new JButton("Follow selection");
		btnFollowSelection.addActionListener(this);
		toolBar.add(btnFollowSelection);
		
		Component horizontalGlue = Box.createHorizontalGlue();
		toolBar.add(horizontalGlue);
		
		btnRun = new JButton("Run");
		btnRun.addActionListener(this);
		
		btnFocus = new JButton("Focus");
		toolBar.add(btnFocus);
		btnFocus.addActionListener(this);
		toolBar.add(btnRun);
		
		btnPause = new JButton("Pause");
		btnPause.addActionListener(this);
		toolBar.add(btnPause);
		
		JSplitPane splitPane = new JSplitPane();
		splitPane.setOneTouchExpandable(true);
		getContentPane().add(splitPane, BorderLayout.CENTER);
		physics.universe.gravity = 1000; //XXX
		physics.addBodyCollisionListener(this);
		
		planetarium = new PlanetariumPane(physics);
		planetarium.addKeyListener(this);
		planetarium.addMouseListener(this);
		planetarium.addMouseMotionListener(this);
		planetarium.setFocusable(true);
		splitPane.setRightComponent(planetarium);
		
		JPanel panel = new JPanel();
		panel.setMinimumSize(new Dimension(1, 10));
		splitPane.setLeftComponent(panel);
		panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
		
		JLabel lblBodies = new JLabel("Bodies");
		lblBodies.setAlignmentX(Component.CENTER_ALIGNMENT);
		lblBodies.setBorder(new EmptyBorder(4, 0, 4, 0));
		panel.add(lblBodies);
		
		//this is needed to work with java 6
		jlist_bodies = new JList(new Vector<Body2D>());
		jlist_bodies.setToolTipText("Bodies");
		jlist_bodies.addListSelectionListener(this);
		
		JScrollPane scrollPane_bodies = new JScrollPane();
		panel.add(scrollPane_bodies);
		scrollPane_bodies.setViewportView(jlist_bodies);
		
		JToolBar toolBar_1 = new JToolBar();
		toolBar_1.setFloatable(false);
		getContentPane().add(toolBar_1, BorderLayout.SOUTH);
		
		JMenuBar menuBar_param = new JMenuBar();
		menuBar_param.setAlignmentX(Component.LEFT_ALIGNMENT);
		menuBar_param.setAlignmentY(Component.CENTER_ALIGNMENT);
		toolBar_1.add(menuBar_param);
		
		JLabel lblSimulation = new JLabel("Parameters:");
		menuBar_param.add(lblSimulation);
		lblSimulation.setFont(new Font("Dialog", Font.BOLD, 14));
		lblSimulation.setBorder(new EmptyBorder(2, 0, 2, 0));
		lblSimulation.setAlignmentX(Component.CENTER_ALIGNMENT);
		
		JMenu mnSimulationParam = new JMenu("Simulation");
		menuBar_param.add(mnSimulationParam);
		
		JPanel panel_stepsize = new JPanel();
		panel_stepsize.setBorder(new EmptyBorder(4, 4, 4, 4));
		mnSimulationParam.add(panel_stepsize);
		panel_stepsize.setLayout(new BoxLayout(panel_stepsize, BoxLayout.X_AXIS));
		
		JLabel lblStepSize = new JLabel("Step size:");
		panel_stepsize.add(lblStepSize);
		
		spinner_setpsize = new JSpinner();
		spinner_setpsize.addChangeListener(this);
		panel_stepsize.add(spinner_setpsize);
		spinner_setpsize.setPreferredSize(new Dimension(64, 24));
		spinner_setpsize.setModel(new SpinnerNumberModel(physics.solver.timestep, 0d, Double.MAX_VALUE, 0.01));
		
		JPanel panel_delay = new JPanel();
		panel_delay.setBorder(new EmptyBorder(4, 4, 4, 4));
		mnSimulationParam.add(panel_delay);
		panel_delay.setLayout(new BoxLayout(panel_delay, BoxLayout.X_AXIS));
		
		JLabel lblDisplayPeriod = new JLabel("Display Period:");
		panel_delay.add(lblDisplayPeriod);
		
		spinner_dispPeriod = new JSpinner();
		spinner_dispPeriod.setModel(new SpinnerNumberModel(planetarium.getSleepingTime(), 0L, null, 1L));
		spinner_dispPeriod.setPreferredSize(new Dimension(64, 24));
		spinner_dispPeriod.addChangeListener(this);
		panel_delay.add(spinner_dispPeriod);
		
		JPanel panel_gravity_contant = new JPanel();
		panel_gravity_contant.setBorder(new EmptyBorder(4, 4, 4, 4));
		mnSimulationParam.add(panel_gravity_contant);
		panel_gravity_contant.setLayout(new BoxLayout(panel_gravity_contant, BoxLayout.X_AXIS));
		
		JLabel lblGravityConstant = new JLabel("Gravity Constant:");
		panel_gravity_contant.add(lblGravityConstant);
		
		spinner_gravity = new JSpinner();
		spinner_gravity.setModel(new SpinnerNumberModel(physics.universe.gravity, Double.MIN_VALUE, Double.MAX_VALUE, 0.1d));
		spinner_gravity.setPreferredSize(new Dimension(64, 24));
		spinner_gravity.addChangeListener(this);
		panel_gravity_contant.add(spinner_gravity);
		
		JMenu mnNewBody = new JMenu("Creation");
		menuBar_param.add(mnNewBody);
		
		JPanel panel_newbodysize = new JPanel();
		FlowLayout flowLayout_3 = (FlowLayout) panel_newbodysize.getLayout();
		flowLayout_3.setAlignment(FlowLayout.LEFT);
		mnNewBody.add(panel_newbodysize);
		
		JLabel lblDiameter = new JLabel("Diameter:");
		panel_newbodysize.add(lblDiameter);
		
		spinner_newbodysize = new JSpinner();
		spinner_newbodysize.setModel(new SpinnerNumberModel(Main.settings.newBodyDiameterRatio, 0d, Double.MAX_VALUE, 0.1d));
		spinner_newbodysize.setPreferredSize(new Dimension(64, 24));
		spinner_newbodysize.addChangeListener(this);
		panel_newbodysize.add(spinner_newbodysize);
		
		JPanel panel_newbodydensity = new JPanel();
		FlowLayout flowLayout_4 = (FlowLayout) panel_newbodydensity.getLayout();
		flowLayout_4.setAlignment(FlowLayout.LEFT);
		mnNewBody.add(panel_newbodydensity);
		
		JLabel lblDensity = new JLabel("Density:");
		panel_newbodydensity.add(lblDensity);
		
		spinner_newbodydensity = new JSpinner();
		spinner_newbodydensity.setModel(new SpinnerNumberModel(Main.settings.newBodyDensity, 0d, Double.MAX_VALUE, 0.1d));
		spinner_newbodydensity.setPreferredSize(new Dimension(64, 24));
		spinner_newbodydensity.addChangeListener(this);
		panel_newbodydensity.add(spinner_newbodydensity);
		
		JMenu mnTracing = new JMenu("Tracing");
		menuBar_param.add(mnTracing);
		
		JPanel panel_traceLength = new JPanel();
		FlowLayout fl_panel_traceLength = (FlowLayout) panel_traceLength.getLayout();
		fl_panel_traceLength.setAlignment(FlowLayout.LEFT);
		mnTracing.add(panel_traceLength);
		
		JLabel lblTraceLength = new JLabel("Length:");
		panel_traceLength.add(lblTraceLength);
		
		spinner_tracelength = new JSpinner();
		spinner_tracelength.setModel(new SpinnerNumberModel(20, 1, Integer.MAX_VALUE, 1));
		spinner_tracelength.setPreferredSize(new Dimension(64, 24));
		spinner_tracelength.addChangeListener(this);
		panel_traceLength.add(spinner_tracelength);
		
		JPanel panel_TraceType = new JPanel();
		mnTracing.add(panel_TraceType);
		
		JLabel lblTracerType = new JLabel("Type:");
		panel_TraceType.add(lblTracerType);
		
		rdbtnPoint = new JRadioButton("Point");
		rdbtnPoint.addActionListener(this);
		rdbtnPoint.setSelected(true);
		panel_TraceType.add(rdbtnPoint);
		
		rdbtnLinear = new JRadioButton("Linear");
		rdbtnLinear.addActionListener(this);
		panel_TraceType.add(rdbtnLinear);
		
		rdbtnSpline = new JRadioButton("Spline");
		rdbtnSpline.addActionListener(this);
		panel_TraceType.add(rdbtnSpline);
		
		ButtonGroup group = new ButtonGroup();
		group.add(rdbtnPoint);
		group.add(rdbtnLinear);
		group.add(rdbtnSpline);
		
		JSeparator separator_3 = new JSeparator();
		separator_3.setOrientation(SwingConstants.VERTICAL);
		toolBar_1.add(separator_3);
		
		tglbtnSmooth = new JToggleButton("Smooth");
		toolBar_1.add(tglbtnSmooth);
		tglbtnSmooth.addActionListener(this);
		
		tglbtnTraceOrbit = new JToggleButton("Trace orbit");
		toolBar_1.add(tglbtnTraceOrbit);
		tglbtnTraceOrbit.addActionListener(this);
		
		this.setVisible(true);
		planetarium.requestFocus();
		planetarium.setRunning(true);

		Thread threadGraphics = new Thread()
		{
			@Override
			public void run() {
				planetariumRepainting();
			}
		};
		threadGraphics.start();
	}
	
	/////////////////////////////// Listeners ////////////////////////////////////
	
	@Override
	public void windowActivated(WindowEvent e) {}

	@Override
	public void windowClosed(WindowEvent e) {}

	@Override
	public void windowClosing(WindowEvent e) 
	{
		exitApp();
	}

	@Override
	public void windowDeactivated(WindowEvent e) {}

	@Override
	public void windowDeiconified(WindowEvent e) {}

	@Override
	public void windowIconified(WindowEvent e) {}

	@Override
	public void windowOpened(WindowEvent e) {}

	@Override
	public void keyPressed(KeyEvent arg0) {
		switch(arg0.getKeyCode())
		{
			case KeyEvent.VK_UP:
				if(viewChange == null)
					viewChange = new Vector2D(0, -viewMovementSpeed);
				else
					viewChange.y = -viewMovementSpeed;
				break;
				
			case KeyEvent.VK_DOWN:
				if(viewChange == null)
					viewChange = new Vector2D(0, viewMovementSpeed);
				else
					viewChange.y = viewMovementSpeed;
				break;
				
			case KeyEvent.VK_LEFT:
				if(viewChange == null)
					viewChange = new Vector2D(-viewMovementSpeed, 0);
				else
					viewChange.x = -viewMovementSpeed;
				break;
				
			case KeyEvent.VK_RIGHT:
				if(viewChange == null)
					viewChange = new Vector2D(viewMovementSpeed, 0);
				else
					viewChange.x = viewMovementSpeed;
				break;
			
			case KeyEvent.VK_ADD:
			case KeyEvent.VK_EQUALS:
			case KeyEvent.VK_PAGE_UP:
				zoomChange = 1+zoomingSpeed;
				break;
				
			case KeyEvent.VK_SUBTRACT:
			case KeyEvent.VK_MINUS:
			case KeyEvent.VK_PAGE_DOWN:
				zoomChange = 1-zoomingSpeed;
				break;
				
			case KeyEvent.VK_S:
				tglbtnSmooth.setSelected(!planetarium.isUsingAA());
				toogleAA();
				break;
				
			case KeyEvent.VK_O:
				planetarium.recolorAllBodies();
				break;
				
			case KeyEvent.VK_P:
				planetarium.toogleRunning();
				break;
				
			case KeyEvent.VK_T:
				tglbtnTraceOrbit.setSelected(!planetarium.isTracing());
				toogleTracing();
				break;
				
			case KeyEvent.VK_E:
				clearUniverse();
				break;
		}
		
	}

	@Override
	public void keyReleased(KeyEvent arg0) {
		switch(arg0.getKeyCode())
		{
			case KeyEvent.VK_LEFT:
				if(viewChange != null) 
					if(viewChange.x < 0)
						viewChange = null;
				break;
				
			case KeyEvent.VK_RIGHT:
				if(viewChange != null)
					if(viewChange.x > 0)
						viewChange = null;
				break;
				
			case KeyEvent.VK_UP:
				if(viewChange != null)
					if(viewChange.y < 0)
						viewChange = null;
				break;
			
			case KeyEvent.VK_DOWN:
				if(viewChange != null)
					if(viewChange.y > 0)
						viewChange = null;
				break;
				
			case KeyEvent.VK_ADD:
			case KeyEvent.VK_EQUALS:
			case KeyEvent.VK_PAGE_UP:
				if(zoomChange > 1)
					zoomChange = null;
				break;
			
			case KeyEvent.VK_SUBTRACT:
			case KeyEvent.VK_MINUS:
			case KeyEvent.VK_PAGE_DOWN:
				if(zoomChange < 1)
					zoomChange = null;
				break;
				
			case KeyEvent.VK_A:
				btnAdd.setSelected(true);
				toogleAddNewBodyMode(true);
				break;
		}
		
	}

	@Override
	public void keyTyped(KeyEvent arg0){}

	@Override
	public void mouseClicked(MouseEvent e) {
		//first click
		if(waitForClick && planetarium.isFocused())
		{
			Vector2D mouse = new Vector2D(e.getX(), e.getY()).scale(1d/planetarium.getZoom());
			planetarium.setNewBodyPosition(planetarium.getViewportPosition().sum(mouse));
			planetarium.setNewBodyVelocity( new Vector2D() );
			planetarium.repaint();
			waitForClick = false;
			waitForSecondClick = true;
		}
		else if(waitForClick)
		{
			waitForClick = false;
			btnAdd.setSelected(false);
		}
	
		// second click	
		else if(waitForSecondClick && planetarium.isFocused() )
		{
			planetarium.addCurrentCreatingBody();
			planetarium.repaint();
			planetarium.setRunning(true);
			waitForSecondClick = false;
			btnAdd.setSelected(false);
			updateJListBodies();
		}
		else if(waitForSecondClick)
		{	
			planetarium.setNewBodyPosition(null);
			planetarium.setNewBodyVelocity(null);
			waitForSecondClick = false;
			btnAdd.setSelected(false);
		}
		else
		{
			planetarium.attemptSelectionFromClick(e.getX(), e.getY());
			Util.setSelectedValues(jlist_bodies, new HashSet<Body2D>(planetarium.getFocusedBodies()));
		}

	}

	@Override
	public void mouseEntered(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}

	@Override
	public void mousePressed(MouseEvent e) {
		if(e.getSource() == planetarium)
		{
			planetarium.requestFocus();
			
			Vector2D mouse = new Vector2D(e.getX(), e.getY());
			planetarium.setSelectionInitialPosition( mouse.scale(1d/planetarium.getZoom()).add( planetarium.getViewportPosition()) );
			planetarium.setSelectionFinalPosition(planetarium.getSelectionInitialPosition().clone());
		}
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		if(e.getSource() == planetarium)
		{
			planetarium.focusBodiesFromSelectionRect();
			planetarium.setSelectionInitialPosition(null);
			planetarium.setSelectionFinalPosition(null);
			Util.setSelectedValues(jlist_bodies, new HashSet<Body2D>(planetarium.getFocusedBodies()));
		}
	}
	
	@Override
	public void mouseDragged(MouseEvent arg0) {
		if(arg0.getSource() == planetarium)
		{
			if(planetarium.getSelectionFinalPosition() != null)//should be true anyway...
			{
				Vector2D mouse = new Vector2D(arg0.getX(), arg0.getY());
				planetarium.setSelectionFinalPosition(mouse.scale(1d/planetarium.getZoom()).add( planetarium.getViewportPosition()));
			}
		}
	}

	@Override
	public void mouseMoved(MouseEvent arg0) {
		if(arg0.getSource() == planetarium)
		{
			if(planetarium.getNewBodyVelocity() != null)
			{
				planetarium.setNewBodyVelocity(new Vector2D(arg0.getX(), arg0.getY()).subtract(planetarium.getTransposed(planetarium.getNewBodyPosition())).scale(1/planetarium.getZoom()));
			}
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		Object source = e.getSource();
		
		//toolbar buttons
		if(source == mntmExit)
		{
			exitApp();
		}
		
		else if(source == btnRun)
		{
			planetarium.setRunning(true);
		}
		
		else if(source == btnPause)
		{
			planetarium.setRunning(false);
		}
		
		else if(source == btnAdd)
		{
			if(btnAdd.isSelected())
				toogleAddNewBodyMode(true);
			else 
				toogleAddNewBodyMode(false);
		}
		
		else if(source == btnRecolorAll)
		{
			planetarium.recolorAllBodies();
		}
		
		else if(source == btnFollowSelection)
		{
			planetarium.getPhysics().referenceFrame.bodies.clear();
			planetarium.getPhysics().referenceFrame.bodies.addAll(planetarium.getFocusedBodies());
			//planetarium.setViewportPosition(planetarium.getPhysics().referenceFrame.getPosition());
		}
		
		else if(source == btnFocus)
		{
			planetarium.requestFocus();
		}
		
		else if(source == tglbtnSmooth)
		{
			toogleAA();
		}
		
		else if(source == tglbtnTraceOrbit)
		{
			toogleTracing();
		}
		
		else if(source == rdbtnPoint)
		{
			planetarium.setTracerType(PlanetariumPane.OrbitTracerType.POINT);
		}
		
		else if(source == rdbtnLinear)
		{
			planetarium.setTracerType(PlanetariumPane.OrbitTracerType.LINEAR);
		}
		
		else if(source == rdbtnSpline)
		{
			planetarium.setTracerType(PlanetariumPane.OrbitTracerType.SPLINE);
		}
		
		//menu
		else if(source == mntmNewUniverse)
		{
			clearUniverse();
		}
		
		else if(source == mntmSaveAs)
		{
			toogleSaveAsDialog();
		}
		
		else if(source == mntmOpen)
		{
			toogleLoadDialog();
		}
		
		else if(source == mntmPreferences)
		{
			new PreferencesDialog(this).setVisible(true);
			
			//update solver
			if(physics.solver.getClass().getName().equals(Main.settings.solver) == false)
			{
				physics.setSolver(AvailableSolvers.getSolverByClassName(Main.settings.solver).getNewInstance(physics.universe));
				spinner_setpsize.setValue(physics.solver.timestep);
			}
		}
		
		else if(source == mntmAbout)
		{
			JOptionPane.showMessageDialog(this, Main.licence+"\n\n Version "+Main.version, "About Japla", JOptionPane.INFORMATION_MESSAGE);
		}
	}
	

	@Override
	public void stateChanged(ChangeEvent e) 
	{
		Object source = e.getSource();
		if(source == null)
			return;
		
		else if(source == spinner_setpsize)
		{
			physics.solver.timestep = (Double) spinner_setpsize.getValue();
		}
		
		else if(source == spinner_dispPeriod)
		{
			planetarium.setSleepingTime((Long) spinner_dispPeriod.getValue());
		}
		
		else if(source == spinner_gravity)
		{
			physics.universe.gravity = (Double) spinner_gravity.getValue();
		}
		
		else if(source == spinner_newbodysize)
		{
			Main.settings.newBodyDiameterRatio = (Double) spinner_newbodysize.getValue();
		}
		
		else if(source == spinner_newbodydensity)
		{
			Main.settings.newBodyDensity = (Double) spinner_newbodydensity.getValue();
		}
		
		else if(source == spinner_tracelength)
		{
			planetarium.setTraceLength((Integer) spinner_tracelength.getValue());
		}
	}
	

	@Override
	public void onBodyCollision(HashSet<Body2D> collidingSet, Body2D resultingMerger) {
		updateJListBodies();
	}
	
	@Override
	public void valueChanged(ListSelectionEvent e) {
		Object source = e.getSource();
		
		if(source == jlist_bodies)
		{
			planetarium.getFocusedBodies().clear();
			for(Object obj : jlist_bodies.getSelectedValues())
			{
				planetarium.getFocusedBodies().add((Body2D) obj);
			}
		}
	}
	
	//////////////////////////////// functions //////////////////////////////////////
	
	public void sleep(long ms)
	{
		try {
			Thread.sleep(ms);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public void planetariumRepainting()
	{
		while(true)
		{
			if(viewChange != null && !waitForSecondClick)
				planetarium.getViewportPosition().add(viewChange);
		
			if(zoomChange != null && !waitForSecondClick)
			{
				double x_displacement = planetarium.getWidth()*(1d/planetarium.getZoom());
				double y_displacement = planetarium.getHeight()*(1d/planetarium.getZoom());

				planetarium.setZoom( planetarium.getZoom()*zoomChange );
				
				x_displacement -= planetarium.getWidth()*(1d/planetarium.getZoom());
				y_displacement -= planetarium.getHeight()*(1d/planetarium.getZoom());
				
				planetarium.getViewportPosition().x += x_displacement/2;
				planetarium.getViewportPosition().y += y_displacement/2;

				
				if(Main.settings.viewMovementSpeedRelatedToScale)
				{
					viewMovementSpeed = Main.settings.viewMovementSpeed*(1/planetarium.getZoom());
				}
			}
			
			planetarium.repaint();
			sleep(25);
		}
	}
	
	private void exitApp()
	{
		int answer = JOptionPane.showConfirmDialog(this, "Are you sure you want to quit?", "Quit", JOptionPane.YES_NO_OPTION);
		if(answer == JOptionPane.NO_OPTION)
			return;
		
		try 
		{
			Main.settings.lastWindowDimension = getSize();
			Main.settings.storeToFile(new File(Main.settingsFileName));
		} 
		catch (IOException e1) 
		{
			JOptionPane.showMessageDialog(this,  "Error while saving \""+Main.settingsFileName+"\" settings file.");
		}
		
		dispose();
		System.exit(0); //needed to stop other threads...
	}
	
	private void toogleAddNewBodyMode(boolean status)
	{
		waitForClick = status;
		planetarium.setRunning(!status);
	}
	
	private void updateJListBodies()
	{
		Vector<Body2D> v = new Vector<Body2D>();
		for(Body2D body : physics.universe.bodies)
		{
			v.add( body );
		}
		jlist_bodies.setListData(v);
	}
	
	private void toogleAA()
	{
		planetarium.setUsingAA(tglbtnSmooth.isSelected());
	}
	
	private void toogleTracing()
	{
		planetarium.setTracing(tglbtnTraceOrbit.isSelected());
	}
	
	private void clearUniverse()
	{
		physics.setUniverse(new Universe2D());
		physics.universe.gravity = 1000;
		updateJListBodies();
		physics.referenceFrame.bodies.clear();
	}
	
	private void toogleSaveAsDialog()
	{
		boolean previousState = planetarium.isRunning();
		planetarium.setRunning(false);
		
		JFileChooser chooser = new JFileChooser();
		chooser.setDialogTitle("Save universe");
		chooser.addChoosableFileFilter(FileHandler.fileFilterForSerializedJavaObject);
		chooser.setFileFilter(FileHandler.fileFilterForSerializedJavaObject);
		if(chooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION)
		{
			File fileToSave = chooser.getSelectedFile();
			if(fileToSave.getName() != null) if( !fileToSave.getName().equals(""))
			{
				//check type
				if( chooser.getFileFilter() == FileHandler.fileFilterForSerializedJavaObject )
				{
					if(! fileToSave.getName().toLowerCase().endsWith(".sjo"))
						fileToSave = new File(fileToSave.getPath()+".sjo");
					
					synchronized (physics) {	
						try{
							FileHandler.save_sjo(fileToSave, physics.universe);
						}catch(Exception e)	{
							JOptionPane.showMessageDialog(this, e.getLocalizedMessage(), "Error", JOptionPane.ERROR_MESSAGE);
						}
					}
				}
				//TODO save more types
			}
		}
		
		planetarium.setRunning(previousState);
	}
	
	private void toogleLoadDialog()
	{
		JFileChooser chooser = new JFileChooser();
		chooser.setDialogTitle("Load universe");
		chooser.addChoosableFileFilter(FileHandler.fileFilterForSerializedJavaObject);
		chooser.setFileFilter(FileHandler.fileFilterForSerializedJavaObject);
		if(chooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION)
		{
			File fileToLoad = chooser.getSelectedFile();
			if(fileToLoad.getName() != null) if( !fileToLoad.getName().equals(""))
			{
				if( chooser.getFileFilter() == FileHandler.fileFilterForSerializedJavaObject )
				{
					synchronized (physics) {
						//FIXME bodies end up being null after loading...
						
						try{
							Universe2D u = FileHandler.load_sjo(fileToLoad);
							physics.setUniverse(u);
							updateJListBodies();
						}catch(Exception e)	{
							JOptionPane.showMessageDialog(this, e.getLocalizedMessage(), "Error", JOptionPane.ERROR_MESSAGE);
						}
					}
					planetarium.setRunning(false);
				}
			}
			//TODO open more types
		}
	}
}
