#include <afxwin.h>

#include "resource.h"
#include "my_mfc_git_test_class.h"

//class GraphicObject :public CObject
IMPLEMENT_SERIAL(GraphicObject, CObject, 1);
GraphicObject::GraphicObject() {}
GraphicObject::GraphicObject(int shape_num, BOOL fill_state, COLORREF fill_color, COLORREF line_color, int line_width, CPoint start_point, CPoint end_point)
		:shape_num_(shape_num), fill_state_(fill_state), fill_color_(fill_color), line_color_(line_color), line_width_(line_width), start_point_(start_point), end_point_(end_point) {

}
GraphicObject::GraphicObject(GraphicObject& g)
		:shape_num_(g.shape_num_), fill_state_(g.fill_state_), fill_color_(g.fill_color_), line_color_(g.line_color_), line_width_(g.line_width_), start_point_(g.start_point_), end_point_(g.end_point_) {
}
GraphicObject& GraphicObject::operator= (GraphicObject& g) {
	shape_num_ = g.shape_num_;
	fill_state_ = g.fill_state_;
	fill_color_ = g.fill_color_;
	line_color_ = g.line_color_;
	line_width_ = g.line_width_;
	start_point_ = g.start_point_;
	end_point_ = g.end_point_;
	return *this;
}
void GraphicObject::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << shape_num_ << fill_state_ << fill_color_ << line_color_ << line_width_ << start_point_ << end_point_;
	}
	else {
		ar >> shape_num_ >> fill_state_ >> fill_color_ >> line_color_ >> line_width_ >> start_point_ >> end_point_;
	}
}

//class Shape
Shape::Shape() {}
Shape::Shape(const Shape& s) : start_point_(s.start_point_),end_point_(s.end_point_), shape_num_(s.shape_num_){}
Shape::Shape(CPoint start_point, CPoint end_point, int shape_num) : start_point_(start_point), end_point_(end_point), shape_num_(shape_num) {}
Shape& Shape::operator = (const Shape& s) {
	start_point_ = s.start_point_;
	end_point_ = s.end_point_;
	return *this;
}
int Shape::GetShapeNum() {
	return shape_num_;
}
void Shape::SetPoint(CPoint start_point, CPoint end_point) {
	start_point_ = start_point;
	end_point_ = end_point;
}

//class LineShape :public Shape
LineShape::LineShape() {
	shape_num_ = 0;
}
LineShape::LineShape(CPoint start_point, CPoint end_point) : Shape(start_point, end_point, 0) {}
LineShape::LineShape(const LineShape& l) : Shape(l.start_point_, l.end_point_, 0) {}
LineShape& LineShape::operator= (LineShape& l) {
	start_point_ = l.start_point_;
	end_point_ = l.end_point_;
	return *this;
}
void LineShape::draw(CDC& a_dc, COLORREF color, COLORREF fcolor, int width, BOOL filled) {
	CPen pen(PS_SOLID, width, color);
	CPen* old_pen = a_dc.SelectObject(&pen);
	a_dc.MoveTo(start_point_);
	a_dc.LineTo(end_point_);
	a_dc.SelectObject(old_pen);
}

//class RectangleShape :public Shape
RectangleShape::RectangleShape() {
	shape_num_ = 1;
}
RectangleShape::RectangleShape(CPoint start_point, CPoint end_point) : Shape(start_point, end_point, 1) {}
RectangleShape::RectangleShape(const RectangleShape& r) : Shape(r.start_point_, r.end_point_, 1) {}
RectangleShape& RectangleShape::operator= (RectangleShape& r) {
	start_point_ = r.start_point_;
	end_point_ = r.end_point_;
	return *this;
}
void RectangleShape::draw(CDC& a_dc, COLORREF color, COLORREF fcolor, int width, BOOL filled) {
	CRect rect(start_point_, end_point_);
	CPen pen(PS_SOLID, width, color);
	CPen* old_pen = a_dc.SelectObject(&pen);
	a_dc.SelectStockObject(NULL_BRUSH);
	a_dc.Rectangle(rect);
	a_dc.SelectObject(old_pen);
}

//class EllipseShape :public Shape
EllipseShape::EllipseShape() {
	shape_num_ = 2;
}
EllipseShape::EllipseShape(CPoint start_point, CPoint end_point) : Shape(start_point, end_point, 2) {}
EllipseShape::EllipseShape(const EllipseShape& e) : Shape(e.start_point_, e.end_point_, 2) {}
EllipseShape& EllipseShape::operator= (EllipseShape& e) {
	start_point_ = e.start_point_;
	end_point_ = e.end_point_;
	return *this;
}
void EllipseShape::draw(CDC& a_dc, COLORREF color, COLORREF fcolor, int width, BOOL filled) {
	CRect rect(start_point_, end_point_);
	CPen pen(PS_SOLID, width, color);
	CPen* old_pen = a_dc.SelectObject(&pen);
	a_dc.SelectStockObject(NULL_BRUSH);
	a_dc.Ellipse(rect);
	a_dc.SelectObject(old_pen);
}

