#pragma once

#include <QMainWindow>
#include "SoloCell.h"
#include "ui_SoloWindow.h"
#include <QThread>
#include <QSet>
class SoloWindow : public QMainWindow
{
	Q_OBJECT

public:
	SoloWindow(QMainWindow*parent = nullptr);
	~SoloWindow();

	void InitCells();
	SoloCell* GetCellByPixel(int width, int height);

	bool CheckInvalid(int val, int top, int bottom = 0);

	void Generate();

	void GenerateFull();
	void GenerateHoles(int count);

	/**
	 * 检查宫内剩余的数字.
	 * \param startRow 宫的起始行
	 * \param startCol 宫的起始列
	 * \param regionCnt 宫的大小(默认为3)
	 * \return
	 */
	QSet<int> NumbersLeftInRegion(int startRow, int startCol);
	QSet<int> NumbersLeftInRow(int row);
	QSet<int> NumbersLeftInCol(int col);

	/**
	 * 获取当前行列单元格所在的宫序号.
	 * 例如：SoloCell(0,1)位于宫 0 中, SoloCell(5,7)位于宫 7 中
	 * 
	 * \param row 指定行
	 * \param col 指定列
	 * \return 格子所在宫的序号
	 */
	int Region(int row, int col);

	void paintEvent(QPaintEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void keyPressEvent(QKeyEvent* e) override;
private:
	Ui::SoloWindowClass ui;
	int m_paintAreaX;	// 可绘制区域的左上角坐标 x
	int m_paintAreaY;	// 可绘制区域的左上角坐标 y
	int m_regionCnt;
	int m_borderWidth;
	int m_gridWidth;
	SoloCell* m_selectedCell;
	QList<QList<SoloCell>> m_cells;
	QList<QSet<int>> m_numInRows;
	QList<QSet<int>> m_numInCols;
	QList<QSet<int>> m_numInRegions;

	QSet<int> GetInitNumSet(int setCnt);
	int GridCnt() { return m_regionCnt * m_regionCnt; }
};
