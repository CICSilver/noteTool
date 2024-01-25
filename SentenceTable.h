#pragma once

#include "WordTableWidget.h"

class SentenceTable  : public WordTableWidget
{
	Q_OBJECT

public:
	SentenceTable(QWidget* parent = nullptr);
	~SentenceTable();
};
