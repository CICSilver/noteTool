#include "SoloCell.h"

SoloCell::SoloCell(int row, int col, int value, QObject* paret)
	: m_row(row), m_col(col), m_value(value)
{
	m_isEditable = true;
	m_isEditingCadidates = false;
	m_isEditingValue = false;
}

int SoloCell::row() const
{
	return m_row;
}

SoloCell::~SoloCell()
{}

SoloCell::SoloCell(const SoloCell & other)
{
	m_row = other.m_row;
	m_col = other.m_col;
	m_value = other.m_value;
	m_isEditable = other.m_isEditable;
	m_isEditingCadidates = other.m_isEditingCadidates;
	m_isEditingValue = other.m_isEditingValue;
	m_candidates = other.m_candidates;
}

int SoloCell::col() const
{
	return m_col;
}

int SoloCell::value() const
{
	return m_value;
}

void SoloCell::setValue(int value)
{
	m_value = value;
}

QList<int> SoloCell::candidates() const
{
	return m_candidates;
}

void SoloCell::setCandidates(QList<int> candidates)
{
	m_candidates = candidates;
}

bool SoloCell::isEditable() const
{
	return m_isEditable;
}

void SoloCell::setEditable(bool isEditable)
{
	m_isEditable = isEditable;
}

bool SoloCell::isEditingValue() const
{
	return m_isEditingValue;
}

void SoloCell::setEditingValue(bool isEditingValue)
{
	m_isEditingValue = isEditingValue;
}

bool SoloCell::isEditingCadidates() const
{
	return m_isEditingCadidates;
}

void SoloCell::setEditingCadidates(bool isEditingCadidates)
{
	m_isEditingCadidates = isEditingCadidates;
}

int SoloCell::Left_X(int gridWidth, int offset)
{
	return offset + gridWidth * m_row;
}

int SoloCell::Right_X(int gridWidth, int offset)
{
	return offset + gridWidth * (m_row + 1);
}

int SoloCell::Top_Y(int gridWidth, int offset)
{
	return offset + m_col * gridWidth;
}

int SoloCell::Bottom_Y(int gridWidth, int offset)
{
	return offset + (m_col + 1) * gridWidth;
}

QRect SoloCell::Rect(int gridWidth, int offsetX, int offsetY)
{
	return QRect();
}
