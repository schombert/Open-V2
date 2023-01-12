#pragma once
#include "common\\common.h"
#include "provinces\\provinces.h"
#include "cultures\\cultures.h"
#include "nations\\nations.h"
#include "scenario\\scenario.h"
#include "economy\\economy.h"
#include "population\\population.h"
#include "variables\\variables.h"
#include "military\\military_containers.h"
#include "gui\\gui.h"
#include "ideologies\\ideologies.h"
#include "nations\\nations_gui.h"
#include "provinces\\province_gui.h"
#include "technologies\\technologies_gui.h"
#include "governments\\governments_gui.h"
#include "population\\population_gui.h"
#include "topbar.h"
#include "economy\\economy_gui.h"
#include "economy\\trade_window.h"
#include "governments\\budget_window.h"
#include "commands\\commands.hpp"
#include "messages.h"
#include "military\\military_gui.h"
#include "events\\events.h"
#include "events\\events_gui.h"
#include "bottombar.h"
#include "graphics\\world_map.h"
#include "modifiers\modifiers_gui.h"
#include "find.h"
#include "menu.h"
#include "graphics/map_modes.h"
#include "nations\nations_containers.h"
#include "provinces\province_containers.h"
#include "population\population_containers.h"

#undef small

class world_state;

namespace current_state {
	enum class crisis_type : uint8_t {
		none,
		liberation,
		claim,
		colonial,
		influence
	};

	struct player_net_income_history {
		float values[32] = { 0.0 };
	};

	constexpr int32_t max_speed = 5;

	struct crisis_state {
		float temperature = 0.0f; // from 0 to 100
		crisis_type type = crisis_type::none;
		nations::country_tag primary_attacker;
		nations::country_tag primary_defender;
		nations::country_tag target;
		nations::country_tag on_behalf_of;
		set_tag<nations::country_tag> attackers;
		set_tag<nations::country_tag> defenders;
		set_tag<nations::country_tag> interested;
		array_tag<military::war_goal, int32_t, false> goals;
		nations::state_tag state;
	};

	struct player_cb_state;

	class state {
	public:
		graphics::map_display map;

		provinces::provinces_state province_s;
		cultures::cultures_state culture_s;
		military::military_state military_s;
		nations::nations_state nation_s;
		economy::economic_state economy_s;
		population::population_state population_s;
		variables::variables_state variable_s;
		ideologies::ideologies_state ideology_s;
		technologies::technologies_state technology_s;
		events::event_state event_s;
		ui::gui_manager gui_m;

		//crisis state
		crisis_state current_crisis;
		//other global state
		date_tag current_date;
		bool great_wars_enabled = false;
		bool world_wars_enabled = false;

		//game speed state
		std::atomic<int32_t> speed = 3;
		std::atomic<bool> paused = true;
		std::atomic<bool> force_paused = false;
		std::atomic<bool> single_step_pending = false;
		std::atomic<bool> end_game = false;

		commands::full_command_set pending_commands;

		//gui state
		map_mode::state map_view;

		nations::diplomacy_window diplomacy_w;
		provinces::province_window province_w;
		technologies::technology_window technologies_w;
		governments::government_window government_w;
		governments::budget_window budget_w;
		population::population_window population_w;
		economy::production_window production_w;
		economy::trade_window trade_w;
		messages::message_window message_w;
		messages::message_settings_window message_settings_w;
		military::fabricate_cb_window fabricate_cb_w;
		events::province_event_window province_event_w;
		events::nation_event_window nation_event_w;
		events::major_event_window major_event_w;
		find_dialog::find_window find_w;
		menu::menu_window menu_w;
		economy::build_factory_window build_factory_w;
		modifiers::national_focus_window national_focus_w;

		topbar topbar_w;
		bottombar bottombar_w;

		//player data
		struct {
			player_net_income_history income_history;
			economy::money_qnty_type collected_poor_tax = 0;
			economy::money_qnty_type collected_middle_tax = 0;
			economy::money_qnty_type collected_rich_tax = 0;
			tagged_vector<array_tag<economy::money_qnty_type, nations::country_tag, true>, economy::goods_tag> imports_by_country;
			tagged_vector<bitfield_type, events::decision_tag> suppressed_decisions;
			tagged_vector<int8_t, events::event_tag> saved_event_choices;
			tagged_vector<tagged_vector<bitfield_type, nations::country_tag>, military::cb_type_tag> triggered_cb_state;
			std::atomic<int32_t> player_chosen_option = -1;
			std::mutex player_choice_guard;
			std::condition_variable player_choice_condition;
		} local_player_data;
		nations::country_tag local_player_nation;

