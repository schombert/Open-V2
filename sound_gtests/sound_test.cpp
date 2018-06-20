#include "gtest\\gtest.h"
#include "sound\\sound.h"
#include "fake_fs\\fake_fs.h"
#include "sound\\sound_io.h"

using namespace sound;

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation interface1 = directory_representation(u"interface", test1);
	directory_representation music1 = directory_representation(u"music", test1);
	directory_representation sound1 = directory_representation(u"sound", test1);

	file_representation m1 = file_representation(u"file_a.mp3", music1,"");
	file_representation m2 = file_representation(u"file_b.mp3", music1, "");
	file_representation m3 = file_representation(u"file_c.mp3", music1, "");
	file_representation m4 = file_representation(u"file_d.mp3", music1, "");

	file_representation sfx = file_representation(u"sound.sfx", interface1,
		"army_move = { file = \"GI_InfantryMove.wav\" }\r\n"
		"click = {\r\n"
		"	file = \"GI_ValidClick.wav\"\r\n"
		"}\r\n"
		"new_fort = {\r\n"
		"	file = Misc_Fortification.wav\r\n"
		"	volume = 0.5\r\n"
		"}");

	file_representation s1 = file_representation(u"GI_ValidClick.wav", sound1, "");
	file_representation s2 = file_representation(u"Misc_Fortification.wav", sound1, "");
	file_representation s3 = file_representation(u"GI_InfantryMove.wav", sound1, "");

	test_files() {
		set_default_root(f_root);
	}
};

TEST(sound_tests, read_music_directory) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));
	
	sound_manager m;

	populate_music(m, f.get_root());

	EXPECT_EQ(4ui64, m.music.size());
	EXPECT_EQ(std::u16string(u"F:\\test1\\music\\file_b.mp3"), m.music[music_tag(1)].filename);
}

TEST(sound_tests, read_sfx_file) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	sound_manager m;
	text_data::text_sequences tex;

	read_effects(m, tex, f.get_root());

	EXPECT_EQ(3ui64, m.sound_effects.size());
	EXPECT_EQ(std::u16string(u"F:\\test1\\sound\\GI_InfantryMove.wav"), m.sound_effects[effect_tag(0)].filename);
	EXPECT_EQ(1.0f, m.sound_effects[effect_tag(0)].volume);
	EXPECT_EQ(std::u16string(u"F:\\test1\\sound\\GI_ValidClick.wav"), m.sound_effects[effect_tag(1)].filename);
	EXPECT_EQ(1.0f, m.sound_effects[effect_tag(1)].volume);
	EXPECT_EQ(std::u16string(u"F:\\test1\\sound\\Misc_Fortification.wav"), m.sound_effects[effect_tag(2)].filename);
	EXPECT_EQ(0.5f, m.sound_effects[effect_tag(2)].volume);
	EXPECT_EQ(effect_tag(1), m.click);
}
