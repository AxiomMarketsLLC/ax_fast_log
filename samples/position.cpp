#include "position.h"
#include "logging.h"
static pthread_mutex_t cache_lock_ = PTHREAD_MUTEX_INITIALIZER;
extern ServerReceiver g_sr;

std::map<std::string, int> Position::prod_pos_map;

Position::Position()
{
	fill_cache_ = new FillArgs[INIT_FILL_SIZE];
	max_fills_ = INIT_FILL_SIZE;
}

Position::~Position()
{
	delete[] fill_cache_;
}


int Position::GetPosition(char* instrument_name, ax_msg_field_value_exch_id _exch, char* _context_id, PositionStyle _style)
{
	std::vector<std::string> parts;
	boost::split(parts, _context_id, boost::is_any_of("_"));

	//combined / base
	std::string key = instrument_name;
	switch(_style)
	{
		//global
		case POS_GLOBAL:
			key += _exch;
			break;
		//context
		case POS_CONTEXT:
			key += _exch;
			if (parts.size() >= 1)
				key += parts[0];
			break;

		//instance
		case POS_INSTANCE:
			key += _exch;
			if (parts.size() >= 1)
				key += parts[0];
			if (parts.size() >= 2)
				key += parts[1];
			break;
	}




	std::map<std::string,int>::iterator i = prod_pos_map.find(key);

	if (i != prod_pos_map.end())
	{
		DEBUG << LOGMESS("Get Position") << ","
			  << LOGVAR(key) << ","
			  << LOGVAR(instrument_name) << ","
			  << LOGVAR(_exch) << ","
			  << LOGVAR(_context_id) << ","
			  << LOGVAR(_style) << ","
			  << "\"position\":\"" << i->second << "\"";

		return i->second;
	}

	DEBUG << LOGMESS("Get Position: Could not find key!") << ","
		  << LOGVAR(key) << ","
		  << LOGVAR(instrument_name) << ","
		  << LOGVAR(_exch) << ","
		  << LOGVAR(_context_id) << ","
		  << LOGVAR(_style);

	return 0;
}


void Position::TradeUpdated(FillArgs _fa)
{
	if (strncmp( _fa.context_id, "mfx", 3) == 0
		|| _fa.is_ip_order_summary == 1)
		{
			DEBUG << LOGMESS("Position Trade Updated: Invalid Context ID/IP Order Summary") << ","
				  << LOGVAR(_fa.context_id) << ","
				  << LOGVAR(_fa.is_ip_order_summary);

			return;
		}

	AddToCache(_fa);
	AddToMap(_fa);

	INFO << LOGMESS("Position Trade updated") << ","
		 << LOGVAR(_fa.instrument_name) << ","
		 << LOGVAR(_fa.context_id) << ","
		 << LOGVAR(_fa.is_ip_order_summary);
}

bool Position::AddToCache(FillArgs _fa)
{
	static int total_fills = 0;

	for(int i = 0; i != total_fills; i++)
	{
		if ( _fa.fill_id == fill_cache_[i].fill_id )
		{
			DEBUG << LOGMESS("Position Add To Cache: Fill Already Added") << ","
				  << LOGVAR(_fa.fill_id) << ","
				  << LOGVAR(_fa.exchage_id) << ","
				  << LOGVAR(_fa.instrument_name) << ","
				  << LOGVAR(_fa.context_id) << ","
				  << LOGVAR(total_fills);

			return false;
		}
	}

	pthread_mutex_lock(&cache_lock_);
	fill_cache_[total_fills] = _fa;
	pthread_mutex_unlock(&cache_lock_);
	total_fills++;

	if (total_fills == max_fills_)
	{
		max_fills_ = 2 * total_fills;
		FillArgs* temp_fill_list = new FillArgs[max_fills_];
		for(int i=0; i < max_fills_; i++)
		{
			temp_fill_list[i] = fill_cache_[i];
		}
		delete[] fill_cache_;
		fill_cache_ = temp_fill_list;
	}

	DEBUG << LOGMESS("Position Add Fill To Cache") << ","
		  << LOGVAR(_fa.fill_id) << ","
		  << LOGVAR(_fa.exchage_id) << ","
		  << LOGVAR(_fa.instrument_name) << ","
		  << LOGVAR(_fa.context_id) << ","
		  << "\"total_fills_cache\":\"" << total_fills << "\"";

	return true;
}

bool Position::AddToMap(FillArgs _fa)
{
	int qty = _fa.is_ask ? -_fa.fill_qty : _fa.fill_qty;
	std::vector<std::string> parts;
	boost::split(parts, _fa.context_id, boost::is_any_of("_"));

	//combined
	std::string key = _fa.instrument_name;
	std::map<std::string,int>::iterator i = prod_pos_map.find(key);

	if (i == prod_pos_map.end()) { prod_pos_map[key] = qty; }
	else { i->second += qty; }

	//global
	key += _fa.exchage_id;
	i = prod_pos_map.find(key);

	if (i == prod_pos_map.end()) { prod_pos_map[key] = qty; }
	else { i->second += qty; }

	//context
	if (parts.size() >= 1)
	{
		key += parts[0];
		i = prod_pos_map.find(key);


		if (i == prod_pos_map.end()) { prod_pos_map[key] = qty; }
		else { i->second += qty; }
	}
	//instance
	if (parts.size() >= 2)
	{
		key += parts[1];
		i = prod_pos_map.find(key);


		if (i == prod_pos_map.end()) { prod_pos_map[key] = qty; }
		else { i->second += qty; }
	}

	DEBUG << LOGMESS("Position Add Fill To Map") << ","
		  << LOGVAR(_fa.fill_id) << ","
		  << LOGVAR(_fa.exchage_id) << ","
		  << LOGVAR(_fa.instrument_name) << ","
		  << LOGVAR(_fa.context_id) << ","
		  << "\"total_fills_map\":\"" << prod_pos_map.size() << "\"";

	return true;
}

bool Position::ClearMap()
{
	//clear the map

	std::map<std::string, int>::iterator del;
	while(!prod_pos_map.empty())
	{
		del = prod_pos_map.begin();
		if(del != prod_pos_map.end())
		{
			//delete del->second;
			//del->second = NULL;
			prod_pos_map.erase(del);
		}
	}

	INFO << LOGMESS("Position Cleared Map");

	return true;
}
