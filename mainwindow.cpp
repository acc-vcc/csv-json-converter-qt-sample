#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->fileSelectButton, &QPushButton::clicked, this, &MainWindow::selectInputFile);
    connect(ui->execButton, &QPushButton::clicked, this, &MainWindow::convert);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectInputFile(){
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("入力ファイルを選択"),
                "",
                tr("CSV Files (*.csv);;JSON FIles (*.json);;AllFiles (*)")
    );

    if(!fileName.isEmpty()){
        ui->filePathLabel->setText(fileName);
    }
}

void MainWindow::convert(){
    QString input = ui->filePathLabel->text();
    QString output = "";
    bool ret = false;
    switch (detectFormat(input)) {
    case DetectResult::CSV:
        log(tr("CSV形式と判定しました。JSON形式に変換します。"), LogLevel::Info);
        output = input + ".json";
        ret = convertCsvToJson(input, output);
        break;
    case DetectResult::JSON:
        log(tr("JSON形式と判定しました。CSV形式に変換します。"), LogLevel::Info);
        output = input + ".csv";
        ret = convertJsonToCsv(input , output);
        break;
    case DetectResult::NOT_SELECTED:
        log(tr("ファイルが選択されていません。"), LogLevel::Error);
        return;
    case DetectResult::OPEN_FAILED:
        log(tr("ファイルを開くのに失敗しました。"), LogLevel::Error);
        return;
    case DetectResult::UNKOWN:
        log(tr("未知のファイル形式です。"), LogLevel::Error);
        return;
    }
    if(ret){
        log(tr("変換に成功しました。<br>出力ファイル:%1").arg(output), LogLevel::Success);
    }else{
        log(tr("変換に失敗しました。"), LogLevel::Error);
    }
}

DetectResult MainWindow::detectFormat(const QString& filePath){
    if(filePath == tr("未選択") || filePath.trimmed().isEmpty()){
        return DetectResult::NOT_SELECTED;
    }

    // 開けるか試す
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return DetectResult::OPEN_FAILED;

    QString ext = QFileInfo(filePath).suffix().toLower();

    if(ext == "csv")
        return DetectResult::CSV;

    if(ext == "json")
        return DetectResult::JSON;

    // 試しに20Byte読む
    QByteArray firstBytes = file.read(20).trimmed();
    if(firstBytes.startsWith("{") || firstBytes.startsWith("["))
        return DetectResult::JSON;

    // カンマがあればCSVと判定する(拡張子無しJSONで誤判定の可能性あり)
    if(firstBytes.contains(","))
        return DetectResult::CSV;

    return DetectResult::UNKOWN;
}

bool MainWindow::convertCsvToJson(const QString& input, const QString& output){
    QFile file(input);
    file.open(QIODevice::ReadOnly | QIODevice::Text);     // detectFormatで既に判定しているため成否判定不要

    QTextStream in(&file);
    QStringList row1, row2;
    if(!parseCsvLine(in, row1))
        return false;

    qint64 pos = in.pos();    // 1行読み取り後の位置取得

    if(!parseCsvLine(in, row2))
        return false;

    file.seek(0);   // 先頭に戻す

    QStringList headers;
    if(isHeader(row1,row2)){
        // 先頭行はヘッダ
        headers = row1;
        file.seek(pos); in.seek(pos);  // ヘッダ行を飛ばす(QFileをseekしてもQTextStreamの内部位置は更新されないため更新)
    }else{
        log(tr("ヘッダ行がないため追加します。"), LogLevel::Warning);
        // 先頭行はデータ
        headers.clear();
        for(int i = 0; i < row1.size(); i++){
            headers << QString("col%1").arg(i + 1);
        }
    }

    QJsonArray array;
    while(!in.atEnd()){
        QStringList values;
        if(!parseCsvLine(in, values))
            return false;

        // 空行は飛ばす
        if(values.size() == 1 && values[0] == "")
            continue;

        QJsonObject obj;
        for(int i = 0; i < headers.size(); i++){
            obj[headers[i]] = values.value(i);
        }
        array.append(obj);
    }

    if(array.size() == 0)
        return false;

    QFile out(output);
    if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    // 注意：QJsonObjectの仕様によりキーはソートされる
    QJsonDocument doc(array);
    out.write(doc.toJson(QJsonDocument::Indented));

    return true;
}

