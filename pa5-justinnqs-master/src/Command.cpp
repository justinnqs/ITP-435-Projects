#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"
#include "DrawCommand.h"
#include "Shape.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineShape.h"
#include "PencilShape.h"
#include "SetPenCommand.h"
#include "SetBrushCommand.h"
#include "DeleteCommand.h"

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;
	
	// TODO: Actually create a command based on the type passed in
    std::shared_ptr<Shape> shape;
    // TODO: Actually create a command based on the type passed in
    switch (type)
    {
        case CM_DrawRect:
            shape = std::make_shared<RectShape>(start);
            break;
        case CM_DrawEllipse:
            shape = std::make_shared<EllipseShape>(start);
            break;
        case CM_DrawLine:
            shape = std::make_shared<LineShape>(start);
            break;
        case CM_DrawPencil:
            shape = std::make_shared<PencilShape>(start);
            break;
        case CM_SetPen:
            shape = model->GetSelectedShape();
            return std::make_shared<SetPenCommand>(start, shape);
        case CM_SetBrush:
            shape = model->GetSelectedShape();
            return std::make_shared<SetBrushCommand>(start, shape);
        case CM_Delete:
            shape = model->GetSelectedShape();
            return std::make_shared<DeleteCommand>(start, shape);
    }
    shape->SetBrush(model->GetBrush());
    shape->SetPen(model->GetPen()); 
    model->AddShape(shape);
    return std::make_shared<DrawCommand>(start, shape);
}
