//
// Created by sam on 2018/9/18.
//
#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

/*执行delete语句的物理计划，返回删除的记录条数
 * 返回大于等于0的值，表示删除的记录条数；
 * 返回小于0的值，表示删除过程中出现错误。
 * */

int ExecutionPlan::executeDelete(DongmenDB *db, sql_stmt_delete *sqlStmtDelete, Transaction *tx)
{
  /*删除语句以select的物理操作为基础实现。
   * 1. 使用 sql_stmt_delete 的条件参数，调用 generateSelect 创建select的物理计划并初始化;
   * 2. 执行 select 的物理计划，完成 delete 操作
   */
  //记录删除的条数
  size_t deleted_count = 0;
  //创建一个扫描
  Scan *scan = generateSelect(db, sqlStmtDelete->where, tx);
  //将扫描指针放到最前
  scan->beforeFirst();
  //利用循环进行遍历
  while(scan->next())
  {
    //删除记录
    scan->deleteRecord();
    ++deleted_count;
  }
  scan->close();
  return deleted_count;
}