bool MainWindow::parseCsvLine(QTextStream& ts, QStringList& row)
{
    QString field;
    bool inQuotes = false;

    while(!ts.atEnd()){
        QString oneLine = ts.readLine();
        for (int i = 0; i < oneLine.size(); ++i) {
            QChar c = oneLine[i];

            if (inQuotes) {
                if (c == '"') {
                    // 次が " ならエスケープ
                    if (i + 1 < oneLine.size() && oneLine[i + 1] == '"') {
                        field += '"';
                        i++; // 次の " をスキップ
                    } else {
                        inQuotes = false;
                    }
                } else {
                    field += c;
                }
            } else {
                if (c == '"') {
                    inQuotes = true;
                } else if (c == ',') {
                    row << field;
                    field.clear();
                } else {
                    field += c;
                }
            }
        }

        // 「"」の範囲が終わっていなければ続行
        if(!inQuotes)
            break;

        field += '\n';  // 続行する前に改行を挿入
    }

    row << field;

    // 「"」が閉じているかを判定
    return !inQuotes;
}

bool MainWindow::isHeader(const QStringList& fields, const QStringList& nextField){
    // 先頭行が全て数字の場合はヘッダではない
    bool isAllNumeric = true;
    for(const QString& f : fields){
        bool ok;
        f.toDouble(&ok);
        if(!ok){
            isAllNumeric = false;
            break;
        }
    }
    if(isAllNumeric)
        return false;

    // 全て空の場合はヘッダではない
    bool isAllEmpty = true;
    for(const QString& f : fields){
        if(!f.trimmed().isEmpty()){
            isAllEmpty = false;
            break;
        }
    }
    if(isAllEmpty)
        return false;

    // 2行目の方が数字の項目の数が多い場合はヘッダ
    int numeric1 = 0, numeric2 = 0;
    for(const QString& f : fields){
        bool ok;
        f.toDouble(&ok);
        if(ok){
            numeric1++;
        }
    }
    for(const QString& f : nextField){
        bool ok;
        f.toDouble(&ok);
        if(ok){
            numeric2++;
        }
    }

    if(numeric1 < numeric2)
        return true;

    return false;
}


bool MainWindow::convertJsonToCsv(const QString &input, const QString &output){
    QFile file(input);
    file.open(QIODevice::ReadOnly);     // detectFormatで既に判定しているため成否判定不要
    QByteArray data = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    if(array.isEmpty())
        return false;

    // ここからの変換は配列の中にオブジェクトが並んでいる形式とする
    // 注意：QJsonObjectの仕様によりキーはソートされる
    QStringList headers = array.first().toObject().keys();

    QFile out(output);
    if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream stream(&out);

    // ヘッダ行
    stream << headers.join(",") << "\n";
    // データ行
    for(const QJsonValue& v : array){
        QJsonObject obj = v.toObject();
        QStringList row;
        for(const QString& h : headers){
            QString val = obj.value(h).toString();

            // エスケープ処理
            if(val.contains(",") || val.contains("\"") || val.contains("\n")){
                val.replace("\"", "\"\"");
                val = "\"" + val + "\"";
            }
            row << val;
        }
        // 注意：複数行にまたがるフィールドは LF に統一されます（CRLF は保持されません）
        stream << row.join(",") << "\n";
    }

    return true;
}

void MainWindow::log(const QString& message, LogLevel level){
    QString color;
    switch (level) {
    case LogLevel::Info: color = "black"; break;
    case LogLevel::Warning: color = "orange"; break;
    case LogLevel::Error: color = "red"; break;
    case LogLevel::Success: color = "green"; break;
    }

    ui->logTextArea->append(
        QString("<span style='color:%1; '>%2</span>").arg(color, message)
    );
}
