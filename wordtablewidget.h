#pragma once

#include <QTableWidget>
#include "wordModel.h"

class QPushButton;
class CellBtn;
class WordTableWidget : public QTableWidget
{
	Q_OBJECT

public:
	enum Col
	{
		wordCol = 0,
		transZhCol,
		transEnCol,
		rootCol,
		senetenceCol
	};
public:
	WordTableWidget(QWidget* parent = nullptr);
	~WordTableWidget();

	/*
	 * @brief 向行尾添加新的生词记录
	 * @author xyr
	 * @date 2023/11/30 14:04:19
	 */
	void AppendWordRecord(WordModel word);

	/*
	 * @brief 打包指定行的word model
	 * @author xyr
	 * @date 2023/11/30 15:09:10
	 * @return WordModel 打包好的指定word model
	 */
	WordModel GetWordRecord(int row);

	/*
	 * @brief 打包表格中的全部word
	 * @author xyr
	 * @date 2023/11/30 15:09:28
	 * @return QList<WordModel> word model list
	 */
	QList<WordModel> Pack();

	bool eventFilter(QObject* obj, QEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void TableClicked();

protected slots:
	void ShowSentence();

private slots:
	void onCellClicked(int row, int col);
	void onAddBtnClicked();
private:
	// check if i is not in [bottom, top)
	bool CheckInvalid(int i, int top, int bottom = 0);
	// 末尾添加空行，返回行号
	int AppendRow(bool isSpan = false);
	/*
	 * @brief 检查行是否为空
	 * @author xyr
	 * @date 2023/11/30 14:12:13
	 * @param
	 * @return
	 * @note
	 */
	bool CheckRowEmpty(int row);

	/*
	 * @brief 检查是否为最后一行
	 * @author xyr
	 * @date 2023/12/04 15:28:23
	 * @param 
	 * @return
	 * @note
	 */
	bool CheckIsLastRow(int row);
private:
	const int m_col_count = 4;
	QList<QString> m_sentenceList;
	QList<CellBtn*> m_cellBtnList;
	QPushButton* m_addBtn;
	int m_lastRow;
};
