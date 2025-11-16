#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "datastructures.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    SeqList<Grade> L;
    SubjectIndex SI;
    QString datafile;

    // UI组件
    QTableWidget* tableWidget;
    QTextEdit* logText;
    QLineEdit* searchIdEdit;
    QLineEdit* searchNameEdit;
    QComboBox* searchSubjectCombo;
    
    // 输入字段
    QSpinBox* idSpin;
    QLineEdit* nameEdit;
    QLineEdit* majorEdit;
    QSpinBox* yearSpin;
    QLineEdit* subjectEdit;
    QDoubleSpinBox* scoreSpin;
    QSpinBox* insertPosSpin;

    void setupUI();
    void refreshTable();
    void log(const QString& msg);
    void updateStatus();

    // 文件操作
    bool saveToFile(const QString& path);
    bool loadFromFile(const QString& path);
    static QStringList parseCSVLine(const QString& line);
    static QString csvEscape(const QString& s);

    // 数据操作
    QList<Grade> seedData();
    void loadSeedData();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onAddGrade();
    void onInsertGrade();
    void onDeleteGrade();
    void onEditGrade();
    void onSearchById();
    void onSearchByName();
    void onSearchBySubject();
    void onShowAll();
    void onShowAllBySubject();
    void onLoadFile();
    void onSaveFile();
    void onLoadBackup();
    void onLoadSeed();
    void onTableSelectionChanged();
};

#endif // MAINWINDOW_H

