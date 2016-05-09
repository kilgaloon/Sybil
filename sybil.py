import re
import string

def main():
	path_to_ini = "/etc/sybil/sybil.ini"

	ini = open(path_to_ini).read();
	
	new_ini = re.sub(r'\#.*?\n','',ini)
	new_ini = new_ini.split(";");

	config = {};

	for settings in new_ini:
		setting = settings.split("=")

		if setting[0] != '':
			p = setting[0].strip()
			v = setting[1]


		config[p] = v;


	print(config);


main()