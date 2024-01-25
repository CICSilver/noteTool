#pragma once

#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QKeyEvent>
#include <QDebug>
class CellEdit : public QTextEdit
{
	Q_OBJECT
public:
	CellEdit(QStyledItemDelegate* _delegate, QWidget* parent = nullptr)
		: QTextEdit(parent), m_delegate(_delegate)
	{}
	~CellEdit(){}

	void keyPressEvent(QKeyEvent* e) override
	{
		if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
		{
			m_delegate->commitData(this);
			m_delegate->closeEditor(this);
		}
		else
		{
			QTextEdit::keyPressEvent(e);
		}
	}
private:
	QStyledItemDelegate* m_delegate;
};

class TextEditDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	TextEditDelegate(QObject *parent = nullptr);
	~TextEditDelegate();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model,const QModelIndex& index) const override;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
