#pragma once
#include "common\\common.h"
#include "gui\\gui.h"
#include <ppl.h>
#include <concurrent_queue.h>

namespace scenario {
	class scenario_manager;
}

class world_state;

namespace messages {
	enum class message_type {
		WEGAINCB = 0,
		REVOLTALLIANCE = 1,
		WAR_THEY_ACCEPT = 2,
		WAR_WE_ACCEPT = 3,
		WAR_OTHER_ACCEPT = 4,
		WARGOAL_THEY_ACCEPT = 5,
		WARGOAL_WE_ACCEPT = 6,
		WARGOAL_OTHER_ACCEPT = 7,
		SIEGEOVER = 8,
		SIEGEUS = 9,
		SIEGEUSREBEL = 10,
		SIEGETHEM = 11,
		SIEGETHEMREBEL = 12,
		COLONY_SUCCESS = 13,
		COLONY_FAIL = 14,
		COLONY_ABORT = 15,
		SOCREFORM = 16,
		SOCREFORM_BAD = 17,
		POLREFORM = 18,
		POLREFORM_BAD = 19,
		ECONOMICREFORM = 20,
		ECONOMICREFORM_BAD = 21,
		MILITARYREFORM = 22,
		MILITARYREFORM_BAD = 23,
		SETPARTY = 24,
		UPPERHOUSE = 25,
		ELECTIONSTART = 26,
		ELECTIONDONE = 27,
		BREAKCOUNTRY = 28,
		BREAKCOUNTRYOTHER = 29,
		ANNEX = 30,
		PEACE_WE_ACCEPT = 31,
		PEACE_WE_DECLINE = 32,
		PEACE_THEY_ACCEPT = 33,
		PEACE_THEY_DECLINE = 34,
		PEACE_OTHER_ACCEPT = 35,
		PEACE_OTHER_DECLINE = 36,
		PARTISANS = 37,
		WE_MOBILIZE = 38,
		OTHER_MOBILIZE = 39,
		WE_DEMOBILIZE = 40,
		OTHER_DEMOBILIZE = 41,
		BUILDING_DONE = 42,
		WEDEFECT = 43,
		THEYDEFECT = 44,
		OTHERDEFECT = 45,
		EVENTHAPPENOTHER = 46,
		EVENTHAPPENOTHEROPTION = 47,
		MAJOREVENTHAPPENOTHER = 48,
		MAJOREVENTHAPPENOTHEROPTION = 49,
		EVENTHAPPENUS = 50, // log only 
		INVENTION = 51,
		TECH_ONCE = 52,
		TRIGMOD = 53,
		TRIGMODLOST = 54,
		LEADERDIED = 55,
		ATTACKTHEM = 56,
		ATTACKUS = 57,
		LANDBATTLEOVER = 58,
		NAVALBATTLEOVER = 59,
		ALLYTAKEOVER = 60,
		ALLYTAKEOVERWE = 61,
		INDEPENDANCE = 62,
		PROVINCELOST = 63,
		NAVALATTACKTHEM = 64,
		NAVALATTACKUS = 65,
		UNITARRIVED = 66,
		DECISIONOTHER = 67,
		DECISIONUS = 68,
		WE_NO_LONGER_GREAT_POWER = 69,
		THEY_NO_LONGER_GREAT_POWER = 70,
		WE_BECAME_GREAT_POWER = 71,
		ALLIANCE_THEY_ACCEPT = 72,
		ALLIANCE_WE_ACCEPT = 73,
		ALLIANCE_OTHER_ACCEPT = 74,
		ALLIANCE_THEY_DECLINE = 75,
		ALLIANCE_WE_DECLINE = 76,
		ALLIANCE_OTHER_DECLINE = 77,
		CANCELALLIANCE_THEY_ACCEPT = 78,
		CANCELALLIANCE_WE_ACCEPT = 79,
		CANCELALLIANCE_OTHER_ACCEPT = 80,
		CALLALLY_THEY_ACCEPT = 81,
		CALLALLY_WE_ACCEPT = 82,
		CALLALLY_OTHER_ACCEPT = 83,
		CALLALLY_THEY_DECLINE = 84,
		CALLALLY_WE_DECLINE = 85,
		CALLALLY_OTHER_DECLINE = 86,
		EXPELADVISORS_THEY_ACCEPT = 87,
		EXPELADVISORS_WE_ACCEPT = 88,
		EXPELADVISORS_OTHER_ACCEPT = 89,
		BANEMBASSY_THEY_ACCEPT = 90,
		BANEMBASSY_WE_ACCEPT = 91,
		BANEMBASSY_OTHER_ACCEPT = 92,
		INCREASEOPINION_THEY_ACCEPT = 93,
		INCREASEOPINION_WE_ACCEPT = 94,
		INCREASEOPINION_OTHER_ACCEPT = 95,
		DECREASEOPINION_THEY_ACCEPT = 96,
		DECREASEOPINION_WE_ACCEPT = 97,
		DECREASEOPINION_OTHER_ACCEPT = 98,
		ADDTOSPHERE_THEY_ACCEPT = 99,
		ADDTOSPHERE_WE_ACCEPT = 100,
		ADDTOSPHERE_OTHER_ACCEPT = 101,
		REMOVEFROMSPHERE_THEY_ACCEPT = 102,
		REMOVEFROMSPHERE_WE_ACCEPT = 103,
		REMOVEFROMSPHERE_OTHER_ACCEPT = 104,
		INCREASERELATION_THEY_ACCEPT = 105,
		INCREASERELATION_WE_ACCEPT = 106,
		INCREASERELATION_OTHER_ACCEPT = 107,
		DECREASERELATION_THEY_ACCEPT = 108,
		DECREASERELATION_WE_ACCEPT = 109,
		DECREASERELATION_OTHER_ACCEPT = 110,
		WETARGET_EXPELADVISORS = 111,
		WETARGET_BANEMBASSY = 112,
		WETARGET_DECREASEOPINION = 113,
		WETARGET_REMOVEFROMSPHERE = 114,
		INTERVENTION_OUR_SIDE = 115,
		INTERVENTION_THEIR_SIDE = 116,
		INTERVENTION_OTHER_SIDE = 117,
		WE_INTERVENED = 118,
		BANKRUPTCY = 119,
		THEY_BANKRUPTCY = 120,
		NEW_PARTY_AVAIL = 121,
		PARTY_UNAVAIL = 122,
		GW_DISCOVERED = 123,
		WAR_GAIN_GW_STATUS = 124,
		OUR_CB_DETECTED = 125,
		CB_TOWARDS_US_DETECTED = 126,
		OTHERS_CB_DETECTED = 127,
		CB_JUSTIFY_NO_LONGER_VALID = 128,
		INVESTED_IN_US = 129,
		CRISIS_OFFER_THEY_ACCEPT = 130,
		CRISIS_OFFER_WE_ACCEPT = 131,
		CRISIS_OFFER_OTHER_ACCEPT = 132,
		CRISIS_OFFER_THEY_DECLINE = 133,
		CRISIS_OFFER_WE_DECLINE = 134,
		CRISIS_OFFER_OTHER_DECLINE = 135,
		BACK_CRISIS_THEY_ACCEPT = 136,
		BACK_CRISIS_WE_ACCEPT = 137,
		BACK_CRISIS_OTHER_ACCEPT = 138,
		BACK_CRISIS_THEY_DECLINE = 139,
		BACK_CRISIS_WE_DECLINE = 140,
		BACK_CRISIS_OTHER_DECLINE = 141,
		CRISIS_BACK_DOWN_WE_ACCEPT = 142,
		CRISIS_BACK_DOWN_WE_DECLINE = 143,
		CRISIS_BACK_DOWN_THEY_ACCEPT = 144,
		CRISIS_BACK_DOWN_THEY_DECLINE = 145,
		CRISIS_BACK_DOWN_OTHER_ACCEPT = 146,
		CRISIS_BACK_DOWN_OTHER_DECLINE = 147,
		CRISIS_NO_SIDE_PICKED = 148,
		CRISIS_BECAME_WAR = 149,
		CRISIS_STARTED = 150,
		CRISIS_NOBODY_BACKED = 151
	};

