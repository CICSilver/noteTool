#pragma once

#include <QObject>
#include <QList>
#include <QPoint>
#include <QRect>
class SoloCell  : public QObject
{
	Q_OBJECT

public:
	SoloCell(int row, int col, int value, QObject* paret = nullptr);
	~SoloCell();

	SoloCell(const SoloCell& other);
	int row() const;

	int col() const;

	int value() const;

	void setValue(int value);

	QList<int> candidates() const;

	void setCandidates(QList<int> candidates);

	bool isEditable() const;

	void setEditable(bool isEditable);

	bool isEditingValue() const;

	void setEditingValue(bool isEditingValue);

	bool isEditingCadidates() const;

	void setEditingCadidates(bool isEditingCadidates);
	
	// 获取格子水平坐标
	int Left_X(int gridWidth, int offset);
	int Right_X(int gridWidth, int offset);
	// 获取格子垂直坐标
	int Top_Y(int gridWidth, int offset);
	int Bottom_Y(int gridWidth, int offset);
	// 获取格子区域
	QRect Rect(int gridWidth, int offsetX, int offsetY);
private:
	int m_row;
	int m_col;
	int m_value;
	bool m_isEditable;
	bool m_isEditingValue;
	bool m_isEditingCadidates;
	QList<int> m_candidates;
};
