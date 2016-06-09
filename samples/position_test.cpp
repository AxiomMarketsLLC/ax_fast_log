
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include "global.h"
#include "position.h"
#include "product.h"
#include "logging.h"
#include "server_args.h"

using namespace std;
using namespace boost::multiprecision;

std::istream& operator >>(std::istream& is, ax_msg_field_value_exch_id& t)
{
    int i;
    is >> i;
    t = (ax_msg_field_value_exch_id)i;
    return is;
};


struct InstrumentRow
{
	string instrument_name;
	int exchange_id;
	string context_id;

	int instance_cnt;
	int context_cnt;
	int global_cnt;
	int combined_cnt;
};

struct PositionData
{
	Position pos;
	vector<FillArgs> fills;
	vector<InstrumentRow> tst;
    PositionData()
    {
		boost::log::core::get()->remove_all_sinks();
		boost::log::add_console_log(
            std::cout,
			boost::log::keywords::filter = (
				boost::log::trivial::severity >= boost::log::trivial::debug
			)
		);

		boost::log::add_file_log(
            boost::log::keywords::file_name = "tests/logs/tomahawk_position_test.json",
			boost::log::keywords::filter = (
				boost::log::trivial::severity >= boost::log::trivial::debug
			),
            boost::log::keywords::format = (
                boost::log::expressions::format("{\"timestamp\":\"%1%\",\"severity\":\"%2%\",%3%}")
                    % boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                    % boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity")
                    % boost::log::expressions::smessage
            ),
            boost::log::keywords::auto_flush = true,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0)
		);

		ifstream infile("./tests/data/fill_data_tests.csv");
		BOOST_REQUIRE_MESSAGE(infile, "No File, can't test");
		string line;
		int i = 0;
		while (getline(infile, line))
		{
			istringstream iss(line);
		    fills.push_back(FillArgs());
			iss >> fills[i].fill_id
				>> fills[i].exchage_id
				>> fills[i].client_order_id
				>> fills[i].client_ip_order_id
				>> fills[i].instrument_name
				>> fills[i].is_ask
				>> fills[i].price
				>> fills[i].fill_qty
				>> fills[i].is_leg
				>> fills[i].is_ip_order_summary
				>> fills[i].time
				>> fills[i].context_id
				>> fills[i].is_manual;

		   i++;
		}
		BOOST_TEST_MESSAGE("Loaded " << fills.size() << " sample fills.");

		ifstream pofile("./tests/data/position_tests.csv");
		BOOST_REQUIRE_MESSAGE(pofile, "No File, can't test");
		i = 0;
		while (getline(pofile, line))
		{
			istringstream iss(line);
		    tst.push_back(InstrumentRow());
			iss >> tst[i].instrument_name
				>> tst[i].exchange_id
				>> tst[i].context_id
				>> tst[i].instance_cnt
				>> tst[i].context_cnt
				>> tst[i].global_cnt
				>> tst[i].combined_cnt;
		   i++;
		}
		BOOST_TEST_MESSAGE("Loaded " << tst.size() << " test cases.");
    }

    ~PositionData(){}
};

BOOST_FIXTURE_TEST_SUITE(position, PositionData)

BOOST_AUTO_TEST_CASE(Setup)
{
	pos.Init();
	for(vector<FillArgs>::size_type i = 0; i != fills.size(); i++)
		pos.TradeUpdated(fills[i]);

}

BOOST_AUTO_TEST_CASE(Instance)
{

	for(vector<InstrumentRow>::size_type i = 0; i != tst.size(); i++)
	{
		int val_expected = tst[i].instance_cnt;
		int val_calculated = pos.GetPosition(const_cast<char*>(tst[i].instrument_name.c_str()),
											 (ax_msg_field_value_exch_id)tst[i].exchange_id,
											 const_cast<char*>(tst[i].context_id.c_str()),
											 POS_INSTANCE);

		BOOST_CHECK_MESSAGE(AreEqual(val_expected, val_calculated), "Failed Instnace Position Get on case " << i + 1 << " expected " << val_expected << " but got " << val_calculated << ".");
	}
}
BOOST_AUTO_TEST_CASE(Context)
{
	for(vector<InstrumentRow>::size_type i = 0; i != tst.size(); i++)
	{
		int val_expected = tst[i].context_cnt;
		int val_calculated = pos.GetPosition(const_cast<char*>(tst[i].instrument_name.c_str()),
											 (ax_msg_field_value_exch_id)tst[i].exchange_id,
											 const_cast<char*>(tst[i].context_id.c_str()),
											 POS_CONTEXT);

		BOOST_CHECK_MESSAGE(AreEqual(val_expected, val_calculated), "Failed Context Position Get on case " << i + 1 << " expected " << val_expected << " but got " << val_calculated << ".");
	}

}
BOOST_AUTO_TEST_CASE(Global)
{

	for(vector<InstrumentRow>::size_type i = 0; i != tst.size(); i++)
	{
		int val_expected = tst[i].global_cnt;
		int val_calculated = pos.GetPosition(const_cast<char*>(tst[i].instrument_name.c_str()),
											 (ax_msg_field_value_exch_id)tst[i].exchange_id,
											 const_cast<char*>(tst[i].context_id.c_str()),
											 POS_GLOBAL);

		BOOST_CHECK_MESSAGE(AreEqual(val_expected, val_calculated), "Failed Global Position Get on case " << i + 1 << " expected " << val_expected << " but got " << val_calculated << ".");
	}

}
BOOST_AUTO_TEST_CASE(Combined)
{
	for(vector<InstrumentRow>::size_type i = 0; i != tst.size(); i++)
	{
		int val_expected = tst[i].combined_cnt;
		int val_calculated = pos.GetPosition(const_cast<char*>(tst[i].instrument_name.c_str()),
											 (ax_msg_field_value_exch_id)tst[i].exchange_id,
											 const_cast<char*>(tst[i].context_id.c_str()),
											 POS_COMBINED);

		BOOST_CHECK_MESSAGE(AreEqual(val_expected, val_calculated), "Failed Combined Position Get on case " << i + 1 << " expected " << val_expected << " but got " << val_calculated << ".");
	}
}
BOOST_AUTO_TEST_CASE(ClearMap)
{
	bool actual = pos.ClearMap();
	bool expected = true;

	BOOST_CHECK_MESSAGE(actual == expected, "Failed Position ClearMap: expected " << expected << " but got " << actual << ".");
}

BOOST_AUTO_TEST_SUITE_END()
