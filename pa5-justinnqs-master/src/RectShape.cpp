#include "RectShape.h"

RectShape::RectShape(const wxPoint & start) :Shape(start)
{
}

void RectShape::Draw(wxDC & dc) const
{
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());
    dc.DrawRectangle(wxRect(mTopLeft, mBotRight));
}
