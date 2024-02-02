#include "SoloWindow.h"
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QPen>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QRandomGenerator>
SoloWindow::SoloWindow(QMainWindow* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_gridWidth = this->width() / 10;
	m_borderWidth = m_gridWidth / 2;
	m_regionCnt = 3;
	m_selectedCell = nullptr;
	m_paintAreaX = 0;
	m_paintAreaY = ui.menuBar->height();
	InitCells();

}

SoloWindow::~SoloWindow()
{}

void SoloWindow::InitCells()
{
	int gridCnt = m_regionCnt * m_regionCnt;
	for (int row = 0; row < gridCnt; ++row)
	{
		QList<SoloCell> rowCells;
		for (int col = 0; col < gridCnt; ++col)
		{
			rowCells.append(SoloCell(row, col, 0));
		}
		m_cells.append(rowCells);
	}
}

SoloCell* SoloWindow::GetCellByPixel(int width, int height)
{
	int gridCnt = m_regionCnt * m_regionCnt;
	if (CheckInvalid(width, m_gridWidth * gridCnt + m_borderWidth + m_paintAreaX, m_borderWidth + m_paintAreaX) ||
		CheckInvalid(height, m_gridWidth * gridCnt + m_borderWidth + m_paintAreaY, m_borderWidth + m_paintAreaY))
		return nullptr;
	int x = width - m_borderWidth - m_paintAreaX;
	int y = height - m_borderWidth - m_paintAreaY;
	return &m_cells[x / m_gridWidth][y / m_gridWidth];
}

bool SoloWindow::CheckInvalid(int val, int top, int bottom)
{
	return val < bottom || val > top;
}

void SoloWindow::Generate()
{
	GenerateFull();
	GenerateHoles(60);
}

void SoloWindow::GenerateFull()
{
	int curRegion = 0;
	int gridCnt = GridCnt();
	QSet<int> numsInRegion = GetInitNumSet(gridCnt);
	
	for(int row = 0; row < gridCnt; ++row)
	{
		QSet<int> leftNumInRow = NumbersLeftInRow(row);
		for(int col = 0; col < gridCnt; ++col)
		{
			if(Region(row, col) != curRegion)
			{
				// 切换到新的宫，判断宫内剩余数字
				curRegion = Region(row, col);
				// startRow = region / m_regionCnt, startCol = region / m_regionCnt * m_regionCnt
				numsInRegion = NumbersLeftInRegion(curRegion / m_regionCnt, curRegion / m_regionCnt * m_regionCnt);
			}
		}
	}
}

void SoloWindow::GenerateHoles(int count)
{
}

QSet<int> SoloWindow::NumbersLeftInRegion(int startRow, int startCol)
{
	int gridCnt = m_regionCnt * m_regionCnt;
	QSet<int> numSet = GetInitNumSet(gridCnt);
	for(int row = startRow; row < startRow + m_regionCnt; ++row)
	{
		for(int col = startCol;col < startCol + m_regionCnt; ++col)
		{
			if (CheckInvalid(row, gridCnt) || CheckInvalid(col, gridCnt))
				return QSet<int>();
			int val = m_cells[row][col].value();
			numSet.remove(val);
		}
	}
	return numSet;
}

QSet<int> SoloWindow::NumbersLeftInRow(int row)
{
	QSet<int> numSet = GetInitNumSet(GridCnt());
	for (int col = 0; col < GridCnt(); ++col)
	{
		int val = m_cells[row][col].value();
		numSet.remove(val);
	}
	return numSet;
}

QSet<int> SoloWindow::NumbersLeftInCol(int col)
{
	QSet<int> numSet = GetInitNumSet(GridCnt());
	for (int row = 0; row < GridCnt(); ++row)
	{
		int val = m_cells[row][col].value();
		numSet.remove(val);
	}
	return numSet;
}

int SoloWindow::Region(int row, int col)
{
	return row / m_regionCnt + col / m_regionCnt * m_regionCnt;
}

