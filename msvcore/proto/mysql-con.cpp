#pragma comment(lib,"../../mysql-connector-c-6.1.6/lib/libmysql.lib") 

#ifdef WIN32
#include "mysql.h"
#else
#include "mysql/mysql.h"
#endif

#define MSQLS_CONNECTED 1

// updates:
// optimize fields list

class MySQLCon{
	int status;

	// connection data
	MString con_host, con_user, con_pass, con_db; int con_port;

	// MySQL
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_FIELD *field;
	MYSQL_ROW row;

public:
	MySQLCon(){
		res = 0;
		field = 0;

		mysql_init(&mysql);
		Release();
	}

	void Release(){
		status = 0;
		con_host.Clean();
		con_user.Clean();
		con_pass.Clean();
		con_db.Clean();
		con_port = 0;

		mysql_free_result(res);

		res = 0;
		field = 0;
		return ;
	}

	~MySQLCon(){
		Release();
		mysql_close(&mysql);
	}

	// connect
	int Connect(VString host, VString user, VString pass, VString db, int port = 3306){
		con_host = host;
		con_user = user;
		con_pass = pass;
		con_db = db;
		con_port = port;

		return Connect();
	}

	int Connect(){
		if(!mysql_real_connect(&mysql, con_host, con_user, con_pass, con_db, con_port, NULL, 0))
			return 0;
		
		status = MSQLS_CONNECTED;

		return 1;
	}

	int SelectDB(VString db){
		con_db = db;
		return !mysql_select_db(&mysql, con_db);
	}

	int Query(VString query){
		if(status != MSQLS_CONNECTED)
			return 0;

		if(!mysql_real_query(&mysql, query, query)){
			if(res)
				mysql_free_result(res);
			res = mysql_store_result(&mysql);
			return 1;
		}

		return 0;
	}

	int GetNumFields(){
		return res ? mysql_num_fields(res) : 0;
	}

	int GetNumRows(){
		return res ? (int)mysql_num_rows(res) : 0;
	}

	VString GetFieldName(int i){
		if(!res)
			return VString();

		field = mysql_fetch_field_direct(res, i);
		if(!field)
			return VString();

		return field->name;
	}

	int GetFieldNameId(VString name){
		if(!res)
			return -1;

		VString table = PartLine(name, name, ".");
		if(!name){
			name = table;
			table.Clean();
		}

		int fs = GetNumFields();
		for(int i = 0; i < fs; i++){
			field = mysql_fetch_field_direct(res, i);
			if(name == VString(field->name) && (!table || table == VString(field->table)))
				return i;
		}

		return -1;
	}

	MYSQL_ROW GetRow(){
		if(!res)
			return 0;
		row = mysql_fetch_row(res);
		return row;
	}

	VString GetRowVal(VString name){
		if(!row)
			return VString();

		int fid = GetFieldNameId(name);
		if(fid < 0)
			return VString();

		return row[fid];		
	}

	VString GetRowVal(int fid){
		if(!row)
			return VString();

		if(fid < 0 || fid >= GetNumFields())
			return VString();

		return row[fid];		
	}



	VString operator [](VString name){
		return GetRowVal(name);
	}

	int GetInsertId(){
		return mysql_insert_id(&mysql);
	}
	
	MString GetError(){
		return mysql_error(&mysql);
	}

};