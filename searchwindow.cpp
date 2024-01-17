#include "searchwindow.h"
#include <QFile>

SearchWindow::SearchWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_api = SearchApi::Instance();
	connect(m_api, &SearchApi::WordFetched, this, &SearchWindow::onWordFetched);
}

SearchWindow::~SearchWindow()
{}

void SearchWindow::Clear()
{
    ui.textEdit->clear();
}

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
    html += "<body>";
    html += "<div class='subblock' style='margin-top: 13px; margin-bottom: 13px;'>";
    for (const SubWordInfo& subInfo : info.subWordInfoList) {
		html += "<h1>" + info.word + "</h1>";
        html += "<div class='pos dpos'> [" + subInfo.partOfSpeech + "] </div>";
		html += "<hr class='hr-dotted'>";

        for (const WordMeaning& meaning : subInfo.wordMeanings) {
            if(!meaning.guideWord.isEmpty())
            {
                html += "<h3>Guide Word: " + meaning.guideWord + "</h3>";
            }
            html += "<div class='ddef_d sonspic'>";
			html += "<div class='indent'>" + meaning.enTranslation + "<br><span class='dtrans'>" + meaning.zhTranslation + "</span></div>";
            html += "</div>";

            //html += "<div class='sonspic'>";
            //html += "<ul>";
            html += "<div class='ddexamp'>";
            for (auto it = meaning.sentences.begin(); it != meaning.sentences.end(); ++it) {
                html += "<div style='margin-top: 10px'>";
                html += "<span class='dexmap ddexamp eg deg'> " + it.key() + "</span><br>"
                    +"<span class='ddexamp trans dtrans dtrans-se hdb break-cj'>"+ it.value() + "</span><br>";
                html += "</div>";
            }
            //html += "</ul>";
            html += "</div>";
            html += "<hr class='hr-dotted'>";
        }
        html += "</div>";
        html += "<hr class='hr-solid'>"; // 添加分割线
    }
    html += "</body>";
    return html;
}

void SearchWindow::FetchWord(QString word)
{
    m_api->FetchWord(word);
}

void SearchWindow::onWordFetched(FetchedWordInfo wordInfo)
{
	QString html = toHtml(wordInfo);
	ui.textEdit->setHtml(html);
    this->show();
    this->setFocus();
}
