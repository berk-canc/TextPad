#include "wx/wxprec.h"
#include "TextPad.h"
#include "Frame.h"

IMPLEMENT_APP(TextPad)

bool TextPad::OnInit()
{
	Frame* pFrame = new Frame();
	pFrame->Show(true);
	SetTopWindow(pFrame);

	return true;
}