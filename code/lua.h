#ifndef LUA_H
#define LUA_H

class Lua {

public:

	lua_State* l;
	bool error;

	// Lua() {};
	void init(char *lua_file);
	void loadSettings(int &width, int &height, double &viewportScale, bool &fullscreen, std::string &title, bool &limitFrames);

	char *get_table_var(char *table, char *var);
	bool get_table_table_var(char *table, char *table2, char *var);
	void set_table_table_number(char *table, char *table2, char *var, float num);
	void set_table_table_bool(char *table, char *table2, char *var, bool b);
	void set_table_table_digital_button(char *table, char *table2, digital_button b);
	void set_table_number(char *table, char *var, double num);

	bool confVar(std::string var);
	bool appVar(std::string var);
	void appSetVar(std::string var, int value);
	void appFunc(std::string func);
	void appFunc(std::string func, int value);

	void registerTable(std::string tableName, const luaL_Reg* functions);
	void registerTables();
	
	void registerFunction (std::string table, std::string function, /*std::function<int()>*/lua_CFunction func);

	void create_lua_func(char *name, lua_CFunction func);

};

#endif