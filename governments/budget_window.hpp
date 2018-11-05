#pragma once
#include "common\\common.h"
#include "budget_window.h"
#include "gui\\gui.hpp"
#include "governments_functions.h"
#include "economy\\economy_functions.h"
#include "world_state\\world_state.h"

namespace governments {
	class budget_window_base : public ui::draggable_region {
	public:
		template<typename window_type>
		void on_create(window_type& win, world_state& ws);
	};

	class bw_close_button {
	public:
		void button_function(ui::simple_button<bw_close_button>&, world_state& ws);
	};

	class tarrif_percent {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class poor_tax_collected {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class middle_tax_collected {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class rich_tax_collected {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gold_income {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class total_income {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class total_funds {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class debt_total {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class debt_label {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class fixed_incomes_label {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class variable_expenses_label {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class fixed_incomes_funds {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class variable_expenses_funds {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class hidden_button {
	public:
		void on_create(ui::simple_button<hidden_button>& b, world_state& ws);
	};
	class hidden_text {
	public:
		void on_create(ui::fixed_text<hidden_text>& b, world_state& ws);
	};
	class stockpile_cost {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class industrial_subsidies_cost {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class education_pay {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class administrative_pay {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class military_pay {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class social_spending_cost {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class national_administrative_efficiency {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class expenses_total {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class debt_and_saving_label {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class debt_and_saving_amount {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class tarrif_amount {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class budget_total_amount {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};


	class budget_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<bw_close_button>,
		CT_STRING("tariffs_percent"), ui::display_text<tarrif_percent>,
		CT_STRING("diplomatic_desc"), ui::display_text<debt_and_saving_label, -10>,
		CT_STRING("tax_0_inc"), ui::display_text<poor_tax_collected>,
		CT_STRING("tax_1_inc"), ui::display_text<middle_tax_collected>,
		CT_STRING("tax_2_inc"), ui::display_text<rich_tax_collected>,
		CT_STRING("gold_inc"), ui::display_text<gold_income>,
		CT_STRING("total_inc"), ui::display_text<total_income>,
		CT_STRING("total_funds_val"), ui::display_text<total_funds, -12>,
		CT_STRING("national_bank_desc"), ui::display_text<debt_label, -12>,
		CT_STRING("national_bank_val"), ui::display_text<debt_total, -12>,
		CT_STRING("debt_desc"), ui::display_text<fixed_incomes_label, -10>,
		CT_STRING("interest_desc"), ui::display_text<variable_expenses_label, -10>,
		CT_STRING("debt_val"), ui::display_text<fixed_incomes_funds, -10>,
		CT_STRING("interest_val"), ui::display_text<variable_expenses_funds, -10>,
		CT_STRING("tab_takenloans"), ui::simple_button<hidden_button>,
		CT_STRING("tab_givenloans"), ui::simple_button<hidden_button>,
		CT_STRING("takenloans_text"), ui::fixed_text<hidden_text>,
		CT_STRING("givenloans_text"), ui::fixed_text<hidden_text>,
		CT_STRING("debt_sort_country"), ui::simple_button<hidden_button>,
		CT_STRING("debt_sort_amount"), ui::simple_button<hidden_button>,
		CT_STRING("nat_stock_est"), ui::display_text<stockpile_cost>,
		CT_STRING("nat_stock_val"), ui::fixed_text<hidden_text>,
		CT_STRING("mil_cost_desc"), ui::fixed_text<hidden_text>,
		CT_STRING("mil_cost_val"), ui::fixed_text<hidden_text>,
		CT_STRING("overseas_desc"), ui::fixed_text<hidden_text>,
		CT_STRING("overseas_cost_val"), ui::fixed_text<hidden_text>,
		CT_STRING("ind_sub_val"), ui::display_text<industrial_subsidies_cost>,
		CT_STRING("admin_efficiency"), ui::display_text<national_administrative_efficiency>,
		CT_STRING("exp_val_0"), ui::display_text<education_pay>,
		CT_STRING("exp_val_1"), ui::display_text<administrative_pay>,
		CT_STRING("exp_val_2"), ui::display_text<social_spending_cost>,
		CT_STRING("exp_val_3"), ui::display_text<military_pay>,
		CT_STRING("total_exp"), ui::display_text<expenses_total>,
		CT_STRING("tariff_val"), ui::display_text<tarrif_amount>,
		CT_STRING("diplomatic_balance"), ui::display_text<debt_and_saving_amount>,
		CT_STRING("balance"), ui::display_text<budget_total_amount>,
		budget_window_base
	> {};

	template<typename window_type>
	void budget_window_base::on_create(window_type & win, world_state & ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
	}
	template<typename W>
	void tarrif_percent::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::percent, float(player->tarrifs) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void poor_tax_collected::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.local_player_data.collected_poor_tax * float(player->poor_tax) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void middle_tax_collected::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.local_player_data.collected_middle_tax * float(player->middle_tax) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void rich_tax_collected::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.local_player_data.collected_rich_tax * float(player->rich_tax) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void gold_income::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename W>
	void total_income::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				auto tax_income = ws.w.local_player_data.collected_poor_tax * float(player->poor_tax) / 100.0f +
					ws.w.local_player_data.collected_middle_tax * float(player->middle_tax) / 100.0f +
					ws.w.local_player_data.collected_rich_tax * float(player->rich_tax) / 100.0f;
				economy::money_qnty_type tarrif_income = 0;
				for(uint32_t i = 1; i < ws.s.economy_m.goods_count; ++i) {
					tarrif_income += ws.w.nation_s.collected_tarrifs.get(pid, economy::goods_tag(economy::goods_tag::value_base_t(i)));
				}

				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, tax_income + tarrif_income);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	template<typename W>
	void total_funds::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, ws.w.nation_s.national_stockpiles.get(pid, economy::money_good));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	template<typename W>
	void debt_total::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, player->national_debt);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void debt_label::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::total_debt], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	template<typename W>
	void fixed_incomes_label::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::fixed_income], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	template<typename W>
	void variable_expenses_label::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::variable_expenses], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	template<typename W>
	void fixed_incomes_funds::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	
	template<typename W>
	void variable_expenses_funds::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	
	template<typename W>
	void stockpile_cost::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void industrial_subsidies_cost::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void education_pay::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void administrative_pay::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void military_pay::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void social_spending_cost::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void national_administrative_efficiency::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void expenses_total::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	
	template<typename W>
	void debt_and_saving_label::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::borrowing_and_saving], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	template<typename W>
	void debt_and_saving_amount::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename W>
	void tarrif_amount::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				economy::money_qnty_type tarrif_income = 0;
				for(uint32_t i = 1; i < ws.s.economy_m.goods_count; ++i) {
					tarrif_income += ws.w.nation_s.collected_tarrifs.get(pid, economy::goods_tag(economy::goods_tag::value_base_t(i)));
				}

				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, tarrif_income);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	template<typename W>
	void budget_total_amount::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
}
