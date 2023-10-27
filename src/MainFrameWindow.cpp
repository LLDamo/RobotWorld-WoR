#include "MainFrameWindow.hpp"

#include "Button.hpp"
#include "Checkbox.hpp"
#include "Client.hpp"
#include "FileTraceFunction.hpp"
#include "Logger.hpp"
#include "LogTextCtrl.hpp"
#include "MainApplication.hpp"
#include "MathUtils.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "Radiobox.hpp"
#include "Robot.hpp"
#include "RobotShape.hpp"
#include "RobotWorld.hpp"
#include "RobotWorldCanvas.hpp"
#include "Shape2DUtils.hpp"
#include "StdOutTraceFunction.hpp"
#include "Trace.hpp"
#include "WidgetTraceFunction.hpp"

#include <array>
#include <iostream>

namespace Application
{
	/**
	 * IDs for the controls and the menu commands
	 * If there are (default) wxWidget ID's: try to maintain
	 * compatibility, especially wxID_ABOUT because on a Mac it is special
	 */
	enum
	{
		ID_QUIT 	= wxID_EXIT,         	//!< ID_QUIT
		ID_OPTIONS 	= wxID_PROPERTIES,		//!< ID_OPTIONS
		ID_ABOUT 	= wxID_ABOUT,        	//!< ID_ABOUT
		ID_WIDGET_TRACE_FUNCTION, 			//!< ID_WIDGET_TRACE_FUNCTION
		ID_STDCOUT_TRACE_FUNCTION, 			//!< ID_STDCOUT_TRACE_FUNCTION
		ID_FILE_TRACE_FUNCTION 				//!< ID_FILE_TRACE_FUNCTION
	};
	/**
	 *
	 */
	MainFrameWindow::MainFrameWindow( const std::string& aTitle) :
																wxFrame( nullptr, wxID_ANY, aTitle, wxDefaultPosition, wxSize( 1200, 600)),
																clientPanel( nullptr),
																menuBar( nullptr),
																splitterWindow( nullptr),
																lhsPanel( nullptr),
																robotWorldCanvas( nullptr),
																rhsPanel( nullptr),
																logTextCtrl( nullptr),
																logDestination( nullptr),
																configPanel(nullptr),
																drawOpenSetCheckbox(nullptr),
																drawParticleFilterCheckbox(nullptr),
																drawKalmanFilterCheckbox(nullptr),
																speedSpinCtrl(nullptr),
																lidarStdDevSpinCtrl(nullptr),
																compasStdDevSpinCtrl(nullptr),
																odometerStdDevSpinCtrl(nullptr),
																worldNumber(nullptr),
																buttonPanel( nullptr)
	{
		initialise();
	}
	/**
	 *
	 */
	void MainFrameWindow::initialise()
	{
		SetMenuBar( initialiseMenuBar());

		wxGridBagSizer* sizer = new wxGridBagSizer( 5, 5);

		sizer->Add( initialiseClientPanel(),
					wxGBPosition( 0, 0), 	// row ,col
					wxGBSpan( 1, 1), 		// row ,col
					wxGROW);
		sizer->AddGrowableCol( 0);
		sizer->AddGrowableRow( 0);

		SetSizer( sizer);
		sizer->SetSizeHints( this);

		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](wxCommandEvent& anEvent){ this->OnQuit(anEvent);},
			  ID_QUIT);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](wxCommandEvent& anEvent){ this->OnWidgetTraceFunction(anEvent);},
			  ID_WIDGET_TRACE_FUNCTION);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](wxCommandEvent& anEvent){ this->OnStdOutTraceFunction(anEvent);},
			  ID_STDCOUT_TRACE_FUNCTION);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](wxCommandEvent& anEvent){ this->OnFileTraceFunction(anEvent);},
			  ID_FILE_TRACE_FUNCTION);
		Bind( wxEVT_COMMAND_MENU_SELECTED,
			  [this](wxCommandEvent& anEvent){ this->OnAbout(anEvent);},
			  ID_ABOUT);

		// By default we use the WidgettraceFunction as we expect that this is what the user wants....
		Base::Trace::setTraceFunction( std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));
	}
	/**
	 *
	 */
	wxMenuBar* MainFrameWindow::initialiseMenuBar()
	{
		wxMenu* fileMenu = new wxMenu;
		fileMenu->Append( ID_QUIT, "E&xit\tAlt-X", "Exit the application");

		wxMenu* debugMenu = new wxMenu;
		debugMenu->AppendRadioItem( ID_WIDGET_TRACE_FUNCTION,  "Widget",  "Widget");
		debugMenu->AppendRadioItem( ID_STDCOUT_TRACE_FUNCTION,  "StdOut",  "StdOut");
		debugMenu->AppendRadioItem( ID_FILE_TRACE_FUNCTION,  "File",  "File");

		wxMenu* helpMenu = new wxMenu;
		helpMenu->Append( ID_ABOUT, "&About...\tF1", "Show about dialog");

		menuBar = new wxMenuBar;
		menuBar->Append( fileMenu, "&File");
		menuBar->Append( debugMenu, "&Debug");
		menuBar->Append( helpMenu, "&Help");

		return menuBar;
	}
	/**
	 *
	 */
	wxPanel* MainFrameWindow::initialiseClientPanel()
	{
		clientPanel = new wxPanel( this);

		wxGridBagSizer* sizer = new wxGridBagSizer();

		sizer->Add( 5, 5,
					wxGBPosition( 0, 0));

		sizer->Add( initialiseSplitterWindow(),
					wxGBPosition( 1, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableRow( 1);
		sizer->AddGrowableCol( 1);

		sizer->Add( 5, 5,
					wxGBPosition( 2, 2));

		clientPanel->SetSizer( sizer);
		sizer->SetSizeHints( clientPanel);

		if(MainApplication::isArgGiven("-debug_grid"))
		{
			showGridFor(clientPanel,sizer);
		}

		return clientPanel;
	}
	/**
	 *
	 */
	wxSplitterWindow* MainFrameWindow::initialiseSplitterWindow()
	{
		splitterWindow = new wxSplitterWindow( clientPanel);
		splitterWindow->SetSashInvisible();
		splitterWindow->SplitVertically( initialiseLhsPanel(), initialiseRhsPanel());
		splitterWindow->SetSashPosition(lhsPanel->GetSize().GetWidth());
		return splitterWindow;
	}
	/**
	 *
	 */
	wxPanel* MainFrameWindow::initialiseLhsPanel()
	{
		lhsPanel = new wxPanel( splitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER);

		wxGridBagSizer* sizer = new wxGridBagSizer();

		sizer->Add( 5, 5,
					wxGBPosition( 0, 0),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableRow( 0);

		sizer->Add( robotWorldCanvas = new View::RobotWorldCanvas( lhsPanel),
					wxGBPosition( 1, 1),
					wxGBSpan( 1, 1),
					wxSHRINK);
		robotWorldCanvas->SetMinSize( wxSize( 1024,1024));

		sizer->Add( 5, 5,
					wxGBPosition( 2, 2),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableRow( 2);

		lhsPanel->SetSizer( sizer);
		sizer->SetSizeHints( lhsPanel);

		if(MainApplication::isArgGiven("-debug_grid"))
		{
			showGridFor(lhsPanel,sizer);
		}

		return lhsPanel;
	}
	/**
	 *
	 */
	wxPanel* MainFrameWindow::initialiseRhsPanel()
	{
		rhsPanel = new wxPanel( splitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER);

		wxGridBagSizer* sizer = new wxGridBagSizer();

		sizer->Add( 5, 5,
					wxGBPosition( 0, 0),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableRow( 0);

		sizer->Add( logPanel = initialiseLogPanel(),
					wxGBPosition( 2, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( configPanel = initialiseConfigPanel(),
					wxGBPosition( 4, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( buttonPanel = initialiseButtonPanel(),
					wxGBPosition( 6, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableCol( 1);

		sizer->Add( 5, 5,
					wxGBPosition( 7, 2),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableRow( 7);


		rhsPanel->SetSizer( sizer);
		sizer->SetSizeHints( rhsPanel);

		if(MainApplication::isArgGiven("-debug_grid"))
		{
			showGridFor(rhsPanel,sizer);
		}

		return rhsPanel;
	}
	/**
	 *
	 */
	wxPanel* MainFrameWindow::initialiseLogPanel()
	{
		wxPanel* panel = new wxPanel( rhsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER);

		wxGridBagSizer* sizer = new wxGridBagSizer();

		sizer->Add( 5, 5,
					wxGBPosition( 0, 0),
					wxGBSpan( 1, 1),
					wxSHRINK);

		std::array<std::string,3> choicesArray
		{
			"Window",
			"StdOut",
			"File"
		};

		sizer->Add(	logDestination = makeRadiobox(	panel,
													choicesArray,
													[this](wxCommandEvent& event)
													{
														wxRadioBox* radiobox = dynamic_cast< wxRadioBox* >(event.GetEventObject());
														if(radiobox)
														{
															switch(radiobox->GetSelection())
															{
																case 0:
																{
																	OnWidgetTraceFunction(event);
																	break;
																}
																case 1:
																{
																	OnStdOutTraceFunction(event);
																	break;
																}
																case 2:
																{
																	OnFileTraceFunction(event);
																	break;
																}
																default:
																{
																	TRACE_DEVELOP("Unknown trace destination");
																}
															}
														}
													},
													"Log destination",
													wxRA_SPECIFY_COLS),
					wxGBPosition( 1, 1),
					wxGBSpan( 1, 1),
					wxALIGN_CENTER);

		sizer->Add( logTextCtrl = new LogTextCtrl( panel, wxID_ANY, wxTE_MULTILINE | wxTE_DONTWRAP),
					wxGBPosition( 2, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableRow( 2);
		sizer->AddGrowableCol( 1);
		logTextCtrl->SetMinSize( wxSize( 500, 300));

		sizer->Add( makeButton( panel,
								"Clear log window",
								[this](wxCommandEvent& /*anEvent*/){logTextCtrl->Clear();}),
					wxGBPosition( 3, 1),
					wxGBSpan( 1, 1),
					wxGROW |wxALIGN_CENTER);

		sizer->Add( 5, 5,
					wxGBPosition( 4, 2),
					wxGBSpan( 1, 1),
					wxSHRINK);

		panel->SetSizerAndFit( sizer);

		if(MainApplication::isArgGiven("-debug_grid"))
		{
			showGridFor(panel,sizer);
		}

		return panel;
	}
	/**
	 *
	 */
	wxPanel* MainFrameWindow::initialiseConfigPanel()
	{
		wxPanel* panel = new wxPanel( rhsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER);

		wxGridBagSizer* sizer = new wxGridBagSizer();

		sizer->Add( 5, 5,
					wxGBPosition( 0, 0),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableCol( 0);

		sizer->Add( drawOpenSetCheckbox = Application::makeCheckbox( panel,
																	 "Draw OpenSet",
																	 [this]( wxCommandEvent& event){this-> OnDrawOpenSet(event);}),
					wxGBPosition( 1, 1),
					wxGBSpan( 1, 1),
					wxSHRINK | wxALIGN_CENTRE);

		sizer->Add( drawParticleFilterCheckbox = Application::makeCheckbox( panel,
																	 "Particle Filter",
																	 [this]( wxCommandEvent& event){this-> OnDrawParticleFilter(event);}),
					wxGBPosition( 3, 1),
					wxGBSpan( 1, 1),
					wxSHRINK | wxALIGN_CENTRE);

		sizer->Add(new wxStaticText(panel,
									wxID_ANY,
									"Lidar stdDev"),
				   wxGBPosition( 3, 2),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		sizer->Add(lidarStdDevSpinCtrl = new wxSpinCtrl(panel,
												  wxID_ANY),
				   wxGBPosition( 3, 3),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		lidarStdDevSpinCtrl->SetValue(static_cast<int>(10));
		lidarStdDevSpinCtrl->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,[this](wxCommandEvent& event){this->OnLidarStdDevSpinCtrlUpdate(event);});

		sizer->Add( drawKalmanFilterCheckbox = Application::makeCheckbox( panel,
																	 "Kalman Filter",
																	 [this]( wxCommandEvent& event){this-> OnDrawKalmanFilter(event);}),
					wxGBPosition( 4, 1),
					wxGBSpan( 1, 1),
					wxSHRINK | wxALIGN_CENTRE);

		sizer->Add(new wxStaticText(panel,
									wxID_ANY,
									"Compas stdDev"),
				   wxGBPosition( 4, 2),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		sizer->Add(compasStdDevSpinCtrl = new wxSpinCtrl(panel,
												  wxID_ANY),
				   wxGBPosition( 4, 3),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		compasStdDevSpinCtrl->SetValue(static_cast<int>(2));
		compasStdDevSpinCtrl->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,[this](wxCommandEvent& event){this->OnCompasStdDevSpinCtrlUpdate(event);});

		sizer->Add(new wxStaticText(panel,
									wxID_ANY,
									"Odometer stdDev"),
				   wxGBPosition( 5, 2),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		sizer->Add(odometerStdDevSpinCtrl = new wxSpinCtrl(panel,
												  wxID_ANY),
				   wxGBPosition( 5, 3),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		odometerStdDevSpinCtrl->SetValue(static_cast<int>(1));
		odometerStdDevSpinCtrl->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,[this](wxCommandEvent& event){this->OnOdometerStdDevSpinCtrlUpdate(event);});

		sizer->Add( makeButton( panel,
								"Save",
								[this](wxCommandEvent& anEvent){this->OnSaveConfiguration(anEvent);}),
					wxGBPosition( 6, 3),
					wxGBSpan( 1, 1),
					wxGROW);

		sizer->Add( makeButton( panel,
								"Load",
								[this](wxCommandEvent& anEvent){this->OnLoadConfiguration(anEvent);}),
					wxGBPosition( 6, 2),
					wxGBSpan( 1, 1),
					wxGROW);

		/////// Speed
		sizer->Add(new wxStaticText(panel,
									wxID_ANY,
									"Speed"),
				   wxGBPosition( 1, 2),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		sizer->Add(speedSpinCtrl = new wxSpinCtrl(panel,
												  wxID_ANY),
				   wxGBPosition( 1, 3),
				   wxGBSpan( 1, 1),
				   wxSHRINK | wxALIGN_CENTER);
		speedSpinCtrl->SetValue(static_cast<int>(10));
		speedSpinCtrl->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,[this](wxCommandEvent& event){this->OnSpeedSpinCtrlUpdate(event);});

		std::array<std::string,3> choicesArray
		{
			"1 Default world",
			"2 Student world 1",
			"3 Student world 2"
		};

		sizer->Add(	worldNumber = makeRadiobox(	panel,
												choicesArray,
												[this](wxCommandEvent& event)
												{
													wxRadioBox* radiobox = dynamic_cast< wxRadioBox* >(event.GetEventObject());
													if(radiobox)
													{
														switch(radiobox->GetSelection())
														{
															case 0:
															{
																OnWorld1(event);
																break;
															}
															case 1:
															{
																OnWorld2(event);
																break;
															}
															case 2:
															{
																OnWorld3(event);
																break;
															}
															default:
															{
																TRACE_DEVELOP("Unknown world selection");
															}
														}
													}
												},
												"World number",
												wxRA_SPECIFY_ROWS),
					wxGBPosition( 2, 1),
					wxGBSpan( 1, 1),
					wxSHRINK | wxALIGN_CENTER);
		sizer->AddGrowableRow( 3);
		sizer->AddGrowableCol( 1);

		sizer->Add( 5, 5,
					wxGBPosition( 7, 4),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableCol( 3);

		panel->SetSizerAndFit( sizer);

		MainSettings& mainSettings = MainApplication::getSettings();
		drawOpenSetCheckbox->SetValue(mainSettings.getDrawOpenSet());
		drawParticleFilterCheckbox->SetValue(mainSettings.getDrawParticleFilter());
		drawKalmanFilterCheckbox->SetValue(mainSettings.getDrawKalmanFilter());
		speedSpinCtrl->SetValue(static_cast<int>(mainSettings.getSpeed()));
		lidarStdDevSpinCtrl->SetValue(static_cast<int>(mainSettings.getLidarStdDev()));
		compasStdDevSpinCtrl->SetValue(static_cast<int>(mainSettings.getCompasStdDev()));
		odometerStdDevSpinCtrl->SetValue(static_cast<int>(mainSettings.getOdometerStdDev()));
		worldNumber->SetSelection(static_cast<int>(mainSettings.getWorldNumber()));

		if(MainApplication::isArgGiven("-debug_grid"))
		{
			showGridFor(panel,sizer);
		}

		return panel;
	}
	/**
	 *
	 */
	wxPanel* MainFrameWindow::initialiseButtonPanel()
	{
		wxPanel* panel = new wxPanel( rhsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER);

		wxGridBagSizer* sizer = new wxGridBagSizer();

		sizer->Add( 5, 5,
					wxGBPosition( 0, 0),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableCol( 0);

		sizer->Add( makeButton( panel,
								"Populate",
								[this](wxCommandEvent& anEvent){this->OnPopulate(anEvent);}),
					wxGBPosition( 1, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( makeButton( panel,
								"Unpopulate",
								[this](wxCommandEvent& anEvent){this->OnUnpopulate(anEvent);}),
					wxGBPosition( 1, 2),
					wxGBSpan( 1, 1),
					wxGROW);

		sizer->Add( makeButton( panel,
								"Start robot",
								[this](wxCommandEvent& anEvent){this->OnStartRobot(anEvent);}),
					wxGBPosition( 3, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( makeButton( panel,
								"Stop robot",
								[this](wxCommandEvent& anEvent){this->OnStopRobot(anEvent);}),
					wxGBPosition( 3, 2),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( makeButton( panel,
								"Start listening",
								[this](wxCommandEvent& anEvent){this->OnStartListening(anEvent);}),
					wxGBPosition( 5, 1),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( makeButton( panel,
								"Send message",
								[this](wxCommandEvent& anEvent){this->OnSendMessage(anEvent);}),
					wxGBPosition( 5, 2),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->Add( makeButton( panel,
								"Stop listening",
								[this](wxCommandEvent& anEvent){this->OnStopListening(anEvent);}),
					wxGBPosition( 5, 3),
					wxGBSpan( 1, 1),
					wxGROW);

		sizer->Add( 5, 5,
					wxGBPosition( 6, 4),
					wxGBSpan( 1, 1),
					wxGROW);
		sizer->AddGrowableCol( 4);

		panel->SetSizerAndFit( sizer);

		if(MainApplication::isArgGiven("-debug_grid"))
		{
			showGridFor(panel,sizer);
		}

		return panel;
	}
	/**
	 *
	 */
	void MainFrameWindow::OnQuit( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Base::StdOutTraceFunction>());
		Close( true);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnWidgetTraceFunction( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));

		wxMenuItem* item = menuBar->FindItem(ID_WIDGET_TRACE_FUNCTION);
		if(item && item->IsRadio() && !item->IsCheck())
		{
			item->Check();
		}
		logDestination->SetSelection(0);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStdOutTraceFunction( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Base::StdOutTraceFunction>());

		wxMenuItem* item = menuBar->FindItem(ID_STDCOUT_TRACE_FUNCTION);
		if(item && item->IsRadio() && !item->IsCheck())
		{
			item->Check();
		}
		logDestination->SetSelection(1);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnFileTraceFunction( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Base::Trace::setTraceFunction( std::make_unique<Base::FileTraceFunction>("trace", "log", true));

		wxMenuItem* item = menuBar->FindItem(ID_FILE_TRACE_FUNCTION);
		if(item && item->IsRadio() && !item->IsCheck())
		{
			item->Check();
		}
		logDestination->SetSelection(2);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnAbout( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		wxMessageBox( "ESD 2012-present RobotWorld.\n", "About RobotWorld", wxOK | wxICON_INFORMATION, this);
	}
	/**
	 *
	 */
	void MainFrameWindow::OnDrawOpenSet( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
//		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
//		if (robot)
//		{
//			robotWorldCanvas->selectShapeAt(robot->getPosition());
//
//			if(std::shared_ptr<View::RobotShape> robotShape = std::dynamic_pointer_cast<View::RobotShape>(robotWorldCanvas->getSelectedShape()))
//			{
//				robotShape->drawOpenSet( drawOpenSetCheckbox->IsChecked());
//			}
//			robotWorldCanvas->Refresh();
//		}
//
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setDrawOpenSet(drawOpenSetCheckbox->IsChecked());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnDrawParticleFilter( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setDrawParticleFilter(drawParticleFilterCheckbox->IsChecked());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnDrawKalmanFilter( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setDrawKalmanFilter(drawKalmanFilterCheckbox->IsChecked());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnSpeedSpinCtrlUpdate( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
//		TRACE_DEVELOP(anEvent.GetString().ToStdString());
//		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
//		if (robot)
//		{
//			robot->setSpeed(static_cast<float>(speedSpinCtrl->GetValue()));
//		}

		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setSpeed(speedSpinCtrl->GetValue());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnLidarStdDevSpinCtrlUpdate( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setLidarStdDev(lidarStdDevSpinCtrl->GetValue());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnCompasStdDevSpinCtrlUpdate( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setCompasStdDev(compasStdDevSpinCtrl->GetValue());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnOdometerStdDevSpinCtrlUpdate( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setOdometerStdDev(odometerStdDevSpinCtrl->GetValue());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnSaveConfiguration( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.saveConfiguration();
	}
	/**
	 *
	 */
	void MainFrameWindow::OnLoadConfiguration( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.loadConfiguration();
		lidarStdDevSpinCtrl->SetValue(static_cast<int>(mainSettings.getLidarStdDev()));
		compasStdDevSpinCtrl->SetValue(static_cast<int>(mainSettings.getCompasStdDev()));
		odometerStdDevSpinCtrl->SetValue(static_cast<int>(mainSettings.getOdometerStdDev()));
	}
	/**
	 *
	 */
	void MainFrameWindow::OnWorld1( wxCommandEvent& anEvent)
	{
		TRACE_DEVELOP(anEvent.GetString().ToStdString());

		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setWorldNumber(worldNumber->GetSelection());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnWorld2( wxCommandEvent& anEvent)
	{
		TRACE_DEVELOP(anEvent.GetString().ToStdString());

		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setWorldNumber(worldNumber->GetSelection());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnWorld3( wxCommandEvent& anEvent)
	{
		TRACE_DEVELOP(anEvent.GetString().ToStdString());

		MainSettings& mainSettings = MainApplication::getSettings();
		mainSettings.setWorldNumber(worldNumber->GetSelection());
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStartRobot( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot && !robot->isActing())
		{
			robot->startActing();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStopRobot( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot && robot->isActing())
		{
			robot->stopActing();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnPopulate( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		switch(worldNumber->GetSelection())
		{
			case 0:
			{
				robotWorldCanvas->populateFirstWorld( 4);
				// TODO Do something...
//				std::shared_ptr<View::RobotShape> robotShape = std::dynamic_pointer_cast<View::RobotShape>(robotWorldCanvas->getSelectedShape());
//				if(robotShape)
//				{
//					TRACE_DEVELOP("It should be checked...");
//					drawOpenSetCheckbox->SetValue(robotShape->getDrawOpenSet());
//				}else
//				{
//					TRACE_DEVELOP("No robotShape? It is not checked...");
//				}
				break;
			}
			case 1:
			{
				robotWorldCanvas->populateSecondWorld( 5);
				break;
			}
			case 2:
			{
				robotWorldCanvas->populateThirdWorld( 6);
				break;
			}
			default:
			{
				TRACE_DEVELOP("Huh?");
				break;
			}
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnUnpopulate( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		robotWorldCanvas->unpopulate();

		logTextCtrl->Clear();
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStartListening( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot)
		{
			robot->startCommunicating();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnSendMessage( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot)
		{
			std::string remoteIpAdres = "localhost";
			std::string remotePort = "12345";

			if (MainApplication::isArgGiven( "-remote_ip"))
			{
				remoteIpAdres = MainApplication::getArg( "-remote_ip").value;
			}
			if (MainApplication::isArgGiven( "-remote_port"))
			{
				remotePort = MainApplication::getArg( "-remote_port").value;
			}

			// We will request an echo message. The response will be "Hello World", if all goes OK,
			// "Goodbye cruel world!" if something went wrong.
			Messaging::Client c1ient( remoteIpAdres,
									  static_cast<unsigned short>(std::stoi(remotePort)),
									  robot);
			Messaging::Message message( Messaging::EchoRequest, "Hello world!");
			c1ient.dispatchMessage( message);
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::OnStopListening( wxCommandEvent& UNUSEDPARAM(anEvent))
	{
		Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot( "Robot");
		if (robot)
		{
			robot->stopCommunicating();
		}
	}
	/**
	 *
	 */
	void MainFrameWindow::showGridFor(wxPanel* aPanel, wxGridBagSizer* aSizer)
	{
		aPanel->Bind(wxEVT_PAINT,[aPanel,aSizer](wxPaintEvent& /*event*/)
		{
			wxPaintDC dc( aPanel );

			wxGridBagSizer* gridBagSizer = aSizer;
			if(gridBagSizer)
			{
				wxSize screenSize = aPanel->GetSize();

				dc.SetPen( wxPen("BLACK", 1, wxPENSTYLE_SOLID));

				int rowY = 0;
				for(int row = 0; row < gridBagSizer->GetRows(); ++row)
				{
					wxSize cellSize = gridBagSizer->GetCellSize(row,0);
					rowY += cellSize.y;
					dc.DrawLine(0, rowY, screenSize.x, rowY);
				}
				int colX = 0;
				for(int col = 0; col < gridBagSizer->GetCols(); ++col)
				{
					wxSize cellSize = gridBagSizer->GetCellSize(0,col);
					colX += cellSize.x;
					dc.DrawLine(colX, 0, colX, screenSize.y);
				}
			}
		});
		aPanel->Bind(wxEVT_SIZE,[aPanel,aSizer](wxSizeEvent& /*event*/)
		{
			aPanel->Layout();
			aPanel->Refresh();
		});
	}
} // namespace Application
