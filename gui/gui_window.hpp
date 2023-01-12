#pragma once
#include "gui.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

namespace ui {
	namespace detail {
		template<typename ...REST>
		struct window_get {
			template<typename T>
			static T apply(T&&) {
				static_assert(std::is_same_v<T, void>, "no matching element found for that name");
			}
		};
		template<typename i, typename INDEX, typename TYPE, typename ...REST>
		struct window_get<i, INDEX, TYPE, REST...> {
			static auto& apply(gui_window<INDEX, TYPE, REST ...>& w) {
				return window_get<i, REST...>::apply(w);
			}
			static auto& apply(const gui_window<INDEX, TYPE, REST ...>& w) {
				return window_get<i, REST...>::apply(w);
			}
		};
		template<typename INDEX, typename TYPE, typename ...REST>
		struct window_get<INDEX, INDEX, TYPE, REST...> {
			static TYPE& apply(gui_window<INDEX, TYPE, REST ...>& w) {
				return w.m_object;
			}
			static const TYPE& apply(const gui_window<INDEX, TYPE, REST ...>& w) {
				return w.m_object;
			}
		};

		template<typename RESULT, typename BASE, typename ... REST>
		struct can_create_dynamic_si : public std::false_type {
			template<typename WS, typename TGUO, typename ET>
			static bool run(BASE&, WS&, TGUO, ET, char const*, char const*) {
				return false;
			}
		};
		template<typename BASE, typename ... REST>
		struct can_create_dynamic_si<decltype(void(std::declval<BASE>().create_dynamic(std::declval<REST>()...))), BASE, REST...> : public std::true_type {
			template<typename WS, typename TGUO, typename ET>
			static bool run(BASE& base, WS& ws, TGUO tguo, ET et, char const* a, char const* b) {
				return base.create_dynamic(ws, tguo, et, a, b);
			}
		};

		template<typename BASE, typename ... REST>
		using can_create_dynamic_s = can_create_dynamic_si<void, BASE, REST ...>;

		template<typename BASE, typename ... REST>
		constexpr bool can_create_dynamic = can_create_dynamic_si<void, BASE, REST ...>::value;
	}
}

template<typename INDEX, typename TYPE, typename ... REST>
class ui::gui_window<INDEX, TYPE, REST ...> : public ui::gui_window<REST ...> {
protected:
	TYPE m_object;
	bool create_named_member(world_state& ws, tagged_gui_object win, ui::element_tag t, const char* ns, const char* ne);
	template<typename def_type>
	ui::tagged_gui_object create_window(world_state& ws, const def_type& def);
	template<typename window_type>
	void member_init_in_window(window_type& w, world_state& m);

	void set_window_id(ui::gui_object_tag t) { ui::gui_window<REST ...>::set_window_id(t); }
public:
	using base_behavior_t = typename ui::gui_window<REST ...>::base_behavior_t;

	template<typename window_type>
	void member_update_in_window(window_type& w, world_state& s);

	gui_window(ui::gui_window<INDEX, TYPE, REST ...>&&) = default;
	template<typename ...PARAMS>
	gui_window(PARAMS&& ... params) : gui_window<REST ...>(std::forward<PARAMS>(params) ...), m_object(std::forward<PARAMS>(params) ...) {}

	template<typename i>
	auto& get();
	template<typename i>
	auto& get() const;

	template<typename def_type>
	ui::tagged_gui_object create(world_state& manager, const def_type& def);
	virtual void update_data(gui_object_tag, world_state&) override;

	friend struct ui::detail::window_get<INDEX, INDEX, TYPE, REST ...>;
};

template<typename BASE_BEHAVIOR>
class ui::gui_window<BASE_BEHAVIOR> : public BASE_BEHAVIOR {
protected:
	bool create_named_member(world_state& ws, tagged_gui_object win, ui::element_tag t, const char* ns, const char* ne);
	ui::tagged_gui_object create_window(world_state& ws, const ui::window_def& def);
	ui::tagged_gui_object create_window(world_state& ws, const ui::icon_def& def);
	template<typename window_type>
	void member_init_in_window(window_type& w, world_state& m) {
		if constexpr(ui::detail::has_on_create<BASE_BEHAVIOR, world_state&>) {
			BASE_BEHAVIOR::on_create(m);
		} else if constexpr(ui::detail::has_on_create<BASE_BEHAVIOR, window_type&, world_state&>) {
			BASE_BEHAVIOR::on_create(w, m);
		}
	}
	template<typename window_type>
	void member_update_in_window(window_type& w, world_state& s) {
		if constexpr(ui::detail::has_update<BASE_BEHAVIOR, world_state&>)
			BASE_BEHAVIOR::update(s);
	}
public:
	using base_behavior_t = BASE_BEHAVIOR;

