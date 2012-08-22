#include "CK2Building.h"
#include "CK2Barony.h"
#include "CK2Title.h"
#include "CK2Character.h"
#include "..\Parsers\Object.h"
#include "..\Log.h"
#include "..\Configuration.h"



CK2Barony::CK2Barony(Object* obj, CK2Title* newTitle, CK2Province* newProvince, const CK2BuildingFactory* buildingFactory, const religionGroupMapping& religionGroupMap)
{
	title		= newTitle;
	province	= newProvince;

	type = obj->getLeaf("type");

	vector<Object*> leaves = obj->getLeaves();
	for (vector<Object*>::iterator i = leaves.begin(); i < leaves.end(); i++)
	{
		string key = (*i)->getKey();
		if ( (key.substr(0, 3) == "ca_") || (key.substr(0, 3) == "ct_") || (key.substr(0, 3) == "tp_") )
		{
			const CK2Building* newBuilding = buildingFactory->getBuilding(key, title->getHolder(), religionGroupMap);
			if (newBuilding != NULL)
			{
				buildings.push_back(newBuilding);
			}
		}
	}

	proxyMultiplier = 1;
	if (Configuration::getProxyMultiplierMethod() == "counting")
	{
		if (title->getTitleString() == title->getHolder()->getCapitalString())
		{
			vector<CK2Title*> titles = title->getHolder()->getTitles();
			for (vector<CK2Title*>::const_iterator titleItr = titles.begin(); titleItr != titles.end(); titleItr++)
			{
				if ( ((*titleItr)->getTitleString().substr(0,2) == "c_") && (proxyMultiplier < 2) )
				{
					proxyMultiplier = 2;
				}
				if ( ((*titleItr)->getTitleString().substr(0,2) == "d_") && (proxyMultiplier < 3) )
				{
					proxyMultiplier = 3;
				}
				if ( ((*titleItr)->getTitleString().substr(0,2) == "k_") && (proxyMultiplier < 4) )
				{
					proxyMultiplier = 4;
				}
				if ( ((*titleItr)->getTitleString().substr(0,2) == "e_") && (proxyMultiplier < 5) )
				{
					proxyMultiplier = 5;
				}
			}
		}
	}
	else if (Configuration::getProxyMultiplierMethod() == "exponential")
	{
		if (title->getTitleString() == title->getHolder()->getCapitalString())
		{
			vector<CK2Title*> titles = title->getHolder()->getTitles();
			for (vector<CK2Title*>::const_iterator titleItr = titles.begin(); titleItr != titles.end(); titleItr++)
			{
				if ( ((*titleItr)->getTitleString().substr(0,2) == "c_") && (proxyMultiplier < 2) )
				{
					proxyMultiplier = 2;
				}
				if ( ((*titleItr)->getTitleString().substr(0,2) == "d_") && (proxyMultiplier < 4) )
				{
					proxyMultiplier = 4;
				}
				if ( ((*titleItr)->getTitleString().substr(0,2) == "k_") && (proxyMultiplier < 8) )
				{
					proxyMultiplier = 8;
				}
				if ( ((*titleItr)->getTitleString().substr(0,2) == "e_") && (proxyMultiplier < 16) )
				{
					proxyMultiplier = 16;
				}
			}
		}
	}

	determineBaseTaxProxy();
	determinePopProxy();
	determineManpowerProxy();
}


void CK2Barony::determineBaseTaxProxy()
{
	baseTaxProxy = 0.0f;
	if (type == "city")
	{
		baseTaxProxy += 12.0f;
	}
	else if (type == "castle")
	{
		baseTaxProxy += 4.0f;
	}
	else if (type == "temple")
	{
		baseTaxProxy += 8.0f;
	}
	else
	{
		log("Note! Unhandled barony type %s\n", type.c_str());
	}

	for (vector<const CK2Building*>::iterator buildingItr = buildings.begin(); buildingItr != buildings.end(); buildingItr++)
	{
		if ( (*buildingItr)->getFortLevel() > 0.0f )
		{
			continue;
		}
		baseTaxProxy += (*buildingItr)->getTaxIncome();
	}

	baseTaxProxy *= proxyMultiplier;
}


void CK2Barony::determinePopProxy()
{
	popProxy = 0.0f;
	if (type == "city")
	{
		popProxy += 12.0f;
	}
	else if (type == "castle")
	{
		popProxy += 4.0f;
	}
	else if (type == "temple")
	{
		popProxy += 8.0f;
	}
	else
	{
		log("Note! Unhandled barony type %s\n", type.c_str());
	}

	for (vector<const CK2Building*>::iterator buildingItr = buildings.begin(); buildingItr != buildings.end(); buildingItr++)
	{
		popProxy += (*buildingItr)->getTaxIncome();
	}

	popProxy *= proxyMultiplier;
}


void CK2Barony::determineManpowerProxy()
{
	manpowerProxy = 0.0f;
	if (type == "castle")
	{
		manpowerProxy += 225;
	}
	else if (type == "city")
	{
		manpowerProxy += 115;
	}
	else if (type == "temple")
	{
		manpowerProxy += 125;
	}
	else
	{
		log("Note! Unhandled barony type %s\n", type.c_str());
	}

	double levyMultiplier = 0.0f;
	for (vector<const CK2Building*>::iterator buildingItr = buildings.begin(); buildingItr != buildings.end(); buildingItr++)
	{
		manpowerProxy	+= (*buildingItr)->getNumSoldiers();
		levyMultiplier	+= (*buildingItr)->getLevyMultiplier();
	}
	manpowerProxy *= levyMultiplier;
	
	manpowerProxy *= proxyMultiplier;
}