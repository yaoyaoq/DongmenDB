//
// Created by Sam on 2018/1/17.
//

#ifndef DONGMENDB_PARSER_H
#define DONGMENDB_PARSER_H

#include "tokenizer.h"

struct SelectStmt{
    char *fields[];
    char[][] *tables;

};


#endif //DONGMENDB_PARSER_H