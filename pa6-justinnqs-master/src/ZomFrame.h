#pragma once
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "Machine.h"
#include "Traits.h"
#include "World.h"

class ZomFrame : public wxFrame
{
public:
	ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnExit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnSimStart(wxCommandEvent& event);
	void OnTurnTimer(wxTimerEvent& event);
    void OnLoadZombie(wxCommandEvent& event);
    void OnLoadSurvivor(wxCommandEvent& event);
    void OnRandomize(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    
	wxDECLARE_EVENT_TABLE(); //NOLINT
private:
	// Panel for drawing
	class ZomDrawPanel* mPanel;
	
	// Simulation menu
	wxMenu* mSimMenu;
	
	// Turn timer
	wxTimer* mTurnTimer;
	
    int months = 0;
	// TEMP CODE TEST MACHINES
//    MachineState mZombieTestState;
//    Machine<ZombieTraits> mZombieMachine;
	// END TEMP CODE
    bool survivorLoaded = false;
    bool zombieLoaded = false;
	bool mIsActive;
};