		state();
		~state();
		void init_gui_objects(world_state& s);

		void toggle_pause();
		void increase_speed();
		void decrease_speed();

		
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;

	GET_SET_STV(w.culture_s.national_tags_state)
	GET_SET_TV(::nation_tag::holder, w.culture_s.tags_to_holders)
	GET_SET_TFV(::nation_tag::flags, w.culture_s.country_flags_by_government)
	ARRAY_BACKING_BASE(w.culture_s.culture_arrays)
	GET_SET_TV(current_state::player_cb_state, w.local_player_data.triggered_cb_state)
	GET_SET_GEN(w.province_s.province_state_container)
	GET_SET_TFV(province::demographics, w.province_s.province_demographics)
	ARRAY_BACKING_BASE(w.province_s.core_arrays)
	ARRAY_BACKING_BASE(w.province_s.static_modifier_arrays)
	ARRAY_BACKING_BASE(w.province_s.timed_modifier_arrays)
	ARRAY_BACKING_BASE(w.province_s.province_arrays)
	GET_SET_GEN(w.nation_s.nations)
	GET_SET_GEN(w.nation_s.states)
	GET_SET_TDV(nation::demographics, w.nation_s.nation_demographics)
	GET_SET_TDV(state::demographics, w.nation_s.state_demographics)
	GET_SET_TDV(nation::colonial_demographics, w.nation_s.nation_colonial_demographics)
	GET_SET_MAPPED(nation::nation_modifiers_map, w.nation_s.modifier_values)
	ARRAY_BACKING_BASE(w.nation_s.static_modifier_arrays)
	ARRAY_BACKING_BASE(w.nation_s.timed_modifier_arrays)
	ARRAY_BACKING_BASE(w.nation_s.state_arrays)
	ARRAY_BACKING_BASE(w.nation_s.influence_arrays)
	ARRAY_BACKING_BASE(w.nation_s.nations_arrays)
	ARRAY_BACKING_BASE(w.nation_s.state_tag_arrays)
	ARRAY_BACKING_BASE(w.nation_s.relations_arrays)
	ARRAY_BACKING_BASE(w.nation_s.truce_arrays)
	GET_SET_GEN(w.military_s.armies)
	GET_SET_GEN(w.military_s.wars)
	GET_SET_GEN(w.military_s.fleets)
	GET_SET_GEN(w.military_s.army_orders)
	GET_SET_GEN(w.military_s.strategic_hqs)
	GET_SET_GEN(w.military_s.leaders)
	GET_SET_GEN(w.military_s.borders)
	ARRAY_BACKING_BASE(w.military_s.hq_commitment_arrays)
	ARRAY_BACKING_BASE(w.military_s.fleet_presence_arrays)
	ARRAY_BACKING_BASE(w.military_s.naval_control_arrays)
	ARRAY_BACKING_BASE(w.military_s.cb_arrays)
	ARRAY_BACKING_BASE(w.military_s.war_goal_arrays)
	ARRAY_BACKING_BASE(w.military_s.war_arrays)
	ARRAY_BACKING_BASE(w.military_s.fleet_arrays)
	ARRAY_BACKING_BASE(w.military_s.orders_arrays)
	ARRAY_BACKING_BASE(w.military_s.army_arrays)
	ARRAY_BACKING_BASE(w.military_s.leader_arrays)
	ARRAY_BACKING_BASE(w.military_s.hq_arrays)
	ARRAY_BACKING_BASE(w.population_s.pop_arrays)
	GET_SET_GEN(w.population_s.pops)

