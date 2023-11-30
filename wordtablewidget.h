#pragma once

#include <QTableWidget>
#include "ui_wordtablewidget.h"
#include "wordModel.h"

class WordTableWidget : public QTableWidget
{
	Q_OBJECT

public:
	enum Col
	{
		wordCol = 0,
		transCol = 1,
		rootCol = 2,
		senetenceCol = 3
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

protected slots:
	void ShowSentence();

private:
	// check if i is not in [bottom, top)
	bool CheckInvalid(int i, int top, int bottom = 0);
	// 末尾添加空行，返回行号
	int AppendRow();
	/*
	 * @brief 检查行是否为空
	 * @author xyr
	 * @date 2023/11/30 14:12:13
	 * @param
	 * @return
	 * @note
	 */
	bool CheckRowEmpty(int row);
private:
	const int m_col_count = 4;
	QList<QString> m_sentenceList;
};
