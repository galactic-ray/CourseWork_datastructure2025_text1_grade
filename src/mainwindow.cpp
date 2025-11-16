#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QSplitter>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QInputDialog>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#else
#include <QTextCodec>
#endif
#include <QDir>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // 设置默认数据文件路径为 data/ 目录
    QDir dataDir("data");
    if(!dataDir.exists()) {
        dataDir.mkpath(".");
    }
    datafile = "data/test.csv";
    
    setWindowTitle("学生成绩管理系统");
    setMinimumSize(1000, 700);
    resize(1200, 800);

    setupUI();
    updateStatus();
    
    // 尝试加载文件
    if(QFile::exists(datafile)) {
        loadFromFile(datafile);
    } else {
        loadSeedData();
        saveToFile(datafile);
    }
    refreshTable();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // 创建菜单栏
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("文件(&F)");
    QAction* loadAction = fileMenu->addAction("加载文件(&L)");
    QAction* saveAction = fileMenu->addAction("保存文件(&S)");
    QAction* backupAction = fileMenu->addAction("从备份恢复(&B)");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("退出(&X)");
    
    QMenu* dataMenu = menuBar->addMenu("数据(&D)");
    QAction* seedAction = dataMenu->addAction("加载示例数据(&S)");
    
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(backupAction, &QAction::triggered, this, &MainWindow::onLoadBackup);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(seedAction, &QAction::triggered, this, &MainWindow::onLoadSeed);
    
    // 创建分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(splitter);
    
    // 左侧面板
    QWidget* leftPanel = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    
    // 数据输入组
    QGroupBox* inputGroup = new QGroupBox("添加/编辑成绩");
    QGridLayout* inputLayout = new QGridLayout(inputGroup);
    
    inputLayout->addWidget(new QLabel("学号:"), 0, 0);
    idSpin = new QSpinBox;
    idSpin->setRange(10000000, 99999999);
    inputLayout->addWidget(idSpin, 0, 1);
    
    inputLayout->addWidget(new QLabel("姓名:"), 1, 0);
    nameEdit = new QLineEdit;
    inputLayout->addWidget(nameEdit, 1, 1);
    
    inputLayout->addWidget(new QLabel("专业:"), 2, 0);
    majorEdit = new QLineEdit;
    majorEdit->setText("计算机科学与技术");
    inputLayout->addWidget(majorEdit, 2, 1);
    
    inputLayout->addWidget(new QLabel("年级:"), 3, 0);
    yearSpin = new QSpinBox;
    yearSpin->setRange(2000, 2100);
    yearSpin->setValue(2011);
    inputLayout->addWidget(yearSpin, 3, 1);
    
    inputLayout->addWidget(new QLabel("科目:"), 4, 0);
    subjectEdit = new QLineEdit;
    inputLayout->addWidget(subjectEdit, 4, 1);
    
    inputLayout->addWidget(new QLabel("成绩:"), 5, 0);
    scoreSpin = new QDoubleSpinBox;
    scoreSpin->setRange(0, 100);
    scoreSpin->setDecimals(1);
    inputLayout->addWidget(scoreSpin, 5, 1);
    
    inputLayout->addWidget(new QLabel("插入位置:"), 6, 0);
    insertPosSpin = new QSpinBox;
    insertPosSpin->setRange(0, 1000);
    inputLayout->addWidget(insertPosSpin, 6, 1);
    
    QPushButton* addBtn = new QPushButton("添加(追加)");
    QPushButton* insertBtn = new QPushButton("插入");
    QPushButton* editBtn = new QPushButton("修改");
    QPushButton* deleteBtn = new QPushButton("删除");
    
    QHBoxLayout* btnLayout1 = new QHBoxLayout;
    btnLayout1->addWidget(addBtn);
    btnLayout1->addWidget(insertBtn);
    QHBoxLayout* btnLayout2 = new QHBoxLayout;
    btnLayout2->addWidget(editBtn);
    btnLayout2->addWidget(deleteBtn);
    
    inputLayout->addLayout(btnLayout1, 7, 0, 1, 2);
    inputLayout->addLayout(btnLayout2, 8, 0, 1, 2);
    
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddGrade);
    connect(insertBtn, &QPushButton::clicked, this, &MainWindow::onInsertGrade);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::onEditGrade);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteGrade);
    
    leftLayout->addWidget(inputGroup);
    
    // 查询组
    QGroupBox* searchGroup = new QGroupBox("查询");
    QVBoxLayout* searchLayout = new QVBoxLayout(searchGroup);
    
    QHBoxLayout* searchIdLayout = new QHBoxLayout;
    searchIdLayout->addWidget(new QLabel("学号:"));
    searchIdEdit = new QLineEdit;
    searchIdLayout->addWidget(searchIdEdit);
    QPushButton* searchIdBtn = new QPushButton("查询");
    searchIdLayout->addWidget(searchIdBtn);
    searchLayout->addLayout(searchIdLayout);
    
    QHBoxLayout* searchNameLayout = new QHBoxLayout;
    searchNameLayout->addWidget(new QLabel("姓名:"));
    searchNameEdit = new QLineEdit;
    searchNameLayout->addWidget(searchNameEdit);
    QPushButton* searchNameBtn = new QPushButton("查询");
    searchNameLayout->addWidget(searchNameBtn);
    searchLayout->addLayout(searchNameLayout);
    
    QHBoxLayout* searchSubjectLayout = new QHBoxLayout;
    searchSubjectLayout->addWidget(new QLabel("科目:"));
    searchSubjectCombo = new QComboBox;
    searchSubjectCombo->setEditable(true);
    searchSubjectLayout->addWidget(searchSubjectCombo);
    QPushButton* searchSubjectBtn = new QPushButton("查询");
    searchSubjectLayout->addWidget(searchSubjectBtn);
    searchLayout->addLayout(searchSubjectLayout);
    
    QPushButton* showAllBtn = new QPushButton("显示全部");
    QPushButton* showAllBySubjectBtn = new QPushButton("按科目分组显示");
    searchLayout->addWidget(showAllBtn);
    searchLayout->addWidget(showAllBySubjectBtn);
    
    connect(searchIdBtn, &QPushButton::clicked, this, &MainWindow::onSearchById);
    connect(searchNameBtn, &QPushButton::clicked, this, &MainWindow::onSearchByName);
    connect(searchSubjectBtn, &QPushButton::clicked, this, &MainWindow::onSearchBySubject);
    connect(showAllBtn, &QPushButton::clicked, this, &MainWindow::onShowAll);
    connect(showAllBySubjectBtn, &QPushButton::clicked, this, &MainWindow::onShowAllBySubject);
    
    leftLayout->addWidget(searchGroup);
    
    // 文件操作组
    QGroupBox* fileGroup = new QGroupBox("文件操作");
    QVBoxLayout* fileLayout = new QVBoxLayout(fileGroup);
    QPushButton* loadBtn = new QPushButton("加载文件");
    QPushButton* saveBtn = new QPushButton("保存文件");
    QPushButton* backupBtn = new QPushButton("从备份恢复");
    QPushButton* seedBtn = new QPushButton("加载示例数据");
    fileLayout->addWidget(loadBtn);
    fileLayout->addWidget(saveBtn);
    fileLayout->addWidget(backupBtn);
    fileLayout->addWidget(seedBtn);
    
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadFile);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveFile);
    connect(backupBtn, &QPushButton::clicked, this, &MainWindow::onLoadBackup);
    connect(seedBtn, &QPushButton::clicked, this, &MainWindow::onLoadSeed);
    
    leftLayout->addWidget(fileGroup);
    leftLayout->addStretch();
    
    splitter->addWidget(leftPanel);
    
    // 右侧面板
    QWidget* rightPanel = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    
    // 表格
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(6);
    QStringList headers = {"学号", "姓名", "专业", "年级", "科目", "成绩"};
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setAlternatingRowColors(true);
    
    connect(tableWidget, &QTableWidget::itemSelectionChanged, 
            this, &MainWindow::onTableSelectionChanged);
    
    rightLayout->addWidget(new QLabel("成绩列表:"));
    rightLayout->addWidget(tableWidget);
    
    // 日志区域
    rightLayout->addWidget(new QLabel("操作日志:"));
    logText = new QTextEdit;
    logText->setMaximumHeight(150);
    logText->setReadOnly(true);
    rightLayout->addWidget(logText);
    
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    
    // 状态栏
    statusBar()->showMessage("就绪");
}

