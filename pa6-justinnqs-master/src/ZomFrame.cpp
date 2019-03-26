#include "ZomFrame.h"
#include "Machine.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include "ZomDrawPanel.h"

enum
{
	ID_SIM_START = 1000,
	ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR,
    ID_RANDOMIZE,
    ID_RESET
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame) // NOLINT
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SIM_START, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
    EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
    EVT_MENU(ID_RESET, ZomFrame::OnReset)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(nullptr, wxID_ANY, title, pos, size)
, mPanel(nullptr)
, mSimMenu(nullptr)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SIM_START, "Start/stop\tSpace", "Start or stop the simulation");
	mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie...", "Load in a Zombie");
    mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Survivor...", "Load in a Survivor");
    mSimMenu->Append(ID_RANDOMIZE, "Randomize", "Add 20 zombies and 10 humans randomly");
    mSimMenu->Append(ID_RESET, "Reset", "Reset grid");
    
    mSimMenu->Enable(ID_SIM_START, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mSimMenu->Enable(ID_RESET, false);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	wxFrame::SetMenuBar(menuBar);
	wxFrame::CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	wxFrame::SetSizer(sizer);
	wxFrame::SetAutoLayout(true);
	wxFrame::Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine
//    MachineState zombieState;
//    World::Get().mZombieStates.push_back(zombieState);
//    World::Get().mZombieMachine.BindState(World::Get().mZombieStates[0]);
//    mZombieMachine.LoadMachine(std::string(""));
//    mZombieMachine.BindState(mZombieTestState);
	// END TEMP CODE
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
//    mZombieMachine.TakeTurn(mZombieTestState);
	// END TEMP CODE
    months++;
    for(int i = 0; i < World::Get().mZombieStates.size(); i++)
    {
        World::Get().mZombieMachine.TakeTurn(World::Get().mZombieStates[i]);
        mPanel->PaintNow();
    }
    for(int i = 0; i < World::Get().mSurvivorStates.size(); i++)
    {
        World::Get().mSurvivorMachine.TakeTurn(World::Get().mSurvivorStates[i]);
        mPanel->PaintNow();
    }
}

void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Load Zombie", "./zom", "",
                                "ZOM Files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;     // the user changed idea...
    }
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream inputStream(openFileDialog.GetPath());
    if (!inputStream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
    }
    else
    {
        std::string filename = openFileDialog.GetPath().ToStdString();
        World::Get().mZombieMachine.LoadMachine(filename);
    }
    zombieLoaded = true;
    if(zombieLoaded && survivorLoaded)
    {
        mSimMenu->Enable(ID_SIM_START, true);
        mSimMenu->Enable(ID_RANDOMIZE, true);
        mSimMenu->Enable(ID_RESET, true);
    }
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Load Survivor", "./zom", "",
                                "ZOM Files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;     // the user changed idea...
    }
    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream inputStream(openFileDialog.GetPath());
    if (!inputStream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
    }
    else
    {
        std::string filename = openFileDialog.GetPath().ToStdString();
        World::Get().mSurvivorMachine.LoadMachine(filename);
    }
    survivorLoaded = true;
    if(zombieLoaded && survivorLoaded)
    {
        mSimMenu->Enable(ID_SIM_START, true);
        mSimMenu->Enable(ID_RANDOMIZE, true);
        mSimMenu->Enable(ID_RESET, true);
    }
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
//    // add 20 zombies
    for(int i = 0; i < 19; i++)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;
        int randO = rand() % (4-1) + 1;
        MachineState zombieState;
        if(World::Get().mOccupiedGrid[randX][randY] != 1 && World::Get().mOccupiedGrid[randX][randY] != 2)
        {
            World::Get().mOccupiedGrid[randX][randY] = 1;
            zombieState.mXCoord = (randX*30) + 10;
            zombieState.mYCoord = (randY*30) + 10;
            zombieState.mGridX = randX;
            zombieState.mGridY = randY;
            switch (randO)
            {
                case (1) :
                    zombieState.mFacing = MachineState::UP;
                    break;
                case (2) :
                    zombieState.mFacing = MachineState::DOWN;
                    break;
                case (3) :
                    zombieState.mFacing = MachineState::RIGHT;
                    break;
                case (4) :
                    zombieState.mFacing = MachineState::LEFT;
                    break;
            }
            World::Get().mGrid[randX][randY] = zombieState;
            World::Get().mZombieStates.push_back(zombieState);
            World::Get().mZombieMachine.BindState(World::Get().mZombieStates[World::Get().mZombieStates.size()]);
        }
        else
        {
            i--;
            continue;
        }
    }
    
    for(int i = 0; i < 10; i++)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;
        int randO = rand() % (4-1) + 1;
        MachineState survivorState;
        if(World::Get().mOccupiedGrid[randX][randY] != 1 && World::Get().mOccupiedGrid[randX][randY] != 2)
        {
            World::Get().mOccupiedGrid[randX][randY] = 2;
            survivorState.mXCoord = (randX*30) + 10;
            survivorState.mYCoord = (randY*30) + 10;
            survivorState.mGridX = randX;
            survivorState.mGridY = randY;
            switch (randO)
            {
                case (1) :
                    survivorState.mFacing = MachineState::UP;
                    break;
                case (2) :
                    survivorState.mFacing = MachineState::DOWN;
                    break;
                case (3) :
                    survivorState.mFacing = MachineState::RIGHT;
                    break;
                case (4) :
                    survivorState.mFacing = MachineState::LEFT;
                    break;
            }
            World::Get().mGrid[randX][randY] = survivorState;
            World::Get().mSurvivorStates.push_back(survivorState);
            World::Get().mSurvivorMachine.BindState(World::Get().mSurvivorStates[World::Get().mSurvivorStates.size()]);
        }
        else
        {
            i--;
            continue;
        }
    }
    mPanel->PaintNow();
}

void ZomFrame::OnReset(wxCommandEvent& event)
{
    World::Get().mSurvivorStates.clear();
    World::Get().mZombieStates.clear();
    mTurnTimer->Stop();
    mIsActive = false;
    months = 0;
    mPanel->PaintNow();
    
}
