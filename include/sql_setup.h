#ifndef SQL_SETUP_H
#define SQL_SETUP_H

#include <occi.h>

// Creates required stored procedures in the Oracle database
void createProcedures(oracle::occi::Statement* stmt);

#endif 
