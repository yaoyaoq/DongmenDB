//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现基础上，实现delete from子句
 *
 *  支持的delete语法：
 *
 *  DELETE FROM <table_nbame>
 *  WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_delete 结构
 */

sql_stmt_delete *DeleteParser::parse_sql_stmt_delete(){
  char *tableName = nullptr;
  SRA_t *where = nullptr;
  Token *token = this->parseNextToken();

  //匹配delete
  if(!this->matchToken(TOKEN_RESERVED_WORD, (char*)"delete"))
  {
    strcpy(this->parserMessage, "语法错误.");
    return nullptr;
  }

  //获取表名
  token = this->parseNextToken();
  if(token->type != TOKEN_WORD)
  {
    strcpy(this->parserMessage, "invalid sql: missing table name.");
    return nullptr;
  }
  tableName = new_id_name();
  strcpy(tableName, token->text);
  TableReference_t *tableReference = TableReference_make(tableName, nullptr);
  SRA_t *table = SRATable(tableReference);
  where = table;

  //如果没有where
  if(token == nullptr || token->type == TOKEN_SEMICOLON)
    where = table;
  else if(token->type == TOKEN_RESERVED_WORD)
  {
    //匹配where
    if(!this->matchToken(TOKEN_RESERVED_WORD, "where"))
    {
      strcpy(this->parserMessage, "invalid sql: missing where.");
      return nullptr;
    }
    //解析where语句
    SRA_t *table = SRATable(tableReference);
    Expression *expression = this->parseExpressionRD();
    if(this->parserStateType == PARSER_WRONG)
      return nullptr;
    SRA_t *select = SRASelect(table, expression);
    where = select;
  }

  sql_stmt_delete *sqlStmtDelete = static_cast<sql_stmt_delete *>(calloc(sizeof(sql_stmt_delete), 1));
  sqlStmtDelete->where = where;
  sqlStmtDelete->tableName = tableName;
  return sqlStmtDelete;
};