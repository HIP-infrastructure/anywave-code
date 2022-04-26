// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef AWWAITWIDGET_H
#define AWWAITWIDGET_H
#include <AwGlobal.h>
#include <QWidget>
namespace Ui {class AwWaitWidgetUi;};
#include <QDialog>
#include <thread>

class AW_WIDGETS_EXPORT AwWaitWidget : public QDialog
{
	Q_OBJECT

public:
	AwWaitWidget(const QString& title, QWidget *parent = nullptr);
	~AwWaitWidget();
	template <typename F>
	void run(F function);
	//template <typename F, typename T>
	//void run(F function, T arg);
	template <typename F, typename ...T>
	void run(F function, T... args);
	template<class C,  typename T>
	void run(C* ptr, void (C::* method)(T), T arg);
public slots:
	void setText(const QString& text);
	void setCurrentProgress(int value);
	void initProgress(int min, int max);
	/** WaitWidget should never be closed by reject action **/
	void reject() override {} 
	void accept() override;
	int exec() override;
private:
	Ui::AwWaitWidgetUi *ui;
	std::thread *m_thread;
};

template<typename F>
void AwWaitWidget::run(F function)
{
	m_thread = new std::thread(function);
	exec();
}

template<typename F, typename ...T>
void AwWaitWidget::run(F function, T... args)
{
	m_thread = new std::thread(function, args...);
	exec();
}

//template<typename F, typename ...T>
//void AwWaitWidget::run2(F function, T... args)
//{
//	m_thread = new std::thread(function, args...);
//	exec();
//}


template<class C, typename T>
void AwWaitWidget::run(C* ptr, void (C::*method)(T), T arg)
{
	m_thread = new std::thread(method, ptr, arg);
	exec();
}

#endif // AWWAITWIDGET_H
