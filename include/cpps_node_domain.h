#ifndef CPPS_NODE_DOMAIN_CPPS_HEAD_
#define CPPS_NODE_DOMAIN_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	88481106@qq.com
//@Date			:	2015/11/25 (yy/mm/dd)
//@Module		:	CPPS_DOMAIN
//@Description	:	½ÚµãÓò¿Õ¼ä
//@website		:	http://cpps.wiki
//==================================



namespace cpps
{
	struct cpps_node_domain
	{
		cpps_node_domain(node *n,cpps_node_domain* p, char t, std::string nm);

		cpps_node_domain* parent;
		node			* nodedomain;
		usint8			  domainType;
		std::string		  name;
	};
}
#endif //CPPS_NODE_DOMAIN_CPPS_HEAD_