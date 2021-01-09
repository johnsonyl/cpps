#include "cpps/cpps.h"

namespace cpps
{

	cpps_node_domain::cpps_node_domain(node* n, cpps_node_domain* p, char t, std::string nm)
	{
		parent = p;
		nodedomain = n;
		domainType = t;
		name = nm;
	}

}