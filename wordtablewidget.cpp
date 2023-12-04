#include "wordtablewidget.h"
#include "cellbtn.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPushButton>
WordTableWidget::WordTableWidget(QWidget* parent)
	: QTableWidget(parent)
{
	m_addBtn = new QPushButton("+", this);
	m_addBtn->setToolTip("添加释义");
	m_addBtn->setGeometry(0, 0, 16, 16);
	m_addBtn->setContentsMargins(QMargins(0, 0, 0, 0));
	// 设置背景色半透明
	m_addBtn->setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");
	m_addBtn->hide();
	setColumnCount(m_col_count);
	setFocusPolicy(Qt::NoFocus);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setHorizontalHeaderLabels(QStringList() << "生词" << "释义" << "词根" << "例句");
	AppendRow();
	// 最后一行写入后增加新行
	connect(this, &WordTableWidget::itemChanged, this, [this](QTableWidgetItem *item)
		{
			if (!item->text().isEmpty() && item->row() == this->rowCount() - 1)
				AppendRow();
		});

	connect(this, &WordTableWidget::cellClicked, this, &WordTableWidget::onCellClicked);
	connect(m_addBtn, &QPushButton::clicked, this, &WordTableWidget::onAddBtnClicked);
}

int WordTableWidget::AppendRow()
{
	int newRowCnt = rowCount() + 1;
	setRowCount(newRowCnt);
	m_rowSpanMap.insert(newRowCnt, 1);
	//CellBtn* btn = new CellBtn(newRowCnt - 1);
	//btn->setText("显示");
	//connect(btn, &CellBtn::clicked, this, &WordTableWidget::ShowSentence);
	//setCellWidget(newRowCnt - 1, senetenceCol, btn);
	//m_cellBtnList.push_back(btn);
	return (newRowCnt - 1);
}

bool WordTableWidget::CheckRowEmpty(int row)
{
	return (item(row, wordCol) == nullptr)
		&& (item(row, transCol) == nullptr)
		&& (item(row, rootCol) == nullptr);
}

WordTableWidget::~WordTableWidget()
{}

void WordTableWidget::AppendWordRecord(const WordModel word)
{
	if (word.isEmpty())
		return;
	// 若最后一行为空，则在最后一行添加;否则额外增加一行
	int row = CheckRowEmpty(rowCount() - 1) ? rowCount() - 1 : AppendRow();
	this->setItem(row, wordCol, new QTableWidgetItem(word.GetWord()));
	this->setItem(row, transCol, new QTableWidgetItem(word.GetTranslation()));
	this->setItem(row, rootCol, new QTableWidgetItem(word.GetRoot()));
	if (row > m_sentenceList.length())
		m_sentenceList.append(word.GetSentence());
	else
		m_sentenceList.insert(row, word.GetSentence());
}

WordModel WordTableWidget::GetWordRecord(int row)
{
	if (CheckInvalid(row, rowCount()))
		return NULL;
	WordModel word;
	if(item(row, wordCol))
		word.SetWord(item(row, wordCol)->text());
	if(item(row, transCol))
		word.SetTranslation(item(row, transCol)->text());
	if(item(row, rootCol))
		word.SetRoot(item(row, rootCol)->text());
	if(row < m_sentenceList.size())
		word.SetSentence(m_sentenceList.at(row));
	return word;
}

QList<WordModel> WordTableWidget::Pack()
{
	QList<WordModel> wordList;
	for (size_t row = 0; row < rowCount(); row++)
	{
		WordModel word = GetWordRecord(row);
		if (!word.isEmpty())
			wordList.push_back(word);
	}
	return wordList;
}

bool WordTableWidget::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		// 处理按键事件
		qDebug() << "keypress";
		keyPressEvent(keyEvent);
		return true; // 返回true表示事件已被处理，不再传播
	}
	return false; // 返回false继续事件传播
}

void WordTableWidget::keyPressEvent(QKeyEvent* event)
{
	// 监测delete键
	if (event->key() == Qt::Key_Delete)
	{
		qDebug() << "delete";
		QList<QTableWidgetItem*> items = selectedItems();
		if (items.isEmpty())
			return;
		int row = items.at(0)->row();
		// 删除行
		removeRow(row);
		// 删除例句
		if (row < m_sentenceList.size())
			m_sentenceList.removeAt(row);
	}
	QTableWidget::keyPressEvent(event);
}

void WordTableWidget::mousePressEvent(QMouseEvent* event)
{
	emit TableClicked();
	m_addBtn->hide();
	QTableWidget::mousePressEvent(event);
}

void WordTableWidget::ShowSentence()
{
	CellBtn* btn = qobject_cast<CellBtn*>(sender());
	int row = btn->GetRow();
	if (!CheckInvalid(row, m_sentenceList.size()))
	{
		qDebug() << "ShowSentence " << btn->GetRow() << m_sentenceList.at(row);
		QMessageBox::information(this, "例句", m_sentenceList.at(row));
	}
}
void WordTableWidget::onCellClicked(int row, int col)
{
	// 仅点击第一列时触发
	if (col == wordCol)
	{
        QModelIndex index = model()->index(row, col);
        QRect rect = visualRect(index);
		QPoint newPos(rect.topRight().x() + 5, rect.y() + m_addBtn->height() * 2);
		m_addBtn->move(newPos);
		qDebug() << rect << newPos << m_addBtn->pos();
		m_addBtn->show();
	}
	else
	{
		m_addBtn->hide();
	}
}

void WordTableWidget::onAddBtnClicked()
{
	// 获取当前行
	int row = currentRow();
	qDebug() << "onAddBtnClicked " << row;
	insertRow(row + 1);
	setSpan(row, wordCol, 2, 1);
}

bool WordTableWidget::CheckInvalid(int i, int top, int bottom)
{
	return (i < bottom || i >= top);
}
