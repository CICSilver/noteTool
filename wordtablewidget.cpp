#include "wordtablewidget.h"
#include "cellbtn.h"
#include <QHeaderView>
#include <QDebug>

WordTableWidget::WordTableWidget(QWidget* parent)
	: QTableWidget(parent)
{
	setColumnCount(m_col_count);
	setFocusPolicy(Qt::NoFocus);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setHorizontalHeaderLabels(QStringList() << "生词" << "释义" << "词根" << "例句");
	AppendRow();
	connect(this, &WordTableWidget::itemChanged, this, [this](QTableWidgetItem *item)
		{
			if (!item->text().isEmpty() && item->row() == this->rowCount() - 1)
				AppendRow();
		});
}

int WordTableWidget::AppendRow()
{
	int newRowCnt = rowCount() + 1;
	setRowCount(newRowCnt);
	CellBtn* btn = new CellBtn(newRowCnt - 1);
	btn->setText("显示");
	connect(btn, &CellBtn::clicked, this, &WordTableWidget::ShowSentence);
	setCellWidget(newRowCnt - 1, senetenceCol, btn);
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
	return QList<WordModel>();
}

void WordTableWidget::ShowSentence()
{
	CellBtn* btn = qobject_cast<CellBtn*>(sender());
	int row = btn->GetRow();
	if (CheckInvalid(row, m_sentenceList.size()))
		return;
	qDebug() << "ShowSentence " << btn->GetRow() << m_sentenceList.at(row);
};

bool WordTableWidget::CheckInvalid(int i, int top, int bottom)
{
	return (i < bottom || i >= top);
}
