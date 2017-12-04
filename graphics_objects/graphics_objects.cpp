#include "graphics_objects.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

MEMBER_DEF(graphics::xy_pair, x, "x");
MEMBER_DEF(graphics::xy_pair, y, "y");

struct discard_type {
	void any(int) {}
};

int discard_to_int(const token_and_type&, association_type, const discard_type&) {
	return 0;
}
int discard_from_full(const token_and_type&, association_type, const token_and_type&) {
	return 0;
}

token_and_type token_from_rh(association_type, const token_and_type& t) {
	return t;
}

MEMBER_FDEF(discard_type, any, "any");

struct parsing_environment {
	graphics::name_maps& nmaps;
	graphics::object_definitions& defs;
	std::vector<std::pair<std::string, graphics::errors>>& errors_generated;
	const texture_lookup th_f;
	std::string file;

	parsing_environment(
		graphics::name_maps& a,
		graphics::object_definitions& b,
		std::vector<std::pair<std::string, graphics::errors>>& c,
		const texture_lookup& d) :
		nmaps(a), defs(b), errors_generated(c), th_f(d) {
	}
};

struct parsing_object {
	graphics::object internal_definition;
	const parsing_environment& env;

	parsing_object(const parsing_environment& e) : env(e) {};

	std::string name;
	token_and_type primary_texture;
	token_and_type secondary_texture;

	bool horizontal = true;

	void setup_textures(graphics::object& dest) const {
		if (primary_texture.start != primary_texture.end)
			dest.primary_texture_handle = env.th_f(primary_texture.start, primary_texture.end);
		if (secondary_texture.start != secondary_texture.end)
			dest.type_dependant = env.th_f(secondary_texture.start, secondary_texture.end);
	}

	void clicksound(const token_and_type& t) {
		if (is_fixed_token_ci(t, "click")) {
			internal_definition.flags |= graphics::object::has_click_sound;
		} else {
			env.errors_generated.emplace_back(env.file, graphics::errors::invalid_click_sound);
		}
	}
	void noOfFrames(int n) {
		if ((n > 0) & (n < 256)) {
			internal_definition.number_of_frames = (uint8_t)n;
		} else {
			env.errors_generated.emplace_back(env.file, graphics::errors::invalid_number_of_frames);
		}
	}
	void flipv(bool f) {
		if (f)
			internal_definition.flags |= graphics::object::flip_v;
	}
	void alwaystransparent(bool f) {
		if (f)
			internal_definition.flags |= graphics::object::always_transparent;
	}
	void transparencecheck(bool f) {
		if (f)
			internal_definition.flags |= graphics::object::do_transparency_check;
	}
	void borderSize(const graphics::xy_pair& sz) {
		if ((sz.x != sz.y) | sz.x <= 0) {
			env.errors_generated.emplace_back(env.file, graphics::errors::invalid_number_of_frames);
		} else {
			internal_definition.type_dependant = (uint16_t)sz.x;
		}
	}
	void single_size(int n) {
		internal_definition.size.x = n;
		internal_definition.size.y = n;
	}
	void unknown_attribute(int) {
		env.errors_generated.emplace_back(env.file, graphics::errors::unknown_attribute);
	}
	void ignore(const discard_type&) {
	}
	void ignore_int(int) {
	}
};

MEMBER_DEF(parsing_object, name, "name");
MEMBER_DEF(parsing_object, horizontal, "horizontal");
MEMBER_DEF(parsing_object, internal_defintion.size, "size");
MEMBER_FDEF(parsing_object, alwaystransparent, "allwaystransparent");
MEMBER_FDEF(parsing_object, transparencecheck, "transparencecheck");
MEMBER_FDEF(parsing_object, flipv, "flipv");
MEMBER_FDEF(parsing_object, noOfFrames, "noofframes");
MEMBER_FDEF(parsing_object, clicksound, "clicksound");
MEMBER_FDEF(parsing_object, borderSize, "borderSize");
MEMBER_FDEF(parsing_object, single_size, "single_size");
MEMBER_FDEF(parsing_object, unknown_attribute, "unknown");
MEMBER_FDEF(parsing_object, ignore, "ignore");
MEMBER_FDEF(parsing_object, ignore_int, "ignore_int");
MEMBER_DEF(parsing_object, primary_texture, "primary_texture");
MEMBER_DEF(parsing_object, secondary_texture, "secondary_texture");

struct spritetypes {
	const parsing_environment& env;
	spritetypes(const parsing_environment& e) : env(e) {};

	void sprite(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::generic_sprite;
		}
	}
	void corneredtilesprite(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::bordered_rect;
		}
	}
	void textsprite(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::text_sprite;
		}
	}
	void progressbar(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			if(o.horizontal)
				env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::horizontal_progress_bar;
			else
				env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::vertical_progress_bar;
		}
	}
	void maskedshield(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::flag_mask;
		}
	}
	void tilesprite(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::tile_sprite;
		}
	}
	void barchart(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::barchart;
		}
	}
	void piechart(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::piechart;
		}
	}
	void linechart(const parsing_object& o) {
		if (auto f = env.nmaps.names.find(o.name); f != env.nmaps.names.end()) {
			env.defs.definitions[f->second] = o.internal_definition;
			o.setup_textures(env.defs.definitions[f->second]);
			env.defs.definitions[f->second].flags |= (uint8_t)graphics::object_type::linegraph;
		}
	}
	void unknown_key(int) {
		env.errors_generated.emplace_back(env.file, graphics::errors::unknown_sprite_type);
	};
	void discard(const discard_type&) {}
};

