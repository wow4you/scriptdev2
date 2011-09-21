-- Flame Leviathan
UPDATE creature_template SET ScriptName='boss_flame_leviathan' WHERE entry=33113;
UPDATE creature_template SET ScriptName='mob_defense_turret' WHERE entry=33142;

-- Ignis
UPDATE creature_template SET ScriptName='boss_ignis' WHERE entry=33118;
UPDATE creature_template SET ScriptName='npc_iron_construct' WHERE entry=33121;

-- Razorscale
UPDATE creature_template SET ScriptName='boss_razorscale' WHERE entry=33186;
UPDATE gameobject_template SET ScriptName='go_broken_harpoon' WHERE entry=194565;
-- mole machines & adds
UPDATE creature_template SET ScriptName='mob_mole_machine' WHERE entry=33245;
UPDATE creature_template SET ScriptName='mob_dark_rune_watcher' WHERE entry=33453;
UPDATE creature_template SET ScriptName='mob_dark_rune_sentinel' WHERE entry=33846;
UPDATE creature_template SET ScriptName='mob_dark_rune_guardian' WHERE entry=33388;
UPDATE creature_template SET ScriptName='npc_expedition_commander' WHERE entry=33210;
UPDATE creature_template SET ScriptName='mob_devouring_flame_target' WHERE entry=34188;

-- XT002
UPDATE creature_template SET ScriptName='boss_xt002' WHERE entry=33293;
UPDATE creature_template SET ScriptName='mob_pummeler' WHERE entry=33344;
UPDATE creature_template SET ScriptName='mob_boombot' WHERE entry=33346;
UPDATE creature_template SET ScriptName='mob_xtheart' WHERE entry=33329;
UPDATE creature_template SET ScriptName='mob_voidzone' WHERE entry=34001;
UPDATE creature_template SET ScriptName='mob_lifespark' WHERE entry=34004;

-- Iron council
UPDATE creature_template SET ScriptName='boss_brundir' WHERE entry=32857;
UPDATE creature_template SET ScriptName='boss_molgeim' WHERE entry=32927;
UPDATE creature_template SET ScriptName='boss_steelbreaker' WHERE entry=32867;
UPDATE creature_template SET ScriptName='mob_rune_of_power' WHERE entry=33705;
UPDATE creature_template SET ScriptName='mob_rune_of_summoning' WHERE entry=33051;
UPDATE creature_template SET ScriptName='mob_ulduar_lightning_elemental' WHERE entry=32958;

-- Kologarn
UPDATE creature_template SET ScriptName='boss_kologarn' WHERE entry=32930;
UPDATE creature_template SET ScriptName='boss_right_arm' WHERE entry=32934;
UPDATE creature_template SET ScriptName='boss_left_arm' WHERE entry=32933;
UPDATE creature_template SET ScriptName='mob_ulduar_rubble' WHERE entry IN (33768, 33809);

-- Auriaya
UPDATE creature_template SET scriptname='boss_auriaya' WHERE entry=33515;
UPDATE creature_template SET scriptname='mob_feral_defender' WHERE entry=34035;
UPDATE creature_template SET scriptname='mob_seeping_feral_essence' WHERE entry=34098;
UPDATE creature_template SET ScriptName='mob_sanctum_sentry' WHERE entry=34014;

-- Freya
UPDATE creature_template SET ScriptName='boss_freya' WHERE entry=32906;
UPDATE creature_template SET ScriptName='boss_elder_brightleaf' WHERE entry=32915;
UPDATE creature_template SET ScriptName='boss_elder_ironbranch' WHERE entry=32913;
UPDATE creature_template SET ScriptName='boss_elder_stonebark' WHERE entry=32914;
UPDATE creature_template SET ScriptName='mob_iron_roots' WHERE entry IN (33088, 33168);
UPDATE creature_template SET ScriptName='mob_freya_ground' WHERE entry IN (33215, 33228, 33170, 33050, 34129);
UPDATE creature_template SET ScriptName='mob_freya_spawned' WHERE entry IN (32916, 32919, 33202, 33203, 32918);

-- Hodir
UPDATE creature_template SET ScriptName='boss_hodir' WHERE entry=32845;
UPDATE creature_template SET ScriptName='mob_toasty_fire' WHERE entry=33342;
UPDATE creature_template SET ScriptName='mob_flashFreeze' WHERE entry IN (32926);
UPDATE creature_template SET ScriptName='mob_icicle' WHERE entry=33169;
-- flash freeze that will lock the npcs IN iceblock
UPDATE creature_template SET ScriptName='mob_npc_flashFreeze' WHERE entry=32938;

UPDATE creature_template SET ScriptName='npc_hodir_priest' WHERE entry IN (32897, 33326, 32948, 33330);
UPDATE creature_template SET ScriptName='npc_hodir_druid' WHERE entry IN (33325, 32901, 32941, 33333);
UPDATE creature_template SET ScriptName='npc_hodir_shaman' WHERE entry IN (33328, 32900, 33332, 32950);
UPDATE creature_template SET ScriptName='npc_hodir_mage' WHERE entry IN (32893, 33327, 33331, 32946);

