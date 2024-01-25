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
	 * ��鹬��ʣ�������.
	 * \param startRow ������ʼ��
	 * \param startCol ������ʼ��
	 * \param regionCnt ���Ĵ�С(Ĭ��Ϊ3)
	 * \return
	 */
	QSet<int> NumbersLeftInRegion(int startRow, int startCol);
	QSet<int> NumbersLeftInRow(int row);
	QSet<int> NumbersLeftInCol(int col);

	/**
	 * ��ȡ��ǰ���е�Ԫ�����ڵĹ����.
	 * ���磺SoloCell(0,1)λ�ڹ� 0 ��, SoloCell(5,7)λ�ڹ� 7 ��
	 * 
	 * \param row ָ����
	 * \param col ָ����
	 * \return �������ڹ������
	 */
	int Region(int row, int col);

	void paintEvent(QPaintEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void keyPressEvent(QKeyEvent* e) override;
private:
	Ui::SoloWindowClass ui;
	int m_paintAreaX;	// �ɻ�����������Ͻ����� x
	int m_paintAreaY;	// �ɻ�����������Ͻ����� y
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