void MainWindow::refreshTable() {
    tableWidget->setRowCount(L.size());
    for(size_t i=0; i<L.size(); ++i) {
        const Grade& g = L[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(g.id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(g.name));
        tableWidget->setItem(i, 2, new QTableWidgetItem(g.major));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(g.year)));
        tableWidget->setItem(i, 4, new QTableWidgetItem(g.subject));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(g.score, 'f', 1)));
    }
    tableWidget->resizeColumnsToContents();
    updateStatus();
    
    // 更新科目下拉框
    QStringList subjects = SI.getAllSubjects();
    searchSubjectCombo->clear();
    searchSubjectCombo->addItems(subjects);
}

void MainWindow::log(const QString& msg) {
    logText->append(QDateTime::currentDateTime().toString("hh:mm:ss") + " - " + msg);
}

void MainWindow::updateStatus() {
    statusBar()->showMessage(QString("共 %1 条记录").arg(L.size()));
}

QStringList MainWindow::parseCSVLine(const QString& line) {
    QStringList fields;
    QString current;
    bool inQuotes = false;
    
    for(int i=0; i<line.length(); ++i) {
        QChar c = line[i];
        if(inQuotes) {
            if(c == '"') {
                if(i+1 < line.length() && line[i+1] == '"') {
                    current += '"';
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                current += c;
            }
        } else {
            if(c == '"') {
                inQuotes = true;
            } else if(c == ',') {
                fields.append(current.trimmed());
                current.clear();
            } else {
                current += c;
            }
        }
    }
    fields.append(current.trimmed());
    return fields;
}

QString MainWindow::csvEscape(const QString& s) {
    if(s.contains(',') || s.contains('"') || s.contains('\n') || s.contains('\r')) {
        QString result = "\"";
        for(QChar c : s) {
            if(c == '"') result += "\"\"";
            else result += c;
        }
        result += "\"";
        return result;
    }
    return s;
}

bool MainWindow::saveToFile(const QString& path) {
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件进行写入: " + path);
        return false;
    }
    
    // 备份
    if(QFile::exists(path)) {
        QFile::copy(path, path + ".bak");
    }
    
    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out << "id,name,major,year,subject,score\n";
    
    for(size_t i=0; i<L.size(); ++i) {
        const Grade& g = L[i];
        out << g.id << ","
            << csvEscape(g.name) << ","
            << csvEscape(g.major) << ","
            << g.year << ","
            << csvEscape(g.subject) << ","
            << g.score << "\n";
    }
    
    log("已保存到: " + path);
    return true;
}

