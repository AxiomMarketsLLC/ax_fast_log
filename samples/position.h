#ifndef POSITION_H
#define POSITION_H

/***************/
/* includes	*/
/***************/

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <boost/lexical_cast.hpp>;
#include <boost/algorithm/string.hpp>
#include "global.h"
#include "server_receiver.h"
#include "server_args.h"
#include "product.h"
#include "ax_gui.h"

using namespace std;
using boost::lexical_cast;



class Position
{
	public:
		/**********/
		/* data	*/
		/**********/
				FillArgs* 		fill_cache_;
		/********************/
		/* constructors	    */
		/********************/
				void			Init(){};
								Position();

		/********************/
		/* deconstructor	*/
		/********************/
								~Position();

		/***************/
		/* functions   */
		/***************/
				int				GetPosition(char*, ax_msg_field_value_exch_id, char*, PositionStyle);
				void 			TradeUpdated(FillArgs);
				bool			ClearMap();
	private:
		/**********/
		/* data	  */
		/**********/
				static 			std::map<std::string, int> prod_pos_map;
				int 			is_back_leg_ ;
				int				max_fills_;


		/***************/
		/* functions   */
		/***************/
				bool 			AddToCache(FillArgs);
				bool			AddToMap(FillArgs);




};

#endif