	constexpr int32_t message_count = 152;

	enum class message_setting : int8_t {
		discard = 0,
		log = 1,
		popup = 2,
		popup_and_pause = 3
	};

	struct message_settings {
		message_setting self = message_setting::popup;
		message_setting interesting_countries = message_setting::log;
		message_setting all_countries = message_setting::log;
	};

	using display_function = std::function<void(world_state&, ui::tagged_gui_object, ui::line_manager&, ui::text_format&)>;

	struct message_instance {
		display_function func;
		std::variant<std::monostate, nations::country_tag, provinces::province_tag> goto_tag;
	};

	constexpr int32_t maximum_displayed_messages = 128;

	using message_queue = concurrency::concurrent_queue<message_instance, concurrent_allocator<message_instance>>;


	class message_window_t;

	struct messages_manager {
		text_data::text_tag log_text[message_count];
		struct { text_data::text_tag line[6]; } message_text[message_count];
	};

	class message_window {
	public:
		std::unique_ptr<message_window_t> win;
		message_queue pending_messages;
		message_settings settings[message_count] = {};

		message_instance displayed_messages[maximum_displayed_messages] = {};
		int32_t current_message_count = 0;
		int32_t last_replaced_index = 0;
		int32_t currently_displayed_index = 0;

		message_window();
		~message_window();

		void hide_message_window(ui::gui_manager& gui_m);
		void update_message_window(ui::gui_manager& gui_m);
		void show_message_window(ui::gui_manager& gui_m);
		void init_message_window(world_state& ws);
	};

	void init_message_text(scenario::scenario_manager& s);
}
