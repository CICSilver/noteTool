#include "searchwindow.h"
#include <QFile>

SearchWindow::SearchWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_api = SearchApi::Instance();
	m_api->FetchWord("test");
	connect(m_api, &SearchApi::WordFetched, this, &SearchWindow::onWordFetched);
}

SearchWindow::~SearchWindow()
{}

QString SearchWindow::toHtml(const FetchedWordInfo& info) {
    qDebug() << "toHtml";
    QFile file(":/MainWIndow/searchwindow.css");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";

	QTextStream in(&file);
	QString css = in.readAll();
	file.close();
    //qDebug() << css;
	QString html = "<style>" + css + "</style>";

    for (const SubWordInfo& subInfo : info.subWordInfoList) {
		html += "<h1>" + info.word + "</h1>";
        html += "<div class='pos dpos'> [" + subInfo.partOfSpeech + "] </div>";
		html += "<hr class='hr-solid'>";

        for (const WordMeaning& meaning : subInfo.wordMeanings) {
            if(!meaning.guideWord.isEmpty())
            {
                html += "<h3>Guide Word: " + meaning.guideWord + "</h3>";
            }
            //html += "<div class='translation'";
			html += "<div class='ddef_d entrance'>" + meaning.enTranslation + "</div>";
			html += "<div class='ddef_d dtrans'>" + meaning.zhTranslation + "</div>";
            //html += "</div>";

            html += "<ul>";
            for (auto it = meaning.sentences.begin(); it != meaning.sentences.end(); ++it) {
                html += "<li><span class='eg deg'> " + it.key() + "</span><br>"
                    +"<span class='trans dtrans dtrans-se hdb break-cj'>"+ it.value() + "</span></li>";
            }
            html += "</ul>";
            html += "<hr class='hr-solid'>";
        }

        html += "<hr class='hr-gradient'>"; // 添加分割线
    }

    return html;
}

void SearchWindow::onWordFetched(FetchedWordInfo wordInfo)
{
	QString html = toHtml(wordInfo);
	ui.textEdit->setHtml(html);
    this->show();
}
