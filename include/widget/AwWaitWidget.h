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
	AwWaitWidget(const QString& title, QWidget *parent = Q_NULLPTR);
	~AwWaitWidget();
	template <typename F>
	void run(F function);
	template <typename F, typename T>
	void run(F function, T arg);
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

template<typename F, typename T>
void AwWaitWidget::run(F function, T arg)
{
	m_thread = new std::thread(function, arg);
	exec();
}

template<class C, typename T>
void AwWaitWidget::run(C* ptr, void (C::*method)(T), T arg)
{
	m_thread = new std::thread(method, ptr, arg);
	exec();
}

#endif // AWWAITWIDGET_H
