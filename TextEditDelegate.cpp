#include "TextEditDelegate.h"
#include <QTextEdit>
TextEditDelegate::TextEditDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

TextEditDelegate::~TextEditDelegate()
{}

QWidget* TextEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//QTextEdit* editor = new QTextEdit(parent);
	CellEdit* editor = new CellEdit(const_cast<TextEditDelegate*>(this), parent);
	return editor;
}

void TextEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QString val = index.model()->data(index, Qt::EditRole).toString();
	CellEdit* newTextEdit = static_cast<CellEdit*>(editor);
	newTextEdit->setText(val);
}

void TextEditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	CellEdit* newTextEdit = static_cast<CellEdit*>(editor);

	QString val = newTextEdit->toPlainText();
	model->setData(index, val, Qt::EditRole);
}

void TextEditDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
