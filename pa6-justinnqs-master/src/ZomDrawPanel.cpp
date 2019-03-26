#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include "World.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel) // NOLINT
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
	
	// TODO: Draw rest of grid/zombies/etc
    for(int i = 10; i < 600; i+=30)
    {
        wxPoint start, end;
        start.x = i;
        start.y = 10;
        end.x = i;
        end.y = 610;
        dc.DrawLine(start, end);
    }
    
    for(int i = 10; i < 600; i+=30)
    {
        wxPoint start, end;
        start.x = 10;
        start.y = i;
        end.x = 610;
        end.y = i;
        dc.DrawLine(start, end);
    }
    if(World::Get().mZombieStates.size() != 0)
    {
        dc.SetBrush(*wxGREEN_BRUSH);
        
        for(MachineState state: World::Get().mZombieStates)
        {
            wxPoint mPointVector[3];
            switch (state.mFacing)
            {
                case (MachineState::UP) :
                    mPointVector[0] = wxPoint(state.mXCoord + 15, state.mYCoord);
                    mPointVector[1] = wxPoint(state.mXCoord, state.mYCoord + 30);
                    mPointVector[2] = wxPoint(state.mXCoord + 30, state.mYCoord + 30);
                    dc.DrawPolygon(3, mPointVector);
                    break;
                case (MachineState::DOWN) :
                    mPointVector[0] = wxPoint(state.mXCoord, state.mYCoord);
                    mPointVector[1] = wxPoint(state.mXCoord + 30, state.mYCoord);
                    mPointVector[2] = wxPoint(state.mXCoord + 15, state.mYCoord + 30);
                    dc.DrawPolygon(3, mPointVector);
                    break;
                case (MachineState::LEFT) :
                    mPointVector[0] = wxPoint(state.mXCoord, state.mYCoord+15);
                    mPointVector[1] = wxPoint(state.mXCoord+30, state.mYCoord);
                    mPointVector[2] = wxPoint(state.mXCoord+30, state.mYCoord+30);
                    dc.DrawPolygon(3, mPointVector);
                    break;
                case (MachineState::RIGHT) :
                    mPointVector[0] = wxPoint(state.mXCoord, state.mYCoord);
                    mPointVector[1] = wxPoint(state.mXCoord, state.mYCoord + 30);
                    mPointVector[2] = wxPoint(state.mXCoord + 30, state.mYCoord + 15);
                    dc.DrawPolygon(3, mPointVector);
                    break;
            }
        }
    }
    
    if(World::Get().mSurvivorStates.size() != 0)
    {
        dc.SetBrush(*wxRED_BRUSH);
        
        for(MachineState state: World::Get().mSurvivorStates)
        {
            wxPoint mPointVector[3];
            switch (state.mFacing)
            {
                case (MachineState::UP) :
                    mPointVector[0] = wxPoint(state.mXCoord + 15, state.mYCoord);
                    mPointVector[1] = wxPoint(state.mXCoord, state.mYCoord + 30);
                    mPointVector[2] = wxPoint(state.mXCoord + 30, state.mYCoord + 30);
                    dc.DrawPolygon(3, mPointVector);
                    break;
                case (MachineState::DOWN) :
                    mPointVector[0] = wxPoint(state.mXCoord, state.mYCoord);
                    mPointVector[1] = wxPoint(state.mXCoord + 30, state.mYCoord);
                    mPointVector[2] = wxPoint(state.mXCoord + 15, state.mYCoord + 30);
                    dc.DrawPolygon(3, mPointVector);
                    break;
                case (MachineState::LEFT) :
                    mPointVector[0] = wxPoint(state.mXCoord, state.mYCoord+15);
                    mPointVector[1] = wxPoint(state.mXCoord+30, state.mYCoord);
                    mPointVector[2] = wxPoint(state.mXCoord+30, state.mYCoord+30);
                    dc.DrawPolygon(3, mPointVector);
                    break;
                case (MachineState::RIGHT) :
                    mPointVector[0] = wxPoint(state.mXCoord, state.mYCoord);
                    mPointVector[1] = wxPoint(state.mXCoord, state.mYCoord + 30);
                    mPointVector[2] = wxPoint(state.mXCoord + 30, state.mYCoord + 15);
                    dc.DrawPolygon(3, mPointVector);
                    break;
            }
        }
    }
}