bool MainWindow::loadFromFile(const QString& path) {
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + path);
        return false;
    }
    
    QTextStream in(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8);
#else
    in.setCodec("UTF-8");
#endif
    
    SeqList<Grade> tmp;
    int lineNum = 0;
    int okCount = 0;
    
    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        ++lineNum;
        
        if(line.isEmpty() || line.startsWith("#")) continue;
        if(lineNum == 1 && !line[0].isDigit()) continue; // 跳过表头
        
        QStringList fields = parseCSVLine(line);
        if(fields.size() < 6) continue;
        
        bool ok;
        Grade g;
        g.id = fields[0].toInt(&ok);
        if(!ok) continue;
        g.name = fields[1];
        g.major = fields[2];
        g.year = fields[3].toInt(&ok);
        if(!ok) continue;
        g.subject = fields[4];
        g.score = fields[5].toDouble(&ok);
        if(!ok || g.score < 0 || g.score > 100) continue;
        
        tmp.push_back(g);
        ++okCount;
    }
    
    L.clear();
    SI.clear();
    for(size_t i=0; i<tmp.size(); ++i) {
        L.push_back(tmp[i]);
        SI.add(tmp[i]);
    }
    
    refreshTable();
    log(QString("已从文件加载 %1 条记录: %2").arg(okCount).arg(path));
    return true;
}

QList<Grade> MainWindow::seedData() {
    return {
        {20110001, "张三", "计算机科学与技术", 2011, "数据结构", 88.5},
        {20110001, "张三", "计算机科学与技术", 2011, "离散数学", 91},
        {20110002, "李四", "计算机科学与技术", 2011, "数据结构", 76.5},
        {20110003, "王五", "计算机科学与技术", 2011, "计算机组成原理", 84},
        {20110003, "王五", "计算机科学与技术", 2011, "数据结构", 92.5}
    };
}

void MainWindow::loadSeedData() {
    L.clear();
    SI.clear();
    QList<Grade> seeds = seedData();
    for(const Grade& g : seeds) {
        L.push_back(g);
        SI.add(g);
    }
    refreshTable();
    log("已加载示例数据");
}

