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
	namespace message_type {
		constexpr int32_t WEGAINCB = 0;
		constexpr int32_t REVOLTALLIANCE = 1;
		constexpr int32_t WAR_THEY_ACCEPT = 2;
		constexpr int32_t WAR_WE_ACCEPT = 3;
		constexpr int32_t WAR_OTHER_ACCEPT = 4;
		constexpr int32_t WARGOAL_THEY_ACCEPT = 5;
		constexpr int32_t WARGOAL_WE_ACCEPT = 6;
		constexpr int32_t WARGOAL_OTHER_ACCEPT = 7;
		constexpr int32_t SIEGEOVER = 8;
		constexpr int32_t SIEGEUS = 9;
		constexpr int32_t SIEGEUSREBEL = 10;
		constexpr int32_t SIEGETHEM = 11;
		constexpr int32_t SIEGETHEMREBEL = 12;
		constexpr int32_t COLONY_SUCCESS = 13;
		constexpr int32_t COLONY_FAIL = 14;
		constexpr int32_t COLONY_ABORT = 15;
		constexpr int32_t SOCREFORM = 16;
		constexpr int32_t SOCREFORM_BAD = 17;
		constexpr int32_t POLREFORM = 18;
		constexpr int32_t POLREFORM_BAD = 19;
		constexpr int32_t ECONOMICREFORM = 20;
		constexpr int32_t ECONOMICREFORM_BAD = 21;
		constexpr int32_t MILITARYREFORM = 22;
		constexpr int32_t MILITARYREFORM_BAD = 23;
		constexpr int32_t SETPARTY = 24;
		constexpr int32_t UPPERHOUSE = 25;
		constexpr int32_t ELECTIONSTART = 26;
		constexpr int32_t ELECTIONDONE = 27;
		constexpr int32_t BREAKCOUNTRY = 28;
		constexpr int32_t BREAKCOUNTRYOTHER = 29;
		constexpr int32_t ANNEX = 30;
		constexpr int32_t PEACE_WE_ACCEPT = 31;
		constexpr int32_t PEACE_WE_DECLINE = 32;
		constexpr int32_t PEACE_THEY_ACCEPT = 33;
		constexpr int32_t PEACE_THEY_DECLINE = 34;
		constexpr int32_t PEACE_OTHER_ACCEPT = 35;
		constexpr int32_t PEACE_OTHER_DECLINE = 36;
		constexpr int32_t PARTISANS = 37;
		constexpr int32_t WE_MOBILIZE = 38;
		constexpr int32_t OTHER_MOBILIZE = 39;
		constexpr int32_t WE_DEMOBILIZE = 40;
		constexpr int32_t OTHER_DEMOBILIZE = 41;
		constexpr int32_t BUILDING_DONE = 42;
		constexpr int32_t WEDEFECT = 43;
		constexpr int32_t THEYDEFECT = 44;
		constexpr int32_t OTHERDEFECT = 45;
		constexpr int32_t EVENTHAPPENOTHER = 46;
		constexpr int32_t EVENTHAPPENOTHEROPTION = 47;
		constexpr int32_t MAJOREVENTHAPPENOTHER = 48;
		constexpr int32_t MAJOREVENTHAPPENOTHEROPTION = 49;
		constexpr int32_t EVENTHAPPENUS = 50; // log only 
		constexpr int32_t INVENTION = 51;
		constexpr int32_t TECH_ONCE = 52;
		constexpr int32_t TRIGMOD = 53;
		constexpr int32_t TRIGMODLOST = 54;
		constexpr int32_t LEADERDIED = 55;
		constexpr int32_t ATTACKTHEM = 56;
		constexpr int32_t ATTACKUS = 57;
		constexpr int32_t LANDBATTLEOVER = 58;
		constexpr int32_t NAVALBATTLEOVER = 59;
		constexpr int32_t ALLYTAKEOVER = 60;
		constexpr int32_t ALLYTAKEOVERWE = 61;
		constexpr int32_t INDEPENDANCE = 62;
		constexpr int32_t PROVINCELOST = 63;
		constexpr int32_t NAVALATTACKTHEM = 64;
		constexpr int32_t NAVALATTACKUS = 65;
		constexpr int32_t UNITARRIVED = 66;
		constexpr int32_t DECISIONOTHER = 67;
		constexpr int32_t DECISIONUS = 68;
		constexpr int32_t WE_NO_LONGER_GREAT_POWER = 69;
		constexpr int32_t THEY_NO_LONGER_GREAT_POWER = 70;
		constexpr int32_t WE_BECAME_GREAT_POWER = 71;
		constexpr int32_t ALLIANCE_THEY_ACCEPT = 72;
		constexpr int32_t ALLIANCE_WE_ACCEPT = 73;
		constexpr int32_t ALLIANCE_OTHER_ACCEPT = 74;
		constexpr int32_t ALLIANCE_THEY_DECLINE = 75;
		constexpr int32_t ALLIANCE_WE_DECLINE = 76;
		constexpr int32_t ALLIANCE_OTHER_DECLINE = 77;
		constexpr int32_t CANCELALLIANCE_THEY_ACCEPT = 78;
		constexpr int32_t CANCELALLIANCE_WE_ACCEPT = 79;
		constexpr int32_t CANCELALLIANCE_OTHER_ACCEPT = 80;
		constexpr int32_t CALLALLY_THEY_ACCEPT = 81;
		constexpr int32_t CALLALLY_WE_ACCEPT = 82;
		constexpr int32_t CALLALLY_OTHER_ACCEPT = 83;
		constexpr int32_t CALLALLY_THEY_DECLINE = 84;
		constexpr int32_t CALLALLY_WE_DECLINE = 85;
		constexpr int32_t CALLALLY_OTHER_DECLINE = 86;
		constexpr int32_t EXPELADVISORS_THEY_ACCEPT = 87;
		constexpr int32_t EXPELADVISORS_WE_ACCEPT = 88;
		constexpr int32_t EXPELADVISORS_OTHER_ACCEPT = 89;
		constexpr int32_t BANEMBASSY_THEY_ACCEPT = 90;
		constexpr int32_t BANEMBASSY_WE_ACCEPT = 91;
		constexpr int32_t BANEMBASSY_OTHER_ACCEPT = 92;
		constexpr int32_t INCREASEOPINION_THEY_ACCEPT = 93;
		constexpr int32_t INCREASEOPINION_WE_ACCEPT = 94;
		constexpr int32_t INCREASEOPINION_OTHER_ACCEPT = 95;
		constexpr int32_t DECREASEOPINION_THEY_ACCEPT = 96;
		constexpr int32_t DECREASEOPINION_WE_ACCEPT = 97;
		constexpr int32_t DECREASEOPINION_OTHER_ACCEPT = 98;
		constexpr int32_t ADDTOSPHERE_THEY_ACCEPT = 99;
		constexpr int32_t ADDTOSPHERE_WE_ACCEPT = 100;
		constexpr int32_t ADDTOSPHERE_OTHER_ACCEPT = 101;
		constexpr int32_t REMOVEFROMSPHERE_THEY_ACCEPT = 102;
		constexpr int32_t REMOVEFROMSPHERE_WE_ACCEPT = 103;
		constexpr int32_t REMOVEFROMSPHERE_OTHER_ACCEPT = 104;
		constexpr int32_t INCREASERELATION_THEY_ACCEPT = 105;
		constexpr int32_t INCREASERELATION_WE_ACCEPT = 106;
		constexpr int32_t INCREASERELATION_OTHER_ACCEPT = 107;
		constexpr int32_t DECREASERELATION_THEY_ACCEPT = 108;
		constexpr int32_t DECREASERELATION_WE_ACCEPT = 109;
		constexpr int32_t DECREASERELATION_OTHER_ACCEPT = 110;
		constexpr int32_t WETARGET_EXPELADVISORS = 111;
		constexpr int32_t WETARGET_BANEMBASSY = 112;
		constexpr int32_t WETARGET_DECREASEOPINION = 113;
		constexpr int32_t WETARGET_REMOVEFROMSPHERE = 114;
		constexpr int32_t INTERVENTION_OUR_SIDE = 115;
		constexpr int32_t INTERVENTION_THEIR_SIDE = 116;
		constexpr int32_t INTERVENTION_OTHER_SIDE = 117;
		constexpr int32_t WE_INTERVENED = 118;
		constexpr int32_t BANKRUPTCY = 119;
		constexpr int32_t THEY_BANKRUPTCY = 120;
		constexpr int32_t NEW_PARTY_AVAIL = 121;
		constexpr int32_t PARTY_UNAVAIL = 122;
		constexpr int32_t GW_DISCOVERED = 123;
		constexpr int32_t WAR_GAIN_GW_STATUS = 124;
		constexpr int32_t OUR_CB_DETECTED = 125;
		constexpr int32_t CB_TOWARDS_US_DETECTED = 126;
		constexpr int32_t OTHERS_CB_DETECTED = 127;
		constexpr int32_t CB_JUSTIFY_NO_LONGER_VALID = 128;
		constexpr int32_t INVESTED_IN_US = 129;
		constexpr int32_t CRISIS_OFFER_THEY_ACCEPT = 130;
		constexpr int32_t CRISIS_OFFER_WE_ACCEPT = 131;
		constexpr int32_t CRISIS_OFFER_OTHER_ACCEPT = 132;
		constexpr int32_t CRISIS_OFFER_THEY_DECLINE = 133;
		constexpr int32_t CRISIS_OFFER_WE_DECLINE = 134;
		constexpr int32_t CRISIS_OFFER_OTHER_DECLINE = 135;
		constexpr int32_t BACK_CRISIS_THEY_ACCEPT = 136;
		constexpr int32_t BACK_CRISIS_WE_ACCEPT = 137;
		constexpr int32_t BACK_CRISIS_OTHER_ACCEPT = 138;
		constexpr int32_t BACK_CRISIS_THEY_DECLINE = 139;
		constexpr int32_t BACK_CRISIS_WE_DECLINE = 140;
		constexpr int32_t BACK_CRISIS_OTHER_DECLINE = 141;
		constexpr int32_t CRISIS_BACK_DOWN_WE_ACCEPT = 142;
		constexpr int32_t CRISIS_BACK_DOWN_WE_DECLINE = 143;
		constexpr int32_t CRISIS_BACK_DOWN_THEY_ACCEPT = 144;
		constexpr int32_t CRISIS_BACK_DOWN_THEY_DECLINE = 145;
		constexpr int32_t CRISIS_BACK_DOWN_OTHER_ACCEPT = 146;
		constexpr int32_t CRISIS_BACK_DOWN_OTHER_DECLINE = 147;
		constexpr int32_t CRISIS_NO_SIDE_PICKED = 148;
		constexpr int32_t CRISIS_BECAME_WAR = 149;
		constexpr int32_t CRISIS_STARTED = 150;
		constexpr int32_t CRISIS_NOBODY_BACKED = 151;
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

	inline message_setting merge_message_setting(message_setting a, message_setting b) { return a > b ? a : b; }
	inline message_setting get_relevant_setting(world_state const& ws, message_settings s) { return message_setting::discard; }
	message_setting get_single_setting(world_state const& ws, message_settings s, nations::country_tag n);

	template<typename ... NATIONS>
	message_setting get_relevant_setting(world_state const& ws, message_settings s, nations::country_tag n, NATIONS ... rest);

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
	void submit_message(world_state& ws, std::variant<std::monostate, nations::country_tag, provinces::province_tag> goto_tag, display_function&& f);

	void cb_detected(world_state& ws, nations::country_tag by, nations::country_tag target, military::cb_type_tag type, float infamy_gained);
	void player_cb_construction_invalid(world_state& ws, nations::country_tag target, military::cb_type_tag type);
	void player_acquired_cb(world_state& ws, nations::country_tag target, military::cb_type_tag type);
	void player_technology(world_state& ws, technologies::tech_tag type);
}