MEMBER_FDEF(spritetypes, sprite, "sprite");
MEMBER_FDEF(spritetypes, corneredtilesprite, "corneredtilesprite");
MEMBER_FDEF(spritetypes, progressbar, "progressbar");
MEMBER_FDEF(spritetypes, maskedshield, "maskedshield");
MEMBER_FDEF(spritetypes, textsprite, "textsprite");
MEMBER_FDEF(spritetypes, tilesprite, "tilesprite");
MEMBER_FDEF(spritetypes, barchart, "barchart");
MEMBER_FDEF(spritetypes, piechart, "piechart");
MEMBER_FDEF(spritetypes, linechart, "linechart");
MEMBER_FDEF(spritetypes, unknown_key, "unknown_key");
MEMBER_FDEF(spritetypes, discard, "discard");

struct gfx_file {
	const parsing_environment& env;
	gfx_file(const parsing_environment& e) : env(e) {};

	template<typename T>
	void discard_result(T&&) {};
	void unknown_key(int) {
		env.errors_generated.emplace_back(env.file, graphics::errors::unknown_file_level_type);
	};
};

MEMBER_FDEF(gfx_file, unknown_key, "unknown_key");
MEMBER_FDEF(gfx_file, discard_result, "discard");

constexpr bool accept_all(const char*, const char*) {
	return true;
}

BEGIN_DOMAIN(gfx_file_domain)
BEGIN_TYPE(graphics::xy_pair)
	MEMBER_ASSOCIATION("x", "x", value_from_rh<int>)
	MEMBER_ASSOCIATION("y", "y", value_from_rh<int>)
END_TYPE
BEGIN_TYPE(gfx_file)
	MEMBER_TYPE_ASSOCIATION("discard", "bitmapfonts", discard_type)
	MEMBER_TYPE_ASSOCIATION("discard", "fonts", discard_type)
	MEMBER_TYPE_ASSOCIATION("discard", "lighttypes", discard_type)
	MEMBER_TYPE_ASSOCIATION("discard", "objecttypes", discard_type)
	MEMBER_TYPE_ASSOCIATION("discard", "spritetypes", spritetypes)
	MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, discard_type, discard_to_int)
END_TYPE
BEGIN_TYPE(spritetypes)
	MEMBER_TYPE_ASSOCIATION("sprite", "spritetype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("corneredtilesprite", "corneredtilespritetype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("barchart", "barcharttype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("maskedshield", "maskedshieldtype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("piechart", "piecharttype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("progressbar", "progressbartype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("linechart", "linecharttype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("tilesprite", "tilespritetype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("textsprite", "textspritetype", parsing_object)
	MEMBER_TYPE_ASSOCIATION("discard", "scrollingsprite", discard_type)
	MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, discard_type, discard_to_int)
END_TYPE
BEGIN_TYPE(discard_type)
	MEMBER_VARIABLE_ASSOCIATION("any", accept_all, discard_from_full)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("any", accept_all, discard_type, discard_to_int)
END_TYPE
BEGIN_TYPE(parsing_object)
	MEMBER_ASSOCIATION("name", "name", value_from_rh<std::string>)
	MEMBER_ASSOCIATION("horizontal", "horizontal", value_from_rh<bool>)
	MEMBER_ASSOCIATION("allwaystransparent", "allwaystransparent", value_from_rh<bool>)
	MEMBER_ASSOCIATION("transparencecheck", "transparencecheck", value_from_rh<bool>)
	MEMBER_ASSOCIATION("flipv", "flipv", value_from_rh<bool>)
	MEMBER_ASSOCIATION("noofframes", "noofframes", value_from_rh<int>)
	MEMBER_ASSOCIATION("single_size", "size", value_from_rh<int>)
	MEMBER_ASSOCIATION("clicksound", "clicksound", token_from_rh)
	MEMBER_ASSOCIATION("primary_texture", "texturefile", token_from_rh)
	MEMBER_ASSOCIATION("primary_texture", "texturefile1", token_from_rh)
	MEMBER_ASSOCIATION("secondary_texture", "texturefile2", token_from_rh)
	MEMBER_TYPE_ASSOCIATION("borderSize", "bordersize", graphics::xy_pair)
	MEMBER_TYPE_ASSOCIATION("size", "size", graphics::xy_pair)
	MEMBER_TYPE_ASSOCIATION("ignore", "color", discard_type)
	MEMBER_TYPE_ASSOCIATION("ignore", "color2", discard_type)
	MEMBER_ASSOCIATION("ignore_int", "effectfile", discard_from_rh)
	MEMBER_ASSOCIATION("ignore_int", "loadtype", discard_from_rh)
	MEMBER_ASSOCIATION("ignore_int", "noreftype", discard_from_rh)
	MEMBER_ASSOCIATION("ignore_int", "linewidth", discard_from_rh)
	MEMBER_VARIABLE_ASSOCIATION("unknown", accept_all, discard_from_full)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown", accept_all, discard_type, discard_to_int)
END_TYPE
END_DOMAIN