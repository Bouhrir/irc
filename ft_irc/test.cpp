// << JOIN #fhihichannel
// >> :fhihi!fouad@88ABE6.25BF1D.D03F86.88C9BD.IP JOIN #fhihichannel * :realname
// << MODE #fhihichannel
// << WHO #fhihichannel %chtsunfra,152
// >> :Osmium.AfterNET.Org 353 fhihi = #fhihichannel :fhihi!fouad@88ABE6.25BF1D.D03F86.88C9BD.IP @_i7i!_@88ABE6.25BF1D.D03F86.88C9BD.IP
// >> :Osmium.AfterNET.Org 366 fhihi #fhihichannel :End of /NAMES list.
// >> :Osmium.AfterNET.Org 324 fhihi #fhihichannel +tn
// >> :Osmium.AfterNET.Org 329 fhihi #fhihichannel 1709649017
// >> :Osmium.AfterNET.Org 354 fhihi 152 #fhihichannel fouad 88ABE6.25BF1D.D03F86.88C9BD.IP *.afternet.org fhihi Hx 0 :realname
// >> :Osmium.AfterNET.Org 354 fhihi 152 #fhihichannel _ 88ABE6.25BF1D.D03F86.88C9BD.IP *.afternet.org _i7i H@x 0 :https://www.afternet.org/qwebirc/
// >> :Osmium.AfterNET.Org 315 fhihi #fhihichannel :End of /WHO list.

// >> :Osmium.AfterNET.Org 353 fhihi = #chan :@fhihi!fouad@88ABE6.25BF1D.D03F86.88C9BD.IP
// >> :Osmium.AfterNET.Org 366 fhihi #chan :End of /NAMES list.
// >> :Osmium.AfterNET.Org 324 fhihi #chan +tn 
// >> :Osmium.AfterNET.Org 329 fhihi #chan 1709657437
// >> :Osmium.AfterNET.Org 354 fhihi 152 #chan fouad 88ABE6.25BF1D.D03F86.88C9BD.IP *.afternet.org fhihi H@x 0 :realname
// >> :Osmium.AfterNET.Org 315 fhihi #chan :End of /WHO list.

#include<iostream>
#include <sstream>

int main () {
	std::string str("mode general +o"), cmd, chan, modes;
	std::stringstream os(str);

	os >> cmd >> chan >> modes;
	std::cout << ":" << cmd  << ": :" << chan << ":";
	if (!modes.empty())
		std::cout << " :" << modes << ":";
	std::cout << std::endl;
	
}