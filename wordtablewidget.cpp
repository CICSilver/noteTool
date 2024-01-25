#include "wordtablewidget.h"
#include "TranslationDao.h"
#include "cellbtn.h"
#include "TextEditDelegate.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QCryptographicHash>

WordTableWidget::WordTableWidget(QWidget* parent)
	: QTableWidget(parent)
{
	m_lastRow = -1;
	m_col_count = 4;
	m_textEditDelegate = new TextEditDelegate;
	m_addBtn = new QPushButton("+", this);
	m_addBtn->setToolTip("添加释义");
	m_addBtn->setGeometry(0, 0, 16, 16);
	m_addBtn->setContentsMargins(QMargins(0, 0, 0, 0));
	// 设置背景色半透明
	m_addBtn->setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");
	m_addBtn->hide();
	// 设置隔行变色
	setAlternatingRowColors(true); // 隔行变色
	setPalette(QPalette(Qt::gray)); // 设置隔行变色的颜色  gray灰色
	setColumnCount(m_col_count);
	setFocusPolicy(Qt::NoFocus);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setHorizontalHeaderLabels(QStringList() << "生词" << "中文释义" << "英文释义" << "词根" << "例句");
	this->setItemDelegateForColumn(1, m_textEditDelegate);
	this->setItemDelegateForColumn(2, m_textEditDelegate);
	//this->setWordWrap(true);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	AppendRow();
	// 最后一行写入后增加新行
	connect(this, &WordTableWidget::itemChanged, this, [this](QTableWidgetItem *item)
		{
			int realRow = this->rowSpan(item->row(), item->column()) + item->row() - 1;
			if (!item->text().isEmpty() && realRow == m_lastRow)
				AppendRow();
		});

	connect(this, &WordTableWidget::cellClicked, this, &WordTableWidget::onCellClicked);
	connect(m_addBtn, &QPushButton::clicked, this, &WordTableWidget::onAddBtnClicked);
}

int WordTableWidget::AppendRow(int row, bool isSpan)
{
	if (!isSpan)
	{
		// 末尾添加空行
		int newRowCnt = rowCount() + 1;
		m_lastRow = newRowCnt - 1;
		setRowCount(newRowCnt);
		//m_rowSpanMap.insert(newRowCnt - 1, 1);
		return (newRowCnt - 1);
	}
	else
	{
		++m_lastRow;
		int curRow = row >= 0 ? row : this->currentRow();
		int newRowCnt = curRow + 1;
		int curSpan = rowSpan(curRow, wordCol);
		insertRow(newRowCnt);
		this->setSpan(curRow, wordCol, curSpan + 1, 1);
		return curRow;
	}
}

QByteArray WordTableWidget::Hash()
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	for (int row = 0; row < rowCount(); ++row) {
		for (int column = 0; column < columnCount(); ++column) {
			QTableWidgetItem* item = this->item(row, column);
			if (item) {
				hash.addData(item->text().toUtf8());
			}
		}
	}
	return hash.result().toHex();
}

void WordTableWidget::wheelEvent(QWheelEvent* event)
{
	m_addBtn->hide();
	this->clearSelection();
	QTableWidget::wheelEvent(event);
}

bool WordTableWidget::CheckRowEmpty(int row)
{
	return (item(row, wordCol) == nullptr || item(row, wordCol)->text().isEmpty())
		&& (item(row, transZhCol) == nullptr)
		&&(item(row, transEnCol) == nullptr)
		&& (item(row, rootCol) == nullptr);
}

bool WordTableWidget::CheckIsLastRow(int row)
{
	
	return false;
}

WordTableWidget::~WordTableWidget()
{}

void WordTableWidget::AppendWordRecord(const WordRecord record)
{
	if (record.word.isEmpty())
		return;
	// 若最后一行为空，则在最后一行添加;否则额外增加一行
	int row = CheckRowEmpty(rowCount() - 1) ? rowCount() - 1 : AppendRow();
	dao::TranslationDao* transDao = dao::TranslationDao::Instance();

	this->setItem(row, wordCol, new QTableWidgetItem(record.word.GetWord()));
	for (int i = 0; i < record.transList.size(); ++i)
	{
		int curRow = row + i;
		this->setItem(curRow, transZhCol, new QTableWidgetItem(record.transList[i].GetZhTranslation()));
		this->setItem(curRow, transEnCol, new QTableWidgetItem(record.transList[i].GetEnTranslation()));
		this->setItem(curRow, rootCol, new QTableWidgetItem(record.transList[i].GetRoot()));
		this->setItem(curRow, senetenceCol, new QTableWidgetItem(record.transList[i].GetSentence()));
		// 非最后一个，则继续增加合并行
		if(i != record.transList.size() - 1) AppendRow(curRow - rowSpan(curRow, wordCol) + 1, true);
	}
	//this->setItem(row, transZhCol, new QTableWidgetItem(word.Get))
	//this->setItem(row, transZhCol, new QTableWidgetItem(word.GetTranslation()));
	//this->setItem(row, rootCol, new QTableWidgetItem(word.GetRoot()));
	//if (row > m_sentenceList.length())
	//	m_sentenceList.append(word.GetSentence());
	//else
	//	m_sentenceList.insert(row, word.GetSentence());
}

WordRecord WordTableWidget::GetWordRecord(int row)
{
	WordRecord record;
	if (CheckInvalid(row, rowCount()))
		return record;
	if(item(row, wordCol))
		record.word.SetWord(item(row, wordCol)->text());
	int spanCnt = this->rowSpan(row, wordCol);
	// translation count > 1
	for (int i = 0; i < spanCnt; ++i)
	{
		int curRow = row + i;
		TranslationModel trans;
		if(item(curRow, transZhCol))
			trans.SetZhTranslation(item(curRow, transZhCol)->text());
		if (item(curRow, transEnCol))
			trans.SetEnTranslation(item(curRow, transEnCol)->text());
		if (item(curRow, senetenceCol))
			trans.SetSentence(item(curRow, senetenceCol)->text());
		if (item(curRow, rootCol))
			trans.SetRoot(item(curRow, rootCol)->text());
		trans.SetSubId(i);
		record.transList.append(trans);
	}
	return record;
}

void WordTableWidget::Init()
{
	setRowCount(0);
	AppendRow();
}

QList<WordRecord> WordTableWidget::Pack()
{
	QList<WordRecord> recordList;
	int row = this->currentRow();
	for (size_t row = 0; row < rowCount(); )
	{
		WordRecord record = GetWordRecord(row);
		if (!record.word.isEmpty())
			recordList.push_back(record);
		row += rowSpan(row, wordCol);
	}
	return recordList;
}

bool WordTableWidget::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		// 处理按键事件
		keyPressEvent(keyEvent);
		return true; // 返回true表示事件已被处理，不再传播
	}
	return QTableWidget::eventFilter(obj, event); // 返回false继续事件传播
}

void WordTableWidget::keyPressEvent(QKeyEvent* event)
{
	// 监测delete键
	if (event->key() == Qt::Key_Delete)
	{
		QList<QTableWidgetItem*> items = selectedItems();
		int row = currentRow();
		// 删除行
		removeRow(row);
		// 删除例句
		if (row < m_sentenceList.size())
			m_sentenceList.removeAt(row);
	}
	else if (event->key() == Qt::Key_Insert)
	{
		AppendRow();
		event->ignore();
	}
	else
	{
		QTableWidget::keyPressEvent(event);
	}
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
	
	AppendRow(-1, true);
	//insertRow(row + 1);
}

bool WordTableWidget::CheckInvalid(int i, int top, int bottom)
{
	return (i < bottom || i >= top);
}