	gui_window(ui::gui_window<BASE_BEHAVIOR>&&) = default;
	template<typename ...PARAMS>
	gui_window(PARAMS&& ... params) : BASE_BEHAVIOR(std::forward<PARAMS>(params) ...) {}

	ui::gui_object_tag window_object;
	template<typename def_type>
	ui::tagged_gui_object create(world_state& manager, const def_type& def);
	virtual void update_data(gui_object_tag, world_state& ws) override {
		if constexpr(ui::detail::has_update<BASE_BEHAVIOR, world_state&>)
			BASE_BEHAVIOR::update(ws);
	}
protected:
	void set_window_id(ui::gui_object_tag t) { window_object = t; }
};

template<typename INDEX, typename TYPE, typename ...REST>
template<typename i>
auto& ui::gui_window<INDEX, TYPE, REST...>::get() { return detail::window_get<i, INDEX, TYPE, REST...>::apply(*this); }

template<typename INDEX, typename TYPE, typename ...REST>
template<typename i>
auto& ui::gui_window<INDEX, TYPE, REST...>::get() const { return detail::window_get<i, INDEX, TYPE, REST...>::apply(*this); }

template<typename INDEX, typename TYPE, typename ...REST>
template<typename def_type>
ui::tagged_gui_object ui::gui_window<INDEX, TYPE, REST...>::create_window(world_state& ws, const def_type & def) {
	return gui_window<REST...>::create_window(ws, def);
}

template<typename INDEX, typename TYPE, typename ...REST>
void ui::gui_window<INDEX, TYPE, REST...>::update_data(gui_object_tag, world_state& w) {
	member_update_in_window(*this, w);
}

template<typename INDEX, typename TYPE, typename ...REST>
template<typename window_type>
void ui::gui_window<INDEX, TYPE, REST...>::member_update_in_window(window_type& w, world_state& s) {
	gui_window<REST...>::member_update_in_window(w, s);
	if constexpr(ui::detail::has_windowed_update<TYPE, window_type&, world_state&>)
		m_object.windowed_update(w, s);
}

template<typename INDEX, typename TYPE, typename ...REST>
template<typename window_type>
void ui::gui_window<INDEX, TYPE, REST...>::member_init_in_window(window_type& w, world_state& m) {
	if constexpr(ui::detail::has_initialize_in_window<TYPE, window_type&, world_state&>)
		m_object.initialize_in_window(w, m);
	gui_window<REST...>::member_init_in_window(w, m);
}

namespace ui {
	namespace detail {
		template<typename MOBJECT>
		struct visitor_helper {
			MOBJECT& object;
			tagged_gui_object win;
			world_state& ws;

#ifdef _DEBUG
			const char* ns;
			const char* ne;
#endif

#ifdef _DEBUG
			visitor_helper(MOBJECT& o, tagged_gui_object w, world_state& s, const char* a, const char* b) : object(o), win(w), ws(s), ns(a), ne(b) {}
#else
			visitor_helper(MOBJECT& o, tagged_gui_object w, world_state& s) : object(o), win(w), ws(s) {}
#endif

			template<typename TAG>
			void operator()(TAG tag) {
				if constexpr(ui::detail::can_create<TAG, MOBJECT> == true)
					create_static_element(ws, tag, win, object);
				else {
#ifdef _DEBUG
					OutputDebugStringA("Unable to instantiate window element: bad tag type\n");
					std::string element_name(ns, ne);
					OutputDebugStringA(element_name.c_str());
					OutputDebugStringA("\n");
					OutputDebugStringA(typeid(TAG).name());
					OutputDebugStringA("\n");
					OutputDebugStringA(typeid(MOBJECT).name());
					OutputDebugStringA("\n");
					if(ui::detail::can_create<TAG, MOBJECT>)
						OutputDebugStringA("can create true\n");
					else
						OutputDebugStringA("can create false\n");
					std::abort();
#endif
				}
			}
		};
	}
}

template<typename INDEX, typename TYPE, typename ...REST>
bool ui::gui_window<INDEX, TYPE, REST...>::create_named_member(world_state& ws, tagged_gui_object win, ui::element_tag t, const char* ns, const char* ne) {
	if (compile_time_str_compare_ci<INDEX>(ns, ne) == 0) {
#ifdef _DEBUG
		ui::detail::visitor_helper<TYPE> vhelper(m_object, win, ws, ns, ne);
#else
		ui::detail::visitor_helper<TYPE> vhelper(m_object, win, ws);
#endif
		std::visit(vhelper, t);
		return true;
	} else {
		return gui_window<REST...>::create_named_member(ws, win, t, ns, ne);
	}
}

