#pragma once

#include "hui.h"
using namespace HUI;
using namespace Control;
class MainWindow : public UIWindow
{
public:
	BOOL AfterCreated();

	void OnBtnClick(const UIControl *sender);

	void OnLabelClick(const UIControl *sender);

	void OnTxtChanged(const UIControl *sender);

private:
	UIButton *button;
	UILabel *label;
	UIEdit *edit;
private:
	void CreateControls();
};