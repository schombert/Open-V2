#include "common\\common.h"
#include "budget_window.hpp"

namespace governments {
	budget_window::budget_window() : win(std::make_unique<budget_window_t>()) {}
	budget_window::~budget_window() {}
	void budget_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void budget_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_budget"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void budget_window::update(ui::gui_manager & gui_m) {
		if((win->associated_object->flags.load(std::memory_order_acquire) & (ui::gui_object::visible | ui::gui_object::visible_after_update)) != 0)
			ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void budget_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void bw_close_button::button_function(ui::simple_button<bw_close_button>&, world_state & ws) {
		ws.w.budget_w.hide(ws.w.gui_m);
	}
	void hidden_button::on_create(ui::simple_button<hidden_button>& b, world_state & ws) {
		b.set_visibility(ws.w.gui_m, false);
	}
	void hidden_text::on_create(ui::fixed_text<hidden_text>& b, world_state &) {
		ui::hide(*b.associated_object);
	}
	void poor_tax_scrollbar::on_position(int32_t pos) {}
	void poor_tax_scrollbar::update(ui::scrollbar<poor_tax_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tax]), player->modifier_values[modifiers::national_offsets::max_tax] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tax]) : 100);
			sb.update_position(player->poor_tax);
		}
	}
	void middle_tax_scrollbar::on_position(int32_t pos) {}
	void middle_tax_scrollbar::update(ui::scrollbar<middle_tax_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tax]), player->modifier_values[modifiers::national_offsets::max_tax] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tax]) : 100);
			sb.update_position(player->middle_tax);
		}
	}
	void rich_tax_scrollbar::on_position(int32_t pos) {}
	void rich_tax_scrollbar::update(ui::scrollbar<rich_tax_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tax]), player->modifier_values[modifiers::national_offsets::max_tax] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tax]) : 100);
			sb.update_position(player->rich_tax);
		}
	}
	void land_spending_scrollbar::on_position(int32_t pos) {}
	void land_spending_scrollbar::update(ui::scrollbar<land_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->army_stockpile_spending);
		}
	}
	void naval_spending_scrollbar::on_position(int32_t pos) {}
	void naval_spending_scrollbar::update(ui::scrollbar<naval_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->navy_stockpile_spending);
		}
	}
	void projects_spending_scrollbar::on_position(int32_t pos) {}
	void projects_spending_scrollbar::update(ui::scrollbar<projects_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->projects_stockpile_spending);
		}
	}
	void administrative_pay_scrollbar::on_position(int32_t pos) {}
	void administrative_pay_scrollbar::update(ui::scrollbar<administrative_pay_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->administrative_spending);
		}
	}
	void education_pay_scrollbar::on_position(int32_t pos) {}
	void education_pay_scrollbar::update(ui::scrollbar<education_pay_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->education_spending);
		}
	}
	void military_pay_scrollbar::on_position(int32_t pos) {}
	void military_pay_scrollbar::update(ui::scrollbar<military_pay_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_military_spending]), player->modifier_values[modifiers::national_offsets::max_military_spending] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_military_spending]) : 100);
			sb.update_position(player->military_spending);
		}
	}
	void social_spending_scrollbar::on_position(int32_t pos) {}
	void social_spending_scrollbar::update(ui::scrollbar<social_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_social_spending]), player->modifier_values[modifiers::national_offsets::max_social_spending] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_social_spending]) : 100);
			sb.update_position(player->social_spending);
		}
	}
	void tarrif_setting_scrollbar::on_position(int32_t pos) {}
	void tarrif_setting_scrollbar::update(ui::scrollbar<tarrif_setting_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_range(ws.w.gui_m, -100, 100);
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tariff]), player->modifier_values[modifiers::national_offsets::max_tariff] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tariff]) : 100);
			sb.update_position(player->tarrifs);
		}
	}
}
