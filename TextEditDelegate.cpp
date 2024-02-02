#include "TextEditDelegate.h"
#include <QTextEdit>
#include <QPainter>
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

void HighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	// ��ȡword��Ԫ�������
	QString word = index.model()->index(index.row(), m_wordCol).data().toString();

	// ����������ʽ
	QTextCharFormat highlightFormat;
	highlightFormat.setForeground(Qt::red);
	highlightFormat.setFontWeight(QFont::Bold);

	// ����һ���ĵ��������ĵ��Ͻ���QTextCursor����
	QTextDocument doc;
	doc.setPlainText(opt.text);
	QTextCursor cursor(&doc);
	while (!cursor.isNull() && !cursor.atEnd()) {
		cursor = doc.find(word, cursor);
		if (!cursor.isNull()) {
			cursor.mergeCharFormat(highlightFormat);
		}
	}

	// �����ĵ�������
	painter->save();
	painter->translate(opt.rect.topLeft());
	doc.drawContents(painter);
	painter->restore();
}