void MainWindow::onAddGrade() {
    Grade g;
    g.id = idSpin->value();
    g.name = nameEdit->text().trimmed();
    g.major = majorEdit->text().trimmed();
    if(g.major.isEmpty()) g.major = "计算机科学与技术";
    g.year = yearSpin->value();
    g.subject = subjectEdit->text().trimmed();
    g.score = scoreSpin->value();
    
    if(g.name.isEmpty() || g.subject.isEmpty()) {
        QMessageBox::warning(this, "错误", "姓名和科目不能为空");
        return;
    }
    
    L.push_back(g);
    SI.add(g);
    refreshTable();
    saveToFile(datafile);
    log(QString("已添加: %1 - %2 - %3").arg(g.id).arg(g.name).arg(g.subject));
}

void MainWindow::onInsertGrade() {
    Grade g;
    g.id = idSpin->value();
    g.name = nameEdit->text().trimmed();
    g.major = majorEdit->text().trimmed();
    if(g.major.isEmpty()) g.major = "计算机科学与技术";
    g.year = yearSpin->value();
    g.subject = subjectEdit->text().trimmed();
    g.score = scoreSpin->value();
    
    if(g.name.isEmpty() || g.subject.isEmpty()) {
        QMessageBox::warning(this, "错误", "姓名和科目不能为空");
        return;
    }
    
    size_t pos = insertPosSpin->value();
    if(pos > L.size()) pos = L.size();
    
    L.insert(pos, g);
    SI.add(g);
    refreshTable();
    saveToFile(datafile);
    log(QString("已插入到位置 %1: %2 - %3").arg(pos).arg(g.name).arg(g.subject));
}

void MainWindow::onDeleteGrade() {
    int id = idSpin->value();
    QString subject = subjectEdit->text().trimmed();
    
    if(subject.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入科目");
        return;
    }
    
    bool found = false;
    for(size_t i=0; i<L.size(); ++i) {
        if(L[i].id == id && L[i].subject == subject) {
            Grade g = L[i];
            SI.remove_one(g);
            L.remove_at(i);
            found = true;
            break;
        }
    }
    
    if(found) {
        refreshTable();
        saveToFile(datafile);
        log(QString("已删除: 学号 %1, 科目 %2").arg(id).arg(subject));
    } else {
        QMessageBox::information(this, "提示", "未找到匹配的记录");
    }
}

void MainWindow::onEditGrade() {
    int id = idSpin->value();
    QString subject = subjectEdit->text().trimmed();
    
    if(subject.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入科目");
        return;
    }
    
    // 查找匹配的记录
    QList<size_t> matches;
    for(size_t i=0; i<L.size(); ++i) {
        if(L[i].id == id && L[i].subject == subject) {
            matches.append(i);
        }
    }
    
    if(matches.isEmpty()) {
        QMessageBox::information(this, "提示", "未找到匹配的记录");
        return;
    }
    
    size_t idx = matches[0];
    if(matches.size() > 1) {
        // 如果有多个匹配，让用户选择
        QStringList options;
        for(size_t i : matches) {
            const Grade& g = L[i];
            options.append(QString("学号:%1 姓名:%2 科目:%3 成绩:%4")
                          .arg(g.id).arg(g.name).arg(g.subject).arg(g.score));
        }
        bool ok;
        QString choice = QInputDialog::getItem(this, "选择要修改的记录", 
                                              "找到多条匹配记录，请选择:", 
                                              options, 0, false, &ok);
        if(!ok) return;
        idx = matches[options.indexOf(choice)];
    }
    
    Grade& g = L[idx];
    SI.remove_one(g);
    
    // 更新字段（如果输入框有值）
    if(!nameEdit->text().trimmed().isEmpty()) g.name = nameEdit->text().trimmed();
    if(!majorEdit->text().trimmed().isEmpty()) g.major = majorEdit->text().trimmed();
    g.year = yearSpin->value();
    if(!subjectEdit->text().trimmed().isEmpty()) g.subject = subjectEdit->text().trimmed();
    g.score = scoreSpin->value();
    
    SI.add(g);
    refreshTable();
    saveToFile(datafile);
    log(QString("已修改: 学号 %1, 科目 %2").arg(id).arg(subject));
}

void MainWindow::onSearchById() {
    bool ok;
    int id = searchIdEdit->text().toInt(&ok);
    if(!ok) {
        QMessageBox::warning(this, "错误", "请输入有效的学号");
        return;
    }
    
    QList<Grade> results;
    for(size_t i=0; i<L.size(); ++i) {
        if(L[i].id == id) {
            results.append(L[i]);
        }
    }
    
    if(results.isEmpty()) {
        QMessageBox::information(this, "查询结果", "未找到学号为 " + QString::number(id) + " 的记录");
        return;
    }
    
    tableWidget->setRowCount(results.size());
    for(int i=0; i<results.size(); ++i) {
        const Grade& g = results[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(g.id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(g.name));
        tableWidget->setItem(i, 2, new QTableWidgetItem(g.major));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(g.year)));
        tableWidget->setItem(i, 4, new QTableWidgetItem(g.subject));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(g.score, 'f', 1)));
    }
    tableWidget->resizeColumnsToContents();
    log(QString("查询学号 %1: 找到 %2 条记录").arg(id).arg(results.size()));
}

