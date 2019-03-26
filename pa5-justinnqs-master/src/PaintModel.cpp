#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
    mCurrentCommand = nullptr;
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
    mSelectedShape = nullptr;
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
    // TODO
    if (mLoadMap.IsOk())
    {
        dc.DrawBitmap(mLoadMap,0,0);
    }
    for(std::shared_ptr<Shape> shape : mShapes)
    {
//        dc.SetPen(shape->GetPen());
//        dc.SetBrush(shape->GetBrush());
        shape->Draw(dc);
        if (mSelectedShape!=nullptr)
        {
            mSelectedShape->DrawSelection(dc);
        }
    }
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
    // TODO
    mLoadMap = wxBitmap();
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
    mCurrentCommand = nullptr;
    mSelectedShape = nullptr;
    while (!mRedoStack.empty())
    {
        mRedoStack.pop();
    }
    while (!mUndoStack.empty())
    {
        mUndoStack.pop();
    }
    mShapes.clear();
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
    mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
    auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
    if (iter != mShapes.end())
    {
        mShapes.erase(iter);
    }
}
bool PaintModel::HasActiveCommand() const
{
    return mCurrentCommand != nullptr;
}
void PaintModel::CreateCommand(CommandType type, const wxPoint& start)
{
    mCurrentCommand = CommandFactory::Create(shared_from_this(), type, start);
}
void PaintModel::UpdateCommand(wxPoint& newPoint)
{
    mCurrentCommand->Update(newPoint);
}
void PaintModel::Finalize()
{
    mCurrentCommand->Finalize( shared_from_this());
    mUndoStack.push(mCurrentCommand);
    mCurrentCommand = nullptr;
    while (!mRedoStack.empty())
    {
        mRedoStack.pop();
    }
}
void PaintModel::Undo()
{
    mUndoStack.top()->Undo(shared_from_this());
    mRedoStack.push(mUndoStack.top());
    mUndoStack.pop();
}

void PaintModel::Redo()
{
    mRedoStack.top()->Redo(shared_from_this());
    mUndoStack.push(mRedoStack.top());
    mRedoStack.pop();
}

void PaintModel::Intersect(const wxPoint& point)
{
    for ( int i = static_cast<int> (mShapes.size()-1); i >= 0; i--)
    {
        if (mShapes[i]->Intersects(point))
        {
            mSelectedShape = mShapes[i];
            return;
        }
    }
    mSelectedShape = nullptr;

}

void PaintModel::Export(const std::string& filename, const wxSize& size)
{
    Unselect();
    std::string extension;
    bool start = false;
    for (unsigned int i = 0; i <filename.size(); i++)
    {
        if (filename[i] == '.')
        {
            start = true;
            continue;
        }
        if (start)
        {
            extension += filename[i];
        }
    }
    wxBitmapType type;
    
    if (extension == "jpg" || extension == "jpeg")
    {
        type = wxBITMAP_TYPE_JPEG;
    }
    else if (extension == "bmp")
    {
        type = wxBITMAP_TYPE_BMP;
    }
    else if (extension == "png")
    {
        type = wxBITMAP_TYPE_PNG;
    }
    else
    {
        return;
    }
    
    wxBitmap bitmap;
    // Create the bitmap of the specified wxSize
    bitmap.Create(size);
    // Create a memory DC to draw to the bitmap
    wxMemoryDC dc(bitmap);
    // Clear the background color
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    // Draw all the shapes (make sure not the selection!)
    DrawShapes(dc);
    // Write the bitmap with the specified file name and wxBitmapType
    bitmap.SaveFile(filename, type);
    
}
void  PaintModel::Import(const std::string& filename)
{
    Unselect();
    std::string extension;
    bool start = false;
    for (unsigned int i = 0; i <filename.size(); i++)
    {
        if (filename[i] == '.')
        {
            start = true;
            continue;
        }
        if (start)
        {
            extension += filename[i];
        }
    }
    wxBitmapType type;
    
    if (extension == "jpg" || extension == "jpeg")
    {
        type = wxBITMAP_TYPE_JPEG;
    }
    else if (extension == "bmp")
    {
        type = wxBITMAP_TYPE_BMP;
    }
    else if (extension == "png")
    {
        type = wxBITMAP_TYPE_PNG;
    }
    else
    {
        return;
    }
    if (mLoadMap.IsOk())
    {
        New();
    }
    mLoadMap.LoadFile(filename, type);
}

