/*
 * YaST2: Core system
 *
 * Description:
 *   YaST2 SCR: PPD agent implementation
 *
 * Authors:
 *   Jiri Srain <jsrain@suse.cz>
 *
 * $Id$
 */

#include "Y2Logger.h"
#include "PPDAgent.h"

char last_error[1024];

/**
 * Constructor
 */
PPDAgent::PPDAgent() : SCRAgent()
{
}

/**
 * Destructor
 */
PPDAgent::~PPDAgent() {
}

/**
 * Dir
 */
YCPValue PPDAgent::Dir(const YCPPath& path) 
{
    return YCPError (string ("Wrong path '") + path->toString() + string ("' in Dir()."));
}

/**
 * Read
 */
YCPValue PPDAgent::Read(const YCPPath &path, const YCPValue& arg)
{
    if (path->length() == 2 && path->component_str(0)=="db") {
	if(path->component_str(1)=="creation_status")
	    return YCPInteger(database.creationStatus ());
        if(path->component_str(1)=="changed")
            return YCPBoolean(database.changed(NULL));
	if (path->component_str(1)=="vendorname" && arg->isString ())
	    return YCPString (database.getVendorId (arg->asString()->value_cstr ()));
	if (path->component_str(1)=="modelname" && arg->isList ())
	{
	    YCPList l = arg->asList ();
	    if (l->size () == 2 && l->value(0)->isString ()
		&& l->value(1)->isString())
	    {
		return YCPString (database.getModelId (
		    l->value(0)->asString ()->value_cstr (),
		    l->value(1)->asString ()->value_cstr ()));
	    }
	}
    }
    else if (path->length() == 2 && path->component_str(0) == "file")
    {
	if (path->component_str(1)=="open" && arg->isList ())
	{
	    YCPList l = arg->asList();
	    if ((l->size () == 2 && l->value(0)->isString ()
		&& l->value(1)->isString()))
	    {
		return fileops.openPpdFile (l->value(0)->asString (),
		    l->value(1)->asString ());
	    }
	}
	if (path->component_str(1)=="option_groups" && arg->isString ())
	{
	    return fileops.getOptionsGroups (arg->asString ());
	}
	if (path->component_str(1)=="options" && arg->isList ())
	{
	    YCPList l = arg->asList();
	    if ((l->size () == 2 && l->value(0)->isString ()
		 && l->value(1)->isString()))
	    {
		
		return fileops.getOptions (l->value(0)->asString (),
		    l->value(1)->asString ());
	    }
	}
    }
    // fallback...
    return YCPError(string ("Wrong path '") + path->toString() + string ("' in Read()."));
}

/**
 * Write
 */
YCPValue PPDAgent::Write(const YCPPath &path, const YCPValue& value, const YCPValue& arg)
{

    if(path->length()==2 && path->component_str(0)=="db") {
        if(path->component_str(1)=="create")
            return YCPBoolean(database.createdb());

    }
    // CHANGES handled via special PERL agent

    // fallback...
    return YCPError(string ("Wrong path '%s' in Write().") + path->toString());
}

/**
 * otherCommand
 */
YCPValue PPDAgent::otherCommand(const YCPTerm& term) {
    string sym = term->symbol()->symbol();

    if (sym == "PPDAgent") {
        /* Your initialization */
        return YCPVoid();
    }

    return YCPNull();
}