void SoloWindow::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	int gridCnt = m_regionCnt * m_regionCnt;
	QPen boldPen(QBrush(Qt::black), 3);
	QPen normalPen(QBrush(Qt::black), 1);
	QPen editPen(QBrush(Qt::darkGreen), 2);
	QPen fixedPen(QBrush(Qt::black), 2);

	painter.setPen(boldPen);
	int vertical_offset = ui.menuBar->height();
	QPoint leftTop(m_paintAreaX + m_borderWidth, m_borderWidth + m_paintAreaY);
	QPoint rightTop(m_paintAreaX + m_borderWidth + gridCnt * m_gridWidth, m_borderWidth + m_paintAreaY);
	QPoint leftBottom(m_paintAreaX + m_borderWidth, m_borderWidth + gridCnt * m_gridWidth + m_paintAreaY);
	QPoint rightBottom(m_paintAreaX + m_borderWidth + gridCnt * m_gridWidth, m_borderWidth + gridCnt * m_gridWidth + m_paintAreaY);

	painter.drawLine(leftTop, rightTop);
	painter.drawLine(leftTop, leftBottom);
	painter.drawLine(rightTop, rightBottom);
	painter.drawLine(leftBottom, rightBottom);

	// 表格线
	for (int i = 1; i < gridCnt; i++)
	{
		if (i % 3 == 0)
		{
			painter.setPen(boldPen);
		}else
		{
			painter.setPen(normalPen);
		}
		// 横向
		painter.drawLine(
		QPoint(leftTop.x(), leftTop.y() + i * m_gridWidth),
		QPoint(rightTop.x(), rightTop.y() + i * m_gridWidth));
		// 纵向
		painter.drawLine(
		QPoint(leftTop.x() + i * m_gridWidth, leftTop.y()),
		QPoint(leftBottom.x() + i * m_gridWidth, leftBottom.y()));
	}

	// 绘制已经有值的格子中的数字
	for (int row = 0; row < gridCnt; ++row)
	{
		for (int col = 0; col < gridCnt; ++col)
		{
			SoloCell& cell = m_cells[row][col];
			if (cell.value() != 0)
			{
				if (cell.isEditable())
					painter.setPen(editPen);
				else
					painter.setPen(fixedPen);

				painter.setFont(QFont("Arial", 20));
				painter.drawText(QRect(QPoint(m_borderWidth + row * m_gridWidth, m_borderWidth + col * m_gridWidth + m_paintAreaY),
					QPoint(m_borderWidth + (row + 1) * m_gridWidth, m_borderWidth + (col + 1) * m_gridWidth + m_paintAreaY)),
					Qt::AlignCenter,
					QString::number(cell.value()));
			}
		}
	}

	// 绘制当前选中的格子
	if (m_selectedCell)
	{
		int row = m_selectedCell->row();
		int col = m_selectedCell->col();
		int offsetX = m_borderWidth + m_paintAreaX;
		int offsetY = m_borderWidth + m_paintAreaY;
		QPoint cell_leftTop(m_selectedCell->Left_X(m_gridWidth, offsetX), m_selectedCell->Top_Y(m_gridWidth, offsetY));
		QPoint cell_rightTop(m_selectedCell->Right_X(m_gridWidth, offsetX), m_selectedCell->Top_Y(m_gridWidth, offsetY));
		QPoint cell_leftBottom(m_selectedCell->Left_X(m_gridWidth, offsetX), m_selectedCell->Bottom_Y(m_gridWidth, offsetY));
		QPoint cell_rightBottom(m_selectedCell->Right_X(m_gridWidth, offsetX), m_selectedCell->Bottom_Y(m_gridWidth, offsetY));
		QPoint cell_centerTop(m_selectedCell->Left_X(m_gridWidth, offsetX) + m_gridWidth / 2, m_selectedCell->Top_Y(m_gridWidth, offsetY));
		QPoint cell_centerLeft(m_selectedCell->Left_X(m_gridWidth, offsetX), m_selectedCell->Top_Y(m_gridWidth, offsetY) + m_gridWidth / 2);

		QColor semiTransparentGray(128, 128, 128, 100);
		painter.setBrush(semiTransparentGray);
		painter.setPen(Qt::NoPen);
		if (m_selectedCell->isEditingValue())
			painter.fillRect(QRect(cell_leftTop, cell_rightBottom), QBrush(semiTransparentGray));
		else if(m_selectedCell->isEditingCadidates())
			painter.drawPolygon(QPolygon() << cell_leftTop << cell_centerLeft << cell_centerTop);

		// 如果有值，在格子中间绘制
		if (m_selectedCell->value() != 0)
		{
			painter.setPen(editPen);
			painter.setFont(QFont("Arial", 20));
			painter.drawText(QRect(cell_leftTop, cell_rightBottom), Qt::AlignCenter, QString::number(m_selectedCell->value()));
		}
	}
}

void SoloWindow::mousePressEvent(QMouseEvent* e)
{
	SoloCell* curSelectedCell = GetCellByPixel(e->pos().x(), e->pos().y());
	if(curSelectedCell && curSelectedCell->isEditable())
	{
		if(m_selectedCell && curSelectedCell != m_selectedCell)
		{
			m_selectedCell->setEditingValue(false);
			m_selectedCell->setEditingCadidates(false);
		}
		curSelectedCell->setEditingValue(e->button() == Qt::LeftButton && !curSelectedCell->isEditingValue());
		curSelectedCell->setEditingCadidates(e->button() == Qt::RightButton && !curSelectedCell->isEditingCadidates());
		m_selectedCell = curSelectedCell;
	}
	update();
}

void SoloWindow::keyPressEvent(QKeyEvent* e)
{
	int key = e->key();
	if (key > 48 && key <= 57)
	{
		if (m_selectedCell)
		{
			m_selectedCell->setValue(key - 48);
			update();
		}
	}
}

QSet<int> SoloWindow::GetInitNumSet(int setCnt)
{
	QSet<int> res;
	for (int i = 1; i <= setCnt; ++i)
	{
		res.insert(i);
	}
	return res;
}