template<typename INDEX, typename TYPE, typename ...REST>
template<typename def_type>
ui::tagged_gui_object ui::gui_window<INDEX, TYPE, REST...>::create(world_state& manager, const def_type& definition) {
	const auto win = create_window(manager, definition);

	if constexpr(std::is_same_v<def_type, ui::window_def>) {
		for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			auto rn = manager.s.gui_m.nmaps.get_raw_name(*i);
			const char* rn_s = rn.get_str(manager.s.gui_m.ui_definitions.name_data);
			const char* rn_e = rn_s + rn.length();

			if(!create_named_member(manager, win, *i, rn_s, rn_e)) {
				if constexpr(ui::detail::can_create_dynamic<typename gui_window<INDEX, TYPE, REST...>::base_behavior_t, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					if(!gui_window<INDEX, TYPE, REST...>::base_behavior_t::create_dynamic(manager, win, *i, rn_s, rn_e)) {
						std::visit([&manager, &win](auto tag) {
							if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								ui::create_dynamic_element(manager, tag, win);
						}, *i);
					}
				} else {
					std::visit([&manager, &win](auto tag) {
						if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							ui::create_dynamic_element(manager, tag, win);
					}, *i);
				}
			}
		}
		set_window_id(win.id);
		member_init_in_window(*this, manager);
	} else {
		set_window_id(win.id);
	}

	return win;
}

template<typename BASE_BEHAVIOR>
bool ui::gui_window<BASE_BEHAVIOR>::create_named_member(world_state& ws, tagged_gui_object win, ui::element_tag t, const char* ns, const char* ne) {
	return false;
}

template<typename BASE_BEHAVIOR>
ui::tagged_gui_object ui::gui_window<BASE_BEHAVIOR>::create_window(world_state& ws, const ui::window_def& definition) {
	const auto window = ws.w.gui_m.gui_objects.emplace();

	window.object.align = alignment_from_definition(definition);

	if (is_valid_index(definition.background_handle)) {
		const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
		ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
	} else {
		window.object.type_dependant_handle.store(0, std::memory_order_release);
	}

	window.object.associated_behavior = this;
	BASE_BEHAVIOR::associated_object = &window.object;

	window.object.size = definition.size;
	window.object.position = definition.position;

	return window;
}

template<typename BASE_BEHAVIOR>
ui::tagged_gui_object ui::gui_window<BASE_BEHAVIOR>::create_window(world_state& ws, const ui::icon_def& icon_def) {
	auto new_gobj = ws.w.gui_m.gui_objects.emplace();

	const uint16_t rotation =
		(icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
		ui::gui_object::rotation_upright :
		((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);

	new_gobj.object.position = icon_def.position;
	new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
	new_gobj.object.align = alignment_from_definition(icon_def);

	ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);

	if(rotation == ui::gui_object::rotation_right) {
		new_gobj.object.position = ui::xy_pair{
			int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
			int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
		new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
	}

	new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
	new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);

	new_gobj.object.associated_behavior = this;
	BASE_BEHAVIOR::associated_object = &new_gobj.object;

	return new_gobj;
}

template<typename BASE_BEHAVIOR>
template<typename def_type>
ui::tagged_gui_object ui::gui_window<BASE_BEHAVIOR>::create(world_state& ws, const def_type& definition) {
	const auto win = create_window(ws, definition);

	if constexpr(std::is_same_v<def_type, ui::window_def>) {
		for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			if constexpr(ui::detail::can_create_dynamic<BASE_BEHAVIOR, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
				auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
				const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
				const char* rn_e = rn_s + rn.length();

				if(!BASE_BEHAVIOR::create_dynamic(ws, win, *i, rn_s, rn_e)) {
					std::visit([&ws, &win](auto tag) {
						if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							ui::create_dynamic_element(ws, tag, win);
					}, *i);
				}
			} else {
				std::visit([&ws, &win](auto tag) {
					if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
						ui::create_dynamic_element(ws, tag, win);
				}, *i);
			}
		}
	}
	window_object = win.id;

	if constexpr(ui::detail::has_on_create<BASE_BEHAVIOR, world_state&>) {
		BASE_BEHAVIOR::on_create(ws);
	} else if constexpr(ui::detail::has_on_create<BASE_BEHAVIOR, ui::gui_window<BASE_BEHAVIOR>&, world_state&>) {
		BASE_BEHAVIOR::on_create(*this, ws);
	}

	return win;
}

template<typename ... REST>
ui::tagged_gui_object ui::create_static_element(world_state& ws, window_tag handle, tagged_gui_object parent, gui_window<REST...>& b) {
	const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	const auto res = b.create(ws, window_definition);
	ui::add_to_back(ws.w.gui_m, parent, res);
	ws.w.gui_m.flag_minimal_update();
	return res;
}

template<typename ... REST>
ui::tagged_gui_object ui::create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, gui_window<REST...>& b) {
	const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	const auto res = b.create(ws, window_definition);
	ui::add_to_back(ws.w.gui_m, parent, res);
	ws.w.gui_m.flag_minimal_update();
	return res;
}

#include "gui.hpp"