	GET_SET_STV(s.culture_m.culture_groups)
	GET_SET_STV(s.culture_m.religions)
	GET_SET_STV(s.culture_m.culture_container)
	GET_SET_STV(s.culture_m.national_tags)
	GET_SET_TV(::culture::union_tag, s.culture_m.cultures_to_tags)
	GET_SET_TV(::culture::group_direct, s.culture_m.cultures_to_groups)
	GET_SET_TV(::culture_group::union_tag_direct, s.culture_m.groups_to_tags)
	GET_SET_TV(::nation_tag::culture_group, s.culture_m.tags_to_groups)
	GET_SET_TFV(::nation_tag::government_names, s.culture_m.country_names_by_government)
	GET_SET_TFV_VV(::culture::first_names, s.culture_m.first_names_by_culture)
	GET_SET_TFV_VV(::culture::last_names, s.culture_m.last_names_by_culture)
	GET_SET_TFV_VV(::culture_group::cultures, s.culture_m.culture_by_culture_group)
	GET_SET_TV(::cultures::leader_pictures, s.culture_m.leader_pictures)
	GET_SET_GEN(s.province_m.province_container)
	GET_SET_TV(state_region::name, s.province_m.state_names)
	GET_SET_TFV_VV(state_region::provinces, s.province_m.states_to_province_index)
	GET_SET_TFV_VV(province::same_type_adjacency, s.province_m.same_type_adjacency)
	GET_SET_TFV_VV(province::coastal_adjacency, s.province_m.coastal_adjacency)
	GET_SET_STV(s.military_m.cb_types)
	GET_SET_TV(military::leader_trait_name, s.military_m.leader_traits)
	GET_SET_TV(::military::personality_traits, s.military_m.personality_traits)
	GET_SET_TV(::military::background_traits, s.military_m.background_traits)
	GET_SET_TFV(military::leader_trait_values, s.military_m.leader_trait_definitions)
	GET_SET_TV(::cb_type::construction_speed_direct, s.military_m.cb_type_to_speed)

	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto add_item(index_tag& i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type>> {
		::add_item(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto add_item(index_tag& i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type>> {
		::add_item(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto find(index_tag i, value_type v) const -> std::enable_if_t<std::is_trivially_copyable_v<value_type>, decltype(::find(array_backing<individuator_of<index_tag>>(), i, v))> {
		return ::find(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto find(index_tag i, value_type const& v) const -> std::enable_if_t<!std::is_trivially_copyable_v<value_type>, decltype(::find(array_backing<individuator_of<index_tag>>(), i, v))> {
		return ::find(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto remove_item(index_tag& i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type>> {
		::remove_item(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto remove_item(index_tag& i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type>> {
		::remove_item(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto contains_item(index_tag i, value_type v) const -> std::enable_if_t<std::is_trivially_copyable_v<value_type>, bool> {
		return ::contains_item(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag, typename value_type>
	RELEASE_INLINE auto contains_item(index_tag i, value_type const& v) const -> std::enable_if_t<!std::is_trivially_copyable_v<value_type>, bool> {
		return ::contains_item(array_backing<individuator_of<index_tag>>(), i, v);
	}
	template<typename index_tag>
	RELEASE_INLINE void resize(index_tag& i, uint32_t sz) {
		::resize(array_backing<individuator_of<index_tag>>(), i, sz);
	}
	template<typename index_tag>
	RELEASE_INLINE auto get_range(index_tag i) const -> decltype(::get_range(array_backing<individuator_of<index_tag>>(), i)) {
		return ::get_range(array_backing<individuator_of<index_tag>>(), i);
	}
	template<typename index_tag>
	RELEASE_INLINE auto get_size(index_tag i) const -> decltype(::get_size(array_backing<individuator_of<index_tag>>(), i)) {
		return ::get_size(array_backing<individuator_of<index_tag>>(), i);
	}
	template<typename index_tag, typename FN>
	RELEASE_INLINE void remove_item_if(index_tag& i, const FN& f) {
		::remove_item_if(array_backing<individuator_of<index_tag>>(), i, f);
	}
	template<typename index_tag>
	RELEASE_INLINE void clear(index_tag& i) {
		::clear(array_backing<individuator_of<index_tag>>(), i);
	}

	template<typename tag_type, typename F>
	std::enable_if_t<std::is_same_v<tag_type, military::cb_type_tag>> for_each(F const& f) const {
		int32_t const cmax = int32_t(s.military_m.cb_types.size());
		for(int32_t i = 0; i < cmax; ++i) {
			f(military::cb_type_tag(military::cb_type_tag::value_base_t(i)));
		}
	}
	template<typename tag_type, typename F, typename partitioner_t = concurrency::auto_partitioner>
	std::enable_if_t<std::is_same_v<tag_type, military::cb_type_tag>> par_for_each(F const& f, partitioner_t&& p = concurrency::auto_partitioner()) const {
		int32_t const cmax = int32_t(s.military_m.cb_types.size());
		concurrency::parallel_for(0, cmax, [&f](int32_t i) {
			f(military::cb_type_tag(military::cb_type_tag::value_base_t(i)));
		}, p);
	}
	template<typename tag_type, typename F>
	std::enable_if_t<std::is_same_v<tag_type, military::cb_type_tag>, bool> any_of(F const& f) const {
		int32_t const cmax = int32_t(s.military_m.cb_types.size());
		for(int32_t i = 0; i < cmax; ++i) {
			if(f(military::cb_type_tag(military::cb_type_tag::value_base_t(i))))
				return true;
		}
		return false;
	}

	template<typename tag_type, typename F>
	RELEASE_INLINE std::enable_if_t<std::is_same_v<tag_type, nations::country_tag>> for_each(F const& f) const {
		w.nation_s.nations.for_each(f);
	}
	template<typename tag_type, typename F, typename partitioner_t = concurrency::auto_partitioner>
	RELEASE_INLINE std::enable_if_t<std::is_same_v<tag_type, nations::country_tag>> par_for_each(F const& f, partitioner_t&& p = concurrency::auto_partitioner()) const {
		w.nation_s.nations.parallel_for_each(f, p);
	}

	text_data::text_tag get_text_handle(const char* key_start, const char* key_end) {
		return text_data::get_text_handle(s.gui_m.text_data_sequences, key_start, key_end);
	}
	text_data::text_tag get_thread_safe_text_handle(const char* key_start, const char* key_end) {
		return text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, key_start, key_end);
	}
	text_data::text_tag get_existing_text_handle(const char* key_start, const char* key_end) const {
		return text_data::get_existing_text_handle(s.gui_m.text_data_sequences, key_start, key_end);
	}
	text_data::text_tag get_thread_safe_existing_text_handle(const char* key_start, const char* key_end) const {
		return text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, key_start, key_end);
	}

	template<size_t N>
	text_data::text_tag get_thread_safe_existing_text_handle(const char(&t)[N]) const {
		return get_thread_safe_existing_text_handle(t, t + N - 1);
	}
	template<size_t N>
	text_data::text_tag get_existing_text_handle(const char(&t)[N]) const {
		return get_existing_text_handle(t, t + N - 1);
	}
	template<size_t N>
	text_data::text_tag get_thread_safe_text_handle(const char(&t)[N]) {
		return get_thread_safe_text_handle(t, t + N - 1);
	}
	template<size_t N>
	text_data::text_tag get_text_handle(const char(&t)[N]) {
		return get_text_handle(t, t + N - 1);
	}

	template<typename index_t>
	std::enable_if_t<std::is_same_v<index_t, cultures::national_tag>, boost::container::flat_map<uint32_t, cultures::national_tag>&> name_map() {
		return s.culture_m.national_tags_index;
	}
	template<typename index_t>
	std::enable_if_t<std::is_same_v<index_t, cultures::national_tag>, boost::container::flat_map<uint32_t, cultures::national_tag> const&> name_map() const {
		return s.culture_m.national_tags_index;
	}
	template<typename index_t>
	std::enable_if_t<std::is_same_v<index_t, military::leader_trait_tag>, name_map_t<military::leader_trait_tag>&> name_map() {
		return s.military_m.named_leader_trait_index;
	}
	template<typename index_t>
	std::enable_if_t<std::is_same_v<index_t, military::leader_trait_tag>, name_map_t<military::leader_trait_tag> const&> name_map() const {
		return s.military_m.named_leader_trait_index;
	}
};

void world_state_non_ai_update(world_state & ws);
void world_state_update_loop(world_state& ws);
void apply_new_settings(world_state& ws);

//called after loading a scenario & before loading a specific world state
void ready_world_state(world_state& ws);
