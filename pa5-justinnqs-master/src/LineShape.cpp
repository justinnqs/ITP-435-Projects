#include "LineShape.h"

LineShape::LineShape(const wxPoint & start):Shape(start)
{
}

void LineShape::Draw(wxDC & dc) const
{
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());
    dc.DrawLine(mStartPoint, mEndPoint);
}