-- Mimiron
UPDATE creature_template SET ScriptName='boss_mimiron' WHERE entry=33350;
UPDATE creature_template SET ScriptName='boss_leviathan_mk' WHERE entry=33432;
UPDATE creature_template SET ScriptName='leviathan_turret' WHERE entry=34071;
UPDATE creature_template SET ScriptName='mob_mimiron_flames' WHERE entry IN (34363, 34121);
UPDATE creature_template SET ScriptName='boss_vx001' WHERE entry=33651;
UPDATE creature_template SET ScriptName='boss_aerial_command_unit' WHERE entry=33670;
UPDATE gameobject_template SET ScriptName='go_red_button' WHERE entry=194739;
UPDATE creature_template SET ScriptName='mob_proximity_mine' WHERE entry=34362;
UPDATE creature_template SET ScriptName='mob_bomb_bot' WHERE entry IN (33836, 34192);
UPDATE creature_template SET ScriptName='mob_emergency_bot' WHERE entry=34147;
UPDATE creature_template SET ScriptName='mob_frost_bomb_ulduar' WHERE entry=34149;
UPDATE creature_template SET ScriptName='mob_mimiron_inferno' WHERE entry=33370;
UPDATE creature_template SET ScriptName='mob_assault_bot' WHERE entry=34057;
UPDATE creature_template SET ScriptName='mob_magnetic_core' WHERE entry=34068;

-- Thorim
UPDATE creature_template SET ScriptName='boss_thorim' WHERE entry=32865;
UPDATE creature_template SET ScriptName='boss_runic_colossus' WHERE entry=32872;
UPDATE creature_template SET ScriptName='boss_ancient_rune_giant' WHERE entry=32873;
UPDATE creature_template SET ScriptName='npc_lightning_orb' WHERE entry=33138;
UPDATE creature_template SET ScriptName='mob_thorim_trap_bunny' WHERE entry IN (33725, 33054);
UPDATE creature_template SET ScriptName='mob_thorim_preadds' WHERE entry IN (32885, 32883, 32907, 32908, 32882);
UPDATE creature_template SET ScriptName='npc_sif' WHERE entry=33196;
-- adds
UPDATE creature_template SET ScriptName='mob_dark_rune_acolyte' WHERE entry=33110;
UPDATE creature_template SET ScriptName='mob_dark_rune_champion' WHERE entry=32876;
UPDATE creature_template SET ScriptName='mob_dark_rune_commoner' WHERE entry=32904;
UPDATE creature_template SET ScriptName='mob_dark_rune_warbringer' WHERE entry=32877;
UPDATE creature_template SET ScriptName='mob_dark_rune_ring_guard' WHERE entry=32874;
UPDATE creature_template SET ScriptName='mob_dark_rune_honor_guard' WHERE entry=33125;
UPDATE creature_template SET ScriptName='mob_dark_rune_evoker' WHERE entry=32878;

-- Yogg
UPDATE creature_template SET ScriptName='boss_yogg_saron' WHERE entry=33288;
update creature_template SET ScriptName='boss_sara' where entry=33134;
update creature_template SET ScriptName='boss_brain_of_yogg_saron' where entry=33890;
UPDATE creature_template SET ScriptName='mob_corruptor_tentacle' WHERE entry=33985;
UPDATE creature_template SET ScriptName='mob_constrictor_tentacle' WHERE entry=33983;
UPDATE creature_template SET ScriptName='mob_vision_tentacle' WHERE entry=33943;
UPDATE creature_template SET ScriptName='mob_crusher_tentacle' WHERE entry=33966;
UPDATE creature_template SET ScriptName='mob_guardian_of_yogg_saron' WHERE entry=33136;
UPDATE creature_template SET ScriptName='mob_immortal_guardian' WHERE entry=33988;
UPDATE creature_template SET ScriptName='mob_death_orb' WHERE entry=33882;
UPDATE creature_template SET ScriptName='mob_sanity_well' WHERE entry=33991;
UPDATE creature_template SET scriptname='mob_madness_portal' WHERE `entry`=34072;
UPDATE creature_template SET scriptname='mob_laughing_skull' WHERE `entry`=33990;
UPDATE creature_template SET scriptname='mob_ominous_cloud' WHERE `entry`=33292;

-- Keepers
UPDATE creature_template SET ScriptName='keeper_hodir' WHERE entry=33213;
UPDATE creature_template SET ScriptName='keeper_freya' WHERE entry=33241;
UPDATE creature_template SET ScriptName='keeper_thorim' WHERE entry=33242;
UPDATE creature_template SET ScriptName='keeper_mimiron' WHERE entry=33244;

-- Algalon
UPDATE creature_template SET ScriptName='boss_algalon' WHERE entry=32871;
UPDATE creature_template SET ScriptName='mob_collapsing_star' WHERE entry=32955;
UPDATE creature_template SET ScriptName='mob_living_constellation' WHERE entry=33052;
UPDATE creature_template SET ScriptName='mob_black_hole' WHERE entry=32953;
UPDATE creature_template SET ScriptName='mob_cosmic_smash_target' WHERE entry IN (33105, 33104);
UPDATE gameobject_template SET ScriptName='go_celestial_acces' WHERE entry IN (194628, 194752);
