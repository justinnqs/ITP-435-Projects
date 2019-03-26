#include "SetBrushCommand.h"
#include "PaintModel.h"

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape = model->GetSelectedShape();
    mOldBrush = mShape->GetBrush();
    mShape->SetBrush(model->GetBrush());
    mNewBrush = mShape->GetBrush();
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
    mShape->SetBrush(mOldBrush);
    mOldBrush = mNewBrush;
    mNewBrush = mShape->GetBrush();
    
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
    mShape->SetBrush(mOldBrush);
    mOldBrush = mNewBrush;
    mNewBrush = mShape->GetBrush();
}