//class MyDocument :public CDocument
IMPLEMENT_DYNCREATE(MyDocument,CDocument)
BEGIN_MESSAGE_MAP(MyDocument, CDocument)
END_MESSAGE_MAP()
void MyDocument::AddGraph(GraphicObject& add_graph) {
	SetModifiedFlag(true);
	record_graph_array_.Add(add_graph);
}
GraphicObject& MyDocument::GetGraph(int i) {
	return record_graph_array_[i];
}
int MyDocument::GetObjectSize() {
	return record_graph_array_.GetSize();
}
void MyDocument::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	record_graph_array_.Serialize(ar);
}
void MyDocument::DeleteContents() {
	record_graph_array_.RemoveAll();
	CDocument::DeleteContents();
}

//class MyFrame : public CFrameWnd 
IMPLEMENT_DYNCREATE(MyFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(MyFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()
MyFrame::MyFrame() {}
MyFrame::~MyFrame() {}
afx_msg int MyFrame::OnCreate(LPCREATESTRUCT lp_create_struct) {
	if (CFrameWnd::OnCreate(lp_create_struct)) { return 1; }
	tool_bar_.Create(this);
	tool_bar_.LoadToolBar(IDR_MyFrame);
	tool_bar_.EnableDocking(CBRS_ALIGN_ANY);
	tool_bar_.SetBarStyle(tool_bar_.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&tool_bar_);
	static UINT indicators[] = {
		ID_SEPARATOR, IDS_COLOR, IDS_SHAPE
	};
	status_bar_.Create(this);
	status_bar_.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	return 0;
}

//class GlobalView :public CView 
IMPLEMENT_DYNCREATE(GlobalView, CView)
BEGIN_MESSAGE_MAP(GlobalView,CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()
GlobalView::GlobalView() {
	dc_size_ = CSize(800, 800);
}
afx_msg void GlobalView::OnDraw(CDC* p_dc) {
	CView::OnDraw(p_dc);
	CPen* p_pen = new CPen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* p_old_pen = p_dc->SelectObject(p_pen);
	p_dc->Rectangle(0, 0, int(dc_size_.cx*scale_), int(dc_size_.cy*scale_));
	p_dc->SelectObject(p_old_pen);
	delete p_pen;
	MyFrame* p_frame = (MyFrame*)GetParentFrame();
	main_view_ = ((MyView*)p_frame->static_split_.GetPane(0, 1))->GetViewPos();
	DataCoorToDCCoor(&main_view_.TopLeft());
	DataCoorToDCCoor(&main_view_.BottomRight());
	p_pen = new CPen(PS_DASH, 1, RGB(0, 0, 0));
	p_dc->SelectObject(p_pen);
	p_dc->Rectangle(main_view_);
	p_dc-> SelectObject(p_old_pen);
	delete p_pen;

	Shape* p_drawing_shape;
	MyDocument* p_doc = (MyDocument*)GetDocument();
	int num = p_doc->GetObjectSize();
	for (int i; i < num; ++i) {
		GraphicObject* p_record_graph = &(p_doc->GetGraph(i));
		switch (p_record_graph->shape_num_) {
		case 0:
			p_drawing_shape = new LineShape;
			break;
		case 1:
			p_drawing_shape = new RectangleShape;
			break;
		case 2:
			p_drawing_shape = new EllipseShape;
			break;
		default:
			break;
		}
		CPoint new_start_point = p_record_graph->start_point_;
		CPoint new_end_point = p_record_graph->end_point_;
		int new_line_width = (int)(p_record_graph->line_width_*scale_ < 1 ? 1 : p_record_graph->line_width_*scale_);
		DataCoorToDCCoor(&new_start_point);
		DataCoorToDCCoor(&new_end_point);
		p_drawing_shape->SetPoint(new_start_point, new_end_point);
		p_drawing_shape->draw((*p_dc), p_record_graph->line_color_, p_record_graph->fill_color_, new_line_width, p_record_graph->fill_state_);
		delete p_drawing_shape;
	}
}
void GlobalView::UpdateMainViewRect(CRect repaint_rect) {
	InvalidateRect(&main_view_);
	main_view_ = repaint_rect;
	DataCoorToDCCoor(&main_view_.TopLeft());
	DataCoorToDCCoor(&main_view_.BottomRight());
	CClientDC a_dc(this);
	CPen* p_pen = new CPen(PS_DASH, 1, RGB(0, 0, 0));
	CPen* p_old_pen = a_dc.SelectObject(p_pen);
	a_dc.Rectangle(main_view_);
	a_dc.SelectObject(p_old_pen);
	delete p_pen;
}
void GlobalView::OnSize(UINT n_type, int cx, int cy) {
	CView::OnSize(n_type, cx, cy);
	scale_ = (double)cx / dc_size_.cx;
}
void GlobalView::DataCoorToDCCoor(CPoint* p_point) {
	p_point->x = (int)(p_point->x * scale_);
	p_point->y = (int)(p_point->y * scale_);
}
void GlobalView::OnUpdate(CView* p_sender, LPARAM l_hint, CObject* p_hint) {
	if (p_hint != NULL) {
		CRect rect((CRect*)p_hint);
		rect.NormalizeRect();
		DataCoorToDCCoor(&rect.TopLeft());
		DataCoorToDCCoor(&rect.BottomRight());
		InvalidateRect(&rect);
	}
	else {
		CView::OnUpdate(p_sender, l_hint, p_hint);
	}
}
//class MyView :public CScrollView
IMPLEMENT_DYNCREATE(MyView, CScrollView)
BEGIN_MESSAGE_MAP(MyView, CScrollView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_COLORMENU_RED, OnRed)
	ON_COMMAND(ID_COLORMENU_GREEN, OnGreen)
	ON_COMMAND(ID_COLORMENU_BLUE, OnBlue)
	ON_UPDATE_COMMAND_UI(ID_COLORMENU_RED, OnUpdateRed)
	ON_UPDATE_COMMAND_UI(ID_COLORMENU_GREEN, OnUpdateGreen)
	ON_UPDATE_COMMAND_UI(ID_COLORMENU_BLUE, OnUpdateBlue)
	ON_COMMAND(ID_SHAPE_LINE, OnLine)
	ON_COMMAND(ID_SHAPE_RECTANGLE, OnRect)
	ON_COMMAND(ID_SHAPE_ELLIPSE, OnEllipse)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_LINE, OnUpdatUpdateLine)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_RECTANGLE, OnUpdatRect)
	ON_UPDATE_COMMAND_UI(ID_SHAPE_ELLIPSE, OnUpdatEllipse)
END_MESSAGE_MAP()
MyView::MyView() {
	line_color_ = RGB(255, 0, 0);
	fill_color_ = RGB(0, 0, 0);
	p_shape_ = &line_shape_;
	line_width_ = 2;
}
MyView::~MyView() {}
afx_msg int MyView::OnCreate(LPCREATESTRUCT lp_create_struct) {
	if (CScrollView::OnCreate(lp_create_struct) == -1) {
		return -1;
	}
	CSize dc_size(800, 800);
	SetScrollSizes(MM_TEXT, dc_size);
	return 0;
}
afx_msg void MyView::OnDraw(CDC* p_dc) {
	CView::OnDraw(p_dc);                                    //do nothing 
	MyDocument* doc = (MyDocument*)GetDocument();
	int num = doc->GetObjectSize();
	for (int i = 0; i < num; ++i) {
		GraphicObject* p_graph = &(doc->GetGraph(i));
		switch (p_graph->shape_num_) {
		case 0:
			rd_shape_ = new LineShape;
			break;
		case 1:
			rd_shape_ = new RectangleShape;
			break;
		case 2:
			rd_shape_ = new EllipseShape;
			break;
		}
		rd_shape_->SetPoint(p_graph->start_point_, p_graph->end_point_);
		rd_shape_->draw((*p_dc), p_graph->line_color_, p_graph->fill_color_, p_graph->line_width_);
		delete rd_shape_;
	}

}
afx_msg void MyView::OnLButtonDown(UINT n_flags, CPoint point) {
	SetCapture();
	if (this == GetCapture()) {
		DCCoortoDataCoor(&point);
		(*p_shape_).start_point_ = (*p_shape_).end_point_ = point;
	}
}
afx_msg void MyView::OnMouseMove(UINT n_flags, CPoint point) {
	if (this == GetCapture()) {
		CClientDC a_dc(this);
		a_dc.SetROP2(R2_NOT);
		OnPrepareDC(&a_dc);
		DCCoortoDataCoor(&point);
		(*p_shape_).draw(a_dc, line_color_, fill_color_, 2);
		(*p_shape_).end_point_ = point;
		(*p_shape_).draw(a_dc, line_color_, fill_color_, 2);
	}
}
afx_msg void MyView::OnLButtonUp(UINT n_flags, CPoint point) {
	if (this == GetCapture()) {
		CClientDC a_dc(this);
		DCCoortoDataCoor(&point);
		(*p_shape_).end_point_ = point;
		(*p_shape_).draw(a_dc, line_color_, fill_color_, line_width_);
		GraphicObject graphic(p_shape_->GetShapeNum(), true, fill_color_, line_color_, line_width_, p_shape_->start_point_, p_shape_->end_point_);
		MyDocument* doc = (MyDocument*)GetDocument();
		doc->AddGraph(graphic);
		DataCoorToDCCoor(&p_shape_->start_point_);
		DataCoorToDCCoor(&p_shape_->end_point_);
		CRect rect(p_shape_->start_point_, p_shape_->end_point_);
		rect.NormalizeRect();
		rect.InflateRect(5, 5);
		InvalidateRect(&rect);
		ReleaseCapture();
	}
}
afx_msg void MyView::OnRed() {
	line_color_ = RGB(255, 0, 0);
	((MyFrame*)GetParentFrame())->status_bar_.SetPaneText(1, _T("Red"));
}
afx_msg void MyView::OnGreen() {
	line_color_ = RGB(0, 255, 0);
	((MyFrame*)GetParentFrame())->status_bar_.SetPaneText(1, _T("Green"));
}
afx_msg void MyView::OnBlue() {
	line_color_ = RGB(0, 0, 255);
	((MyFrame*)GetParentFrame())->status_bar_.SetPaneText(1, _T("Blue"));
}
afx_msg void MyView::OnUpdateRed(CCmdUI* a_cmd_ui) {
	a_cmd_ui->SetCheck(line_color_ == RGB(255, 0, 0));
}
afx_msg void MyView::OnUpdateGreen(CCmdUI* a_cmd_ui) {
	a_cmd_ui->SetCheck(line_color_ == RGB(0, 255, 0));
}
afx_msg void MyView::OnUpdateBlue(CCmdUI* a_cmd_ui) {
	a_cmd_ui->SetCheck(line_color_ == RGB(0, 0, 255));
}
afx_msg void MyView::OnLine() {
	p_shape_ = &line_shape_;
	((MyFrame*)GetParentFrame())->status_bar_.SetPaneText(2, _T("Line"));
}
afx_msg void MyView::OnRect() {
	p_shape_ = &rectangle_shape_;
	((MyFrame*)GetParentFrame())->status_bar_.SetPaneText(2, _T("Rectangle"));
}
afx_msg void MyView::OnEllipse() {
	p_shape_ = &ellipse_shape_;
	((MyFrame*)GetParentFrame())->status_bar_.SetPaneText(2, _T("Ellipse"));
}
afx_msg void MyView::OnUpdatUpdateLine(CCmdUI* a_cmd_ui) {
	a_cmd_ui->SetCheck((*p_shape_).shape_num_ == 0);
}
afx_msg void MyView::OnUpdatRect(CCmdUI* a_cmd_ui) {
	a_cmd_ui->SetCheck((*p_shape_).shape_num_ == 1);
}
afx_msg void MyView::OnUpdatEllipse(CCmdUI* a_cmd_ui) {
	a_cmd_ui->SetCheck((*p_shape_).shape_num_ == 2);
}
void MyView::DCCoortoDataCoor(CPoint* p_point) {
	CPoint scroll_position = GetScrollPosition();
	p_point->x = p_point->x + scroll_position.x;
	p_point->y = p_point->y + scroll_position.y;
}
void MyView::DataCoorToDCCoor(CPoint* p_point) {
	CPoint scroll_position = GetScrollPosition();
	p_point->x = p_point->x - scroll_position.x;
	p_point->y = p_point->y - scroll_position.y;
}
CRect MyView::GetViewPos() {
	//TODO(Charles20190114):
}

//class MyApp : public CWinApp 
BEGIN_MESSAGE_MAP(MyApp,CWinApp)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()
BOOL MyApp::InitInstance() {
	CSingleDocTemplate* a_doc_template;
	a_doc_template = new CSingleDocTemplate(IDR_MyFrame,
																					RUNTIME_CLASS(MyDocument),
																					RUNTIME_CLASS(MyFrame),
																					RUNTIME_CLASS(MyView));
	AddDocTemplate(a_doc_template);
	CDocument* a_doc = a_doc_template->CreateNewDocument();
	CFrameWnd* p_frame = a_doc_template->CreateNewFrame(a_doc, NULL);
	m_pMainWnd = p_frame;
	a_doc_template->InitialUpdateFrame(p_frame, a_doc);
	p_frame->ShowWindow(SW_SHOW);
	return true;
}

