/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_optionparser.h"
#include "re_helpers.h"

RE_OptionParser::RE_OptionParser()
{
}

RE_OptionParser::~RE_OptionParser()
{
	arg_options.clear();
}

void RE_OptionParser::add(std::string name, std::string defaults, bool required, int expectArgs, char delim, std::string args_help, std::string help, std::string flag1, std::string flag2, std::string flag3, std::string flag4)
{
	RE_Option opt;
	opt.name = name;
	opt.defaults = defaults;
	opt.required = required;
	opt.expectArgs = expectArgs;
	opt.delim = delim;
	opt.args_help = args_help;
	opt.help = help;
	
	if(flag1.size() > 0)
		options[flag1]=opt;
	
	if(flag2.size() > 0)
		options[flag2]=opt;
	
	if(flag3.size() > 0)
		options[flag3]=opt;
	
	if(flag4.size() > 0)
		options[flag4]=opt;
	
}

void RE_OptionParser::parse(std::vector<std::string> args)
{
	for(int i=0; i < args.size(); ++i) 
	{
		std::string s = args[i];

		if (options.count(s) > 0)
		{
			//flag found
			//retrieve and validate option
			RE_Option opt = options.find(s)->second;
			std::vector<std::string> opt_args;
			if (opt.expectArgs > 0)
			{
				i++;
				if(i < args.size())
				{
					RE_Helpers *helpers = new RE_Helpers();
					opt_args = helpers->split(args[i], opt.delim);
					delete(helpers);
					if(opt_args.size() != opt.expectArgs)
					{
						errors.push_back("unexpected number of arguments passed by '"+s+"' flag");
					}
				}
			}
			
			arg_options[opt.name] = opt_args;
		}
	}
}

bool RE_OptionParser::has_option(std::string option_name)
{
	return(arg_options.count(option_name) > 0);
}

std::vector<std::string> RE_OptionParser::get_args(std::string option_name)
{
	std::vector<std::string> empty_vect;
	
	if (arg_options.count(option_name) > 0)
	{
		return arg_options[option_name];
	}
	else
		return (empty_vect);
}

std::string RE_OptionParser::display_help()
{
	std::map<std::string, RE_Option>::iterator it;
	std::string help_msg="";
	
	if(overview.size() > 0)
		help_msg += overview +"\n";
	if(syntax.size() > 0)
		help_msg += syntax +"\n";
	if(example.size() > 0)
		help_msg += example +"\n";
	
	help_msg += "\nOPTIONS:\n";
	for(it = options.begin(); it != options.end(); it++)
	{
		if((*it).second.args_help.size() + (*it).first.size() >= 14)
			help_msg += (*it).first+" "+(*it).second.args_help+"\t:"+(*it).second.help+"\n";
		else if((*it).second.args_help.size() + (*it).first.size() >= 7)
			help_msg += (*it).first+" "+(*it).second.args_help+"\t\t:"+(*it).second.help+"\n";
		else
			help_msg += (*it).first+" "+(*it).second.args_help+"\t\t\t:"+(*it).second.help+"\n";
	}
	help_msg += "\n";
	
	if(footer.size() > 0)
		help_msg += footer +"\n";
		
	return help_msg;
}

std::string RE_OptionParser::display_errors()
{
	std::string errors_msg="";
	for(int i=0; i<errors.size(); i++)
	{
		errors_msg += errors[i] + "\n";
	}
	errors_msg +="\n";
	return errors_msg;
}
