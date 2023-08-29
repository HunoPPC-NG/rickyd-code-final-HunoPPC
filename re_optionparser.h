/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_optionparser
#define header_re_optionparser

#include "precomp.h"

class RE_Option
{
public:
	std::string name;
	std::string defaults;
	bool required;
	int expectArgs;
	char delim;
	std::string args_help;
	std::string help;
};

class RE_OptionParser
{
public:
	RE_OptionParser();
	~RE_OptionParser();
	
	//add option
	void add(std::string name, std::string defaults, bool required, int expectArgs, char delim, std::string args_help, std::string help, std::string flag1, std::string flag2="", std::string flag3="", std::string flag4 = "");
	//parse args
	void parse(std::vector<std::string> args);
	
	bool has_errors(){return errors.size();}
	bool has_option(std::string option_name);
	std::vector<std::string> get_args(std::string option_name);
	std::string display_help();
	std::string display_errors();
	
	std::string overview;
	std::string syntax;
	std::string example;
	std::string footer;
	
protected:
	std::map<std::string, RE_Option> options;
	std::map<std::string, std::vector<std::string> > arg_options;
	std::vector<std::string> errors;
};

#endif