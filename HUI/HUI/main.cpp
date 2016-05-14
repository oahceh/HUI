#include "main.h"

UIButtonConfig* GetDefaultButtonConfig() {
	static UIButtonConfig *config = NULL;
	if (!config) {
		config = new UIButtonConfig();
		config->text_disabled_color = RGB(163,163,163);
		config->text_normal_color = RGB(0,0,255);
		config->text_focus_color = RGB(0,255,0);
		config->normal_img = Utils::GetAppPath() + L"/resources/button_normal.bmp";
		config->hover_img = Utils::GetAppPath() + L"/resources/button_hover.bmp";	
		config->pressed_img = Utils::GetAppPath() +  L"/resources/button_down.bmp";
		config->disabled_img = Utils::GetAppPath() +  L"/resources/common_disable.png";
	}
	return config;
}


BOOL MainWindow::AfterCreated() {
	SetIcon((Utils::GetAppPath() + L"resources\\icon.ico").c_str());
	CreateControls();
	return TRUE;
}


void MainWindow::CreateControls() {
	//UIContainer *pRootControl = new UIContainer();

	Layout::UIGrid *pRootControl = new Layout::UIGrid();
	pRootControl->SetName("根Grid");
	pRootControl->SetRowsAndCols(2, 1);
	pRootControl->SetHeightScale(2, 5, 1);
	pRootControl->SetWidthScale(1, 1);
	//pRootControl->ShowGridLine(true);
	pRootControl->SetAlignment(AlignmentFill);
	SetRootControl(pRootControl);

	Layout::UIGrid *grid = new Layout::UIGrid();
	grid->SetName("子Grid");
	grid->SetRowsAndCols(1, 5);
	grid->SetWidthScale(5, 1, 1, 1, 1, 1);
	grid->SetHeightScale(1, 1);
	//grid->ShowGridLine(true);
	grid->SetAlignment(AlignmentFill);

	pRootControl->Add(grid);
	pRootControl->SetPos(grid, 1, 0);

	button = new UIButton();
	button->SetName("Test Button");
	button->SetSize(40, 30);
	button->SetAlignment(AlignmentCenter);
	button->LoadButtonConfig(*GetDefaultButtonConfig());
	button->SetText(L"上一张");
	button->SetTextFontSize(10);
	button->SetTextFontFamily(L"黑体");
	button->MouseClick += EventHandler<MainWindow>(&MainWindow::OnBtnClick, this);

	UIButton *button1 = new UIButton();
	button1->SetName("播放");
	button1->SetSize(40, 30);
	button1->SetAlignment(AlignmentCenter);
	button1->LoadButtonConfig(*GetDefaultButtonConfig());
	button1->SetText(L"打开");
	button1->SetTextFontSize(10);
	button1->SetTextFontFamily(L"黑体");

	UIButton *button2 = new UIButton();
	button2->SetName("下一张");
	button2->SetSize(40, 30);
	button2->SetAlignment(AlignmentCenter);
	button2->LoadButtonConfig(*GetDefaultButtonConfig());
	button2->SetText(L"下一张");
	button2->SetTextFontSize(10);
	button2->SetTextFontFamily(L"黑体");

	UIImage *img = new UIImage();
	//img->SetSize(100, 100);
	img->SetAlignment(AlignmentFill);
	//img->ShowBorder(true);
	img->SetImg((Utils::GetAppPath() + L"resources\\user.png").c_str());

	pRootControl->Add(img);
	pRootControl->SetPos(img, 0, 0);

	grid->Add(button);
	grid->Add(button1);
	grid->Add(button2);

	grid->SetPos(button, 0, 1);
	grid->SetPos(button1, 0, 2);
	grid->SetPos(button2, 0, 3);
}

void MainWindow::OnBtnClick(const UIControl *sender) {
	Utils::OpenURL(label->GetURL());
}

void MainWindow::OnLabelClick(const UIControl *sender) {
	UILabel *label = (UILabel*)(sender);
}

void MainWindow::OnTxtChanged(const UIControl *sender) {
	label->SetURL(((UIEdit*)sender)->GetEditText());
}

