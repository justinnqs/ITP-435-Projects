#include "SetPenCommand.h"
#include "PaintModel.h"
void SetPenCommand::Update(const wxPoint & newPoint)
{
    
}
void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape = model->GetSelectedShape();
    mOldPen = mShape->GetPen();
    mShape->SetPen(model->GetPen());
    mNewPen = mShape->GetPen();
}

void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mOldPen);
    mOldPen = mNewPen;
    mNewPen = mShape->GetPen();
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mOldPen);
    mOldPen = mNewPen;
    mNewPen = mShape->GetPen();
}