void MainWindow::onSearchByName() {
    QString name = searchNameEdit->text().trimmed();
    if(name.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入姓名");
        return;
    }
    
    QList<Grade> results;
    for(size_t i=0; i<L.size(); ++i) {
        if(L[i].name == name) {
            results.append(L[i]);
        }
    }
    
    if(results.isEmpty()) {
        QMessageBox::information(this, "查询结果", "未找到姓名为 \"" + name + "\" 的记录");
        return;
    }
    
    tableWidget->setRowCount(results.size());
    for(int i=0; i<results.size(); ++i) {
        const Grade& g = results[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(g.id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(g.name));
        tableWidget->setItem(i, 2, new QTableWidgetItem(g.major));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(g.year)));
        tableWidget->setItem(i, 4, new QTableWidgetItem(g.subject));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(g.score, 'f', 1)));
    }
    tableWidget->resizeColumnsToContents();
    log(QString("查询姓名 %1: 找到 %2 条记录").arg(name).arg(results.size()));
}

void MainWindow::onSearchBySubject() {
    QString subject = searchSubjectCombo->currentText().trimmed();
    if(subject.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入科目");
        return;
    }
    
    QList<Grade> results = SI.getBySubject(subject);
    
    if(results.isEmpty()) {
        QMessageBox::information(this, "查询结果", "未找到科目为 \"" + subject + "\" 的记录");
        return;
    }
    
    tableWidget->setRowCount(results.size());
    for(int i=0; i<results.size(); ++i) {
        const Grade& g = results[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(g.id)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(g.name));
        tableWidget->setItem(i, 2, new QTableWidgetItem(g.major));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(g.year)));
        tableWidget->setItem(i, 4, new QTableWidgetItem(g.subject));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(g.score, 'f', 1)));
    }
    tableWidget->resizeColumnsToContents();
    log(QString("查询科目 %1: 找到 %2 条记录").arg(subject).arg(results.size()));
}

void MainWindow::onShowAll() {
    refreshTable();
    log("显示全部记录");
}

void MainWindow::onShowAllBySubject() {
    refreshTable();
    log("按科目分组显示全部记录");
    // 可以在这里添加按科目分组的显示逻辑
}

void MainWindow::onLoadFile() {
    QString path = QFileDialog::getOpenFileName(this, "选择文件", datafile, "CSV文件 (*.csv);;所有文件 (*)");
    if(!path.isEmpty()) {
        datafile = path;
        loadFromFile(path);
    }
}

void MainWindow::onSaveFile() {
    QString path = QFileDialog::getSaveFileName(this, "保存文件", datafile, "CSV文件 (*.csv);;所有文件 (*)");
    if(!path.isEmpty()) {
        datafile = path;
        if(saveToFile(path)) {
            QMessageBox::information(this, "成功", "文件已保存: " + path);
        }
    }
}

void MainWindow::onLoadBackup() {
    QString bakPath = datafile + ".bak";
    if(!QFile::exists(bakPath)) {
        QMessageBox::warning(this, "错误", "未找到备份文件: " + bakPath);
        return;
    }
    
    if(loadFromFile(bakPath)) {
        QMessageBox::information(this, "成功", "已从备份恢复数据");
    }
}

void MainWindow::onLoadSeed() {
    int ret = QMessageBox::question(this, "确认", "加载示例数据将清空当前数据，是否继续？",
                                   QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes) {
        loadSeedData();
    }
}

void MainWindow::onTableSelectionChanged() {
    QList<QTableWidgetItem*> items = tableWidget->selectedItems();
    if(items.isEmpty()) return;
    
    int row = items[0]->row();
    if(row < 0 || row >= (int)L.size()) return;
    
    const Grade& g = L[row];
    idSpin->setValue(g.id);
    nameEdit->setText(g.name);
    majorEdit->setText(g.major);
    yearSpin->setValue(g.year);
    subjectEdit->setText(g.subject);
    scoreSpin->setValue(g.score);
}

