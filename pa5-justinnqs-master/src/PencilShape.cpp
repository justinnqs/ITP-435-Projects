#include "PencilShape.h"

PencilShape::PencilShape(const wxPoint & start): Shape(start)
{
    mPointVector.push_back(start);
    mTopLeft.y = mStartPoint.y;
    mBotRight.y = mStartPoint.y;
    
    mBotRight.x = mStartPoint.x;
    mTopLeft.x = mStartPoint.x;
}

void PencilShape::Update(const wxPoint & newPoint)
{
    Shape::Update(newPoint);
    mPointVector.push_back(newPoint);
}

void PencilShape::Finalize()
{
    for(wxPoint wPoint : mPointVector)
    {
        if (wPoint.x > mBotRight.x)
        {
            mBotRight.x = wPoint.x;
        }
        if (wPoint.x < mTopLeft.x)
        {
            mTopLeft.x = wPoint.x;
        }
        if (wPoint.y < mTopLeft.y)
        {
            mTopLeft.y = wPoint.y;
        }
        if (wPoint.y > mBotRight.y)
        {
            mBotRight.y = wPoint.y;
        }
    }
}

void PencilShape::Draw(wxDC& dc) const
{
    dc.SetPen(GetPen());
    dc.SetBrush(GetBrush());
    if (mPointVector.size()>1)
    {
        dc.DrawLines(mPointVector.size(), mPointVector.data());
    }
    else
    {
        dc.DrawPoint(mPointVector.front());
    }
}
