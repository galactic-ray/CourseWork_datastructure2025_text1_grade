#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <QString>
#include <QList>
#include "datastructures.h"

// 打印管理器 - 用于格式化输出成绩数据
class PrintManager {
public:
    // 格式化单个成绩记录为字符串
    static QString formatGrade(const Grade& g);
    
    // 格式化成绩列表为表格字符串
    static QString formatGradeList(const QList<Grade>& grades);
    
    // 按科目分组格式化
    static QString formatBySubject(const QList<Grade>& grades);
    
    // 生成CSV格式字符串
    static QString toCSV(const QList<Grade>& grades);
};

#endif // PRINTMANAGER_H

